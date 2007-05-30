/* Copyright (C) 2004 Jean-Marc Valin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include "SourceInfo.h"
#include "sphere.h"
#include "Map.h"
#include <sstream>
#include "stream_wrap.h"
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>

using namespace FD;

class ServerSaveWav;

DECLARE_NODE(ServerSaveWav)
/*Node
 *
 * @name ServerSaveWav
 * @category ManyEars
 * @description Saves all sources to a different file
 *
 * @input_name SOURCES
 * @input_type Map<int,ObjectRef>
 * @input_description Source audio
 *
 * @input_name SRC_POS
 * @input_type Vector<ObjectRef>
 * @input_description Position of sources
 *
 * @output_name OUTPUT
 * @output_type Map<int,ObjectRef>
 * @output_description Same as input
 *
 * @parameter_name BASENAME
 * @parameter_type string
 * @parameter_description Base string for file names
 * @parameter_value source_
 *
 * @parameter_name HOST
 * @parameter_type string
 * @parameter_description Host where server is running
 * @parameter_value localhost
 *
 * @parameter_name PORT
 * @parameter_type int
 * @parameter_description Port where server is running
 *
 * @parameter_name SAVE_TO_FILE
 * @parameter_type bool
 * @parameter_description Whether to save the separated sources to file
 * @parameter_value false
 *
 * @parameter_name SAVE_TO_SOCKET
 * @parameter_type bool
 * @parameter_description Whether to save the separated sources to the socket
 * @parameter_value false
 *
 * @parameter_name SPLIT_STRENGTH
 * @parameter_type float
 * @parameter_description Splitting the stream into multiple parts when strenght < SPLIT_STRENGTH
 * @parameter_value 0.01
 *
END*/


class ServerSaveWav : public BufferedNode {
   
  int sourcesID;
  int srcPosID;
  int outputID;
  string m_baseName;
  string host;
  int port;
  map<int, ostream*> m_socketStreams;
  map<int, ostream*> m_fileStreams;
  //map<int, int> start_pos;
  map<int, int> numFrames;
  map<int, Vector<ObjectRef> > m_frameAccumulator;
  map<int, int> m_accumulatorLastCount;
  bool m_saveToFile;
  bool m_saveToSocket;
  //iostream* m_stream;
  //int currentId;
  //std::vector<int> m_ports;
  float m_splitStrength;
  
  
  
public:
  ServerSaveWav(string nodeName, ParameterSet params)
    : BufferedNode(nodeName, params), m_saveToFile(false), m_splitStrength(0)
  {
    sourcesID = addInput("SOURCES");
    srcPosID = addInput("SRC_POS");
    outputID = addOutput("OUTPUT");
    m_baseName = object_cast<String> (parameters.get("BASENAME"));
    host = object_cast<String> (parameters.get("HOST"));
    port = dereference_cast<int> (parameters.get("PORT"));
    

    if (parameters.exist("SAVE_TO_FILE"))
    {
      m_saveToFile = dereference_cast<bool> (parameters.get("SAVE_TO_FILE"));
    }

    if (parameters.exist("SAVE_TO_SOCKET"))
    {
      m_saveToSocket = dereference_cast<bool> (parameters.get("SAVE_TO_SOCKET"));
    }
   
    if (parameters.exist("SPLIT_STRENGTH"))
    {
      m_splitStrength = dereference_cast<float>(parameters.get("SPLIT_STRENGTH"));
    }
   
    inOrder = true;
  }
  
  ~ServerSaveWav()
  {
    closeFiles();
  }
  
  void reset()
  {
    closeFiles();
  }
  
  void closeFiles()
  {
    //DELETING ALL SOCKETS
    map<int,ostream*>::iterator it = m_socketStreams.begin();
    while (it != m_socketStreams.end())
    {
      delete it->second;
      m_socketStreams.erase(it);
      it = m_socketStreams.begin();
    }

    //DELETING ALL FILES
    map<int,ostream*>::iterator it2 = m_fileStreams.begin();
    while (it2 != m_fileStreams.end())
    {
      delete it->second;
      m_fileStreams.erase(it2);
      it2 = m_fileStreams.begin();
    }

  }
  
  ostream *openSocketStream(int id)
  {
    struct sockaddr_in addr;
    int sock;
    struct hostent *entp;
    
    if ((sock = socket(PF_INET, SOCK_STREAM, 0))==-1)
    {
      perror("socket creation failed");
      return NULL;
    }
    
    memset(&addr, 0, sizeof(struct sockaddr));
    
    addr.sin_family = AF_INET;
    
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(0);
    
    cerr << "connecting to " << host << ":" << port << endl;
    
    if((entp = gethostbyname(host.c_str())) == NULL) {
      char message[256];
      fprintf(stderr, "Can't get host by name\n");
      return NULL;
    }
    
    memcpy(&addr.sin_addr, entp->h_addr_list[0], entp->h_length);
    
    addr.sin_port = htons(port);
    if (connect (sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
      perror("connect: ");
      return NULL;
    }
    

    //CREATING STREAM
    ostream *stream = new fd_ostream(sock);

    //WRITING STREAM HEADER
    //short shortid = htons(id);
    //stream->write((const char *)&shortid, sizeof(short)); 

    return stream;
  }
  
  void writeFrames(ostream &out, int id) {
    

    for (int frameNum = 0; frameNum < m_frameAccumulator[id].size(); frameNum++)
    {	
	Vector<float> &frame = object_cast<Vector<float> >(m_frameAccumulator[id][frameNum]);
	short buff[frame.size()];
	int i, k;
	
	//The 3 is the decimation for 16kHz
	//6 is for 8kHz
	for (k=0,i=0/*start_pos[id]*/;i<frame.size();i+=6,k++)
	{
		buff[k] = 10 * short(frame[i]);
	}
	
	
	//start_pos[id] = i-frame.size();    
	int nb_samples = k;
	out.write((const char *)buff, sizeof(short)*nb_samples);  
	
    }
  
  }
 
void constructHeader(ostream &out, unsigned long dataLength)
{

//Format:           1 (PCM)
//Channels:         1
//Sample rate:      8000
//Bytes per second: 8000
//Bytes per sample: 1
//Bits per channel: 8
//Bytes:            24002
//Samples:          24002
//Seconds:          3.00025
//File pointer:     58
	
//--------------------------------
//the header defined in details at http://www.sonicspot.com/guide/wavefiles.html
//--------------------------------
//		 __________________________
//		| RIFF WAVE Chunk          |
//		|   groupID  = 'RIFF'      |
//		|   riffType = 'WAVE'      |
//		|    __________________    |
//		|   | Format Chunk     |   |
//		|   |	ckID = 'fmt '    |   |
//		|   |__________________|   |
//		|    __________________    |
//		|   | Sound Data Chunk |   |
//		|   |	ckID = 'data'    |   |
//		|   |__________________|   |
//		|__________________________|

	// constants for the canonical WAVE format
	const int fmtChunkLength = 16;  											// length of fmt contents
	const int waveHeaderLength = 4 + 8 + fmtChunkLength + 8;  		// from "WAVE" to sample data		

	unsigned long 	wholeLength 	= waveHeaderLength + dataLength;	//size of the wave with header
	unsigned long 	chunkLength 	= fmtChunkLength;						//chunk header length												
	unsigned short formatType      = 1;										//format PCM
	unsigned short numChannels     = 1;										//1 channel = mono
	unsigned long 	sampleRate     = 8000;									//8kHZ
	unsigned long 	bytesPerSecond = 8000 * 2;									//Bytes per second
	unsigned short bytesPerSample  = 2;										//Bytes per sample
	unsigned short bitsPerChannel  = 16;										//Bits per channel
	//unsigned long 	dataLength	/*	= FIXME; */								//length of the USEFUL data
	
	//fp is the string to push to
	//FIXME ostream fp;
	out.write((const char*)"RIFF",4);
	out.write((const char*)&wholeLength,sizeof(wholeLength));
	out.write((const char*)"WAVE", 4);
	out.write((const char*)"fmt ",4);
	out.write((const char*)&chunkLength,sizeof(chunkLength));
	out.write((const char*)&formatType,sizeof(formatType));
	out.write((const char*)&numChannels,sizeof(numChannels));
	out.write((const char*)&sampleRate, sizeof(sampleRate));
	out.write((const char*)&bytesPerSecond,sizeof(bytesPerSecond));
	out.write((const char*)&bytesPerSample,sizeof(bytesPerSample));
	out.write((const char*)&bitsPerChannel,sizeof(bitsPerChannel));
	out.write((const char*)"data", 4);
	out.write((const char*)&dataLength, sizeof(dataLength));

}


  void writeWavHeader(ostream &out, int id) {
	cerr<<"writing wav header"<<endl;
	unsigned long datasize = m_frameAccumulator[id].size() * 1024 / 6;
	constructHeader(out,datasize);	
  }

  void writeWav(int id) {

	if (m_saveToSocket)
	{
		ostream *outStream = openSocketStream(id);
		//writeWavHeader(*outStream,id);
		writeFrames(*outStream,id);
		delete outStream;
	}

	if (m_saveToFile) 
	{
		stringstream outfile;
		outfile << m_baseName << id << ".wav";
		ostream *outStream = new ofstream(outfile.str().c_str());
		writeWavHeader(*outStream,id);
		writeFrames(*outStream,id);
		delete outStream;
	}


  }


  void calculate(int output_id, int count, Buffer &out)
  {
    ObjectRef input1Value = getInput(sourcesID, count);
    const Map<int,ObjectRef> &src = object_cast<Map<int,ObjectRef> > (input1Value);
    
    ObjectRef input2Value = getInput(srcPosID, count);
    const Vector<ObjectRef> &src_pos = object_cast<Vector<ObjectRef> > (input2Value);
                  
    Map<int,ObjectRef>::const_iterator it = src.begin();
  
    while (it != src.end())
    {
      int id = it->first;

      //initialize numFrames properly
      if (numFrames.find(id) == numFrames.end()) 
      {
	numFrames[id] = 0;      
      }

      numFrames[id]++;
      
      
      //FIND SOURCE STRENGTH
      float strength = 0;
      
      for (int i = 0; i < src_pos.size(); i++)
      {
	const SourceInfo &info = object_cast<SourceInfo>(src_pos[i]);
	if (info.source_id == id) {
	  strength = info.strength;
	  //cerr<<"Found source id : "<<id<<" of strength "<<info.strength<<endl;
	  break;
	}
      }
      
      
     
	//cerr<<"Discard id : "<<id<<" strength "<<strength<<" numFrames: "<<numFrames[id]<<endl;
	
	//ACCUMULATE FRAMES, MAXIMUM 30 PER SOURCE 
	cerr<<"Accumulating : "<<id<<" numFrames : "<<numFrames[id]<<endl;
	m_frameAccumulator[id].push_back(src.find(id)->second);
	m_accumulatorLastCount[id] = count;

      	it++;
    }//WHILE ALL SOURCES 

    //REMOVING UNUSED STREAMS IF REQUIRED (This will close the connexion or file).
    //for(map<int, ostream*>::iterator ite = streamsCopy.begin();
    //ite != streamsCopy.end();
    //ite++)
    for (map<int, Vector<ObjectRef> >::iterator ite = m_frameAccumulator.begin(); 
	ite != m_frameAccumulator.end(); ite++)
    {
     
      //CHECK FOR TIMEOUT
      if (count > m_accumulatorLastCount[ite->first] + 30) 
      {	
	//WRITE DATA
        if (numFrames[ite->first] > 30 ) {
		cerr<<"Writing data to socket or file"<<endl;
	 	writeWav(ite->first);
	}

	//CLEANUP MEMORY	
	numFrames.erase(ite->first);	
	m_accumulatorLastCount.erase(ite->first);	
	m_frameAccumulator.erase(ite->first);

      }
    }
    
    //output audio
    out[count] = input1Value;
  }
  
  IN_ORDER_NODE_SPEEDUP(ServerSaveWav)
      
};

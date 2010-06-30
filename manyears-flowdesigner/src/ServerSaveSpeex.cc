/* Copyright (C) 2006 Dominic Létourneau

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
#include <queue>
#include <fstream>
#include <sstream>
#include "pseudosem.h"
#include "binio.h"

//16KHz downsampling
#define FRAMERATE_DIVISOR 3 

//8KHz downsampling
//#define FRAMERATE_DIVISOR 6 

using namespace FD;

class ServerSaveSpeex;

DECLARE_NODE(ServerSaveSpeex)
/*Node
 *
 * @name ServerSaveSpeex
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


void *rec_thread(void *data);


class ServerSaveSpeex : public BufferedNode {
  
  friend void *rec_thread(void *data);
   
  int sourcesID;
  int srcPosID;
  int outputID;
  string m_baseName;
  string host;
  int port;
  map<int, ostream*> m_socketStreams;
  map<int, ostream*> m_fileStreams;
  //map<int, int> start_pos;

  map<int, Vector<ObjectRef> > m_frameAccumulator;
  map<int, int> m_accumulatorLastCount;
  bool m_saveToFile;
  bool m_saveToSocket;
  bool m_threadRunning;
  //iostream* m_stream;
  //int currentId;
  //std::vector<int> m_ports;
  float m_splitStrength;
  pthread_mutex_t m_recMutex;
  list<int> m_readyQueue;
  pseudosem_t m_semaphore;
  pthread_t m_thread;
  
public:
  ServerSaveSpeex(string nodeName, ParameterSet params)
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

    pthread_mutex_init(&m_recMutex,NULL);
    pseudosem_init(&m_semaphore,0,0);
    m_threadRunning = true;
    pthread_create(&m_thread, NULL, rec_thread, this);
  }
  
  void lock() 
  {
	pthread_mutex_lock(&m_recMutex);
  }
  
  void unlock()
  {
	pthread_mutex_unlock(&m_recMutex);
  }

  ~ServerSaveSpeex()
  {
    	m_threadRunning = false;
	pseudosem_post(&m_semaphore);
	unlock();
	pthread_join(m_thread,NULL); 
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
  
  void writeFrames(ostream &out, map<int,Vector<ObjectRef> > &accum, int id) {
    

    for (int frameNum = 0; frameNum < accum[id].size(); frameNum++)
    {	
	Vector<float> &frame = object_cast<Vector<float> >(accum[id][frameNum]);
	short buff[frame.size()];
	int i, k;
	
	//The 3 is the decimation for 16kHz
	//6 is for 8kHz
	for (k=0,i=0/*start_pos[id]*/;i<frame.size();i+=FRAMERATE_DIVISOR,k++)
	{
		buff[k] = 10 * short(frame[i]);
	}
	
	
	//start_pos[id] = i-frame.size();    
	int nb_samples = k;
	out.write((const char *)buff, sizeof(short)*nb_samples);  
	
    }
  
  }

  
 
void constructHeader(ostream &out, unsigned long dataLength, bool writingForSocket)
{
//--------------------------------
//the header defined in details at http://www.sonicspot.com/guide/wavefiles.html
//--------------------------------
//		 __________________________
//		| RIFF WAVE Chunk          |
//		|   groupID  = 'RIFF'      |
//		|   riffType = 'WAVE'      |
//		|    __________________    |
//		|   | Format Chunk     |   |
//		|   |	ckID = 'fmt '  |   |
//		|   |__________________|   |
//		|    __________________    |
//		|   | Sound Data Chunk |   |
//		|   |	ckID = 'data'  |   |
//		|   |__________________|   |
//		|__________________________|

	// constants for the canonical WAVE format
	const int fmtChunkLength = 16;  					// length of fmt contents
	const int waveHeaderLength = 44;  					// from "WAVE" to sample data		

	

	unsigned long 	wholeLength 	= waveHeaderLength + dataLength;	//size of the wave with header
	unsigned long 	chunkLength 	= fmtChunkLength;			//chunk header length
	unsigned short formatType      = 1;					//format PCM
	unsigned short numChannels     = 1;					//1 channel = mono
	unsigned long 	sampleRate     = 48000 / FRAMERATE_DIVISOR;		//8 - 16 kHZ
	
	unsigned short bitsPerSample  = 16;					//Bits per channel
	unsigned short blockAlign  = (bitsPerSample / 8) * numChannels;		//Bytes per sample
	unsigned long  bytesPerSecond = sampleRate * blockAlign;		//Bytes per second
	
	//unsigned long 	dataLength					//length of the USEFUL data
		
	//we must write the length of the whole file (data) to the socket first
	//TCL wants it like that
/*	if (writingForSocket) {
		unsigned long totalLength = waveHeaderLength + dataLength;	
		out.write((const char*)&totalLength,sizeof(wholeLength));
		out.write((const char*)"\n",1);
		cerr<<"totalLength : " << totalLength << endl;
	}
*/

		
		out.write((const char*)"RIFF",4); //4 bytes
		out.write((const char*)&wholeLength,sizeof(wholeLength)); //4 bytes
		out.write((const char*)"WAVE", 4); //4 bytes
		out.write((const char*)"fmt ",4); //4 bytes
		out.write((const char*)&chunkLength,sizeof(chunkLength)); //4 bytes
		//TOTAL = 20 bytes


		out.write((const char*)&formatType,sizeof(formatType)); //2 bytes
		out.write((const char*)&numChannels,sizeof(numChannels));//2 bytes
		out.write((const char*)&sampleRate, sizeof(sampleRate));//4 bytes
		out.write((const char*)&bytesPerSecond,sizeof(bytesPerSecond));//4 bytes
		out.write((const char*)&blockAlign,sizeof(blockAlign));//2 bytes
		out.write((const char*)&bitsPerSample,sizeof(bitsPerSample));//2 bytes
		//TOTAL = 16 bytes

		out.write((const char*)"data", 4); //4 bytes
		out.write((const char*)&dataLength, sizeof(dataLength)); //4 bytes
		//TOTAL = 8 bytes
		//HEADER = 44 bytes	

}


  void writeWavHeader(ostream &out, map<int,Vector<ObjectRef> > &accum, int id, bool writingForSocket) {
	cerr<<"writeWavHeader : writing wav header"<<endl;
	unsigned long datasize = accum[id].size() * 1024 / FRAMERATE_DIVISOR;
	cerr<<"writeWavHeader : datasize =  "<<datasize<<endl;
	constructHeader(out,datasize, writingForSocket);	
  }

  void writeWav(map<int, Vector<ObjectRef> > &accum, int id) {

	if (m_saveToSocket)
	{
		ostream *outStream = openSocketStream(id);

		if (outStream) {

			writeWavHeader(*outStream,accum, id, true);
			writeFrames(*outStream,accum,id);
			delete outStream;
		}
	}

	if (m_saveToFile) 
	{
		stringstream outfile;
		outfile << m_baseName << id << ".wav";
		ostream *outStream = new ofstream(outfile.str().c_str());
		if (outStream) 
		{
			writeWavHeader(*outStream,accum,id, false);
			writeFrames(*outStream,accum,id);
			delete outStream;
		}
	}


  }
 
  void writeSpeex(map<int, Vector<ObjectRef> > &accum, int id) {

	if (m_saveToFile) 
	{
		stringstream cmd;
		cmd << "speexenc -n -w ";
		cmd << m_baseName << id << ".wav ";
		cmd << m_baseName << id << ".wav.spx; rm "<<m_baseName<<id<<".wav";
		//cmd << m_baseName << id << ".wav.spx;";
		cerr << "writeSpeex : Executing : "<<cmd.str()<<endl;
		system(cmd.str().c_str());
		cerr<<"writeSpeex : done!"<<endl;
	}
  }


  void calculate(int output_id, int count, Buffer &out)
  {
    
    ObjectRef input1Value = getInput(sourcesID, count);
    const Map<int,ObjectRef> &src = object_cast<Map<int,ObjectRef> > (input1Value);
    
    ObjectRef input2Value = getInput(srcPosID, count);
    const Vector<ObjectRef> &src_pos = object_cast<Vector<ObjectRef> > (input2Value);
    
    lock();              
    Map<int,ObjectRef>::const_iterator it = src.begin();
  
    while (it != src.end())
    {
      int id = it->first;

	m_frameAccumulator[id].push_back(src.find(id)->second);
	cerr<<"Accumulating : "<<id<<" numFrames : "<<m_frameAccumulator[id].size()<<endl;
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
        if (m_frameAccumulator[ite->first].size() > 30 ) {
	
	
		bool found = false;
		for (list<int>::iterator qIter = m_readyQueue.begin(); qIter != m_readyQueue.end(); qIter++)
		{
			if (*qIter == ite->first) 
			{
				found = true;
				break;
			}
		}
	
		if (!found)
		{
				m_readyQueue.push_back(ite->first);
				pseudosem_post(&m_semaphore);
				cerr<<"POST SOURCE READY : "<<ite->first<<endl;
		}
	}
	else {
		//remove source
		m_accumulatorLastCount.erase(ite->first);	
		m_frameAccumulator.erase(ite->first);
	}	
      }
    }
    
    //output audio
    out[count] = input1Value;

    unlock();
  }
  
  IN_ORDER_NODE_SPEEDUP(ServerSaveSpeex)
      
};


void *rec_thread(void *data)
{
	cerr<<"rec_thread : starting"<<endl;
	ServerSaveSpeex *node = reinterpret_cast<ServerSaveSpeex*>(data);

  	map<int, Vector<ObjectRef> > frameAccumulator;

	while (node->m_threadRunning) {
	
		//waiting for something to do
		pseudosem_wait(&node->m_semaphore);

		node->lock();
		
		//copy data structure
		//numFrames = node->numFrames;
		//frameAccumulator = node->m_frameAccumulator;
		
		if (!node->m_readyQueue.empty() && node->m_threadRunning)
		{
			int id = node->m_readyQueue.front();
			cerr<<"rec_thread : processing source id : "<<id<<endl;
			node->m_readyQueue.pop_front();
				
		
			//copy source content
			frameAccumulator[id] = node->m_frameAccumulator[id];
			
			//clean up memory for original data structures				
			node->m_accumulatorLastCount.erase(id);	
			node->m_frameAccumulator.erase(id);

			cerr<<"rec_thread : copying data done"<<endl;
			node->unlock();
			//do something			
			//will now work on local thread data

			node->writeWav(frameAccumulator,id);
			node->writeSpeex(frameAccumulator,id);
		
			cerr<<"rec_thread : source completed id  "<<id<<endl;

			//clean up local memory
			frameAccumulator.erase(id);

		}
		else 
		{
			node->unlock();
		}
	}
	cerr<<"rec_thread : done!"<<endl;
	return NULL;
	
}



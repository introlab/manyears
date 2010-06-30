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

class ServerSaveRaw;

DECLARE_NODE(ServerSaveRaw)
/*Node
 *
 * @name ServerSaveRaw
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


class ServerSaveRaw : public BufferedNode {
   
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
  ServerSaveRaw(string nodeName, ParameterSet params)
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
  
  ~ServerSaveRaw()
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
    short shortid = htons(id);
    stream->write((const char *)&shortid, sizeof(short)); 

    return stream;
  }
  
  void writeFrame(const Vector<float> &frame, int id) {
    
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
	  
	
    //WRITE TO SOCKET IF REQUIRED
    if (m_saveToSocket)
    {
      m_socketStreams[id]->write((const char *)buff, sizeof(short)*nb_samples);
    }


    //SAVING TO FILE IF REQUIRED
    if (m_saveToFile)
    {
      m_fileStreams[id]->write((const char *)buff, sizeof(short)*nb_samples);
    }	  	

  }


  void calculate(int output_id, int count, Buffer &out)
  {
    ObjectRef input1Value = getInput(sourcesID, count);
    const Map<int,ObjectRef> &src = object_cast<Map<int,ObjectRef> > (input1Value);
    
    ObjectRef input2Value = getInput(srcPosID, count);
    const Vector<ObjectRef> &src_pos = object_cast<Vector<ObjectRef> > (input2Value);
    
      
    map<int, ostream*> streamsCopy;

    if (m_saveToSocket) 
    {
      streamsCopy = m_socketStreams;
    }
    else if (m_saveToFile)
    {
      streamsCopy = m_fileStreams;
    }
    
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
      
      
      //FRAMES ARE 1024 SAMPLES @ 48KHz
      //30 frames = 640ms
      if(numFrames[id] > 30)
      {

	if (m_socketStreams.find(id) == m_socketStreams.end() && 
	    m_fileStreams.find(id) == m_fileStreams.end())
	{
		//CREATE SOCKET IF REQUIRED	
		if (m_saveToSocket)
		{
		if (m_socketStreams.find(id) == m_socketStreams.end())
		{  
		std::cerr << "Activating send of source " << id << std::endl;	   
		m_socketStreams[id] = openSocketStream(id);
		}
		}
		
		if (m_saveToFile) 
		{
		if (m_fileStreams.find(id) == m_fileStreams.end())
		{
		//Opening new file 
		stringstream outfile;
		outfile << m_baseName << id << ".wav";
		m_fileStreams[id] = new ofstream(outfile.str().c_str());
	
		}
		}
		//SEND ACCUMULATED FRAMES 
		for (int i = 0; i < m_frameAccumulator[id].size(); i++) {
			const Vector<float> &aud = object_cast<Vector<float> > (m_frameAccumulator[id][i]);	  
			writeFrame(aud,id);	    
		}	
	}	  
      	
	
	//WRITE AUDIO CURRENT FRAME
	const Vector<float> &aud = object_cast<Vector<float> > (src.find(id)->second);
	writeFrame(aud,id);

	//REMOVE FROM UNACTIVE LIST IF STRENGTH IS HIGH ENOUGH
	//ELSE A NEW STREAM WILL BE CREATED...
	//WE WILL THEN AVOID SENDING VERY LONG STREAMS TO NUANCE AND TAKING THE CPU TOO LONGER
	//if (strength > m_splitStrength) {
	streamsCopy.erase(id);	  
	//}
	//else
	//{
	//NOT REMOVING THE STREAM ID FROM STREAMS COPY WILL CAUSE TO CLOSE THE CONNEXION
	//cerr<<"Source id : "<<id<<" splitting strength : "<<strength<<endl;
	//}
      }
      else {
	//cerr<<"Discard id : "<<id<<" strength "<<strength<<" numFrames: "<<numFrames[id]<<endl;

	//ACCUMULATE FRAMES, MAXIMUM 30 PER SOURCE 
	cerr<<"Accumulating : "<<id<<" numFrames : "<<numFrames[id]<<endl;
	m_frameAccumulator[id].push_back(src.find(id)->second);
	m_accumulatorLastCount[id] = count;

	//AVOID REMOVING SOURCE
	if (streamsCopy.find(id) != streamsCopy.end()) {
	  streamsCopy.erase(id);
	}

      }
      it++;
    }//WHILE ALL SOURCES 

    //REMOVING UNUSED STREAMS (This will close the connexion).
    for(map<int, ostream*>::iterator ite = streamsCopy.begin();
	ite != streamsCopy.end();
	ite++)
    {
      //CHECK FOR TIMEOUT
      if (count > m_accumulatorLastCount[ite->first] + 30) 
      {
	//CLEANUP MEMORY	
	numFrames.erase(ite->first);	
	m_accumulatorLastCount.erase(ite->first);	
	m_frameAccumulator.erase(ite->first);
	
	//CLOSE FILE
	if (m_saveToFile) 
	{
	  //(ofstream*)(m_fileStreams[ite->first])->close();
	  delete m_fileStreams[ite->first];
	  m_fileStreams.erase(ite->first);
	  std::cerr << "Liberating file streams source: " << ite->first<< std::endl;
	}

	//CLOSE SOCKET
	if (m_saveToSocket)
	{
	  delete m_socketStreams[ite->first];
	  m_socketStreams.erase(ite->first);
	  std::cerr << "Liberating socket streams source: " << ite->first<< std::endl;
	}
      }
    }
    
    //output audio
    out[count] = input1Value;
  }
  
  IN_ORDER_NODE_SPEEDUP(ServerSaveRaw)
      
};

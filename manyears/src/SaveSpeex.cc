/* Copyright (C) 2007 Dominic Letourneau

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

class SaveSpeex;

DECLARE_NODE(SaveSpeex)
/*Node
 *
 * @name SaveSpeex
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
END*/


void *savespeex_rec_thread(void *data);


class SaveSpeex : public BufferedNode {
  
  protected:

  friend void *savespeex_rec_thread(void *data);
   
  int sourcesID;
  int srcPosID;
  int outputID;
  string m_baseName;
  string host;
  int port;
  map<int, ostream*> m_socketStreams;
  map<int, ostream*> m_fileStreams;
  map<int, int> start_pos;

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
  SaveSpeex(string nodeName, ParameterSet params)
    : BufferedNode(nodeName, params), m_saveToFile(false), m_splitStrength(0)
  {
    sourcesID = addInput("SOURCES");
    srcPosID = addInput("SRC_POS");
    outputID = addOutput("OUTPUT");
    m_baseName = object_cast<String> (parameters.get("BASENAME"));
    pthread_mutex_init(&m_recMutex,NULL);
    pseudosem_init(&m_semaphore,0,0);
    m_threadRunning = true;
    pthread_create(&m_thread, NULL, savespeex_rec_thread, this);
  }
  
  void lock() 
  {
	pthread_mutex_lock(&m_recMutex);
  }
  
  void unlock()
  {
	pthread_mutex_unlock(&m_recMutex);
  }

  ~SaveSpeex()
  {
    m_threadRunning = false;
	pseudosem_post(&m_semaphore);
	unlock();
	pthread_join(m_thread,NULL); 
  }
  
    void writeFrames(ostream &out, map<int,Vector<ObjectRef> > &accum, int id) {
        
    
        for (int frameNum = 0; frameNum < accum[id].size(); frameNum++)
        {   
            Vector<float> &frame = object_cast<Vector<float> >(accum[id][frameNum]);
            short buff[frame.size()];
            int i, k;
            
            //The 3 is the decimation for 16kHz
            //6 is for 8kHz
            for (k=0,i=start_pos[id];i<frame.size();i+=FRAMERATE_DIVISOR)
            {
                float tmp = 3 * frame[i];
                if (tmp > 32767)
                   tmp = 32767;
                else if (tmp < -32767)
                   tmp = -32767;
                buff[k++] = short(.5+floor(tmp));
            }
        
        
            start_pos[id] = i-frame.size();    
            int nb_samples = k;
            
            out.write((const char *)buff, sizeof(short)*nb_samples);          
        }
    
    }


    void constructWavHeader(ostream &out, unsigned long dataLength)
    {

        //--------------------------------
        //the header defined in details at http://www.sonicspot.com/guide/wavefiles.html
        //--------------------------------
        //       __________________________
        //      | RIFF WAVE Chunk          |
        //      |   groupID  = 'RIFF'      |
        //      |   riffType = 'WAVE'      |
        //      |    __________________    |
        //      |   | Format Chunk     |   |
        //      |   |   ckID = 'fmt '  |   |
        //      |   |__________________|   |
        //      |    __________________    |
        //      |   | Sound Data Chunk |   |
        //      |   |   ckID = 'data'  |   |
        //      |   |__________________|   |
        //      |__________________________|

        // constants for the canonical WAVE format
        const int fmtChunkLength = 16;                      // length of fmt contents
        const int waveHeaderLength = 36;                    // from "WAVE" to sample data       
    
        
    
        unsigned int   wholeLength     = waveHeaderLength + dataLength;    //size of the wave with header
        unsigned int   chunkLength     = fmtChunkLength;           //chunk header length
        unsigned short formatType      = 1;                 //format PCM
        unsigned short numChannels     = 1;                 //1 channel = mono
        unsigned int   sampleRate     = 48000 / FRAMERATE_DIVISOR;     //8 - 16 kHZ
        
        unsigned short bitsPerSample  = 16;                 //Bits per channel
        unsigned short blockAlign  = (bitsPerSample / 8) * numChannels;     //Bytes per sample
        unsigned int  bytesPerSecond = sampleRate * blockAlign;        //Bytes per second
        
        //unsigned long     dataLength                  //length of the USEFUL data
                            
        out.write((const char*)"RIFF",4); //4 bytes
        out.write((const char*)&wholeLength,4); //4 bytes
        out.write((const char*)"WAVE", 4); //4 bytes
        out.write((const char*)"fmt ",4); //4 bytes
        out.write((const char*)&chunkLength,4); //4 bytes
        //TOTAL = 20 bytes


        out.write((const char*)&formatType,2); //2 bytes
        out.write((const char*)&numChannels,2);//2 bytes
        out.write((const char*)&sampleRate, 4);//4 bytes
        out.write((const char*)&bytesPerSecond,4);//4 bytes
        out.write((const char*)&blockAlign,2);//2 bytes
        out.write((const char*)&bitsPerSample,2);//2 bytes
        //TOTAL = 16 bytes

        out.write((const char*)"data", 4); //4 bytes
        out.write((const char*)&dataLength, 4); //4 bytes
        //TOTAL = 8 bytes
        //HEADER = 44 bytes 
    
    }

  void writeWavHeader(ostream &out, map<int,Vector<ObjectRef> > &accum, int id) 
  {
    unsigned int datasize = 0;
    for (int frameNum = 0; frameNum < accum[id].size(); frameNum++)
    {   
       Vector<float> &frame = object_cast<Vector<float> >(accum[id][frameNum]);
       datasize += frame.size();
    }
    datasize = 2 * ((datasize+FRAMERATE_DIVISOR-1) / FRAMERATE_DIVISOR);
    //unsigned long datasize = accum[id].size() * (1024 / FRAMERATE_DIVISOR + 1);
    //cerr<<"writeWavHeader : datasize =  "<<datasize<<endl;
    constructWavHeader(out,datasize);    
  }

  void writeWav(map<int, Vector<ObjectRef> > &accum, int id) {

		stringstream fname;
		fname << m_baseName << id << ".wav";
		
        //CREATE STREAM, WAIT ON CLOSE, OWNER
        ofstream my_stream(fname.str().c_str());
        
        writeWavHeader(my_stream,accum,id);
        writeFrames(my_stream,accum,id);
        
        my_stream.close();
  }


  void writeSpeex(map<int, Vector<ObjectRef> > &accum, int id) {

		stringstream cmd;
		cmd << "/usr/bin/speexenc -w - ";
		cmd << m_baseName << id << ".spx";
		//cmd << m_baseName << id << ".wav.spx;";
		cerr << "writeSpeex: Executing Command : "<<cmd.str()<<endl;

        //CREATE STREAM, WAIT ON CLOSE, OWNER
        IOStream my_stream(new pipe_iostream(cmd.str(),true),true);

        //WRITE DATA
        writeWavHeader(my_stream,accum,id);
        writeFrames(my_stream,accum,id);

        my_stream.flush();
        my_stream << EOF;
	                

	cerr<<"writeSpeex : done!"<<endl;
  }


  void calculate(int output_id, int count, Buffer &out)
  {
    
    ObjectRef input1Value = getInput(sourcesID, count);
    const Map<int,ObjectRef> &src = object_cast<Map<int,ObjectRef> > (input1Value);
    
    ObjectRef input2Value = getInput(srcPosID, count);
    const Vector<ObjectRef> &src_pos = object_cast<Vector<ObjectRef> > (input2Value);
    
    lock();              
    Map<int,ObjectRef>::const_iterator it = src.begin();
  
    //ACCUMULATING FRAMES
    while (it != src.end())
    {
        int id = it->first;

	    m_frameAccumulator[id].push_back(src.find(id)->second);
	    //cerr<<"Accumulating : "<<id<<" numFrames : "<<m_frameAccumulator[id].size()<<endl;
	    m_accumulatorLastCount[id] = count;

      	it++;
    }//WHILE ALL SOURCES 

    //SAVING TERMINATED STREAMS IF LONG ENOUGH 
    for (map<int, Vector<ObjectRef> >::iterator ite = m_frameAccumulator.begin(); 
	ite != m_frameAccumulator.end(); ite++)
    {
     
        //CHECK FOR TIMEOUT ON SOURCE
        if (count > m_accumulatorLastCount[ite->first] + 30) 
        {	
            //WRITE DATA IF STREAM IS BIG ENOUGH
            if (m_frameAccumulator[ite->first].size() > 30 ) 
            {

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
                        //cerr<<"SOURCE READY FOR SAVING: "<<ite->first<<endl;     
                }
            }
            else
            {
                //remove source (length too small, discarding)
                m_accumulatorLastCount.erase(ite->first);   
                m_frameAccumulator.erase(ite->first);
                //cerr<<"DISCARDING SOURCE (LENGTH TOO SMALL): "<<ite->first<<endl;
            }
        }
    } //FOREACH ACCUMULATOR

    //output audio
    out[count] = input1Value;

    unlock();
  }
  
  IN_ORDER_NODE_SPEEDUP(SaveSpeex)
};



  

void *savespeex_rec_thread(void *data)
{
	cerr<<"rec_thread : starting"<<endl;
	SaveSpeex *node = reinterpret_cast<SaveSpeex*>(data);

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



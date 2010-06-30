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


#include <unistd.h>
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
#include "binio.h"
#include <QtCore/QMutex>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>

//16KHz downsampling
#define FRAMERATE_DIVISOR 3 

//8KHz downsampling
//#define FRAMERATE_DIVISOR 6 

using namespace FD;

class SaveWavWin32;
DECLARE_NODE(SaveWavWin32)
/*Node
*
* @name SaveWavWin32
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


class SaveWavWin32 : public BufferedNode, public QThread {
    
    friend class savewav_rec_thread;
    
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
    QMutex m_recMutex;
    list<int> m_readyQueue;
    QSemaphore m_semaphore;
    
    FILE* m_pFile;
    typedef struct
    {
        char szRIFF[4];       //0
        long lRIFFSize;       //4
        char szWave[4];       //8    
        char szFmt[4];        //12
        long lFmtSize;        //16
        short wFormatTag;     //20
        short nChannels;      //22
        long nSamplesPerSec;  //27
        long nAvgBytesPerSec; //2/
        short nBlockAlign;    //32
        short wBitsPerSample; //34
        char szData[4];       //36
        long lDataSize;       //40
    } WAVEHEADER;
    
public:
    SaveWavWin32(string nodeName, ParameterSet params)
    : BufferedNode(nodeName, params), m_saveToFile(false), m_splitStrength(0)
    {
        sourcesID = addInput("SOURCES");
        srcPosID = addInput("SRC_POS");
        outputID = addOutput("OUTPUT");
        m_baseName = object_cast<String> (parameters.get("BASENAME"));        
        m_threadRunning = true;
        start();        
    }
    
    ~SaveWavWin32()
    {
        m_threadRunning = false;
        //pseudosem_post(&m_semaphore);
        unlock();
        //pthread_join(m_thread,NULL); 
    }
    
    void lock() 
    {
        m_recMutex.lock();
    }
    
    void unlock()
    {
        m_recMutex.unlock();
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
        cerr<<"writeWavHeader : datasize =  "<<datasize<<endl;
        constructWavHeader(out,datasize);    
    }
    
    void writeWav(map<int, Vector<ObjectRef> > &accum, int id) {
        
		stringstream fname;
		fname << m_baseName << id << ".wav";

        cerr<<"writeWav : filename =  "<<fname.str().c_str()<<endl;
		
        //CREATE STREAM, WAIT ON CLOSE, OWNER
        ofstream my_stream(fname.str().c_str(),ios::binary);
        
        writeWavHeader(my_stream,accum,id);
        writeFrames(my_stream,accum,id);
        
        my_stream.close();
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
                        m_semaphore.release(1);
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
    
    void run()
    {
        cerr<<"rec_thread : starting"<<endl;
        
        map<int, Vector<ObjectRef> > frameAccumulator;
        
        while (m_threadRunning) {
            
            //waiting for something to do
            m_semaphore.acquire(1);
            
            lock();
            
            //copy data structure
            //numFrames = numFrames;
            //frameAccumulator = m_frameAccumulator;
            
            if (!m_readyQueue.empty() && m_threadRunning )
            {
                
                int id = m_readyQueue.front();
                //cerr<<"rec_thread : processing source id : "<<id<<endl;
                m_readyQueue.pop_front();
                
                
                //copy source content
                frameAccumulator[id] = m_frameAccumulator[id];
                
                //clean up memory for original data structures				
                m_accumulatorLastCount.erase(id);	
                m_frameAccumulator.erase(id);
                
                //cerr<<"rec_thread : copying data done"<<endl;
                unlock();
                //do something			
                //will now work on local thread data
                writeWav(frameAccumulator,id);
                
                //cerr<<"rec_thread : source completed id  "<<id<<endl;
                
                //clean up local memory
                frameAccumulator.erase(id);
            }
            else 
            {
                unlock();
            }
        }
        cerr<<"rec_thread : done!"<<endl;
    }   
    
    
    IN_ORDER_NODE_SPEEDUP(SaveWavWin32)
};




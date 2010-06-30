/* Copyright (C) 2008 Mathieu Labbe

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

// TODO : clean headers that we don't need...
#include <unistd.h>
#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include "SourceInfo.h"
#include "sphere.h"
#include "Map.h"
#include <map>
#include "binio.h"

// For socket/stream use
#include <QTcpSocket>
#include <QIODevice>
#include <QString>
#include <QThread>
#include <QSemaphore>


//16KHz downsampling
#define FRAMERATE_DIVISOR 3 

//8KHz downsampling
//#define FRAMERATE_DIVISOR 6 

using namespace FD;

class StreamWriter : public QThread {
private:
    std::vector< Vector<float> > m_framesToWrite;
    std::vector<int> m_framesID;
    map<int, int> m_start_pos;

    // Socket stuff
    static const int TIMEOUT = 5000;
    QTcpSocket* m_socket;
    bool m_initialized;
	string m_host; 
	int m_port;

    // Thread stuff
    static const int THREAD_IDLE     = 0;
    static const int THREAD_RUNNING  = 1;
    static const int THREAD_STOPPING = 2;

    QSemaphore* m_frameMutex;
    QSemaphore* m_frameReadySem;
    QSemaphore* m_waitUntilFinishedSem;
	QSemaphore* m_initSem;

    int m_threadState;
    
public:
    StreamWriter(string host, int port) : m_initialized(false), m_threadState(THREAD_IDLE), m_socket(0), m_host(host), m_port(port)
    {
        m_frameReadySem = new QSemaphore(0);
        m_frameMutex = new QSemaphore(1);
        m_waitUntilFinishedSem = new QSemaphore(0);
		m_initSem = new QSemaphore(0);
    }

    ~StreamWriter()
    {
        if(m_frameReadySem) {
            delete m_frameReadySem;
			m_frameReadySem = 0;
        }
        if(m_frameMutex) {
            delete m_frameMutex;
			m_frameMutex = 0;
        }
        if(m_socket) {
            delete m_socket;
			m_socket = 0;
        }
		if(m_initSem) {
            delete m_initSem;
			m_initSem = 0;
        }
    }

	bool isInitialized()
	{
		m_initSem->acquire(); // Wait for thread initialization
		m_initSem->release(); // Release it for future call
		return m_initialized;
	}

    void addFrame(const Vector<float> &inFrame, const int id)
    {
        m_frameMutex->acquire();
        {
            m_framesToWrite.push_back(inFrame);
            m_framesID.push_back(id);
        }
        m_frameMutex->release();

        m_frameReadySem->release(); // Frame added !
    }

    void killSafe()
    {
        m_threadState = THREAD_STOPPING;
        m_frameReadySem->release();
		
        m_waitUntilFinishedSem->acquire();
    }

protected:  
    virtual void run()
    {
		if(init()) {
	        std::vector< Vector<float> > framesBuffer;
	        std::vector<int> framesIDBuffer;
	        m_threadState = THREAD_RUNNING;
	        while(m_threadState == THREAD_RUNNING) {
	            m_frameReadySem->acquire(); // Wait for a frame
	            
				// If we are still running
				if(m_threadState == THREAD_RUNNING) {
		            m_frameMutex->acquire();
		            {
		                framesBuffer = m_framesToWrite;
		                framesIDBuffer = m_framesID;
		                m_framesToWrite.clear();
		                m_framesID.clear();
		            }
		            m_frameMutex->release();

		            for(int i=0; i<framesBuffer.size() && i<framesIDBuffer.size(); i++) {
		                writeFrame(framesBuffer[i], framesIDBuffer[i]);
		            }

		            framesBuffer.clear();  
		            framesIDBuffer.clear();
				}
	        }
		}
		else {
			std::cerr << "SaveAudioStream : StreamWriter -> Initialization failed..." << std::endl;
		}

        m_waitUntilFinishedSem->release();
    }

private:
	bool init()
    {
        if(m_initialized) {
            return true;
        }
        m_socket = new QTcpSocket();
        m_socket->connectToHost(QString(m_host.c_str()), m_port);

        if (!m_socket->waitForConnected(TIMEOUT)) {
            delete m_socket;
            m_socket = 0;
            m_initialized = false;//throw new NodeException(this, string("Can't connection with host: ") + host, __FILE__, __LINE__);
        }
		else {
			m_initialized = true;
		}
		m_initSem->release(); // Initialization finished
        return m_initialized;
    }
	
    void writeFrame(const Vector<float> &frame, const int id)
    {
        short buff[frame.size()];
        int i, k;
        QIODevice* out = (QIODevice*)m_socket;

        //The 3 is the decimation for 16kHz
        //6 is for 8kHz
        for (k=0,i=m_start_pos[id];i<frame.size();i+=FRAMERATE_DIVISOR)
        {
            float tmp = 3 * frame[i];
            if (tmp > 32767)
               tmp = 32767;
            else if (tmp < -32767)
               tmp = -32767;
            buff[k++] = short(.5+floor(tmp)); 
        }

        m_start_pos[id] = i-frame.size();
        int nb_samples = k;
    
        int result = out->write((const char *)buff, sizeof(short)*nb_samples);
        
        if(!out->waitForBytesWritten(10))
        {            
			cerr << "SaveAudioStreamTCP:StreamWriter : Error to write bytes"<< endl;
        }
    }
};

class SaveAudioStreamTCP;
DECLARE_NODE(SaveAudioStreamTCP)
/*Node
*
* @name SaveAudioStreamTCP
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
* @output_name OUTPUT_SOURCES
* @output_type Map<int,ObjectRef>
* @output_description Same as input SOURCES
*
* @output_name OUTPUT_SRC_POS
* @output_type Vector<ObjectRef>
* @output_description Same as input SRC_POS but with info on server/port of the stream receiver.
*
* @parameter_name HOST0
* @parameter_type string
* @parameter_value localhost
* @parameter_description Hostname of the server receiving the audio stream 0
*
* @parameter_name PORT0
* @parameter_type int
* @parameter_value 7000
* @parameter_description Port number of the server receiving the audio stream 0
*
* @parameter_name HOST1
* @parameter_type string
* @parameter_value localhost
* @parameter_description Hostname of the server receiving the audio stream 1
*
* @parameter_name PORT1
* @parameter_type int
* @parameter_value 7001
* @parameter_description Port number of the server receiving the audio stream 1
*
* @parameter_name HOST2
* @parameter_type string
* @parameter_value localhost
* @parameter_description Hostname of the server receiving the audio stream 2
*
* @parameter_name PORT2
* @parameter_type int
* @parameter_value 7002
* @parameter_description Port number of the server receiving the audio stream 2
*
* @parameter_name HOST3
* @parameter_type string
* @parameter_value localhost
* @parameter_description Hostname of the server receiving the audio stream 3
*
* @parameter_name PORT3
* @parameter_type int
* @parameter_value 7003
* @parameter_description Port number of the server receiving the audio stream 3
*
END*/


class SaveAudioStreamTCP : public BufferedNode {
    

private:
    int sourcesID;
    int srcPosID;
    int outputSourcesID;
    int outputSrcPosID;
 
    static const int DEFAULT_STREAM_NUMBER = 4; // It is not supported over 4

    string m_hosts[DEFAULT_STREAM_NUMBER];
    int m_ports[DEFAULT_STREAM_NUMBER];

    StreamWriter* m_server[DEFAULT_STREAM_NUMBER]; // numServer -> stream
    map<int, int> m_sourceMap;                  // id -> numServer
  
    map<int, int> m_accumulatorLastCount;       // id -> count

    std::vector<int> m_ignoreServerList;
    
public:
    SaveAudioStreamTCP(string nodeName, ParameterSet params)
    : BufferedNode(nodeName, params)
    {
        sourcesID = addInput("SOURCES");
        srcPosID = addInput("SRC_POS");

        outputSourcesID = addOutput("OUTPUT_SOURCES");
        outputSrcPosID = addOutput("OUTPUT_SRC_POS");
        
        m_hosts[0] = object_cast<String> (parameters.get("HOST0"));
        m_hosts[1] = object_cast<String> (parameters.get("HOST1"));
        m_hosts[2] = object_cast<String> (parameters.get("HOST2"));
        m_hosts[3] = object_cast<String> (parameters.get("HOST3"));
        
        m_ports[0] = dereference_cast<int> (parameters.get("PORT0"));
        m_ports[1] = dereference_cast<int> (parameters.get("PORT1"));
        m_ports[2] = dereference_cast<int> (parameters.get("PORT2"));
        m_ports[3] = dereference_cast<int> (parameters.get("PORT3"));

        for(int i=0; i<DEFAULT_STREAM_NUMBER; i++) {
            m_server[i] = 0;
        }  
    }
    
    ~SaveAudioStreamTCP()
    {
        for(int i=0; i<DEFAULT_STREAM_NUMBER; i++) {
            if(m_server[i]) {
                delete m_server[i];
                m_server[i] = 0;
            }
        }
    }
    
    void calculate(int output_id, int count, Buffer &out)
    {
        
        //=================================
        //Get inputs
        //=================================
        ObjectRef input1Value = getInput(sourcesID, count);
        const Map<int,ObjectRef> &src = object_cast <Map<int,ObjectRef> > (input1Value);

        ObjectRef input2Value = getInput(srcPosID, count);
        const Vector<ObjectRef> &src_pos = object_cast<Vector<ObjectRef> > (input2Value);
              
        //=================================
        //Calculate output : OUTPUT_SOURCES
        //=================================
        Map<int,ObjectRef>::const_iterator it = src.begin();
        
        //SEND FRAMES
        while (it != src.end())
        {
            int id = it->first;   

            StreamWriter* outStream = getStream(id); // get stream, if no one exist for this id, one will be created
            if(outStream) {
                outStream->addFrame(object_cast<Vector<float> >(it->second), id);
            }
            else {
                //cerr << "SaveAudioStreamTCP : stream is null, id=" << id << endl;
            }

            m_accumulatorLastCount[id] = count;
            
            it++;
        }//WHILE ALL SOURCES 


		//REMOVE ALL PENDING STREAM
		map<int, int >::iterator ite = m_sourceMap.begin();
        while(ite != m_sourceMap.end()) {
            if (count > m_accumulatorLastCount[ite->first] + 30) {
                removeStream(ite->first);
                m_accumulatorLastCount.erase(ite->first);
				ite = m_sourceMap.begin();
            }
			else {
				ite++;
			}
        }

        //=================================
        //Calculate output : OUTPUT_SRC_POS
        //=================================
        Vector<ObjectRef> &srcInfos = *Vector<ObjectRef>::alloc(0);
        for(size_t i=0; i<src_pos.size(); i++) {
            SourceInfo &s = *new SourceInfo;
            s = object_cast<SourceInfo>(src_pos[i]);
            int srcId = s.source_id;
            if(m_sourceMap.find(srcId) != m_sourceMap.end()) {
                s.host = m_hosts[m_sourceMap[srcId]];
                s.port = m_ports[m_sourceMap[srcId]];
            }
            else {
                s.host = "null";
                s.port = 0;
            }
            s.useAudioStream = true;
            srcInfos.push_back(ObjectRef(&s));
        }

        //=================================
        //Set outputs
        //=================================
        (*(outputs[outputSourcesID].buffer))[count]=input1Value; // Same as input
        (*(outputs[outputSrcPosID].buffer))[count]=&srcInfos; // modified input src_pos with server/port info
    }

private:
    StreamWriter* getStream(int sourceId)
    {
        // find stream mapped to this source
        // or create a new one if enough server exist 
        StreamWriter* out = 0;
        map<int, int >::iterator it = m_sourceMap.find(sourceId);
        if(it != m_sourceMap.end()) {
            out = m_server[it->second];
        }
        else {
            bool gotConnection = false;
            int serverNum = -1;
            while(serverNum == -1)
            {
                serverNum = getWaitingServer(m_ignoreServerList);
                if(serverNum == -1) {
                    cerr << "SaveAudioStreamTCP : All servers are busy." << endl;
                    break;
                }
                else { 
                    out = createStream(m_hosts[serverNum], m_ports[serverNum]);
                    
                    if(out) {
                        cout << "SaveAudioStreamTCP : Connection successful! Mapped sourceId=" 
                             << sourceId << " with server:" << m_hosts[serverNum] << ":" << m_ports[serverNum] << endl;
                        m_server[serverNum] = out;
                        m_sourceMap[sourceId] = serverNum;
                    }
                    else {
                        cerr << "SaveAudioStreamTCP : Can't create a stream to server : " << m_hosts[serverNum] << ":" << m_ports[serverNum] << ". Trying to find another server..." << endl;  
                        m_ignoreServerList.push_back(serverNum); // MAKE SERVER INVALID FOREVER (until the program is closed)
                        serverNum = -1;           
                    }
                }
            }
        }
        return out;   
    }

    int getWaitingServer(std::vector<int> &ignoreServerList)
    {
        bool ignore;
        int i;
        size_t j;
        for(i=0; i<DEFAULT_STREAM_NUMBER; i++) {
            ignore = false;
            for(j=0; j<ignoreServerList.size(); j++) {
                if(ignoreServerList[j] == i) {
                    ignore = true;
                }
            }
            if(m_server[i] == 0 && !ignore) {
                return i;
            }
        }
        return -1;            
    }

    StreamWriter* createStream(string host, int port)
    {
        StreamWriter* stream = new StreamWriter(host, port);
		stream->start();
        if(!stream->isInitialized()) {
			stream->killSafe();
            delete stream;
            stream = 0;
        }      
        return stream;
    }

    void removeStream(int sourceId)
    {	
        StreamWriter* stream = m_server[m_sourceMap[sourceId]];
        if(stream) {   
            stream->killSafe(); 
            delete stream;
            m_server[m_sourceMap[sourceId]] = 0;
        }
        m_sourceMap.erase(sourceId);
    }
    
public:
    IN_ORDER_NODE_SPEEDUP(SaveAudioStreamTCP)
};




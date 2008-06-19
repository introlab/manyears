/* Copyright (C) 2007 Mathieu Labbe

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
#include <QDataStream>
#include <QString>


//16KHz downsampling
#define FRAMERATE_DIVISOR 3 

//8KHz downsampling
//#define FRAMERATE_DIVISOR 6 

using namespace FD;

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
    
    string m_hosts[4];
    int m_ports[4];
    QDataStream* m_server[4];       //numServer -> stream
    map<int, int> m_sourceMap;  // id -> numServer

    map<int, int> start_pos;    
    map<int, int> m_accumulatorLastCount;

    static const int TIMEOUT = 5000;
    
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

        for(int i=0; i<4; i++) {
            m_server[i] = 0;
        }  
    }
    
    ~SaveAudioStreamTCP()
    {
        for(int i=0; i<4; i++) {
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
            
            QDataStream* outStream = getStream(id); // get stream, if no one exist for this id, one will be created
            if(outStream) {
                writeFrames(outStream, it->second, id);
            }
            else {
                //cerr << "SaveAudioStreamTCP : stream is null, id=" << id << endl;
            }

            m_accumulatorLastCount[id] = count;
            
            it++;
        }//WHILE ALL SOURCES 


        //REMOVE ALL PENDING STREAM
        for(map<int, int >::iterator ite = m_sourceMap.begin(); ite != m_sourceMap.end(); ite++) {
            if (count > m_accumulatorLastCount[ite->first] + 30) {
                removeStream(ite->first);
                m_accumulatorLastCount.erase(ite->first); 
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
        (*(outputs[outputSrcPosID].buffer))[count]=&srcInfos; // modified input src_pos wqith server/port info
    }

private:
    void writeFrames(QDataStream *out, const ObjectRef &inFrame, int id)
    {
        Vector<float> &frame = object_cast<Vector<float> >(inFrame);
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

        out->writeRawData((const char *)buff, sizeof(short)*nb_samples);          
    }
    
    QDataStream* getStream(int sourceId)
    {
        // find stream mapped to this source
        // or create a new one if enough exist 
        QDataStream* out = 0;
        map<int, int >::iterator it = m_sourceMap.find(sourceId);
        if(it != m_sourceMap.end()) {
            out = m_server[it->second];
        }
        else {
            bool gotConnection = false;
            int serverNum = -1;
            std::vector<int> ignoreServerList;
            while(serverNum == -1)
            {
                serverNum = getWaitingServer(ignoreServerList);
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
                        ignoreServerList.push_back(serverNum); // MAKE SERVER INVALID FOR THIS COUNT
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
        for(i=0; i<4; i++) {
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

    QDataStream* createStream(string host, int port)
    {
        QTcpSocket* socket = new QTcpSocket();
        socket->connectToHost(QString(host.c_str()), port);

        if (!socket->waitForConnected(TIMEOUT)) {
            delete socket;
            return 0;//throw new NodeException(this, string("Can't connection with host: ") + host, __FILE__, __LINE__);
        }

        QDataStream* stream = new QDataStream(socket);
        stream->setVersion(QDataStream::Qt_4_0);

        return stream;
    }

    void removeStream(int sourceId)
    {
        QDataStream* stream = m_server[m_sourceMap[sourceId]];
        if(stream) {    
            QIODevice* socket = stream->device();
            if(socket) {
                socket->close();
                delete socket;
                stream->setDevice(0);
            }  
            delete stream;
            m_server[m_sourceMap[sourceId]] = 0;
        }
        m_sourceMap.erase(sourceId);
    }
    
public:
    IN_ORDER_NODE_SPEEDUP(SaveAudioStreamTCP)
};




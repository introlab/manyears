/* Copyright (C) 2002-2008 Jean-Marc Valin, Julien D'Ascenzio

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

#include "BufferedNode.h"
#include "operators.h"
#include "RtAudio.h"
#include "Vector.h"
#include <QtCore/QMutex>
#include <QtCore/QSemaphore>
#include <QtCore/QList>
#include "Buffer.h"
#include <fstream>
#include <string.h>

using namespace std;

namespace FD {
    
    class openStreamManyEars;
    
    //ofstream rawOutputTest("rawoutput.raw");
    
    QMutex buffersList_openStreamManyEars;
    QSemaphore newSamplesSemaphore_openStreamManyEars;
    
    
    
    int inOpenStream( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *data )
    {        	
        if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
        
        short* in = (short*) inputBuffer;
        
        Vector<float>* buffer = Vector<float>::alloc(nBufferFrames * 8);
		
        QList<Vector<float>*>* buffersList = (QList<Vector<float>*>*) data;   
        for(int i=0; i<nBufferFrames*8; i++)
        {
            (*buffer)[i]= (float)(in[i]);            
        }
		
        buffersList_openStreamManyEars.lock();
        buffersList->append( buffer );        
        buffersList_openStreamManyEars.unlock();
        
        newSamplesSemaphore_openStreamManyEars.release(1);
        return 0;
    }    
    
    DECLARE_NODE(openStreamManyEars)
    
    /*Node
    *
    * @name openStreamManyEars
    * @category ManyEars
    * @description open a stream audio
    *
    * @parameter_name DEVICE
    * @parameter_type string
    * @parameter_value M-Audio Delta
    * @parameter_description device name
    *
    * @parameter_name SAMPLERATE
    * @parameter_type int
    * @parameter_value 48000
    * @parameter_description sampleRate
    *
    * @parameter_name LENGTH
    * @parameter_type int
    * @parameter_value 8192
    * @parameter_description number of samples
    *
    * @parameter_name ADVANCE
    * @parameter_type int
    * @parameter_value 4096
    * @parameter_description number of samples
    *
    * @parameter_name NCHANNELS
    * @parameter_type int
    * @parameter_value 8
    * @parameter_description number of channels
    *
    * @parameter_name FIRSTCHANNEL
    * @parameter_type int
    * @parameter_value 0
    * @parameter_description first Channel
    *
    * @output_name OUTPUT
    * @output_type Vector<float>
    * @output_description the audio data
    *
    END*/
    
    
    class openStreamManyEars : public BufferedNode 
    {
        
        //Output ID
        int m_outputID;
        
        //Parameters
        int m_sampleRate;
        int m_length;
        int m_advance;
        int m_nChannels;
        int m_firstChannel;
        string m_device;
        
        RtAudio m_adac;
        
        //map of the line_type
        map<string,int> m_formatMap;
        
        //variable
        QList<Vector<float>*> m_buffersList;
        
    public:
        ~openStreamManyEars()
        {
            m_adac.stopStream();
            if ( m_adac.isStreamOpen() ) m_adac.closeStream();
        }
        
        openStreamManyEars(string nodeName, ParameterSet params)
        : BufferedNode(nodeName, params)
        {
            m_outputID = addOutput("OUTPUT");
            //Initialize parameters    
            m_device = object_cast<String>(parameters.get("DEVICE"));
            m_sampleRate = dereference_cast<int>(parameters.get("SAMPLERATE"));
            m_length = dereference_cast<int>(parameters.get("LENGTH"));
            m_advance = dereference_cast<int>(parameters.get("ADVANCE"));
            m_nChannels = dereference_cast<int>(parameters.get("NCHANNELS"));
            m_firstChannel = dereference_cast<int>(parameters.get("FIRSTCHANNEL"));


            unsigned int devices = m_adac.getDeviceCount();
            if ( devices < 1 )
                throw new GeneralException("No audio devices found",__FILE__,__LINE__);

            RtAudio::DeviceInfo info;
            cout << "Audio device count = " << devices << endl;
            for (unsigned int i=0; i<devices; i++) 
            {
                info = m_adac.getDeviceInfo(i); 
                if(info.probed == false) {
                    cout << "Audio device " << i << " is busy or unavailable..." << endl;
                }
                else {
                    cout << "Audio device found = " << info.name << endl;  
                }
            }

            //RtAudio::DeviceInfo info;
            unsigned int deviceID;
            unsigned int i;
            for (i=0; i<devices; i++) 
            {
                info = m_adac.getDeviceInfo(i);
                
                //GET THE FIRST DEVICE WITH ENOUGH AUDIO INPUTS (8)
                if (m_device == "any")
                {
                	if (info.inputChannels >= m_nChannels && info.probed)
                	{
                		m_device = info.name;
                        cout << "Using audio device = " << info.name << endl;
                		deviceID = i;
                		break;
                	}
                	
                }
                else
                {	
                	size_t found = info.name.find(m_device.c_str());
                	if(info.name.find(m_device.c_str()) != string::npos )
                	{
                		deviceID = i;
                		break;                        
                	}
                }
            }
            if(i==devices)
                throw new GeneralException("device: " + m_device + "not found" ,__FILE__,__LINE__);
            
            // Set the input parameters.
            RtAudio::StreamParameters iParams;
            
            iParams.deviceId = deviceID;
            iParams.nChannels = m_nChannels;
            iParams.firstChannel = m_firstChannel;

            
            if(m_advance > m_length)
            {
                throw new GeneralException("Error : ADVANCE must be inferior to LENGTH",__FILE__,__LINE__);                
            }            
            if(m_length%m_length)
            {
                throw new GeneralException("Error : LENGTH % ADVANCE must be equal to zero",__FILE__,__LINE__);                
            }
            if(m_advance%m_nChannels)
            {
                throw new GeneralException("Error : ADVANCE % NCHANNELS must be equal to zero",__FILE__,__LINE__);                
            }
            
            unsigned int nSamples = (int)(m_advance / m_nChannels);
 
            try
            {
                m_adac.openStream( NULL, &iParams, RTAUDIO_SINT16, m_sampleRate, &nSamples, &inOpenStream, &m_buffersList);                
                cout<<"openStreamManyEars -> openStreamManyEars -> OK"<<endl;
            }
            catch ( RtError& e )
            {
                throw new GeneralException(e.getMessage(),__FILE__,__LINE__);
            }
            
            try 
            {
                m_adac.startStream();
                cout<<"openStreamManyEars -> startStream -> OK"<<endl;
            }
            catch ( RtError& e )
            {
                throw new GeneralException(e.getMessage(),__FILE__,__LINE__);
                
            }
            
            
        }
        
        virtual void initialize()
        {
            outputs[m_outputID].lookAhead += 1;           
            this->BufferedNode::initialize();
        }
        
        void calculate(int output_id, int count, Buffer &out)
        {
            Vector<float> *output = Vector<float>::alloc(m_length);            
            
            int outputSize = 0;
            
            if (count == 0)
            {                                
                
                while( outputSize < m_length)
                {
                    newSamplesSemaphore_openStreamManyEars.acquire(1);
                    buffersList_openStreamManyEars.lock();
                    Vector<float> *buffer = m_buffersList.takeFirst();
                    buffersList_openStreamManyEars.unlock();
                    memcpy(&((*output)[outputSize]),&((*buffer)[0]),buffer->size() * sizeof(float));
					outputSize += buffer->size();
					buffer->destroy();
                }                
                
            }
            else
            {
                Vector<float> *previous = &(object_cast<Vector<float> > ((*(outputs[m_outputID].buffer))[count-1]));
				
                memcpy(&((*output)[outputSize]),&((*previous)[m_advance]),(m_length - m_advance) * sizeof(float));
                outputSize += m_length - m_advance;
				
                while( outputSize < m_length)
                {
                    newSamplesSemaphore_openStreamManyEars.acquire(1);
                    buffersList_openStreamManyEars.lock();
                    Vector<float> *buffer = m_buffersList.takeFirst();
                    buffersList_openStreamManyEars.unlock();
                    memcpy(&((*output)[outputSize]),&((*buffer)[0]),buffer->size() * sizeof(float));        
					outputSize += buffer->size();
					buffer->destroy();
                }
            }
            
            (*(outputs[m_outputID].buffer))[count] = ObjectRef(output);
            
        }
        
        NO_ORDER_NODE_SPEEDUP(openStreamManyEars)
    };
}//namespace FD

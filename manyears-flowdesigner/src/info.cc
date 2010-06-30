#include "BufferedNode.h"
#include "operators.h"
#include "RtAudio.h"



using namespace std;

namespace FD {
    
    class info;
    
    DECLARE_NODE(info)
    /*Node
    *
    * @name info
    * @category ManyEars
    * @description Print the device available
    *
    * @output_name OUTPUT
    * @output_description always true
    * @output_type bool
    *
    END*/
    
    
    class info : public BufferedNode 
    {
        //Input ID            
        //int m_inputID;
        //Output ID
        int m_outputID;
        
        public:
        info(string nodeName, ParameterSet params)
        : BufferedNode(nodeName, params)
        {
            m_outputID = addOutput("OUTPUT");
        }      
        
        void calculate(int output_id, int count, Buffer &out)
        {
            //Handle            
            std::map<int, std::string> apiMap;
            apiMap[RtAudio::MACOSX_CORE] = "OSX Core Audio";
            apiMap[RtAudio::WINDOWS_ASIO] = "Windows ASIO";
            apiMap[RtAudio::WINDOWS_DS] = "Windows Direct Sound";
            apiMap[RtAudio::UNIX_JACK] = "Jack Client";
            apiMap[RtAudio::LINUX_ALSA] = "Linux ALSA";
            apiMap[RtAudio::LINUX_OSS] = "Linux OSS";
            apiMap[RtAudio::RTAUDIO_DUMMY] = "RtAudio Dummy";
            
            std::vector< RtAudio::Api > apis;
            RtAudio :: getCompiledApi( apis );
            
            std::cout << "\nCompiled APIs:\n";
            for ( unsigned int i=0; i<apis.size(); i++ )
                std::cout << "  " << apiMap[ apis[i] ] << std::endl;
            
            RtAudio audio;
            RtAudio::DeviceInfo info;
            
            std::cout << "\nCurrent API: " << apiMap[ audio.getCurrentApi() ] << std::endl;
            
            unsigned int devices = audio.getDeviceCount();
            std::cout << "\nFound " << devices << " device(s) ...\n";
            
            for (unsigned int i=0; i<devices; i++) {
                cout<<endl<<"####### device number: "<<i<<endl;
                info = audio.getDeviceInfo(i);
                
                std::cout << "\nDevice Name = " << info.name << '\n';
                if ( info.probed == false )
                    std::cout << "Probe Status = UNsuccessful\n";
                else {
                    std::cout << "Probe Status = Successful\n";
                    std::cout << "Output Channels = " << info.outputChannels << '\n';
                    std::cout << "Input Channels = " << info.inputChannels << '\n';
                    std::cout << "Duplex Channels = " << info.duplexChannels << '\n';
                    if ( info.isDefaultOutput ) std::cout << "This is the default output device.\n";
                    else std::cout << "This is NOT the default output device.\n";
                    if ( info.isDefaultInput ) std::cout << "This is the default input device.\n";
                    else std::cout << "This is NOT the default input device.\n";
                    if ( info.nativeFormats == 0 )
                        std::cout << "No natively supported data formats(?)!";
                    else {
                        std::cout << "Natively supported data formats:\n";
                        if ( info.nativeFormats & RTAUDIO_SINT8 )
                            std::cout << "  8-bit int\n";
                        if ( info.nativeFormats & RTAUDIO_SINT16 )
                            std::cout << "  16-bit int\n";
                        if ( info.nativeFormats & RTAUDIO_SINT24 )
                            std::cout << "  24-bit int\n";
                        if ( info.nativeFormats & RTAUDIO_SINT32 )
                            std::cout << "  32-bit int\n";
                        if ( info.nativeFormats & RTAUDIO_FLOAT32 )
                            std::cout << "  32-bit float\n";
                        if ( info.nativeFormats & RTAUDIO_FLOAT64 )
                            std::cout << "  64-bit float\n";
                    }
                    if ( info.sampleRates.size() < 1 )
                        std::cout << "No supported sample rates found!";
                    else {
                        std::cout << "Supported sample rates = ";
                        for (unsigned int j=0; j<info.sampleRates.size(); j++)
                            std::cout << info.sampleRates[j] << " ";
                    }
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
            
            
            (*(outputs[m_outputID].buffer))[count] = ObjectRef(Bool::alloc( true ));
            
        }
        
        NO_ORDER_NODE_SPEEDUP(info)
    };
}//namespace FD

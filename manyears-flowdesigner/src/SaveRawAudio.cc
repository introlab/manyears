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

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include <sstream>
#include <fstream>
#include <sstream>
#include "binio.h"
#include <string>


namespace FD
{

	using namespace std;

	class SaveRawAudio;
	DECLARE_NODE(SaveRawAudio)
	
	/*Node
	*
	* @name SaveRawAudio
	* @category ManyEars
	* @description Saves all sources to a different file
	*
	* @input_name AUDIO
	* @input_type Vector<float>
	* @input_description Audio data
	*
	* @input_name FILENAME
	* @input_type String
	* @input_description Save file name.
	*
	* @output_name OUTPUT
	* @output_type Vector<float>
	* @output_description Same as input
	*
	* @parameter_name NB_CHANNELS
	* @parameter_type int
	* @parameter_value 8
	* @parameter_description The number of input channels
	*
	END*/

	class SaveRawAudio : public BufferedNode 
	{
	    protected:  
	  
		ofstream *m_outputStream;
	  	
		//inputs
		int m_audioID;
		int m_filenameID;
		
		//outputs
		int m_outputID;
	  
	    public:
	    
		SaveRawAudio(string nodeName, ParameterSet params)
			: BufferedNode(nodeName, params),m_outputStream(NULL)
	    {
			//inputs
			m_audioID = addInput("AUDIO");			
			m_filenameID = addInput("FILENAME");
			
			//outputs
			m_outputID = addOutput("OUTPUT");
			
	    }
	   
	    
	    void calculate(int output_id, int count, Buffer &out)
	    {
			try
			{
				//Read Inputs
				RCPtr<String> filename = getInput(m_filenameID,count);			
				RCPtr<Vector<float> > audio = getInput(m_audioID,count);
				
				//create stream if required
				if (filename->size() > 0 && m_outputStream == NULL)
				{
					m_outputStream = new ofstream(filename->c_str(),ios::binary);
				}
				
				if (m_outputStream)
				{
					for (unsigned int i = 0; i < audio->size(); i++)
					{
						short data = (short) (*(audio))[i];
						m_outputStream->write((char*) &data,sizeof(short));					
					}
				}
				
				//output = input
				out[count] = audio;				
			}
			catch (BaseException *e)
			{
				throw e->add(new GeneralException("Cannot read input",__FILE__,__LINE__));			
			}
	    }
	    	    
	    IN_ORDER_NODE_SPEEDUP(SaveRawAudio)
	};
}//namespace FD

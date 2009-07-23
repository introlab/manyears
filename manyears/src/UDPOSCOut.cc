/* Copyright (C) 2008 Dominic LŽtourneau

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
#include <QUdpSocket>
#include "SourceInfo.h"
#include <string>
#include <sstream>
#include "binio.h"

using namespace FD;
namespace FD {

	using namespace std;

	class UDPOSCOut;
	
	DECLARE_NODE(UDPOSCOut)
	/*Node
	 *
	 * @name UDPOSCOut
	 * @category ManyEars
	 * @description Convert source information to Open Sound Control
	 *
	 * @input_name INPUT
	 * @input_type Vector<ObjectRef>
	 * @input_description Output vector (after gain)
	 *
	 * @output_name OUTPUT
	 * @output_type Vector<Stringt>
	 * @output_description OSC Output strings
	 *
	 * @parameter_name HOST
	 * @parameter_type string
	 * @parameter_value localhost
	 * @parameter_description The host where to connect
	 * 
	 * @parameter_name PORT
	 * @parameter_type int
	 * @parameter_value 5678
	 * @parameter_description the port number
	END*/

	class UDPOSCOut : public BufferedNode
	{
		 int inputID;
		 int outputID;
		 string hostname;
		 int portnumber;

		 QUdpSocket socket;
		 
		public:
			UDPOSCOut(std::string nodeName, ParameterSet params)
		   : BufferedNode(nodeName, params)
		   {
		      inputID = addInput("INPUT");
		      outputID = addOutput("OUTPUT");
		      
		      hostname = object_cast<String>(parameters.get("HOST"));
		      portnumber = dereference_cast<int>(parameters.get("PORT"));
		      
		      //Initialize socket
		      //socket.bind(QHostAddress::Any,portnumber);
		      
		   }

		   void calculate(int output_id, int count, Buffer &out)
		   {
			   //GET INPUT
			   RCPtr<Vector<ObjectRef> > inputVect = getInput(inputID,count);
			   
			   Vector<ObjectRef> *outputs = new Vector<ObjectRef>;
			   
			   for (unsigned int i = 0; i<inputVect->size(); i++)
			   {
				   RCPtr<SourceInfo> info = (*inputVect)[i];
				   
				   int id = info->source_id;
				   float x = info->x[0];
				   float y = info->x[1];
				   float z = info->x[2];
				   float strength = info->strength;
				   float phi = -180.0 * atan2(info->x[2], sqrt(info->x[0] * info->x[0] + info->x[1] * info->x[1])) / M_PI;;
				   float theta = 180.0 * atan2(info->x[1],info->x[0]) / M_PI;
				   float distance = sqrt(info->x[0] * info->x[0] + info->x[1] * info->x[1] + info->x[2] * info->x[2]);
				 
				   stringstream outputString;
				   
				   char header[12] = {'/','m','a','n','y','e','a','r','s', '\0', '\0','\0'};
				   char tags[12] = {',','i','f','f','f','f','f','f','f','\0','\0','\0'};
				   
				   outputString.write(header,12);
				   outputString.write(tags,12);
				   
				   BinIO::write<int>(outputString,&id,1);
				   BinIO::write<float>(outputString,&x,1);
				   BinIO::write<float>(outputString,&y,1);
				   BinIO::write<float>(outputString,&z,1);
				   BinIO::write<float>(outputString,&strength,1);
				   BinIO::write<float>(outputString,&theta,1);
				   BinIO::write<float>(outputString,&phi,1);
				   BinIO::write<float>(outputString,&distance,1);
				   				   
				   //write to the socket
				   int size = socket.writeDatagram(outputString.str().c_str(),outputString.str().size(),
						   QHostAddress(hostname.c_str()),portnumber);
				   
				   if (size < 0)
				   {
					   cerr << "UdpSocket error send size =" << size << socket.errorString().toStdString() << endl;
				   }

				   outputs->push_back(ObjectRef(new String(outputString.str())));
				   
			   }
			   
			   out[count] = ObjectRef(outputs);
			   
		   }

		   NO_ORDER_NODE_SPEEDUP(UDPOSCOut);   
	};
	

}

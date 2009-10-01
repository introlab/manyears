/***********************************************************************************
** Copyright (C) 2006-2008 Dominic Letourneau (Dominic.Letourneau@USherbrooke.ca). 
** All rights reserved. 
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
***********************************************************************************/
#include "BufferedNode.h"
#include "operators.h"
#include <QtNetwork/QTcpSocket>
#include <sstream>


using namespace std;

namespace FD {
   
   class QtTCPBinarySend;
   
   DECLARE_NODE(QtTCPBinarySend)
   
   /*Node
   *
   * @name QtTCPBinarySend
   * @category FDQtTCP
   * @description Create a tcp socket
   *
   * @input_name OBJECT
   * @input_description Object to serialize
   * @input_type ObjectRef
   *
   * @parameter_name HOSTNAME
   * @parameter_type string
   * @parameter_value 127.0.0.1
   * @parameter_description Hostname
   *
   * @parameter_name PORT
   * @parameter_type int
   * @parameter_value 7000
   * @parameter_description Port
   *
   * @parameter_name WAITFORCONNECTED
   * @parameter_type int
   * @parameter_value 1000
   * @parameter_description How many ms that the node wait for connection
   *
   * @output_name BOOLWRITE
   * @output_description A new object was sent
   * @output_type bool
   *
   END*/
   
   class QtTCPBinarySend : public BufferedNode {
      
      public:
      
	  //Input ID
      int m_objectID;
      
	  //Output ID
      int m_boolWriteID;
	  
      //Parameters
      QString m_hostname;
      
	  quint16 m_port;
	  
      int m_waitForConnected;
      //socket
      QTcpSocket m_tcpClient;      
      
      QtTCPBinarySend(string nodeName, ParameterSet params)
      : BufferedNode(nodeName, params)
      {
         //add inputs
         m_objectID = addInput("OBJECT");
         
         //add outputs
         m_boolWriteID = addOutput("BOOLWRITE");
         
         //Initialize parameters
         m_hostname =  QString( ( object_cast<String>( parameters.get("HOSTNAME") ) ).c_str() );   
         m_port = dereference_cast<int>(parameters.get("PORT"));
         m_waitForConnected = dereference_cast<int>(parameters.get("WAITFORCONNECTED"));
      }  
      
      void calculate(int output_id, int count, Buffer &out)
      {
         ObjectRef inputObject = getInput(m_objectID,count);
         if (m_tcpClient.state() != 3)
         {
            m_tcpClient.connectToHost(m_hostname, m_port, QIODevice::WriteOnly);
			
            if(m_tcpClient.waitForConnected(m_waitForConnected))
            {
               cout << "QtTCPBinarySend -> Connect" << endl;
            } 
            else
            {
               cout << "QtTCPBinarySend -> Error of connection : ";
               cout << m_tcpClient.errorString().toStdString() << endl;
               (*(outputs[m_boolWriteID].buffer))[count] = ObjectRef(Bool::alloc(false));
               return;
            }
         }
         
		 //Serialize data
		stringstream buffer;	
		inputObject->serialize(buffer);
		
		//If you want formatted output
		//inputObject->printOn(buffer);
		
		m_tcpClient.write(buffer.str().c_str(),buffer.str().size());

		 
         //m_tcpClient.write( stringIn->c_str(), stringIn->size() );
         
         if(!m_tcpClient.waitForBytesWritten(10))
         {            
            cout << "QtTCPBinarySend -> Error to write bytes"<< endl;
            (*(outputs[m_boolWriteID].buffer))[count] = ObjectRef(Bool::alloc(false));
            return;
         }
         (*(outputs[m_boolWriteID].buffer))[count] = ObjectRef(Bool::alloc(true));
      }    
   };
}//namespace FD



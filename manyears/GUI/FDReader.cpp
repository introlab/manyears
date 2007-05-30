/* Copyright (C) 2006-2007 Eric Beaudry, Simon Briere, Dominic Letourneau

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

#include "FDReader.h"
#include "BaseException.h"
#include "SourceInfo.h"

#include <iostream>
#include <sstream>

using namespace std;


FDReader::FDReader(std::string hostname, int port)
        :    m_hostname(hostname), m_port(port)
{
    //must be called first
    qInitNetworkProtocols();

    m_socket = new QSocket(this);
    
    //Connect signals
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(dataReady()));

    cerr<<"FDReader::FDReader connecting to host : "<<m_hostname<<" port "<<m_port<<endl;
    m_socket->connectToHost(m_hostname.c_str(),m_port);


}

FDReader::~FDReader()
{
    if  (m_socket)
        delete m_socket;      
}

void FDReader::dataReady()
{

    //std::cerr<<"FDReader::dataReady()"<<std::endl;
    //QByteArray array = m_http->readAll();
    //std::cerr<<"FDReader::dataReady() got bytes :"<<array.size()<<std::endl;
    stringstream inputStream;
    
    unsigned long size = m_socket->bytesAvailable();
    
    char buffer[size];
    
    m_socket->readBlock(buffer,size);   
    
    //cerr<<"FDReader::dataReady got bytes : "<<size<<endl;	

    inputStream.write(buffer,size);

    try {                                      
        FD::RCPtr<FD::Vector<FD::ObjectRef> > data;
        inputStream >> data;         

        //data->printOn(cout); 
	//cout.flush();     
        
        FD::RCPtr<SourceInfo> info;
              
        vector<const SourceInfo*> output;      
        
        for (unsigned int i = 0; i < data->size(); i++)
        {
            info = (*data)[i];         
            output.push_back(info.get());
        }         
	
	
	//cerr<<"FDReader::dataReady emit putData signal with nbsources:"<<output.size()<<endl;
        emit putData(output);      
    }
    catch(FD::BaseException *e)
    {
        e->print(cerr);      
        delete e;         
    }
    catch(...)
    {
        cerr<<"Unknown exception in :"<<__FILE__<<":"<<__LINE__<<endl;
    }      

}



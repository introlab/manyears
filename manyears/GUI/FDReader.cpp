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


FDReader::FDReader(int port, int backlog)
        :    QTcpServer()
{

    setMaxPendingConnections(backlog);
    listen(QHostAddress::Any,port);
    cout<<"FDReader::FDReader(int,int) starting socket server on port "<<port<<endl;
}

FDReader::~FDReader()
{

	//Delete all incoming sockets
	for (unsigned int i = 0; i < m_sockets.size(); i++)
	{
		delete m_sockets[i];
	}  
}

void FDReader::dataReady()
{

    //std::cerr<<"FDReader::dataReady()"<<std::endl;
    //Scan for each socket.
    for (unsigned int s = 0; s < m_sockets.size(); s++)
    {

    //QByteArray array = m_http->readAll();
    //std::cerr<<"FDReader::dataReady() got bytes :"<<array.size()<<std::endl;
    stringstream inputStream;
    
    unsigned long size = m_sockets[s]->bytesAvailable();
    
    if (size <= 0) continue;

    char buffer[size];
    
    m_sockets[s]->read(buffer,size);   
    
    //cerr<<"FDReader::dataReady got bytes : "<<size<<endl;	

    inputStream.write(buffer,size);

    try {                                      
        FD::RCPtr<FD::Vector<FD::ObjectRef> > data;
        inputStream >> data;
        //Will send signal putData
        //Object will self destruct when unused
        emit putData(data);      
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
 	     
}

void FDReader::incomingConnection ( int socketDescriptor )
{
    cout<<"FDReader::newConnection : new network connection on socket: "<<socketDescriptor<<endl;

	//Create a new socket
	QTcpSocket *newSocket = new QTcpSocket(this);

    //Set socket descriptor
    newSocket->setSocketDescriptor(socketDescriptor);

	//Connect signals
	connect(newSocket, SIGNAL(readyRead()), this, SLOT(dataReady()));

	//push socket in vector
	m_sockets.push_back(newSocket);

}



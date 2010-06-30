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

#ifndef _FDREADER_H_
#define _FDREADER_H_

#include <QTcpServer> 
#include <QTcpSocket> 
#include <QUdpSocket>
#include <string>
#include <sstream>
#include "Vector.h"
#include "AudioSource.h"


class FDReader : public QTcpServer {
     
    Q_OBJECT;

    public:

    FDReader(int port=30011, int backlog=1);

    ~FDReader();
      
    public slots:

    void dataReady();
	void udpDataReady();
	
    signals:
      
    void putData(FD::RCPtr<FD::Vector<FD::ObjectRef> > sources);
	void sourceReady(AudioSource source);

    protected:  

    virtual void incomingConnection ( int socketDescriptor );

    std::vector<QTcpSocket*> m_sockets;
	
	QUdpSocket *m_udpSocket;

};
#endif

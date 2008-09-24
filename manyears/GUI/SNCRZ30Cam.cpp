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
#include "SNCRZ30Cam.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <QTcpSocket>
#include <QHostAddress>

#define PAN_TILT_UNIT_CONVERSION_TO_CAMERA 1.44


SNCRZ30_Capture::SNCRZ30_Capture(std::string hostname, int port, int fps)
        :    m_hostname(hostname), m_port(port), m_fps(fps)
{
    //must be called first
    //q3InitNetworkProtocols();

    m_http = new QHttp(m_hostname.c_str(),m_port);
    m_httpCommands = new QHttp(m_hostname.c_str(),m_port);

    //Connect signals
    connect(m_http, SIGNAL(readyRead ( const QHttpResponseHeader&)), this, SLOT(dataReady(const QHttpResponseHeader &)));
    connect(m_httpCommands, SIGNAL(readyRead ( const QHttpResponseHeader&)), this, SLOT(dataReadyCommands(const QHttpResponseHeader &)));
    

    reset();
    home();
    autoPanTiltSpeed(false);
    //init (capture)
    init();
}

SNCRZ30_Capture::~SNCRZ30_Capture()
{
    if (m_http)
    {
        delete m_http;
    }
}

void SNCRZ30_Capture::autoPanTiltSpeed(bool enabled)
{
	/*
	QString path = "/command/ptzf.cgi";
	QByteArray data;
	if (enabled)
	{
		data += "VISCA=8101062402FF";
	}
	else
	{
		data += "VISCA=8101062403FF";
	}
	m_httpCommands->post(path,data);
    */
	
	
	QTcpSocket m_socket;
	m_socket.connectToHost(QHostAddress("192.168.3.3"), 80);
	QByteArray data;

	//create command
	std::ostringstream my_stream;
	my_stream << "POST http://192.168.3.3:80/command/visca-gen.cgi HTTP/1.1\r\n";
	my_stream << "Connection: Keep-Alive\r\n";
	my_stream << "Cache-Control: no-cache\r\n";
	my_stream << "Content-Length: 18\r\n\r\n";
	my_stream << "VISCA=8101062403FF\r\n"; //auto-pan-tilt off


	data.append(QByteArray(my_stream.str().c_str(),my_stream.str().size()));
		
		
	m_socket.write(data);
	m_socket.waitForBytesWritten(1000);
	m_socket.flush();
	
}

void SNCRZ30_Capture::home()
{
	QString path = "/command/ptzf.cgi";
	QByteArray data;
	data += "VISCA=81010605FF";
	m_httpCommands->post(path,data);
}

void SNCRZ30_Capture::reset()
{
	QString path = "/command/ptzf.cgi";
	QByteArray data;
	data += "VISCA=81010604FF";
	m_httpCommands->post(path,data);
}

void SNCRZ30_Capture::panTilt(int pan_position, int tilt_position)
{
	static int pan_speed = 0x18;
	static int tilt_speed = 0x14;
	
	if (pan_position < -1700 || pan_position > 1700) {
		std::cerr<<"pan position : "<<pan_position<<" out of range"<<std::endl;
	    pan_position = std::max(std::min(pan_position,1700), -1700);
	}
	
	if (tilt_position < -725 || tilt_position > 425) {
		std::cerr<<"tilt position : "<<tilt_position<<" out of range"<<std::endl;
		tilt_position = std::max(std::min(tilt_position,425), -725);
	}
	
	pan_position = (int)((pan_position + 0.5) * PAN_TILT_UNIT_CONVERSION_TO_CAMERA);
	tilt_position = (int)((tilt_position + 0.5) * PAN_TILT_UNIT_CONVERSION_TO_CAMERA);
	
	
	QString path = "/command/ptzf.cgi";
	QByteArray data;
	data += "VISCA=81010602";
	
	std::stringstream my_stream;
	
	static const unsigned int mask1 = 0x000F;
	static const unsigned int mask2 = 0x00F0;
	static const unsigned int mask3 = 0x0F00;
	static const unsigned int mask4 = 0xF000;
	   
	my_stream << std::hex<<((mask2 & pan_speed)>>4);//pan speed
	my_stream << std::hex<<((mask1 & pan_speed));
	 
	my_stream << std::hex<<((mask2 & tilt_speed)>>4);//tilt speed
	my_stream << std::hex<<((mask1 & tilt_speed));
  
	my_stream << "0"<<std::hex<<((mask4 & pan_position)>>12); //pan position
	my_stream << "0"<<std::hex<<((mask3 & pan_position)>>8);
	my_stream << "0"<<std::hex<<((mask2 & pan_position)>>4);
	my_stream << "0"<<std::hex<<((mask1 & pan_position));
	  
	my_stream << "0"<<std::hex<<((mask4 & tilt_position)>>12); //tilt position
	my_stream << "0"<<std::hex<<((mask3 & tilt_position)>>8);
	my_stream << "0"<<std::hex<<((mask2 & tilt_position)>>4);
	my_stream << "0"<<std::hex<<((mask1 & tilt_position));

	data.append(QByteArray(my_stream.str().c_str(),my_stream.str().size()));
	
	data += "FF";

	m_httpCommands->post(path,data);
	
	/*
	QTcpSocket m_socket;
	m_socket.connectToHost(QHostAddress("192.168.3.3"), 80);
	QByteArray data;
	//data += "81010602";
	std::stringstream my_stream;
		
	static const unsigned int mask1 = 0x000F;
	static const unsigned int mask2 = 0x00F0;
	static const unsigned int mask3 = 0x0F00;
	static const unsigned int mask4 = 0xF000;
	   
	my_stream << "POST http://192.168.3.3:80/command/ptzf.cgi HTTP/1.1\r\n";
	my_stream << "Connection: Keep-Alive\r\n";
	my_stream << "Cache-Control: no-cache\r\n";
	my_stream << "Content-Length: 36\r\n\r\n";
	my_stream << "VISCA=81010602";

	
	my_stream << std::hex<<((mask2 & pan_speed)>>4);//pan speed
	my_stream << std::hex<<((mask1 & pan_speed));
	 
	my_stream << std::hex<<((mask2 & tilt_speed)>>4);//tilt speed
	my_stream << std::hex<<((mask1 & tilt_speed));
  
	my_stream << "0"<<std::hex<<((mask4 & pan_position)>>12); //pan position
	my_stream << "0"<<std::hex<<((mask3 & pan_position)>>8);
	my_stream << "0"<<std::hex<<((mask2 & pan_position)>>4);
	my_stream << "0"<<std::hex<<((mask1 & pan_position));
	  
	my_stream << "0"<<std::hex<<((mask4 & tilt_position)>>12); //tilt position
	my_stream << "0"<<std::hex<<((mask3 & tilt_position)>>8);
	my_stream << "0"<<std::hex<<((mask2 & tilt_position)>>4);
	my_stream << "0"<<std::hex<<((mask1 & tilt_position));

	
	my_stream << "FF\r\n";
	
	data.append(QByteArray(my_stream.str().c_str(),my_stream.str().size()));
	
	
	m_socket.write(data);
	m_socket.waitForBytesWritten(1000);
	m_socket.flush();
	*/
}

void SNCRZ30_Capture::dataReadyCommands(const QHttpResponseHeader & resp )
{
	QByteArray array = m_httpCommands->readAll();
	std::cerr.write(array.constData(),array.size());
}	
	
void SNCRZ30_Capture::dataReady(const QHttpResponseHeader & resp )
{
    static char JPEG_MARKER_START[] = {0xff,0xd8};
    static char JPEG_MARKER_END[] = {0xff,0xd9};
    static std::string input_string;

    //std::cerr<<"SNCRZ30_Capture::dataReady()"<<std::endl;
    QByteArray array = m_http->readAll();
    //std::cerr<<"SNCRZ30_Capture::dataReady() got bytes :"<<array.size()<<std::endl;

    //update string
    input_string += std::string(array.constData () ,array.size());

    unsigned int jpeg_start_pos = input_string.find(std::string(JPEG_MARKER_START,2));

    if (jpeg_start_pos != std::string::npos)
    {
        //std::cerr<<"SNCRZ30_Capture::dataReady() JPEG START AT"<<jpeg_start_pos<<std::endl;
    	unsigned int jpeg_end_pos = input_string.find(std::string(JPEG_MARKER_END,2),jpeg_start_pos);
        //unsigned int jpeg_end_pos = input_string.rfind(std::string(JPEG_MARKER_END,2),std::string::npos);

        if (jpeg_end_pos != std::string::npos)
        {
            //std::cerr<<"SNCRZ30_Capture::dataReady() JPEG END AT"<<jpeg_end_pos<<" IMAGE SIZE "<<jpeg_end_pos - jpeg_start_pos<<std::endl;

            if (m_image.loadFromData((unsigned char*) &input_string[jpeg_start_pos],(jpeg_end_pos + 2 - jpeg_start_pos)))
            {
                //std::cerr<<"SNCRZ30_Capture::dataReady() emit putImage signal"<<std::endl;
                //putImage(m_image.mirror(true,true));
            	emit putImage(m_image);
            }

            //cleanup memory
            input_string.erase(0, jpeg_end_pos + 2);

        }
    }
}

void SNCRZ30_Capture::init()
{
    if (m_http)
    {
        //std::cerr<<"SNCRZ30_Capture::init()"<<std::endl;
        std::stringstream getStream;
        getStream << "/image?speed="<<m_fps;
        m_http->get(getStream.str().c_str());
    }
}

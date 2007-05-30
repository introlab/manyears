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


SNCRZ30_Capture::SNCRZ30_Capture(std::string hostname, int port, int fps)
        :    m_hostname(hostname), m_port(port), m_fps(fps)
{
    //must be called first
    qInitNetworkProtocols();

    m_http = new QHttp(m_hostname.c_str(),m_port);

    //Connect signals
    connect(m_http, SIGNAL(readyRead ( const QHttpResponseHeader&)), this, SLOT(dataReady(const QHttpResponseHeader &)));

    //init
    init();
}

SNCRZ30_Capture::~SNCRZ30_Capture()
{
    if (m_http)
    {
        delete m_http;
    }
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
    input_string += std::string(&array[0],array.size());

    unsigned int jpeg_start_pos = input_string.find(std::string(JPEG_MARKER_START,2));

    if (jpeg_start_pos != std::string::npos)
    {
        //std::cerr<<"SNCRZ30_Capture::dataReady() JPEG START AT"<<jpeg_start_pos<<std::endl;
        unsigned int jpeg_end_pos = input_string.find(std::string(JPEG_MARKER_END,2),jpeg_start_pos);

        if (jpeg_end_pos != std::string::npos)
        {
            //std::cerr<<"SNCRZ30_Capture::dataReady() JPEG END AT"<<jpeg_end_pos<<" IMAGE SIZE "<<jpeg_end_pos - jpeg_start_pos<<std::endl;

            if (m_image.loadFromData((unsigned char*) &input_string[jpeg_start_pos],(jpeg_end_pos + 2 - jpeg_start_pos)))
            {
                //std::cerr<<"SNCRZ30_Capture::dataReady() emit putImage signal"<<std::endl;
                putImage(m_image.mirror(true,true));
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

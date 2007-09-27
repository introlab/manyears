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

#ifndef _SNCRZ30_CAPTURE_H_
#define _SNCRZ30_CAPTURE_H_

#include <Qt3Support/q3urloperator.h> 
#include <Qt3Support/q3network.h>
#include <Qt3Support/q3http.h> 
#include <qimage.h> 
#include <string>
#include <sstream>


class SNCRZ30_Capture : public QObject {
     
     Q_OBJECT;
     
     public:
	  SNCRZ30_Capture(std::string hostname, int port=80, int fps=0);
	  ~SNCRZ30_Capture();
	     	  
     public slots:
	  void dataReady(const Q3HttpResponseHeader & resp );
	  
     signals:
	  void putImage(const QImage &image);
	    
     protected:  
	  void init();
	  Q3Http *m_http;
	  std::string m_hostname;
	  int m_port;
	  int m_fps;
	  QImage m_image;
};
#endif

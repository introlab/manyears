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


#include <QHttp>
#include <QPixmap> 
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <string>
#include <iostream>
#include <sstream>


class SNCRZ30_Capture : public QObject {
     
     Q_OBJECT;
     
     public:
	  SNCRZ30_Capture(std::string hostname, int port=80, int fps=0);
	  ~SNCRZ30_Capture();
	     	  
     public slots:
	  void dataReady(const QHttpResponseHeader & resp );
	  void dataReadyCommands(const QHttpResponseHeader & resp );
	  void panTilt(int abs_pan, int abs_tilt);
	  void home();
	  void reset();
	  void autoPanTiltSpeed(bool enabled);
	  
     signals:
	  void putImage(const QPixmap &image);
	    
     protected:  
    	 
	  void init();
	  QHttp *m_http;
	  QHttp *m_httpCommands;
	  std::string m_hostname;
	  int m_port;
	  int m_fps;
	  QPixmap m_image;
};

class SNCRZ30View : public QGraphicsView
{
	Q_OBJECT;

public:
	
	SNCRZ30View(QWidget* parent)
		: QGraphicsView(parent)
		{
			QGraphicsScene *m_scene = new QGraphicsScene(this);
			setScene(m_scene);
			
			m_pixmapItem = new QGraphicsPixmapItem(NULL);
			m_pixmapItem->rotate(180);
			m_scene->addItem(m_pixmapItem);
			centerOn(m_pixmapItem);
			
			QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
		    sizePolicy1.setHorizontalStretch(0);
		    sizePolicy1.setVerticalStretch(0);
		    sizePolicy1.setHeightForWidth(sizePolicy().hasHeightForWidth());
		    setSizePolicy(sizePolicy1);
		    setMinimumSize(QSize(340, 260));
			
			m_capture = new SNCRZ30_Capture("192.168.3.3", 80, 10);
			connect(m_capture,SIGNAL(putImage(const QPixmap&)), this,SLOT(putImage(const QPixmap&)));
			
		}
	
		SNCRZ30_Capture* getCapture() {return m_capture;}
		
    public slots:
	  void putImage(const QPixmap &image)
	  {
		  //std::cerr<<"void putImage(const QPixmap &image)"<<std::endl;
		  m_pixmapItem->setPixmap(image);
	  }
	
protected:

	QGraphicsScene *m_scene;
	QGraphicsPixmapItem *m_pixmapItem;
	SNCRZ30_Capture *m_capture;
};	
#endif

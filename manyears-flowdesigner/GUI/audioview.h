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

#ifndef __AUDIOVIEW_H__
#define __AUDIOVIEW_H__

#include <QTimer> 
#include <QScrollArea>
#include <qtooltip.h>
#include <list>
#include <map>
#include <vector>
#include <math.h>
#include "AudioSource.h"
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>


class RecogTextItem : public QGraphicsRectItem
{
	
	public:
	
	RecogTextItem(QGraphicsItem  *parent, const QString &text, float x, float y)
		: QGraphicsRectItem(parent)
	{
		
		m_textItem = new QGraphicsTextItem(this);
		m_textItem->setPlainText(text);
		
		QRectF  rect = m_textItem->boundingRect();
		setRect(rect);
		
		setPos(x,y);
	}
	
	protected:
		
		QGraphicsTextItem *m_textItem;
	
};

class AudioView : public QGraphicsView {

  Q_OBJECT;

  public:
  
    AudioView(QWidget* parent);

    unsigned long long    getSelectedTime(){return selectedTime;}
    
    std::vector<AudioSource> getSourcesAtSelectedTime();
    
    std::vector<AudioSource> getSourcesAtTime(unsigned long long time);
    
    QColor               getSourceColor(int id);
    void                 addSource(unsigned long long time, const SourceInfo *source);
    void                 addSource(unsigned long long time, const AudioSource &source);  
    void				 addRecog(unsigned long long time, const QString &recog, int index);

  public slots:

    void                 selectTime(unsigned long long time);
    unsigned long long   getCurrentTime();
   

  signals:
  
    void                 timeSelected(unsigned long long time);


  protected:

    double               timeScale; // pixel / seconds
    unsigned long long   minTime, maxTime;
    unsigned long long   selectedTime;
    unsigned long long   m_timeLineUpdateTime;
    bool                 m_mouseClicked;




    int                  nbLines;
    int                  heightLine;



    void                 drawTimeLine(unsigned long long time);

    //MOUSE EVENT(S)
    virtual void         mousePressEvent ( QMouseEvent * e );
    virtual void         mouseMoveEvent ( QMouseEvent * e );
    virtual void         wheelEvent (QWheelEvent * e);

    //TIMER EVENT(S)
    virtual void         timerEvent (QTimerEvent *);

    unsigned long long   xToTime(unsigned long long x) const;
    unsigned long long   smartTimeSelect(int x, int y) const;


    std::vector<QGraphicsLineItem*> m_horizlines;
    QGraphicsLineItem *selectedTimeLine;
    std::vector<QColor>  availableColors;    
    std::map<unsigned long long, std::vector<AudioSource> > m_sources;
};


#endif

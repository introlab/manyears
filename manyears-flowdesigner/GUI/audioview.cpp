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

#include <math.h>
#include <qdatetime.h>
#include <qevent.h>
#include <qpainter.h>
#include <qtooltip.h>
#include "audioview.h"
#include <sstream>
#include <sys/time.h>
#include <unistd.h>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>


#define marginH    40

using namespace std;


AudioView::AudioView(QWidget* parent)
        : QGraphicsView(parent), timeScale(1E-5),  m_timeLineUpdateTime(0)
{

    //Create graphics scene
    QGraphicsScene *my_scene = new QGraphicsScene(0,0,1024,heightLine * (nbLines + 1) + 50 ,this);
    setScene(my_scene);

    //set graphics line configuration
    nbLines = 19;
    heightLine = 15;

    //Set current limits                   
    minTime=getCurrentTime();
    maxTime=minTime + 1 * 1000000LL;

   
    //Create horizontal Lines
    for (int i = 0; i < nbLines; i++)
    {
	    QGraphicsLineItem *my_line = new QGraphicsLineItem(0,i*heightLine,1024,i*heightLine,NULL);
	    scene()->addItem(my_line);
    	m_horizlines.push_back(my_line);
    }

    //Create selected time line
    selectedTimeLine = new QGraphicsLineItem(1,0,1,nbLines*heightLine,NULL);
    selectedTimeLine->setPen(QPen(Qt::red));
    scene()->addItem(selectedTimeLine);

    //initialize available colors
    availableColors.push_back(Qt::red);
    availableColors.push_back(Qt::green);
    availableColors.push_back(Qt::blue);
    availableColors.push_back(Qt::cyan);
    availableColors.push_back(Qt::magenta);
    availableColors.push_back(Qt::yellow);
    availableColors.push_back(Qt::gray);
    availableColors.push_back(Qt::red);
    availableColors.push_back(Qt::green);
    availableColors.push_back(Qt::blue);

    //add vertical line every 10 sec for 100 secs
    for ( m_timeLineUpdateTime = 0;  m_timeLineUpdateTime <= (unsigned long long) 150E6;  m_timeLineUpdateTime+= (unsigned long long) 10E6)
    {
        drawTimeLine(m_timeLineUpdateTime);        
    }
    
    //This will update the timeline every 10 secs
    startTimer(10000);
}

void AudioView::mousePressEvent(QMouseEvent * e)
{

    //cerr<<"AudioView::mousePressEvent(QMouseEvent * e)"<<endl;
    QGraphicsView::mousePressEvent(e);

    if (e->button() == Qt::LeftButton)
    {    
        QPointF point = mapToScene(e->pos());    
        unsigned long long t = xToTime((unsigned long long) point.x());
        selectTime(t);
        
        e->accept();
    }
}

void AudioView::mouseMoveEvent ( QMouseEvent * e )
{
    //cerr<<"AudioView::mouseMoveEvent ( QMouseEvent * e )"<<endl;
    QGraphicsView::mouseMoveEvent(e);
    
    if (e->buttons() == Qt::LeftButton)
    {
    
        QPointF point = mapToScene(e->pos());    
        unsigned long long t = xToTime((unsigned long long) point.x());
        selectTime(t);
        
        e->accept();
    }

}


void AudioView::drawTimeLine(unsigned long long elapsedTime)
{
    //DRAW VERTICAL LINE	
    QGraphicsLineItem *my_line= new QGraphicsLineItem(elapsedTime * timeScale,0,elapsedTime * timeScale,nbLines*heightLine,NULL);
   	my_line->setPen(QPen(Qt::black));
    scene()->addItem(my_line);

	//ADD TIME STAMP
	QGraphicsTextItem *my_text = new QGraphicsTextItem(QString::number(floor(elapsedTime / 1E6)));
	QFont font = my_text->font();
	font.setPointSize(12);
	font.setWeight(QFont::Bold);
	my_text->setFont(font);	
	my_text->setDefaultTextColor(Qt::red);
	float width = my_text->boundingRect().width();
	my_text->setPos(elapsedTime * timeScale - width/2,nbLines*heightLine + 20);
	scene()->addItem(my_text);
	
	//UPDATE SCENE SIZE
	//float scene_width = elapsedTime * timeScale;
	//float scene_height = max((float)scene()->height(),(float)(heightLine * nbLines + 30));	
	//scene()->setSceneRect (0,0,scene_width,scene_height);
	scene()->setSceneRect(scene()->itemsBoundingRect()); 
	
	//DRAW HORIZONTAL LINE LABELS
	if (elapsedTime % (unsigned long long)(50E6) == 0)
	{
	    for (unsigned int i = 0; i < nbLines; i++)
	    {
	        QGraphicsTextItem *label = new QGraphicsTextItem(QString::number((360.0/(nbLines - 1)) * i - 180));
	        font = label->font();
	        font.setPointSize(8);	    
	        label->setFont(font);	
	        label->setPos(elapsedTime * timeScale,i*heightLine);
	        scene()->addItem(label);
	    }
	}
		
	//STRETCH HORIZONTAL LINES IF NECESSARY
	for (unsigned int i = 0; i< m_horizlines.size(); i++)
	{
		QLineF line = m_horizlines[i]->line();
		line.setLength(max(line.length(),elapsedTime * timeScale));
		m_horizlines[i]->setLine(line);		
	}
	
	//setRect(scene()->getRect());
}

void AudioView::timerEvent(QTimerEvent *)
{
    //Draw current time
    drawTimeLine(m_timeLineUpdateTime);
    
    //increment time for next redraw
    m_timeLineUpdateTime += (unsigned long long) 10E6;
   
}

void AudioView::selectTime(unsigned long long time)
{    
    selectedTime = min(time,getCurrentTime());
    emit timeSelected(selectedTime);

    unsigned long long elapsedTime = selectedTime - minTime;

    //draw selection
    selectedTimeLine->setLine((float) elapsedTime * timeScale,0,(float) elapsedTime * timeScale,nbLines*heightLine);

    //make sure the selection is visible
    selectedTimeLine->ensureVisible();

}


void AudioView::wheelEvent (QWheelEvent * e)
{
    //TODO SCROLL WITH MOUSE?
    e->accept();
}

unsigned long long AudioView::xToTime(unsigned long long x) const
{
    return minTime + (unsigned long long) (x / timeScale);
}


std::vector<AudioSource> AudioView::getSourcesAtTime(unsigned long long time)
{
    std::vector<AudioSource> sources;    
    std::map<unsigned long long, std::vector<AudioSource> >::const_iterator iter = m_sources.lower_bound(time);
           
    if (iter != m_sources.end())
    {
        if (iter->first >= time - (unsigned long long)(1E6))
        {
            sources = iter->second;                 
        }
    }      

    return sources;
}


std::vector<AudioSource>  AudioView::getSourcesAtSelectedTime()
{
    return getSourcesAtTime(selectedTime);
}

QColor AudioView::getSourceColor(int id)
{
    return availableColors[id % 10];
}

void AudioView::addSource(unsigned long long time, const SourceInfo* source)
{
    if (source)
    {
        addSource(time,AudioSource(source));
    }   
}

void AudioView::addSource(unsigned long long time, const AudioSource &source)
{
    unsigned long long index = (time / 10000)  * 10000; //100th second precision
    bool found = false;
    
    for (unsigned int i = 0; i < m_sources[index].size(); i++)
    {
        if (m_sources[index][i].m_id == source.m_id)
        {
            found = true;
            break;
        }
    }
    
    if (!found)
    {
	    //add to source list
        m_sources[index].push_back(source);
	    //create Item to display
	    float x = (time - minTime) * timeScale;
	    float y = (source.m_theta+180) * (nbLines*heightLine / 360.0);
	    QGraphicsEllipseItem *my_ellipse = new QGraphicsEllipseItem(x - 1,y - 1,2,2,NULL);
	    //set the color
	    my_ellipse->setPen(QPen(getSourceColor(source.m_id)));
	    //add item
	    scene()->addItem(my_ellipse);
	    //set ref to this ellipse
	    m_sources[index].back().m_item = my_ellipse;
    }
}

void AudioView::addRecog(unsigned long long time, const QString &_recog, int index)
{

   QString recog = _recog;
   recog.remove("\n");
   recog.remove("RESULT:");
	
   std::vector<AudioSource> sources;    
   std::map<unsigned long long, std::vector<AudioSource> >::reverse_iterator iter = m_sources.rbegin();
   
   //create Item to display
   float x = (time - minTime) * timeScale;
   float y = ((nbLines + 5)*heightLine);
   
   RecogTextItem *myItem = new RecogTextItem(NULL,recog,x,y);
   scene()->addItem(myItem);
   
   //Look backward in the source to find the latest source corresponding to our 
   //Recognition port
   for (;iter != m_sources.rend(); iter++)
   {	  
	   bool found = false;
	   
	   sources = iter->second;
	    
	    for (unsigned int i = 0; i < sources.size(); i++)
	    {		    	
		    if (sources[i].m_port == 7000 + index)
		    {
		    	QBrush brush = sources[i].m_item->brush();
		    	myItem->setBrush(QBrush(getSourceColor(sources[i].m_id)));
		    	found = true;
		    	break;
		    }
	    }	
	    
	    if (found)
	    	break;
   }
    
    //Avoid colliding with other text labels
    while(scene()->collidingItems(myItem).size() > 1)
    {	
    	myItem->moveBy(0,30);
    }
    
    
}
   
unsigned long long AudioView::getCurrentTime()
{
    timeval thetime;
    gettimeofday(&thetime, 0);
    unsigned long long time = thetime.tv_sec * 1000000LL + thetime.tv_usec;
    return time;
}


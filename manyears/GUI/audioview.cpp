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
        : QGraphicsView(parent), timeScale(1E-5)
{

    QGraphicsScene *my_scene = new QGraphicsScene(0,0,1024,heightLine * (nbLines + 1) + 50,this);

    setScene(my_scene);

    //setMinimumSize(20, 220);
    nbLines = 19;
    heightLine = 15;
    pointSize = 7;
    dySequence = pointSize-2;
    nbSequence = heightLine / dySequence;
    
    
    timeval thetime;
    gettimeofday(&thetime, 0);
    long long time = thetime.tv_sec * 1000000LL + thetime.tv_usec;
                
    minTime=time;
    maxTime=time + 1 * 1000000LL;
    autoscrollDX = 0;
    
    //TODO REPLACE THIS.
    //viewport()->setEraseColor(Qt::white); 
    //setDragAutoScroll(true);

    //viewport()->setMouseTracking(true);
    //setMouseTracking(true);
    //viewport()->setAcceptDrops(true);
    //setAcceptDrops(true);
    mousemode = mmNone;
    //resize(200, heightLine * nbLines + 30);
    //setMaximumSize(1024,heightLine * (nbLines + 1) + 50);



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

    //add vertical line every 5 sec.
    timerEvent(NULL);
    startTimer(10000);
}

void AudioView::setExtremas(long long t1, long long t2)
{
    if(t1==minTime && t2==maxTime)
        return;

    minTime = t1;
    maxTime = t2;
    long long l = maxTime-minTime;
	
    //TODO REPLACE THAT
    //resizeContents(2*marginH + (int)(l*timeScale), heightLine * nbLines + 30);
    //repaintContents();

     //scene()->resize(2*marginH + (int)(l*timeScale), heightLine * nbLines + 30);


     float width = max((float)scene()->width(),(float)(2*marginH + (int)(l*timeScale)));
     float height = max((float)scene()->height(),(float)(heightLine * nbLines + 30));
     scene()->setSceneRect (0,0,width,height);

     //viewport()->repaint();
}

void AudioView::setTimeScale(double scale)
{
    timeScale = scale;
    long long l = maxTime - minTime;
    
    float width = max((float)scene()->width(),(float)(2*marginH + (int)(l*timeScale)));
    float height = max((float)scene()->height(),(float)(heightLine * nbLines + 30));
    scene()->setSceneRect (0,0,width,height);

    //TODO REPLACE THAT
    //resizeContents(2*marginH + (int)(l*timeScale), heightLine * nbLines + 30);
    //repaintContents();

    //scene()->resize(2*marginH + (int)(l*timeScale), heightLine * nbLines + 30);
    //viewport()->repaint();
}

void AudioView::drawContents(QPainter* p, int cx, int cy, int cw, int )
{

   cerr<<"AudioView::drawContents(QPainter* p, int cx, int cy, int cw, int )"<<endl;

    //logView->mutex.lock();
    int x1=marginH, x2=viewport()->width() -marginH;
    if(x1<cx) x1=cx;
    if(x2>cx+cw) x2=cx+cw;


    //horizontal lines
    p->setPen(Qt::darkCyan);
    for(int i=0;i<nbLines;i++)
    {

        if (i == nbLines / 2)
        {
            p->setPen(Qt::black);
        }
        else
        {
            p->setPen(Qt::darkCyan);
        }

        int y = (i)*heightLine;
        p->drawLine(x1, y, x2, y);
    }

    long long steps[] = {     50000LL,    100000LL,    250000LL,    500000LL,
                              1000000LL,   5000000LL,  10000000LL,   25000000LL,   50000000LL,
                              100000000LL, 250000000LL, 500000000LL, 1000000000LL, 2500000000LL,
                              5000000000LL};
    markStep = 1;
    for(unsigned int i=0;i<sizeof(steps)/sizeof(steps[0]);i++)
    {
        markStep = steps[i];
        if(markStep*timeScale >= 80) break;
    }

    long long clipTime1 = xToTime(cx);
    clipTime1 += -(clipTime1-minTime)%markStep;
    if(clipTime1 < minTime) clipTime1 = minTime;
    long long clipTime2 =  xToTime(cx+cw);
    clipTime2 += markStep-(clipTime2-minTime)%markStep;
    //std::cerr << "clipTime1=" << clipTime1 << std::endl;
    //std::cerr << "clipTime2=" << clipTime2 << std::endl;

    //double l = maxTime - minTime;
    for(long long i=clipTime1;i<=clipTime2 && i<=maxTime;i+=markStep)
    {
        int x = marginH + (int)((i-minTime)*timeScale);
        int y = heightLine * nbLines + 1;
        p->setPen(Qt::gray);
        p->drawLine(x, cy, x, y);
        p->setPen(Qt::black);
        QString str = (i-minTime)%1000000LL==0
                      ? QString::number((i-minTime)/1000000LL)
                      : QString::number( ((i-minTime)%1000000LL) /1000000.0).mid(1);


        p->drawText(x-50, y+10, 100, 20, Qt::AlignHCenter, str);
        QFont oldFont = p->font();

        //draw also angles
        for (int angle = 0; angle <= 360; angle += 20)
        {
            p->setPen(Qt::lightGray);
            QFont newFont(oldFont);
            newFont.setPointSize(8);
            p->setFont(newFont);
            QString angleText = QString::number(angle - 180);
            p->drawText(x,heightLine * angle / 20,50,10,Qt::AlignHCenter,angleText);
        }
        p->setFont(oldFont);

        /*
        if(i==minTime){
          p->setPen(Qt::red);
          p->drawText(x-5, y+38, QString::number(i/1000000LL));
        }
        */
    }


	// Go through all sources
	if(m_sources.size() > 0)
	{
	    std::map<long long, std::vector<AudioSource> >::const_iterator iter = m_sources.lower_bound(clipTime1);
	    std::map<long long, std::vector<AudioSource> >::const_iterator end = m_sources.upper_bound(clipTime2);

	    for(;iter!=end;iter++)
	    {
		for (int i = 0; i < iter->second.size(); i++)
		{
		    AudioSource currentSource = iter->second[i];
		    
		    //read information
		    int source_id = currentSource.id;
		    float theta = currentSource.theta;
		    float phi = currentSource.phi;
		    float strength = currentSource.strength;

		    //default color
		    QColor color = availableColors[source_id % 10];


		    long long ts = iter->first;
		    int x = marginH + (int)((ts-minTime) * timeScale);
		    int y = (int)((theta+180) * nbLines*heightLine / 360.0);

		    if(ts == selectedTime)
		        color = color.light(165);
		    p->setPen(color.dark());
		    p->setBrush(color);
		    p->drawEllipse(x - pointSize /2,y - pointSize / 2, pointSize, pointSize);
		}
	    }
	}
                       
      
    

  

    // Ligne du temps courrant
    int x = marginH + (int)((selectedTime-minTime)*timeScale);
    if(x>=cx && x<=cx+cw)
    {
        p->setPen(Qt::darkRed);
        int y = heightLine * nbLines + 2;
        p->drawLine(x, cy, x, y);
    }
    //logView->mutex.unlock();
}

void AudioView::drawOn(QPainter* painter)
{
    //int w = contentsWidth();
    //int h = contentsHeight();
    int w = viewport()->width();
    int h = viewport()->height();
    drawContents(painter, 0, 0, w, h);
}





void AudioView::repaintTimeInterval(long long t1, long long t2, bool erase)
{
    int x1 = (int)((t1-minTime)*timeScale) + marginH;
    int x2 = (int)((t2-minTime)*timeScale) + marginH;
    update(x1-pointSize/2-1, 0, x2-x1+pointSize+2, nbLines*heightLine+3);
}

void AudioView::mousePressEvent(QMouseEvent * e)
{

    cerr<<"AudioView::mousePressEvent(QMouseEvent * e)"<<endl;
    QGraphicsView::mousePressEvent(e);


    if (e->button() == Qt::LeftButton)
    {
    
        QPointF point = mapToScene(e->pos());    
        long long t = xToTime((long long) point.x());
        selectTime(t);
    }

/*

    if(e->button() == Qt::LeftButton)
    {
        long long t = xToTime(e->x());
        t = smartTimeSelect(e->x(), e->y());
        if(t<minTime) t=minTime; else if(t>maxTime) t=maxTime;
        //if(selectedTime != t){
        long long oldtime = selectedTime;
        selectedTime = t;
        repaintTimeInterval(oldtime, oldtime);
        repaintTimeInterval(t, t);
        emit timeSelected(t);
        //}

        int selline = e->y()/heightLine;
        if(selline>=5) selline = -1;
        if(selline != logLineSelected)
        {
            logLineSelected = selline;
            emit selectedLogLine(logLineSelected);
        }
        mousemode = mmTime;
        startTimer(50);
    }

    if(e->button() == Qt::RightButton)
    {
        startDragPoint = e->pos();
        startDragTimeScale = timeScale;
        mousemode = mmTimeScale;
        startDragTimePoint = xToTime(startDragPoint.x());
    }

*/

}

void AudioView::mouseMoveEvent ( QMouseEvent * e )
{
    QGraphicsView::mouseMoveEvent(e);
/* 
   switch(mousemode)
    {
    case mmTimeScale:
        {
            int d = e->pos().y() - startDragPoint.y();
            int dx = e->pos().x() - viewport()->pos().x();
            setTimeScale(startDragTimeScale * exp(d*0.01));
            dx = marginH + (int) ((startDragTimePoint-minTime) * timeScale) - dx;
            //viewport()->setPos(dx, 0);
            break;
        }
    case mmTime:
        {
            QPoint vp = e->pos();
            autoscrollDX = 0;
            if(vp.x() < 10)
            {
                autoscrollDX = -(10-vp.x()) /5;
                scrollIncrementCount=0;
            }
            else if(vp.x() > viewport()->width()-10)
            {
                autoscrollDX =  (vp.x()-viewport()->width()+10) /5;
                scrollIncrementCount=0;
            }
            long long t = smartTimeSelect(e->x(), e->y());
            if(t<minTime) t=minTime; else if(t>maxTime) t=maxTime;
            if(selectedTime != t)
            {
                long long oldtime = selectedTime;
                selectedTime = t;
                repaintTimeInterval(oldtime, oldtime);
                repaintTimeInterval(t, t);
                emit timeSelected(t);
            }

            int selline = e->y()/heightLine;
            if(selline>=5) selline = -1;
            if(selline != logLineSelected)
            {
                logLineSelected = selline;
                emit selectedLogLine(logLineSelected);
            }

            break;
        }
    case mmNone:
    default:
    {}
    }

*/

}

void AudioView::timerEvent(QTimerEvent *)
{

    //GET THE ACTUAL TIME DIFFERENCE FROM NOW TO START TIME
	timeval thetime;
    gettimeofday(&thetime, 0);
    long long elapsedTime = (thetime.tv_sec * 1000000LL + thetime.tv_usec) - minTime + 50E6;


   	//DRAW VERTICAL LINE	
    QGraphicsLineItem *my_line= new QGraphicsLineItem(elapsedTime * timeScale,0,elapsedTime * timeScale,nbLines*heightLine,NULL);
   	my_line->setPen(QPen(Qt::black));
    scene()->addItem(my_line);

	//ADD TIME STAMP
	QGraphicsTextItem *my_text = new QGraphicsTextItem(QString::number(floor(elapsedTime / 1E6)));
	float width = my_text->textWidth();
	my_text->setPos(elapsedTime * timeScale - width,nbLines*heightLine + 20);
	scene()->addItem(my_text);
	
	float scene_width = elapsedTime * timeScale;
	float scene_height = max((float)scene()->height(),(float)(heightLine * nbLines + 30));
	
	scene()->setSceneRect (0,0,scene_width,scene_height);

	
	//DRAW HORIZONTAL LINE LABELS


	
	//STRETCH HORIZONTAL LINES
	for (int i = 0; i< m_horizlines.size(); i++)
	{
		QLineF line = m_horizlines[i]->line();
		line.setLength(max(line.length(),elapsedTime * timeScale));
		m_horizlines[i]->setLine(line);		
	}


}

void AudioView::selectTime(long long time)
{

    cerr<<"AudioView::selectTime(long long time)"<<endl;
    if(time == selectedTime) return;

    selectedTime = time;
    emit timeSelected(selectedTime);

    unsigned long long elapsedTime = time - minTime;


    selectedTimeLine->setLine((float) elapsedTime * timeScale,0,(float) elapsedTime * timeScale,nbLines*heightLine);
    
	
/*

    long long oldtime = selectedTime;
    selectedTime = time;
    repaintTimeInterval(oldtime, oldtime);
    repaintTimeInterval(time, time);
    emit timeSelected(selectedTime);
    int x = (int)((time-minTime)*timeScale) + marginH;
    if(x>=(viewport()->pos().x()+visibleRegion().boundingRect().width()-22))
    {
	//TODO, CHANGE THIS
        //setContentsPos(x-20, contentsY());
    }
    else if(x<viewport()->pos().x())
        ensureVisible(x, 50, 20, 10);

*/

}

void AudioView::mouseReleaseEvent ( QMouseEvent *)
{
    mousemode = mmNone;
    // code ici pour forcer le tip
    //killTimers();
}

void AudioView::wheelEvent (QWheelEvent * e)
{

    /*	

    int dx = e->x() - viewport()->pos().x();
    long long time = xToTime(e->x());
    setTimeScale(timeScale * exp(e->delta()*0.001));
    dx = marginH + (int) ((time-minTime) * timeScale)-dx;
    scrollContentsBy(dx, 0);

   */

}

long long AudioView::xToTime(long long x) const
{
    return minTime + (long long) (x / timeScale);
}

long long AudioView::smartTimeSelect(int px, int py) const
{
    long long clipTime1 = xToTime(px-pointSize);
    if(clipTime1 < minTime) clipTime1 = minTime;
    long long clipTime2 = xToTime(px+pointSize);

    long long besttime = xToTime(px);
    int       bestray2 = pointSize*pointSize/4;



    // Log behaviors activation/exploitation

    return besttime;
}

void AudioView::filterToggle()
{
}

std::vector<AudioSource>  AudioView::getSourcesAtSelectedTime()
{

    std::vector<AudioSource> sources;
    
    std::map<long long, std::vector<AudioSource> >::const_iterator iter = m_sources.lower_bound(selectedTime);
    
       
    if (iter != m_sources.end())
    {
        sources = iter->second;         
        
    }      

    return sources;
}

QColor AudioView::getSourceColor(int id)
{
    return availableColors[id % 10];
}

void AudioView::addSource(long long time, const SourceInfo* source)
{
    if (source)
    {
        addSource(time,AudioSource(source));
    }   
}
void AudioView::addSource(long long time, const AudioSource &source)
{
    long long index = (time / 10000)  * 10000;
    bool found = false;
    
    for (unsigned int i = 0; i < m_sources[index].size(); i++)
    {
        if (m_sources[index][i].id == source.id)
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
	float y = (source.theta+180) * (nbLines*heightLine / 360.0);
	QGraphicsEllipseItem *my_ellipse = new QGraphicsEllipseItem(x - 1,y - 1,2,2,NULL);
	//set the color
	my_ellipse->setPen(QPen(getSourceColor(source.id)));
	//add item
	scene()->addItem(my_ellipse);
	//set ref to this ellipse
	m_sources[index].back().item = my_ellipse;
    }
}
   
void AudioView::timeout()
{
    
    //get the actual time
    timeval thetime;
    gettimeofday(&thetime, 0);
    long long time = thetime.tv_sec * 1000000LL + thetime.tv_usec;
    

    //setExtremas(minTime,time + 1000000LL);
    //selectTime(time);      
}   



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

#define marginH    40


AudioView::AudioView(QWidget* parent)
        : QScrollView(parent), QToolTip(viewport()), timeScale(10e-6)
{
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
    viewport()->setEraseColor(Qt::white);
    setDragAutoScroll(true);
    viewport()->setMouseTracking(true);
    setMouseTracking(true);
    viewport()->setAcceptDrops(true);
    setAcceptDrops(true);
    mousemode = mmNone;
    resize(200, heightLine * nbLines + 30);
    setMaximumSize(1024,heightLine * (nbLines + 1) + 50);

    //initialize available colors
    availableColors.push_back(Qt::red.light(120));
    availableColors.push_back(Qt::green.light(120));
    availableColors.push_back(Qt::blue.light(120));
    availableColors.push_back(Qt::cyan.light(120));
    availableColors.push_back(Qt::magenta.light(120));
    availableColors.push_back(Qt::yellow.light(120));
    availableColors.push_back(Qt::gray.light(120));
    availableColors.push_back(Qt::red.light(150));
    availableColors.push_back(Qt::green.light(150));
    availableColors.push_back(Qt::blue.light(150));
}

void AudioView::setExtremas(long long t1, long long t2)
{
    if(t1==minTime && t2==maxTime)
        return;

    minTime = t1;
    maxTime = t2;
    long long l = maxTime-minTime;
    resizeContents(2*marginH + (int)(l*timeScale), heightLine * nbLines + 30);
    repaintContents();
}

void AudioView::setTimeScale(double scale)
{
    timeScale = scale;
    long long l = maxTime - minTime;
    resizeContents(2*marginH + (int)(l*timeScale), heightLine * nbLines + 30);
    repaintContents();
}

void AudioView::drawContents(QPainter* p, int cx, int cy, int cw, int )
{
    //logView->mutex.lock();
    int x1=marginH, x2=contentsWidth()-marginH;
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
    int w = contentsWidth();
    int h = contentsHeight();
    drawContents(painter, 0, 0, w, h);
}

void AudioView::maybeTip( const QPoint & p)
{
    QPoint p2 = viewportToContents(p);
    p2 -= viewport()->pos();
    int px = p2.x() ;
    int py = p2.y() ;

    long long clipTime1 = xToTime(px-4);
    if(clipTime1 < minTime) clipTime1 = minTime;
    long long clipTime2 = xToTime(px+4);


    // Log du Workspace
    /*
    if(py >=0 && py<=heightLine){
       WorkspaceLogs::const_iterator iter = logView->getWorkspaceLogs().lower_bound(clipTime1);
       WorkspaceLogs::const_iterator end = logView->getWorkspaceLogs().upper_bound(clipTime2);
       QString message;
       QRect tiprect;
       const Workspace& ws = logView->getCurrentWorkspace();
       for(;iter!=end;iter++)
          if(!logView->isCommandFiltered(iter->second))
          {
             const TimeStamp& ts = iter->first;
             int x = marginH + (int)((ts.time-minTime) * timeScale);
             int y = 1*heightLine - (ts.sequence%nbSequence)*dySequence;
             QRect r(x-pointSize/2, y-pointSize/2, pointSize, pointSize);
             if(r.contains(p2)) {

                 QString str = tipWorkspace(iter, ws);

                 if(message.length())
                   message += "<hr>";
                 message += str;
                 r.moveTopLeft(p);
                 tiprect = r;
                 //tip(r, str);
             }
          }
       if(message.length())
         tip(tiprect, message);

    }

    */

    // Log behaviors activation/exploitation

}



void AudioView::repaintTimeInterval(long long t1, long long t2, bool erase)
{
    int x1 = (int)((t1-minTime)*timeScale) + marginH;
    int x2 = (int)((t2-minTime)*timeScale) + marginH;
    repaintContents(x1-pointSize/2-1, 0, x2-x1+pointSize+2, nbLines*heightLine+3, erase);
}

void AudioView::contentsMousePressEvent(QMouseEvent * e)
{
    QScrollView::contentsMousePressEvent(e);
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
}

void AudioView::contentsMouseMoveEvent ( QMouseEvent * e )
{
    QScrollView::contentsMouseMoveEvent(e);
    switch(mousemode)
    {
    case mmTimeScale:
        {
            int d = e->pos().y() - startDragPoint.y();
            int dx = e->pos().x() - contentsX();
            setTimeScale(startDragTimeScale * exp(d*0.01));
            dx = marginH + (int) ((startDragTimePoint-minTime) * timeScale) - dx;
            setContentsPos(dx, 0);
            break;
        }
    case mmTime:
        {
            QPoint vp = contentsToViewport(e->pos());
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
}

void AudioView::timerEvent(QTimerEvent *)
{
    if(autoscrollDX)
    {
        scrollBy(autoscrollDX, 0);
        if(++scrollIncrementCount%5 == 0)
            autoscrollDX += autoscrollDX>0 ? 1 : -1;
    }
}

void AudioView::selectTime(long long time)
{
    if(time == selectedTime) return;
    long long oldtime = selectedTime;
    selectedTime = time;
    repaintTimeInterval(oldtime, oldtime);
    repaintTimeInterval(time, time);
    emit timeSelected(selectedTime);
    int x = (int)((time-minTime)*timeScale) + marginH;
    if(x>=(contentsX()+visibleWidth()-22))
    {
        setContentsPos(x-20, contentsY());
    }
    else if(x<contentsX())
        ensureVisible(x, 50, 20, 10);
}

void AudioView::contextMenuEvent ( QContextMenuEvent * e )
{
    e->accept();
}

void AudioView::contentsMouseReleaseEvent ( QMouseEvent *)
{
    mousemode = mmNone;
    // code ici pour forcer le tip
    killTimers();
}

void AudioView::contentsWheelEvent (QWheelEvent * e)
{
    int dx = e->x() - contentsX();
    long long time = xToTime(e->x());
    setTimeScale(timeScale * exp(e->delta()*0.001));
    dx = marginH + (int) ((time-minTime) * timeScale)-dx;
    setContentsPos(dx, 0);
}

long long AudioView::xToTime(int x) const
{
    return minTime + (long long) ((x-marginH) / timeScale);
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
        m_sources[time].push_back(AudioSource(source));        
    }      
}
void AudioView::addSource(long long time, const AudioSource &source)
{
    m_sources[time].push_back(source);       
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



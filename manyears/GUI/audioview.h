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

#include <qtimer.h> 
#include <qscrollview.h>
#include <qtooltip.h>
#include <list>
#include <map>
#include <vector>
#include <math.h>
#include "AudioSource.h"

class AudioView;

//



class AudioView : public QScrollView, QToolTip{
  Q_OBJECT

  public:
    AudioView(QWidget* parent);

    void                 repaintTimeInterval(long long t1, long long t2, bool erase=true);

    void                 drawOn(QPainter* painter);
    long long            getSelectedTime(){return selectedTime;}
    
    std::vector<AudioSource> getSourcesAtSelectedTime();
    
    QColor               getSourceColor(int id);
    void                 addSource(long long time, const SourceInfo *source);
    void                 addSource(long long time, const AudioSource &source);   

  public slots:
    void                 setTimeScale(double scale);
    void                 filterToggle();
    void                 setExtremas(long long t1, long long t2);
    void                 selectTime(long long time);
    void                 timeout();   

  signals:
    void                 timeSelected(long long time);
    void                 selectedLogLine(int line);

  protected:

    double               timeScale; // pixel / seconds
    long long            minTime, maxTime;
    long long            selectedTime;
    int                  logLineSelected;

    int                  nbLines;
    int                  heightLine;
    int                  nbSequence;
    int                  dySequence;
    int                  pointSize;
    long long            markStep;

    QPoint               startDragPoint;
    double               startDragTimeScale;
    long long            startDragTimePoint;

    int                  autoscrollDX;
    int                  scrollIncrementCount;



    std::map<QString,bool> filterState;
    std::map<QString,int> filterMenuId;

    enum MouseMode { mmNone, mmTimeScale, mmTime} mousemode;

    virtual void         drawContents(QPainter*, int, int, int, int);

    virtual void         contentsMousePressEvent ( QMouseEvent * e );
    virtual void         contentsMouseMoveEvent ( QMouseEvent * e );
    virtual void         contentsMouseReleaseEvent ( QMouseEvent * e );
    virtual void         contentsWheelEvent (QWheelEvent * e);

    virtual void         contextMenuEvent ( QContextMenuEvent * e );

    virtual void         timerEvent (QTimerEvent *);

    virtual void         maybeTip( const QPoint & );

    long long            xToTime(int x) const;
    long long            smartTimeSelect(int x, int y) const;
    std::vector<QColor>  availableColors;
    
    std::map<long long, std::vector<AudioSource> > m_sources;
};


#endif

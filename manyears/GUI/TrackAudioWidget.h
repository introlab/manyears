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

#ifndef _TRACK_AUDIO_WIDGET_H_
#define _TRACK_AUDIO_WIDGET_H_


#include "audioview.h"
#include "ImageView.h"
#include "FDReader.h"
#include <Qt3Support/q3mainwindow.h>
#include <Qt3Support/q3textedit.h>
#include <qlayout.h>
#include <Qt3Support/q3buttongroup.h>
#include <qpushbutton.h>
#include <qtimer.h>
//Added by qt3to4:
#include <Qt3Support/q3VBoxLayout>
#include <Qt3Support/q3GridLayout>
#include <Qt3Support/q3HBoxLayout>
#include <QLabel>



class TrackAudioWidget : public Q3MainWindow
{
    Q_OBJECT;

public:
    TrackAudioWidget(QWidget* parent=NULL);
    virtual void setTime (long long time);

    bool isSourceActive(int id);
    long long getTime();
    
public slots:
    void getData(const std::vector<const SourceInfo*> &sources);
    void playClicked(int source_id);
    void timeout();   

protected:
    long long maxTimeSetted;
    long long minTime;
       
    AudioView* audioView;
    Q3TextEdit* msgEdit;
    Q3VBoxLayout* Form1Layout;
    Q3HBoxLayout* bottomLayout;
    Q3GridLayout* gridLayout;
    QPushButton* sourceButtons[4];
    QLabel*      sourceID[4];
    QLabel*      sourceTheta[4];
    QLabel*      sourcePhi[4];
    QLabel*      sourceStrength[4];
    Q3ButtonGroup* buttonGroup;
    ImageView*    imageView;
    QTimer*       timer;
    FDReader* 	  reader;
};

#endif

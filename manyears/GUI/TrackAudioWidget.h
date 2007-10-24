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
#include <QMainWindow>
#include <QLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>


class TrackAudioWidget : public QMainWindow
{
    Q_OBJECT;

public:
    TrackAudioWidget(QWidget* parent=NULL);
    virtual void setTime (unsigned long long time);

    bool isSourceActive(int id);
    unsigned long long getTime();
    
public slots:
    void getData(const std::vector<const SourceInfo*> &sources);
    void selectedTime(unsigned long long time);
    void playClicked(int source_id);
    void timeout();   

protected:

    long long maxTimeSetted;  
    AudioView* audioView;
    QVBoxLayout* Form1Layout;
    QHBoxLayout* bottomLayout;
    QGridLayout* gridLayout;
    QPushButton* sourceButtons[4];
    QLabel*      sourceID[4];
    QLabel*      sourceTheta[4];
    QLabel*      sourcePhi[4];
    QLabel*      sourceStrength[4];
    QButtonGroup* buttonGroup;
    ImageView*    imageView;
    QTimer*       timer;
    FDReader* 	  reader;
};

#endif

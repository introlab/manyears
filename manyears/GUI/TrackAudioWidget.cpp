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

#include "TrackAudioWidget.h"
#include <sstream>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <qlayout.h>
#include <iostream>
#include <qprocess.h>

#define MAX_NUM_SOURCES 4

// ---
TrackAudioWidget::TrackAudioWidget(QWidget* parent)
        : QMainWindow(parent), maxTimeSetted(0)
{
    minTime = getTime();   
    Form1Layout = new QVBoxLayout( this, 11, 6, "Form1Layout");

    audioView = new AudioView(this);
    Form1Layout->addWidget(audioView);

    msgEdit = new QTextEdit( this, "msgEdit" );
    QFont msgEdit_font(  msgEdit->font() );
    msgEdit_font.setPointSize( 17 );
    msgEdit->setFont( msgEdit_font );
    msgEdit->setMaximumHeight(60);
    msgEdit->hide();
    Form1Layout->addWidget( msgEdit );

    bottomLayout = new QHBoxLayout(this,11,6,"BottomLayout");
    Form1Layout->addLayout(bottomLayout);

    imageView = new ImageView(this);
    bottomLayout->addWidget(imageView);

    gridLayout = new QGridLayout(this,5,5,11,6,"GridLayout");
    buttonGroup = new QButtonGroup(this);
    buttonGroup->hide();
    connect(buttonGroup, SIGNAL(clicked(int)), this, SLOT(playClicked(int)));

    gridLayout->addWidget(new QLabel("",this),0,0,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("ID",this),0,1,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("Theta",this),0,2,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("Phi",this),0,3,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("Strength",this),0,4,Qt::AlignHCenter);

    for (int i = 0; i < MAX_NUM_SOURCES; i++)
    {
        sourceButtons[i] = new QPushButton("Play",this);
        gridLayout->addWidget(sourceButtons[i],i + 1,0,Qt::AlignHCenter);
        //buttonGroup->insert(sourceButtons[i]);

        sourceID[i] = new QLabel(this);
        gridLayout->addWidget(sourceID[i],i+1,1,Qt::AlignHCenter);

        sourceTheta[i] = new QLabel(this);
        gridLayout->addWidget(sourceTheta[i],i+1,2,Qt::AlignHCenter);

        sourcePhi[i] = new QLabel(this);
        gridLayout->addWidget(sourcePhi[i],i+1,3,Qt::AlignHCenter);

        sourceStrength[i] = new QLabel(this);
        gridLayout->addWidget(sourceStrength[i],i+1,4,Qt::AlignHCenter);

        sourceButtons[i]->hide();
        sourceID[i]->hide();
        sourceTheta[i]->hide();
        sourcePhi[i]->hide();
        sourceStrength[i]->hide();
    }

    //useful?
    gridLayout->setColStretch(0,20);
    gridLayout->setColStretch(1,20);
    gridLayout->setColStretch(2,20);
    gridLayout->setColStretch(3,20);
    gridLayout->setColStretch(4,20);


    bottomLayout->addLayout(gridLayout);
    Form1Layout->activate();
    
    //FlowDesigner reader
    reader = new FDReader();

    connect(reader, SIGNAL(putData(const std::vector<const SourceInfo*>&)), this, SLOT(getData(const std::vector<const SourceInfo*>&)));
    
    
    //Timed update
    timer = new QTimer(this);      
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));  
    timer->start(500);
}

void TrackAudioWidget::setTime (long long time)
{
    audioView->setExtremas(minTime,time);

    //update timeline selector if required   
    if (audioView->getSelectedTime() >= maxTimeSetted)
    {
        audioView->selectTime(time);
        maxTimeSetted = time;
    }

    msgEdit->clear();

    std::vector<AudioSource> sources = audioView->getSourcesAtSelectedTime();

    for (int i = 0; i < MAX_NUM_SOURCES; i++)
    {
        if (i < sources.size())
        {
            int source_id = sources[i].id;
            float phi = sources[i].phi;
            float theta = sources[i].theta;
            float strength = sources[i].strength;

            sourceButtons[i]->show();
            sourceButtons[i]->setPaletteBackgroundColor(audioView->getSourceColor(source_id));
            sourceButtons[i]->setDisabled(isSourceActive(source_id));
            buttonGroup->insert(sourceButtons[i],source_id);

            sourceID[i]->setText(QString::number(source_id));
            sourceID[i]->show();
            sourceTheta[i]->setText(QString::number(theta));
            sourceTheta[i]->show();
            sourcePhi[i]->setText(QString::number(phi));
            sourcePhi[i]->show();
            sourceStrength[i]->setText(QString::number(strength));
            sourceStrength[i]->show();
        }
        else
        {
            buttonGroup->remove(sourceButtons[i]);
            sourceButtons[i]->hide();
            sourceID[i]->hide();
            sourceTheta[i]->hide();
            sourcePhi[i]->hide();
            sourceStrength[i]->hide();
        }
    }
}

bool TrackAudioWidget::isSourceActive(int id)
{
    
    //TODO : reimplementation...   
    
    std::stringstream sourceStream;//(task.getField(AUDIO_SOURCES_FIELD_STR));

    if (sourceStream.str() != "")
    {
        int nb_sources;

        sourceStream >> nb_sources;

        for (int i = 0; i < nb_sources; i++)
        {
            int source_id = 0;
            float phi,theta = 0;
            float strength = 0;

            sourceStream >> source_id >> phi >> theta >> strength;

            if (source_id == id) return true;
        }
    }
    return false;
}


void TrackAudioWidget::playClicked(int source_id)
{
    std::cerr<<"TrackAudioWidget::playClicked with source_id : "<<source_id<<std::endl;

    //TODO: play file...
    QProcess *process = new QProcess(this);
    process->addArgument("mplayer");
    process->addArgument(QString("log/source_")+QString::number(source_id) + QString(".wav"));
    
	//process->addArgument("localhost");   
    //process->addArgument("2314");          
    
    if (!process->start())
    {
        std::cerr<<"Error playing source : " << source_id<<std::endl;
        delete process;                  
    }      
    
}

void TrackAudioWidget::getData(const std::vector<const SourceInfo*> &sources)
{
    //get the actual time
    long long time = getTime();

    //std::cerr<<"TrackAudioWidget::getData nb sources : "<<sources.size()<<std::endl;

    //add sources
    for (unsigned int i =0 ; i < sources.size(); i++)
    {
        audioView->addSource(time,sources[i]);
    }      

}

void TrackAudioWidget::timeout()
{ 
    setTime(getTime());      
}   

long long TrackAudioWidget::getTime()
{
    timeval thetime;
    gettimeofday(&thetime, 0);
    long long time = thetime.tv_sec * 1000000LL + thetime.tv_usec;
    return time;    
}   


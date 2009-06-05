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
#include <QLabel>
#include <iostream>
#include <QProcess>
#include <QStringList>

#ifdef _HAVE_PHONON_
#include <Phonon>
#endif

#define MAX_NUM_SOURCES 4

using namespace std;

// ---
TrackAudioWidget::TrackAudioWidget(QWidget* parent, bool _startSphinxServers, int basePort)
        : QWidget(parent), maxTimeSetted(0), audioView(NULL), m_sphinxBasePort(basePort)
{
	memset(m_sphinxProcess,0,4 * sizeof(QProcess*));
	
    //Add the layout	    
    Form1Layout = new QVBoxLayout(this);
    this->setLayout(Form1Layout);
	
    audioView = new AudioView(this);
    Form1Layout->addWidget(audioView);

    bottomLayout = new QHBoxLayout(NULL);
    Form1Layout->addLayout(bottomLayout);

    //imageView = new ImageView(this);
    //bottomLayout->addWidget(imageView);

    gridLayout = new QGridLayout(NULL);

    buttonGroup = new QButtonGroup(this);

    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(playClicked(int)));

    gridLayout->addWidget(new QLabel("",this),0,0,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("ID",this),0,1,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("Theta",this),0,2,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("Phi",this),0,3,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("Strength",this),0,4,Qt::AlignHCenter);
    gridLayout->addWidget(new QLabel("Distance",this),0,5,Qt::AlignHCenter);
    
    

    for (int i = 0; i < MAX_NUM_SOURCES; i++)
    {
        sourceButtons[i] = new QPushButton("Play",this);
        gridLayout->addWidget(sourceButtons[i],i + 1,0,Qt::AlignHCenter);

        sourceID[i] = new QLabel(this);
        gridLayout->addWidget(sourceID[i],i+1,1,Qt::AlignHCenter);

        sourceTheta[i] = new QLabel(this);
        gridLayout->addWidget(sourceTheta[i],i+1,2,Qt::AlignHCenter);

        sourcePhi[i] = new QLabel(this);
        gridLayout->addWidget(sourcePhi[i],i+1,3,Qt::AlignHCenter);

        sourceStrength[i] = new QLabel(this);
        gridLayout->addWidget(sourceStrength[i],i+1,4,Qt::AlignHCenter);
        
        sourceDistance[i] = new QLabel(this);
        gridLayout->addWidget(sourceDistance[i],i+1,5,Qt::AlignHCenter);

        //sourceButtons[i]->hide();
        //sourceID[i]->hide();
        //sourceTheta[i]->hide();
        //sourcePhi[i]->hide();
        //sourceStrength[i]->hide();
        //sourceDistance[i]->hide();
    }
   
    m_cameraView = new SNCRZ30View(this);
    bottomLayout->addWidget(m_cameraView);
    
    bottomLayout->addLayout(gridLayout);
    Form1Layout->activate();
    
    //Selection will update buttons...
    connect(audioView, SIGNAL(timeSelected(unsigned long long)), this, SLOT(selectedTime(unsigned long long)));
    
    //Timed update
    timer = new QTimer(this);      
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));  
    timer->start(500);
    
    
    //Camera update
    m_cameraTimer = new QTimer(this);
    connect(m_cameraTimer,SIGNAL(timeout()), this, SLOT(cameraTimeout()));
    m_cameraTimer->start(1000);
    
    
    if (_startSphinxServers)
    {
    	startSphinxServers(m_sphinxBasePort);
    }
    
}

void TrackAudioWidget::cameraTimeout()
{
	//std::cerr<<"void TrackAudioWidget::cameraTimeout()"<<std::endl;
	//double theta = 180.0 * atan2((*info).x[1],(*info).x[0]) / M_PI;
    std::vector<AudioSource> sources = audioView->getSourcesAtTime(audioView->getCurrentTime() - 1000000);
    
    //std::cerr<<"source size : "<<sources.size()<<std::endl;
    
    if (sources.size() > 0)
    {
    	m_cameraView->getCapture()->panTilt(sources[0].m_theta*10.0,sources[0].m_phi * 10.0 - 425);
    }
}

void TrackAudioWidget::startSphinxServers(int basePort)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		m_sphinxProcess[i] = new QProcess(this);
		
		QString command = "java";
		
		QStringList args;
		args.append("-classpath");
		args.append(".:sphinx4.jar:TIDIGITS_8gau_13dCep_16k_40mel_130Hz_6800Hz.jar");
		args.append("Sphinx4TcpServer");
		args.append("-p");
		args.append(QString::number(basePort + i));
		
		//TODO Set working directory 
		m_sphinxProcess[i]->setWorkingDirectory(SPHINX_INSTALL_DIR);
		
		//connect process signals
		connect(m_sphinxProcess[i],SIGNAL(error(QProcess::ProcessError)),this,SLOT(sphinxProcessError(QProcess::ProcessError)));
		connect(m_sphinxProcess[i],SIGNAL(started()),this,SLOT(sphinxProcessStarted()));
		connect(m_sphinxProcess[i],SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(sphinxProcessFinished(int,QProcess::ExitStatus)));
		
		//StdErr & StdOut redirections
		connect(m_sphinxProcess[i],SIGNAL(readyReadStandardError()),this,SLOT(sphinxProcessReadStdErr()));
		connect(m_sphinxProcess[i],SIGNAL(readyReadStandardOutput()),this,SLOT(sphinxProcessReadStdOut()));
		
		
		//start the process
		m_sphinxProcess[i]->start(command,args);
	}
}

TrackAudioWidget::~TrackAudioWidget()
{
	
}

void TrackAudioWidget::sphinxProcessError(QProcess::ProcessError error )
{
	switch(error)
	{

		case QProcess::FailedToStart:
			cerr<<"Sphinx Process failed to start"<<endl;
			break;
			
		case QProcess::Crashed:
			cerr<<"Sphinx Process crashed"<<endl;
			break;
			
		case QProcess::Timedout:
			cerr<<"Sphinx Process timed out"<<endl;
			break;
			
		case QProcess::WriteError:
			cerr<<"Sphinx Process write error"<<endl;
			break;
			
		case QProcess::ReadError:
			cerr<<"Sphinx Process read error"<<endl;
			break;
			
		case QProcess::UnknownError:
			cerr<<"Sphinx Process unknown error"<<endl;
			break;

	}
}

void TrackAudioWidget::sphinxProcessStarted()
{
	cout<<"Sphinx process started"<<endl;
	
}

void TrackAudioWidget::sphinxProcessFinished( int exitCode, QProcess::ExitStatus exitStatus)
{
	cout<<"Sphinx process finished with exit code : "<<exitCode<<" and exit status : "<<exitStatus<<endl;
	
}

void TrackAudioWidget::sphinxProcessReadStdErr ()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		QByteArray array = m_sphinxProcess[i]->readAllStandardError ();
		
		if (array.size() > 0)
		{
			QString errorString(array);
			cerr<<errorString.toStdString()<<endl;
		}
	}
}


void TrackAudioWidget::sphinxProcessReadStdOut()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		QByteArray array = m_sphinxProcess[i]->readAllStandardOutput ();
		
		if (array.size() > 0)
		{
			QString recogData(array);
			
			if (recogData.contains("RESULT:"))
			{	
				audioView->addRecog(getTime(), QString(array), i);
			}
			
			//cout << "Got recog : "<<QString(array).toStdString()<<endl;	
		}
	}
}


void TrackAudioWidget::setRecogString(int sourceID, QString recogString)
{
	m_recogString[sourceID] = recogString;
}

void TrackAudioWidget::selectedTime(unsigned long long time)
{

    if (audioView)
    {

	    std::vector<AudioSource> sources = audioView->getSourcesAtSelectedTime();

	    for (int i = 0; i < MAX_NUM_SOURCES; i++)
	    {
		    if (i < sources.size())
		    {
		        int source_id = sources[i].m_id;
		        float phi = sources[i].m_phi;
		        float theta = sources[i].m_theta;
		        float strength = sources[i].m_strength;
		        float distance = sources[i].m_distance;

		        sourceButtons[i]->show();
		        
			
		        //TODO FIX THIS
		        sourceButtons[i]->setDisabled(isSourceActive(source_id));

		        sourceButtons[i]->setPalette(audioView->getSourceColor(source_id));
		        sourceButtons[i]->setForegroundRole(QPalette::Button);
		        buttonGroup->addButton(sourceButtons[i],source_id);

		        sourceID[i]->setText(QString::number(source_id));
		        sourceID[i]->show();
		        sourceTheta[i]->setText(QString::number(theta));
		        sourceTheta[i]->show();
		        sourcePhi[i]->setText(QString::number(phi));
		        sourcePhi[i]->show();
		        sourceStrength[i]->setText(QString::number(strength));
		        sourceStrength[i]->show();
		        sourceDistance[i]->setText(QString::number(distance));
		        sourceDistance[i]->show();
		    }
		    else
		    {
		        buttonGroup->removeButton(sourceButtons[i]);
		        sourceButtons[i]->setDisabled(true);
		        sourceID[i]->setText("");
		        sourceTheta[i]->setText("");
		        sourcePhi[i]->setText("");
		        sourceStrength[i]->setText("");
		        sourceDistance[i]->setText("");
				
		    }
	    }
	}

}

void TrackAudioWidget::setTime (unsigned long long time)
{

    if (audioView)
    {
	    //debug
	    //add a source
        //audioView->addSource(time,AudioSource(1,0.5,0.5,0.5));
        //audioView->addSource(time,AudioSource(2,10.5,10.5,10.5));
        //audioView->addSource(time,AudioSource(3,20.5,20.5,20.5));
        //audioView->addSource(time,AudioSource(4,30.5,30.5,30.5));
  
	    //update timeline selector if required   
	    if (audioView->getSelectedTime() >= maxTimeSetted)
	    {
		    audioView->selectTime(time);
		    maxTimeSetted = time;
	    }

	   
	}
}

bool TrackAudioWidget::isSourceActive(int id)
{
    if (audioView)
    {
        std::vector<AudioSource> sources = audioView->getSourcesAtTime(getTime() - (unsigned long long)(1E6));
        bool found = false;
        for (unsigned i = 0; i< sources.size(); i++)
        {
            if (sources[i].m_id == id)
            {
                found = true;
                break;
            }                
        }        
        return found;
    }
    else
    {
        return false;
    }    
}


void TrackAudioWidget::playClicked(int source_id)
{
    std::cerr<<"TrackAudioWidget::playClicked with source_id : "<<source_id<<std::endl;

#ifdef _HAVE_PHONON_
	Phonon::MediaObject *music =
	Phonon::createPlayer(Phonon::MusicCategory,
						 Phonon::MediaSource(QString("log/source_") + QString::number(source_id) + QString(".wav")));
	music->play();

#else

						 
	QProcess *process = new QProcess(this);
	QString prog;
	QStringList args;						 
						 
 #ifdef WIN32
    prog = QString("sndrec32.exe");
    args += QString("/play");
    args += QString("/close");
    args += QString("log/source_")+QString::number(source_id) + QString(".wav");
 #endif
    
#ifdef linux
    prog = QString("mplayer");
    args += QString("log/source_")+QString::number(source_id) + QString(".wav");
#endif
    
#ifdef __APPLE_CC__
    prog = QString("open");
    args += QString("log/source_")+QString::number(source_id) + QString(".wav");
#endif
    
    if (process)
    {        
        process->start(prog,args);                
    }
						 
#endif
 
}

void TrackAudioWidget::getData(FD::RCPtr<FD::Vector<FD::ObjectRef> > sources)
{
    //get the actual time
    long long time = getTime(); 
    
    //add sources
    for (unsigned int i =0 ; i < sources->size(); i++)
    {
    	FD::RCPtr<SourceInfo> info = (*sources)[i];   
        audioView->addSource(time,info.get());
    }      

}


void TrackAudioWidget::sourceReady(AudioSource source)
{
	//qDebug("TrackAudioWidget::sourceReady(AudioSource source)");
	long long time = getTime(); 
	audioView->addSource(time,source);
}

void TrackAudioWidget::timeout()
{ 
    setTime(getTime());      
}   

unsigned long long TrackAudioWidget::getTime()
{
    timeval thetime;
    gettimeofday(&thetime, 0);
    long long time = thetime.tv_sec * 1000000LL + thetime.tv_usec;
    return time;    
}   


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
#include "FDReader.h"
#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include "UINodeRepository.h"
#include "ObjectRef.h"
#include "path.h"
#include "iextensions.h"

void printusage(){
 std::cout << "audioviewer <port>" << std::endl << std::endl
           << std::endl << std::endl;
}


int main( int argc, char **argv)
{
    
//load FD toolboxes
//    FD::IExtensions::detect();
//    FD::scanDL();
//    FD::UINodeRepository::Scan();

    
    //QApplication::setColorSpec( QApplication::CustomColor );
    QApplication app( argc, argv );

	QMainWindow mainWindow;
	FDReader fdReader;
	TrackAudioWidget audioview(NULL,true);
	
	//FlowDesigner SourceInfo reader
    QObject::connect(&fdReader, SIGNAL(putData(FD::RCPtr<FD::Vector<FD::ObjectRef> >)), &audioview, SLOT(getData(FD::RCPtr<FD::Vector<FD::ObjectRef> >)));
	 QObject::connect(&fdReader, SIGNAL(sourceReady(AudioSource)), &audioview, SLOT(sourceReady(AudioSource)));
	
	mainWindow.setCentralWidget(&audioview);

    app.setActiveWindow(&mainWindow);
    mainWindow.resize(1024, 768);
    mainWindow.show();
    return app.exec();
}

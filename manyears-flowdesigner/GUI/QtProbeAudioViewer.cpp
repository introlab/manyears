
#include "QtProbeAudioViewer.h"
#include "TrackAudioWidget.h"

#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <string>
#include <sstream>

using namespace FD;

DECLARE_PROBE(QtProbeAudioViewer)
DECLARE_PROBE_ALLOWED_DATA_TYPE(QtProbeAudioViewer, "Vector<ObjectRef>")

QtProbeAudioViewer::QtProbeAudioViewer(QWidget *parent, const QString &processHost, const int &processPort, const int &linkId)
	: QtProbe(parent, processHost, processPort, linkId)
{
}

QtProbeAudioViewer::~QtProbeAudioViewer()
{
}

void QtProbeAudioViewer::setupUi()
{
	this->setWindowTitle(tr("AudioViewer - Link %1").arg(m_linkId));
	this->resize(1024, 768);
    
    TrackAudioWidget* trackAudioWidget = new TrackAudioWidget(this);
	this->setCentralWidget(trackAudioWidget);
	
	QObject::connect(this, SIGNAL(putData(FD::RCPtr<FD::Vector<FD::ObjectRef> >)), trackAudioWidget, SLOT(getData(FD::RCPtr<FD::Vector<FD::ObjectRef> >)));
}

void QtProbeAudioViewer::dataReceived(const QByteArray &bytes)
{
	const char* data = bytes.data();
	int size = bytes.size();
	std::stringstream inputStream;
    inputStream.write(data,size);

    try {                                      
        FD::RCPtr<FD::Vector<FD::ObjectRef> > fdData;
        inputStream >> fdData;
        //Will send signal putData
        //Object will self destruct when unused
        emit putData(fdData);      
    }
    catch(FD::BaseException *e)
    {
        e->print(std::cerr);      
        delete e;         
    }
    catch(...)
    {
        std::cerr<<"Unknown exception in :"<<__FILE__<<":"<<__LINE__<<std::endl;
    }
}

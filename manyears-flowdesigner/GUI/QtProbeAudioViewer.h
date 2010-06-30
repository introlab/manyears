#ifndef QTPROBEAUDIOVIEWER_H_
#define QTPROBEAUDIOVIEWER_H_

#include "QtProbe.h"
#include "Vector.h"

using namespace FD;

class QtProbeAudioViewer : public QtProbe
{
	Q_OBJECT;
	
	public:
		QtProbeAudioViewer(QWidget *parent, const QString &processHost, const int &processPort, const int &linkId);
		~QtProbeAudioViewer();
		
		// inherited class must calls QtProbe::stop() at the end of this method
		virtual void stop() {QtProbe::stop();}
		
	public slots:
		// inherited class must calls QtProbe::connected() at the end of this method
	 	virtual void connected() {QtProbe::connected();}
	 	// inherited class must calls QtProbe::error(socketError) at the end of this method
	 	virtual void error(QAbstractSocket::SocketError socketError) {QtProbe::error(socketError);}
	 	
	signals:
    	void putData(FD::RCPtr<FD::Vector<FD::ObjectRef> > sources);
	 	
	protected:
		virtual void dataReceived(const QByteArray &data);
		
	private:
		virtual void setupUi();
};

#endif /*QTPROBEAUDIOVIEWER_H_*/

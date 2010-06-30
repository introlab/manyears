#ifndef _CAMERA_BRIDGE_H_
#define _CAMERA_BRIDGE_H_

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "qextserialport.h"
#include <QTimer>

class CameraBridge : public QMainWindow
{
	Q_OBJECT;

public:
	CameraBridge (QWidget *parent);
	~CameraBridge();

public slots:
	void updateTimeout();
	void absolutePanTilt(int pan_position, int tilt_position,int pan_speed=24, int tilt_speed=20);
	void bytesSent(qint64 bytes);
	void readyRead();

protected:
	void auto_pan_tilt_speed_off();
	void inquiry_pan_tilt();
	Ui::MainWindow *m_uiMainWindow;
	QextSerialPort *m_serialPort;
	QTimer *m_updateTimer;
	bool m_connected;
};



#endif


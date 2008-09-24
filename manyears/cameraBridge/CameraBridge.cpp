#include "CameraBridge.h"
#include "qextserialenumerator.h"
#include <QByteArray>
#include <iostream>
#include <string>

using namespace std;

#define PAN_TILT_UNIT_CONVERTION_FROM_CAMERA 0.6944
#define PAN_TILT_UNIT_CONVERTION_TO_CAMERA 1.44


CameraBridge::CameraBridge (QWidget *parent)
	:	QMainWindow(parent)
{
	m_uiMainWindow = new Ui::MainWindow();
	m_uiMainWindow->setupUi(this);

	PortSettings s;
	s.BaudRate = BAUD19200;
	s.DataBits =  DATA_8;
	s.FlowControl = FLOW_OFF;
	s.Parity = PAR_NONE;
	s.StopBits = STOP_1;
	s.Timeout_Millisec = 1000;
	m_serialPort = new QextSerialPort("/dev/tty.usbserial", s);

	m_connected = m_serialPort->open(QIODevice::ReadWrite);

	if (m_connected)
	{
		auto_pan_tilt_speed_off();
		m_uiMainWindow->textEdit->setText("Open COM success");
		connect(m_serialPort,SIGNAL(bytesWritten(qint64)),this,SLOT(void bytesSent(qint64 )));
		connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(readyRead()));
	}

	//m_serialPort->open();
	m_updateTimer = new QTimer(this);
	connect(m_updateTimer,SIGNAL(timeout()), this, SLOT(updateTimeout()));
	m_updateTimer->start(1000);
}

CameraBridge::~CameraBridge()
{
	cerr<<"CameraBridge::~CameraBridge()"<<endl;
	m_serialPort->close();
}

void CameraBridge::updateTimeout()
{
	cerr<<"void CameraBridge::updateTimeout()"<<endl;
	absolutePanTilt(1400,0);
	inquiry_pan_tilt();
}

void CameraBridge::absolutePanTilt(int pan_position, int tilt_position, int pan_speed, int tilt_speed)
{
	if (m_connected)
	{
		string message;

		int Sep1 = 0x000F;
		int Sep2 = 0x00F0;
		int Sep3 = 0x0F00;
		int Sep4 = 0xF000;

		message += 0x81;
		message += 0x01;
		message += 0x06;
		message += 0x02;

		message += max(0,min(pan_speed,24));
		message += max(0,min(tilt_speed,20));

		pan_position = max(min(pan_position,1700), -1700);
		tilt_position = max(min(tilt_position,425), -575);

		//CONVERSION REQUIRED TO CAMERA UNITS
		pan_position =  (int) (PAN_TILT_UNIT_CONVERTION_TO_CAMERA * (float) pan_position + 0.5);
		tilt_position = (int) (PAN_TILT_UNIT_CONVERTION_TO_CAMERA * (float) tilt_position + 0.5);

		//pan
		message += (Sep4 & pan_position)>>12;
		message += (Sep3 & pan_position)>>8;
		message += (Sep2 & pan_position)>>4;
		message += (Sep1 & pan_position);


		//tilt
		message += (Sep4 & tilt_position)>>12;
		message += (Sep3 & tilt_position)>>8;
		message += (Sep2 & tilt_position)>>4;
		message += (Sep1 & tilt_position);

		//end message
		message += 0xFF;

		cerr<<"Writing : "<<pan_position<<" , "<<tilt_position<<endl;
		m_serialPort->write(message.c_str(),message.size());

	}
}

void CameraBridge::bytesSent( qint64 bytes )
{
	cerr<<"CameraBridge::bytesWritten "<<bytes<<endl;

}

void CameraBridge::readyRead()
{
	cerr<<"void CameraBridge::readyRead()"<<endl;
}

void CameraBridge::auto_pan_tilt_speed_off()
{
	if (m_connected)
	{
		//pan + tilt request
		string AutoOFF;
		AutoOFF += 0x81;
		AutoOFF += 0x01;
		AutoOFF += 0x06;
		AutoOFF += 0x24;
		AutoOFF += 0x03;
		AutoOFF += 0xFF;

		m_serialPort->write(AutoOFF.c_str(),AutoOFF.size());

	}


}

void CameraBridge::inquiry_pan_tilt()
{
	if (m_connected)
	{
		//cerr<<"inquiry pan tilt"<<endl;
		//pan + tilt request
		string PanTiltMessage;
		PanTiltMessage += 0x81;
		PanTiltMessage += 0x09;
		PanTiltMessage += 0x06;
		PanTiltMessage += 0x12;
		PanTiltMessage += 0xFF;

		m_serialPort->write(PanTiltMessage.c_str(),PanTiltMessage.size());
	}
}



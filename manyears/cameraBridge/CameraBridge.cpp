#include "CameraBridge.h"

CameraBridge::CameraBridge (QWidget *parent)
	:	QMainWindow(parent)
{
	m_uiMainWindow = new Ui::MainWindow();
	m_uiMainWindow->setupUi(this);

	PortSettings s;
	s.BaudRate = BAUD38400;
	s.DataBits =  DATA_8;
	s.FlowControl = FLOW_OFF;
	s.Parity = PAR_NONE;
	s.StopBits = STOP_1;
	s.Timeout_Millisec = 1000;
	m_serialPort = new QextSerialPort("COM3", s);

}

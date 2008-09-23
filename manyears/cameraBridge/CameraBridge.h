#ifndef _CAMERA_BRIDGE_H_
#define _CAMERA_BRIDGE_H_

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "qextserialport.h"

class CameraBridge : public QMainWindow
{
	Q_OBJECT;

public:
	CameraBridge (QWidget *parent);

protected:
	Ui::MainWindow *m_uiMainWindow;
	 QextSerialPort *m_serialPort;
};



#endif


#include "CameraBridge.h"

CameraBridge::CameraBridge (QWidget *parent)
	:	QMainWindow(parent) 
{
	m_uiMainWindow = new Ui::MainWindow();
	m_uiMainWindow->setupUi(this);
}		
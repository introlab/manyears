#include <QApplication>
#include "CameraBridge.h"

int main(int argc, char* argv[])
{
	QApplication app(argc,argv, true);

	CameraBridge bridge(NULL);

	bridge.show();

	return app.exec();
}

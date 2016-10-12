#include <QApplication>
#include "MainWindow.h"
#include "keyconfirm.hpp"

////Hide the cmd
//#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
 
#define PLAQUE_QUANT_VERSION "1.0"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	/// Check license
	keyConfirm keyconfirm;
	// if license not found, display ID and stop process
	if (!keyconfirm.checkLicence()) {
		keyconfirm.setWindowTitle(QString("Plaque Quant - v") + QString(PLAQUE_QUANT_VERSION) + QString(" License Check"));
		keyconfirm.show();
		return app.exec();
	}

	MainWindow mainWnd;
	mainWnd.setWindowTitle("Plaque Quant - v" + QString::fromStdString(PLAQUE_QUANT_VERSION));
	mainWnd.SetVersion(PLAQUE_QUANT_VERSION);
	mainWnd.show();

	if (argc == 2)
	{
		QString folder = argv[1];
		folder.replace("\\", "/");
		mainWnd.slotOpenImage(folder);
	}

	return app.exec();
}

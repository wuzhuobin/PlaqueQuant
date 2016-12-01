#include <QApplication>
#include "keyconfirm.hpp"
#include "MainWindow.h"

////Hide the cmd
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
 
#define PLAQUEQUANT_VERSION "2.0"

int main( int argc, char** argv )
{
  QApplication app( argc, argv );

  /// Check license
  //keyConfirm keyconfirm;
  //// if license not found, display ID and stop process
  //if (!keyconfirm.checkLicence()) {
	 // keyconfirm.setWindowTitle(QString("Plaque Quant v") + QString(PLAQUEQUANT_VERSION) + QString(" License Check"));
	 // keyconfirm.show();
	 // return app.exec();
  //}
  vtkObject::SetGlobalWarningDisplay(false);
  vtkObject::SetGlobalWarningDisplay(true);
  MainWindow mainWnd;
  mainWnd.SetVersion(PLAQUEQUANT_VERSION);
  mainWnd.setWindowTitle(QString("Plaque Quant v") + QString(PLAQUEQUANT_VERSION));
  mainWnd.show();

  if (argc==2)
  {
	  QString folder = argv[1];
	  folder.replace("\\","/");
	  //mainWnd.slotOpenImage(folder);
  }

  return app.exec();
}

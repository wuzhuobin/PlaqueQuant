#include <QLibraryInfo>
#include <QApplication>
#include "EncryptionAuthentication.h"
#include "MainWindow.h"

////Hide the cmd
//#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
 
#define PLAQUEQUANT_VERSION "2.0"

int main( int argc, char** argv )
{
  QApplication app( argc, argv );

  MainWindow mainWnd;
  mainWnd.SetVersion(PLAQUEQUANT_VERSION);
  mainWnd.setWindowTitle(QString("Plaque Quant v") + QString(PLAQUEQUANT_VERSION));
  int status = 0;

  EncryptionAuthentication ea(300033583, "PQ543288", "PlaqueQuant",
	  QDateTime(QDate(2017, 1, 1),
		  QTime(0, 0, 0)));
	if (ea.authenticationExec(
		EncryptionAuthentication::EXPIRED_DATE_TIME |
		EncryptionAuthentication::PRODUCT_NAME|
		EncryptionAuthentication::SOFT_ID|
		EncryptionAuthentication::HAVING_KEY
	)) {
	//if (ea.authenticationExec(EncryptionAuthentication::NORMAL)) {
	  mainWnd.show();
	  if (argc == 2)
	  {
		  QString folder = argv[1];
		  folder.replace("\\", "/");
		  //mainWnd.slotOpenImage(folder);
	  }
	  status = app.exec();
  }

  return status;
}

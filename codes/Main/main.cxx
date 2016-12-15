#include <QApplication>
#include "MainWindow.h"
#include "EncryptionAuthentication.h"

// error output
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
extern const int WARNING = 0;
extern const int ENCRYPTION_AUTHENTICATION_DISABLED = 0;
#else
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
extern const int WARNING = 1;
extern const int ENCRYPTION_AUTHENTICATION_DISABLED = 1;
#endif // !_DEBUG
 

extern const char* PLAQUEQUANT_VERSION = "2.0";

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


  vtkObject::SetGlobalWarningDisplay(WARNING);
  MainWindow mainWnd;
  mainWnd.SetVersion(PLAQUEQUANT_VERSION);
  mainWnd.setWindowTitle(QString("Plaque Quant v") + QString(PLAQUEQUANT_VERSION));

  EncryptionAuthentication ea(0, QString(), QString(),
	  QDateTime(QDate(2017, 01, 10),
		  QTime(24, 0, 0)),
	  "68686868");

  ea.enableExpiredDateTimeHint(false);

  if (ENCRYPTION_AUTHENTICATION_DISABLED) {
	  mainWnd.show();
	  return app.exec();
  }
  else if (!ENCRYPTION_AUTHENTICATION_DISABLED &&
	  ea.authenticationExecAndKeyType(
	  EncryptionAuthentication::HAVING_KEY |
	  EncryptionAuthentication::USER_PASSWORD |
	  EncryptionAuthentication::EXPIRED_DATE_TIME) != EncryptionAuthentication::NORMAL) {
	  return EXIT_FAILURE;
  }
  else {
	  //if (argc == 2)
	  //{
		 // qstring folder = argv[1];
		 // folder.replace("\\", "/");
		 // //mainwnd.slotopenimage(folder);
	  //}
	  mainWnd.show();
	  return app.exec();
  }




}

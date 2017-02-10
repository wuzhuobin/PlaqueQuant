#include <QApplication>
#include "MainWindow.h"
#include "EncryptionAuthentication.h"


//#undef _DEBUG


// error output
#ifdef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
extern const bool WARNING = false;
extern const bool ENCRYPTION_AUTHENTICATION_DISABLED = false;
#else
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
extern const bool WARNING = true;
extern const bool ENCRYPTION_AUTHENTICATION_DISABLED = true;
#endif // !_DEBUG
 

extern const char* PLAQUEQUANT_VERSION = "2.0";

int main( int argc, char** argv )
{
  QApplication app( argc, argv );

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
	  // For debug only
	  //QString img = "C:/Users/jieji/Desktop/work/PlaqueQuant/JackyData/nifti_corrected/CUBE T1 corrected.nii";
	/*  QString img = "E:/Andy/blood_vessel_v_1.0.0/Data/JackyData/"
		  "nifti_corrected/CUBE_T1_corrected.nii";
	  img.replace('\\', '/');*/
	  //mainWnd.openOneImage(img);
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

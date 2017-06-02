
#include "Core.h"
#include "MainWindow.h"
#include "EncryptionAuthentication.h"
#include <QtWidgets/QApplication>

#define _BUILD_A
//#define _BUILD_B
//#define _DEBUG

//
// error output
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
extern const bool WARNING = true;
extern const bool ENCRYPTION_AUTHENTICATION_DISABLED = false;
#else
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
extern const bool WARNING = false;
extern const bool ENCRYPTION_AUTHENTICATION_DISABLED = true;
#endif // !_DEBUG

#define PLAQUEQUANT_VERSION "3.0"


int main(int argc, char *argv[])
{
	vtkObject::SetGlobalWarningDisplay(WARNING);
	itk::Object::SetGlobalWarningDisplay(WARNING);

	QApplication a(argc, argv);
	
#ifdef _BUILD_A
	EncryptionAuthentication ea(0, QString(), QString(),
		QDateTime(QDate(2017, 9, 30),
			QTime(24, 0, 0)),
		"39431068");
#endif
#ifdef _BUILD_B
	EncryptionAuthentication ea(0, QString(), QString(),
		QDateTime(QDate(2017, 06, 30),
			QTime(24, 0, 0)),
		"68686868");
#endif

	ea.enableExpiredDateTimeHint(false);

	if (ENCRYPTION_AUTHENTICATION_DISABLED) {


		Core c;
		// For debug only
		//QString img = "C:/Users/jieji/Desktop/work/PlaqueQuant/JackyData/nifti_corrected/CUBE T1 corrected.nii";
		/*  QString img = "E:/Andy/blood_vessel_v_1.0.0/Data/JackyData/"
		"nifti_corrected/CUBE_T1_corrected.nii";
		img.replace('\\', '/');*/
		//mainWnd.openOneImage(img);
		return a.exec();
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
		Core c;
		return a.exec();
	}


	return 0;
}

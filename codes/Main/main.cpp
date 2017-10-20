
#include "EncryptionAuthentication.h"
#include "Core.h"
#include "MainWindow.h"
#include <QtWidgets/QApplication>

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")


int main(int argc, char *argv[])
{
	itk::Object::GlobalWarningDisplayOff();
	vtkObject::GlobalWarningDisplayOff();



		QApplication app(argc, argv);
	// MainWindow mainWind;
	EncryptionAuthentication ea(0, QString(), QString(),
		QDateTime(QDate(2018, 12, 31),
			QTime(24, 0, 0)), 
		"68686868");

	// disable reminder of the expire date
	ea.enableExpiredDateTimeHint(false);
	// enable reminder of the expire date
	//ea.enableExpiredDateTimeHint(true);


	//if (ea.authenticationExec(
	//	EncryptionAuthentication::HAVING_KEY |
	//	EncryptionAuthentication::EXPIRED_DATE_TIME) != EncryptionAuthentication::NORMAL) {
	if (ea.authenticationExecAndKeyType(
		EncryptionAuthentication::HAVING_KEY |
		EncryptionAuthentication::USER_PASSWORD |
		EncryptionAuthentication::EXPIRED_DATE_TIME) != EncryptionAuthentication::NORMAL) {
		return EXIT_FAILURE;
	}
	else {
		Core c;
		// mainWnd.show(); 
		return app.exec();
	}

}

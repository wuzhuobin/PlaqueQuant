#include <QApplication>
#include <qdebug.h>

#include "EncryptionAuthentication.h"

#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	// MainWindow mainWind;
	EncryptionAuthentication ea(0, QString(), QString(),
		QDateTime(QDate(2017, 01, 10),
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
		// mainWnd.show(); 
		return app.exec();
	}
}
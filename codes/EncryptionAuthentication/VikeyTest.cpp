#include <QApplication>
#include <qdebug.h>

#include "EncryptionAuthentication.h"

#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")

void main(int argc, char** argv) {
	QApplication app(argc, argv);
	EncryptionAuthentication ea(0, QString(), QString(),
		QDateTime(QDate(2016, 12, 12),
			QTime(24, 0, 0)));

	ea.enableExpiredDateTimeHint(true);
	qDebug() << ea.authenticationExecAndKeyType(
		EncryptionAuthentication::HAVING_KEY|
		EncryptionAuthentication::EXPIRED_DATE_TIME
	);

	app.exec();
}
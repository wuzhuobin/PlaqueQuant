#include <QApplication>
#include <qdebug.h>

#include "EncryptionAuthentication.h"

#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")

void main(int argc, char** argv) {
	QApplication app(argc, argv);
	EncryptionAuthentication ea(300033583, "PQ543288", "PlaqueQuant",
		QDateTime(QDate(2016, 12, 12),
			QTime(24, 0, 0)));


	qDebug() << ea.authenticationExec(
		EncryptionAuthentication::HAVING_KEY|
		EncryptionAuthentication::EXPIRED_DATE_TIME|
		EncryptionAuthentication::PRODUCT_NAME |
		EncryptionAuthentication::NORMAL
	);

	app.exec();
}
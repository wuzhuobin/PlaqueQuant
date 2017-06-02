#include "RegistrationWizard.h"

#include <qdebug.h>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RegistrationWizard rw("C:/Users/jieji/Desktop/work/PlaqueQuant/JackyData", 2);
	rw.setImageModalityNames(0, "T2 image ");
	rw.setImageModalityNames(1, "MRA image");

	
	if (QWizard::Accepted == rw.exec()) {


		qDebug() << rw.getFileNames(0);
		qDebug() << rw.getFileNames(1);
		qDebug() << rw.getDirectory();

	}
	return a.exec();
}


#include "Core.h"
#include "MainWindow.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
	//itk::Object::GlobalWarningDisplayOff();
	//vtkObject::GlobalWarningDisplayOff();
	QApplication a(argc, argv);
	Core c;
	return a.exec();
}

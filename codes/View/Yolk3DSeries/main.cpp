#include "Yolk3DSeries.h"

#include <QtWidgets/QApplication>
int main(int argc, char** argv)
{
	QApplication app(argc, argv);




	Yolk3DSeries y;
	y.show();

	return app.exec();
}
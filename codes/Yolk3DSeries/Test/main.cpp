#include "Yolk3DSeries.h"

#include <QApplication>
#include <QVector>
#include <QSpinBox>
#include <QGridLayout>
#include <QWidget>
#include <QObject>
#include <QDebug>

#include "MainWindow.h"

#include <itkGDCMSeriesFileNames.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: \n";
		std::cout << "\t Yolk3DSeries.exe [directory]\n";
		return 1;
	}

	QApplication app(argc, argv);

	/* Use GDCMSeriesFileNames to read all files a the argument directory*/
	typedef itk::GDCMSeriesFileNames	InputNamesGeneratorType;
	InputNamesGeneratorType::Pointer inputnamesGenerator = InputNamesGeneratorType::New();
	inputnamesGenerator->SetInputDirectory(argv[1]);

	/* Now excepts QStringList only */
	std::vector<std::string> filenames(inputnamesGenerator->GetInputFileNames());
	QStringList qfilenames;
	for (std::vector<std::string>::const_iterator iter = filenames.cbegin();
		iter != filenames.cend(); iter ++)
	{
		qfilenames.append(QString::fromStdString(*iter));
	}

	Yolk3DSeries* y = new Yolk3DSeries();

	/* Create a spinbox to control which 3D projection slice */
	QSpinBox* spinbox = new QSpinBox(y);
	y->layout()->addWidget(spinbox);
	spinbox->setMinimum(0);
	spinbox->setMaximum(qfilenames.length() - 1);
	QObject::connect(spinbox, SIGNAL(valueChanged(int)), y, SLOT(slotSetSlice(int)));

	y->show();
	y->set3DSeries(qfilenames);

	/* Connect MyViewer object signal to the yolk widget */
	MainWindow* mainwnd = new MainWindow;
	y->connectViewer(mainwnd->GetCore()->Get2DViewer(0));
	y->connectViewer(mainwnd->GetCore()->Get2DViewer(1));
	y->connectViewer(mainwnd->GetCore()->Get2DViewer(2));

	mainwnd->show();

	return app.exec();
}
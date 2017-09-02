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

	MainWindow* mainwnd = new MainWindow;

	typedef itk::GDCMSeriesFileNames	InputNamesGeneratorType;
	InputNamesGeneratorType::Pointer inputnamesGenerator = InputNamesGeneratorType::New();
	inputnamesGenerator->SetInputDirectory(argv[1]);

	std::vector<std::string> filenames(inputnamesGenerator->GetInputFileNames());
	QStringList qfilenames;
	for (std::vector<std::string>::const_iterator iter = filenames.cbegin();
		iter != filenames.cend(); iter ++)
	{
		qfilenames.append(QString::fromStdString(*iter));
	}
	qfilenames.sort();


	QWidget* tempwidget = new QWidget;
	QGridLayout* layout = new QGridLayout(tempwidget);
	QSpinBox* spinbox = new QSpinBox(tempwidget);
	spinbox->setMinimum(0);
	spinbox->setMaximum(qfilenames.length() - 1);
	layout->addWidget(spinbox);

	Yolk3DSeries* y = new Yolk3DSeries();
	y->show();
	tempwidget->show();

	double normal[3] = { 1, 1, 1 };
	double origin[3] = { 0, 0, 10 };
	y->set3DSeries(qfilenames);

	y->connectViewer(mainwnd->GetCore()->Get2DViewer(0));
	y->connectViewer(mainwnd->GetCore()->Get2DViewer(1));
	y->connectViewer(mainwnd->GetCore()->Get2DViewer(2));

	mainwnd->show();

	QObject::connect(spinbox, SIGNAL(valueChanged(int)), y, SLOT(slotSetSlice(int)));
	return app.exec();
}
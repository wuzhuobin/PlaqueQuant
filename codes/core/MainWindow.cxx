#include "MainWindow.h"

MainWindow::MainWindow(QMainWindow* parent) 
	:QMainWindow(parent), group1(parent), group2(parent)
{
	ui.setupUi(this);

	this->setWindowTitle("PlaqueQuant");


	// Set up action signals and slots
	// actionOpenImage, actionExit, actionAbout, actionHelp
	connect(ui.actionOpenImage, SIGNAL(triggered()), this, SLOT(slotOpenImage()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(slotHelp()));
	// action group 1 
	group1.addAction(ui.actionNavigation);
	group1.addAction(ui.actionWindowLevel);
	group1.addAction(ui.actionRuler);
	group1.addAction(ui.actionROI);
	group1.addAction(ui.actionContour);
	group1.addAction(ui.actionBrush);
	group1.setExclusive(true);

	connect(ui.actionNavigation, SIGNAL(triggered()), this, SLOT(slotNavigationMode()));
	connect(ui.actionWindowLevel, SIGNAL(triggered()), this, SLOT(slotWindowLevelMode()));
	connect(ui.actionRuler, SIGNAL(triggered()), this, SLOT(slotOtherMode()));
	connect(ui.actionROI, SIGNAL(triggered()), this, SLOT(slotOtherMode()));
	connect(ui.actionContour, SIGNAL(triggered()), this, SLOT(slotOtherMode()));
	connect(ui.actionBrush, SIGNAL(triggered()), this, SLOT(slotOtherMode()));


	// action group2
	group2.addAction(ui.actionImage1);
	group2.addAction(ui.actionImage2);
	group2.addAction(ui.actionImage3);
	group2.addAction(ui.actionImage4);
	group2.addAction(ui.actionFourViews);
	group2.setExclusive(true);
	// signalMapper for group 2
	QSignalMapper* imageSignalMapper = new QSignalMapper(this);
	imageSignalMapper->setMapping(ui.actionImage1, 1);
	imageSignalMapper->setMapping(ui.actionImage2, 2);
	imageSignalMapper->setMapping(ui.actionImage3, 3);
	imageSignalMapper->setMapping(ui.actionImage4, 4);
	imageSignalMapper->setMapping(ui.actionFourViews, 0);
	
	connect(ui.actionImage1, SIGNAL(triggered()), imageSignalMapper, SLOT(map()));
	connect(ui.actionImage2, SIGNAL(triggered()), imageSignalMapper, SLOT(map()));
	connect(ui.actionImage3, SIGNAL(triggered()), imageSignalMapper, SLOT(map()));
	connect(ui.actionImage4, SIGNAL(triggered()), imageSignalMapper, SLOT(map()));
	connect(ui.actionFourViews, SIGNAL(triggered()), imageSignalMapper, SLOT(map()));
	connect(imageSignalMapper, SIGNAL(mapped(int)), this, SLOT(slotImage(int)));
	//connect(imageSignalMapper, SIGNAL(mapped(int)), this, SLOT(resizeEvent()));


	connect(ui.ULBtn, SIGNAL(clicked()), ui.actionImage1, SLOT(trigger()));
	connect(ui.URBtn, SIGNAL(clicked()), ui.actionImage2, SLOT(trigger()));
	connect(ui.LLBtn, SIGNAL(clicked()), ui.actionImage3, SLOT(trigger()));
	connect(ui.LRBtn, SIGNAL(clicked()), ui.actionImage4, SLOT(trigger()));

	connect(ui.ULBtn2, SIGNAL(clicked()), ui.actionFourViews, SLOT(trigger()));
	connect(ui.URBtn2, SIGNAL(clicked()), ui.actionFourViews, SLOT(trigger()));
	connect(ui.LLBtn2, SIGNAL(clicked()), ui.actionFourViews, SLOT(trigger()));
	connect(ui.LRBtn2, SIGNAL(clicked()), ui.actionFourViews, SLOT(trigger()));



	//initialization
	initializeModule();

	// set 2d viewers
	ui.image1View->SetRenderWindow(controller[0].getViewers(0)->GetRenderWindow());
	ui.image2View->SetRenderWindow(controller[0].getViewers(1)->GetRenderWindow());
	ui.image3View->SetRenderWindow(controller[0].getViewers(2)->GetRenderWindow());
	for (int i = 0; i < 5; ++i) {
		vtkRenderWindow* renderWindow[3] = {
			controller[i].getViewers(0)->GetRenderWindow(),
			controller[i].getViewers(1)->GetRenderWindow(),
			controller[i].getViewers(2)->GetRenderWindow()
		};
		wController[i].setRenderWindows(renderWindow);
	}
	//initialize MultiWidgetsController


	//recent image
	createRecentImageActions();
}

MainWindow::~MainWindow()
{
}


void MainWindow::adjustForCurrentFile(const QString & filePath)
{
	QSettings settings("DIIR", "Project");
	QStringList recentFilePaths = settings.value("recentFiles").toStringList();

	recentFilePaths.removeAll(filePath);
	recentFilePaths.prepend(filePath);
	while (recentFilePaths.size() > maxRecentImage)
		recentFilePaths.removeLast();
	settings.setValue("recentFiles", recentFilePaths);

	// see note
	updateRecentActionList();
}

void MainWindow::updateRecentActionList()
{
	QSettings settings("DIIR", "Project");
	QStringList recentFilePaths =
		settings.value("recentFiles").toStringList();

	int itEnd = 0;
	if (recentFilePaths.size() <= maxRecentImage)
		itEnd = recentFilePaths.size();
	else
		itEnd = maxRecentImage;

	for (int i = 0; i < itEnd; i++)
	{
		recentFileActionList.at(i)->setText(recentFilePaths.at(i));
		recentFileActionList.at(i)->setData(recentFilePaths.at(i));
		recentFileActionList.at(i)->setVisible(true);
	}

	for (int i = itEnd; i < maxRecentImage; i++)
		recentFileActionList.at(i)->setVisible(false);
}

void MainWindow::createRecentImageActions()
{
	QAction* recentFileAction = 0;
	for (int i = 0; i < maxRecentImage; i++) {
		recentFileAction = new QAction(this);
		recentFileAction->setVisible(false);
		connect(recentFileAction, SIGNAL(triggered()), this, SLOT(slotOpenRecentImage()));

		recentFileActionList.append(recentFileAction);
		ui.menuRecentImage->addAction(recentFileAction);
	}

	updateRecentActionList();
}

void MainWindow::slotAbout()
{
	QMessageBox::about(NULL, "About",QString(
		"It is a program of displaying multi-modality images made by Andy Li (2016)")
	);
}

void MainWindow::slotHelp()
{
	//Info Dialog
	dialog.show();
	dialog.setModal(true);
}

void MainWindow::slotOpenImage(QString dir)
{
	RegistrationWizard wizard(this, dir);
	int isFinished = wizard.exec();

	if (isFinished == QWizard::Rejected)
		return;

	//Update Recent Image
	visibleImageNum = wizard.getTotalFileNo();
	if(wizard.getFileNames1() != NULL)
		fileNameList[0] = *wizard.getFileNames1();
	if (wizard.getFileNames2() != NULL)
		fileNameList[1] = *wizard.getFileNames2();
	if (wizard.getFileNames3() != NULL)
		fileNameList[2] = *wizard.getFileNames3();
	if (wizard.getFileNames4() != NULL)
		fileNameList[3] = *wizard.getFileNames4();
	if (wizard.getFileNames5() != NULL)
		fileNameList[4] = *wizard.getFileNames5();
	for (int i = 0; i < 5; ++i) {
		if (fileNameList[i].isEmpty())
			continue;
		else {
			list<string> list1;
			for (QStringList::ConstIterator it = fileNameList[i].cbegin();
				it!= fileNameList[i].cend();++it) {
				list1.push_back(it->toStdString());
			}
			controller[i].setImageName(QFileInfo(*fileNameList[i].cbegin()).baseName().toStdString());
			controller[i].loadImage(list1);
		}
	}

	adjustForCurrentFile(wizard.getDirectory());
	// default begin in NavigationMode
	slotNavigationMode();
	ui.actionNavigation->setChecked(true);
	slotVisualizeImage(0);
}

void MainWindow::slotOpenImage()
{
	this->slotOpenImage("");
}

void MainWindow::slotOpenRecentImage()
{
	QCoreApplication::processEvents();

	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		slotOpenImage(action->data().toString());
	}
}

void MainWindow::slotVisualizeImage(int i)
{
	controller[i].initialize();
	controller[i].setMode(MyVtkInteractorStyleImage::NavaigationMode);
}

void MainWindow::slotNavigationMode()
{
	controller[0].setMode(MyVtkInteractorStyleImage::NavaigationMode);
}

void MainWindow::slotWindowLevelMode()
{
	controller[0].setMode(MyVtkInteractorStyleImage::WindowLevelMode);

}

void MainWindow::slotOtherMode()
{
	controller[0].setMode(MyVtkInteractorStyleImage::OtherMode);

}

void MainWindow::slotImage(int i)
{
	//ui.image1frame->setHidden(true);
	//ui.image2frame->setHidden(true);
	//ui.image3frame->setHidden(true);
	//ui.image4frame->setHidden(true);
	switch (i) {
	case 1:
		ui.image1frame->setHidden(false);
		ui.image2frame->setHidden(true);
		ui.image3frame->setHidden(true);
		ui.image4frame->setHidden(true);
		break;
	case 2:
		ui.image1frame->setHidden(true);
		ui.image2frame->setHidden(false);
		ui.image3frame->setHidden(true);
		ui.image4frame->setHidden(true);
		break;
	case 3:
		ui.image1frame->setHidden(true);
		ui.image2frame->setHidden(true);
		ui.image3frame->setHidden(false);
		ui.image4frame->setHidden(true);
		break;
	case 4:
		ui.image1frame->setHidden(true);
		ui.image2frame->setHidden(true);
		ui.image3frame->setHidden(true);
		ui.image4frame->setHidden(false);
		break;
	default:
		ui.image1frame->setHidden(false);
		ui.image2frame->setHidden(false);
		ui.image3frame->setHidden(false);
		ui.image4frame->setHidden(false);

	}
	controller[0].render();
}

void MainWindow::resizeEvent(QResizeEvent * event) {
	QMainWindow::resizeEvent(event);

	controller[0].render();
}


void MainWindow::initializeModule()
{
	ui.dockWidget->setWidget(&moduleWidget);
}

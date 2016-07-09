#include "MainWindow.h"

MainWindow::MainWindow(QMainWindow* parent) 
	:QMainWindow(parent)
{
	ui.setupUi(this);

	this->setWindowTitle("PlaqueQuant");


	// Set up action signals and slots
	connect(ui.actionOpenImage, SIGNAL(triggered()), this, SLOT(slotOpenImage()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(slotHelp()));
	//connect(ui.actionNavigation, SIGNAL(triggered()), this, SLOT(slotNavigationMode()));
	//connect(ui.actionWindowLevel, SIGNAL(triggered()), this, SLOT(slotWindowLevelMode()));
//	connect(ui.actionContour, SIGNAL(triggered()), this, SLOT(slotContourMode()));
//	connect(ui.actionBrush, SIGNAL(triggered()), this, SLOT(slotBrushMode()));

	//initialization
	initializeModule();

	// set 2d viewers
	ui.image1View->SetRenderWindow(controller.getViewers(0)->GetRenderWindow());
	ui.image2View->SetRenderWindow(controller.getViewers(1)->GetRenderWindow());
	ui.image3View->SetRenderWindow(controller.getViewers(2)->GetRenderWindow());

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
			list<string> list;
			for (QStringList::ConstIterator it = fileNameList[i].cbegin();
				it!= fileNameList[i].cend();++it) {
				list.push_back(it->toStdString());
			}
			controller.loadImage(list);
		}
	}

	adjustForCurrentFile(wizard.getDirectory());
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
	controller.initialize(controller.getImage(i));
}


void MainWindow::initializeModule()
{
	ui.dockWidget->setWidget(&moduleWidget);
}

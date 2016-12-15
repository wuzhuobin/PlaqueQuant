#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ModuleWidget.h"
#include "MeasurementWidget.hpp"

#include <QUrl>
#include <QMessageBox>
#include <QSettings>
#include <QMimeData>
#include <QSignalMapper>

#include <vtkRenderWindow.h>
#include <vtkLookupTable.h>



MainWindow::MainWindow() 
	:widgetGroup(this), viewerGroup(this), viewGroup(this), m_InfoDialog(this)
{
	this->ui = new Ui::MainWindow;
	this->ui->setupUi(this);

	m_core = new Core(this);
	this->imageManager = m_core->GetMyImageManager();
	this->ioManager = m_core->GetIOManager();

	//Initialize
	m_moduleWidget = new ModuleWidget(this);
	m_measurementWidget = new MeasurementWidget(this);
	ui->measurementDockWidget->setWidget(m_measurementWidget);
	ui->widgetDockWidget->setWidget(m_moduleWidget);
	this->tabifyDockWidget(ui->widgetDockWidget, ui->measurementDockWidget);
	ui->widgetDockWidget->raise();
	ui->widgetDockWidget->show();
	setActionsEnable(false);

	ui->image1View->SetRenderWindow(m_core->GetRenderWindow(0));
	ui->image2View->SetRenderWindow(m_core->GetRenderWindow(1));
	ui->image3View->SetRenderWindow(m_core->GetRenderWindow(2));
	ui->image4View->SetRenderWindow(m_core->GetRenderWindow(3));
	
	this->m_core->Initialization();

	connect(this->m_core,			SIGNAL(signalVisualizeAllViewers()) , this,			SLOT(slotVisualizeImage()));
	
	// Open Image
	connect(ui->actionOpenImage,	SIGNAL(triggered())					, ioManager,	SLOT(slotOpenWithWizard()));
	
	// Segmentation
	connect(ui->actionSaveSegmentation,			SIGNAL(triggered())					,ioManager,		SLOT(slotSaveSegmentaitonWithDiaglog()));
	connect(ui->actionOpenSegmentation, SIGNAL(triggered()),
		ioManager, SLOT(slotOpenSegmentationWithDiaglog()));
	connect(ui->actionSaveContour, SIGNAL(triggered()),
		ioManager, SLOT(slotSaveContourWithDiaglog()));


	// different mode
	widgetGroup.addAction(ui->actionNavigation);
	widgetGroup.addAction(ui->actionWindowLevel);
	widgetGroup.addAction(ui->actionContour);
	widgetGroup.addAction(ui->actionBrush);
	widgetGroup.addAction(ui->actionRuler);
	widgetGroup.addAction(ui->actionROI);
	widgetGroup.addAction(ui->actionSeedsPlacer);
	widgetGroup.setExclusive(true);
	connect(&widgetGroup, SIGNAL(triggered(QAction*)), this, SLOT(slotChangeMode(QAction*)));
	connect(ui->actionNavigation,	SIGNAL(triggered()), this->m_core, SLOT(slotNavigationMode()));
	connect(ui->actionWindowLevel,	SIGNAL(triggered()), this->m_core, SLOT(slotWindowLevelMode()));
	connect(ui->actionContour,		SIGNAL(triggered()), this->m_core, SLOT(slotContourMode()));
	connect(ui->actionBrush,		SIGNAL(triggered()), this->m_core, SLOT(slotBrushMode()));
	connect(ui->actionRuler,		SIGNAL(triggered()),	this->m_core, SLOT(slotRulerMode()));
	connect(ui->actionROI,			SIGNAL(triggered()),		this->m_core, SLOT(slotROIMode()));
	connect(ui->actionSeedsPlacer, SIGNAL(triggered()), this->m_core, SLOT(slotSeedsPlacerMode()));

	// view
	viewGroup.addAction(ui->actionMultiPlanarView);
	viewGroup.addAction(ui->actionAllAxialView);
	viewGroup.setExclusive(true);
	connect(ui->actionMultiPlanarView, SIGNAL(triggered()), this->m_core, SLOT(slotMultiPlanarView()));
	connect(ui->actionAllAxialView, SIGNAL(triggered()), this->m_core, SLOT(slotSegmentationView()));
	connect(this->m_core, SIGNAL(signalMultiPlanarView()), ui->actionMultiPlanarView, SLOT(trigger()));
	connect(this->m_core, SIGNAL(signalSegmentationView()), ui->actionAllAxialView, SLOT(trigger()));

	// slice 
	ui->sliceScrollArea->setWidget(
		m_core->Get2DInteractorStyle(Core::DEFAULT_IMAGE)->GetNavigation());

	// UI
	connect(ui->actionExit,		SIGNAL(triggered()), this, SLOT(slotExit()));
	connect(ui->actionAbout,	SIGNAL(triggered()), this, SLOT(slotAbout()));
	connect(ui->actionHelp,		SIGNAL(triggered()), this, SLOT(slotHelp()));
	
	// change modality
	connect(ui->ULSelectImgBtn, SIGNAL(clicked()), this, SLOT(slotChangeBaseImageUL()));
	connect(ui->URSelectImgBtn, SIGNAL(clicked()), this, SLOT(slotChangeBaseImageUR()));
	connect(ui->LLSelectImgBtn, SIGNAL(clicked()), this, SLOT(slotChangeBaseImageLL()));


	//four viewers and maximum
	viewerGroup.addAction(ui->actionImage1);
	viewerGroup.addAction(ui->actionImage2);
	viewerGroup.addAction(ui->actionImage3);
	viewerGroup.addAction(ui->actionImage4);
	viewerGroup.addAction(ui->actionFourViews);
	viewerGroup.setExclusive(true);
	QSignalMapper* viewerMapper = new QSignalMapper(this);
	viewerMapper->setMapping(ui->actionFourViews, 0);
	viewerMapper->setMapping(ui->actionImage1, 1);
	viewerMapper->setMapping(ui->actionImage2, 2);
	viewerMapper->setMapping(ui->actionImage3, 3);
	viewerMapper->setMapping(ui->actionImage4, 4);
	connect(ui->actionImage1, SIGNAL(triggered()), viewerMapper, SLOT(map()));
	connect(ui->actionImage2, SIGNAL(triggered()), viewerMapper, SLOT(map()));
	connect(ui->actionImage3, SIGNAL(triggered()), viewerMapper, SLOT(map()));
	connect(ui->actionImage4, SIGNAL(triggered()), viewerMapper, SLOT(map()));
	connect(ui->actionFourViews, SIGNAL(triggered()), viewerMapper, SLOT(map()));
	connect(viewerMapper, SIGNAL(mapped(int)), this, SLOT(slotImage(int)));
	connect(ui->ULBtn, SIGNAL(clicked()), ui->actionImage1, SLOT(trigger()));
	connect(ui->URBtn, SIGNAL(clicked()), ui->actionImage2, SLOT(trigger()));
	connect(ui->LLBtn, SIGNAL(clicked()), ui->actionImage3, SLOT(trigger()));
	connect(ui->LRBtn, SIGNAL(clicked()), ui->actionImage4, SLOT(trigger()));
	connect(ui->ULBtn2, SIGNAL(clicked()), ui->actionFourViews, SLOT(trigger()));
	connect(ui->URBtn2, SIGNAL(clicked()), ui->actionFourViews, SLOT(trigger()));
	connect(ui->LLBtn2, SIGNAL(clicked()), ui->actionFourViews, SLOT(trigger()));
	connect(ui->LRBtn2, SIGNAL(clicked()), ui->actionFourViews, SLOT(trigger()));


	//3D view
	connect(ui->updateBtn, SIGNAL(clicked()), 
		m_core, SLOT(slotUpdate3DLabelBtn()));
	connect(ui->generateCLBtn, SIGNAL(clicked()), 
		m_core, SLOT(slotGenerateCenterlineBtn()));

	//UI Setting
	ui->ULBtn2->setHidden(true);
	ui->URBtn2->setHidden(true);
	ui->LLBtn2->setHidden(true);
	ui->LRBtn2->setHidden(true);

	//Recent Image
	createRecentImageActions();
}

MainWindow* MainWindow::GetMainWindow()
{
	foreach ( QWidget* w, QApplication::topLevelWidgets() )
	{
		if ( w->objectName() == "MainWindow" )
		{
			return (MainWindow*)w;
		}
	}
	return NULL;
}


Core* MainWindow::GetCore()
{
	return this->m_core;
}

MainWindow::~MainWindow()
{
	ui->image1View->SetRenderWindow(NULL);

	ui->image2View->SetRenderWindow(NULL);

	ui->image3View->SetRenderWindow(NULL);

}

void MainWindow::DisplayErrorMessage(std::string str)
{
	m_core->DisplayErrorMessage(str);

}

void MainWindow::setActionsEnable( bool b )
{	
	//switch after open the image
	//ui->actionOpenImage->setEnabled(!b);
	//ui->menuRecentImage->setEnabled(!b);
	ui->image1View->setEnabled(b);
	ui->image2View->setEnabled(b);
	ui->image3View->setEnabled(b);
	ui->image4View->setEnabled(b);
	ui->sliceScrollArea->setEnabled(b);
	ui->actionSaveSegmentation->setEnabled(b);
	ui->actionSaveContour->setEnabled(b);
	ui->actionNavigation->setEnabled(b);
	ui->actionWindowLevel->setEnabled(b);
	ui->actionContour->setEnabled(b);
	ui->actionBrush->setEnabled(b);
	ui->actionRemoveContour->setEnabled(b);
	ui->actionRuler->setEnabled(b);
	ui->actionROI->setEnabled(b);
	ui->actionSeedsPlacer->setEnabled(b);
	ui->actionInformation->setEnabled(b);
	ui->actionImage1->setEnabled(b);
	ui->actionImage2->setEnabled(b);
	ui->actionImage3->setEnabled(b);
	ui->actionImage4->setEnabled(b);
	ui->actionFourViews->setEnabled(b);
	ui->widgetDockWidget->setEnabled(b);
	ui->measurementDockWidget->setEnabled(b);
	ui->actionMultiPlanarView->setEnabled(b);
	ui->actionAllAxialView->setEnabled(b);
	ui->ULBtn->setEnabled(b);
	ui->URBtn->setEnabled(b);
	ui->LLBtn->setEnabled(b);
	ui->LRBtn->setEnabled(b);
	ui->ULSelectImgBtn->setEnabled(b);
	ui->URSelectImgBtn->setEnabled(b);
	ui->LLSelectImgBtn->setEnabled(b);

	
}

void MainWindow::slotExit() 
{
	qApp->exit();
}


void MainWindow::slotOpenRecentImage()
{
	QCoreApplication::processEvents();

	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		ioManager->slotOpenWithWizard(action->data().toString());
	}
}


void MainWindow::createRecentImageActions()
{
	QAction* recentFileAction = 0;
	for(int i = 0; i < MAX_RECENT_IMAGE; i++){
		recentFileAction = new QAction(this);
		recentFileAction->setVisible(false);
		connect(recentFileAction, SIGNAL(triggered()),this, SLOT(slotOpenRecentImage()));

		recentFileActionList.append(recentFileAction);
		ui->menuRecentImage->addAction(recentFileAction);
	}

	updateRecentActionList();
}

/* Visualization and initialize Module widget UI */
bool MainWindow::slotVisualizeImage()
{	
	adjustForCurrentFile(ioManager->getFilePath());
	//Enable Actions 
	setActionsEnable(true);

	m_moduleWidget->addWidget(m_core->Get2DInteractorStyle(Core::DEFAULT_IMAGE)->GetPaintBrush());
	m_moduleWidget->addWidget(m_core->Get2DInteractorStyle(Core::DEFAULT_IMAGE)->GetRuler());
	m_moduleWidget->addWidget(m_core->Get2DInteractorStyle(Core::DEFAULT_IMAGE)->GetSeedsPlacer());
	m_moduleWidget->addWidget(m_core->Get2DInteractorStyle(Core::DEFAULT_IMAGE)->GetPolygonDraw());
	m_moduleWidget->addWidget(m_core->Get2DInteractorStyle(Core::DEFAULT_IMAGE)->GetROI());

	//connected to slotNavigationMode()
	ui->actionNavigation->trigger();
	// update DICOM header imformation
	m_measurementWidget->slotUpdateImformation();

	// initialize image select function 
	ChangeBaseImageULMenu.clear();
	ChangeBaseImageURMenu.clear();
	ChangeBaseImageLLMenu.clear();
	for (int i = 0; i < this->imageManager->getListOfVtkImages().size(); i++)
	{
		if (this->imageManager->getListOfVtkImages()[i] != NULL) {
			ChangeBaseImageULMenu.addAction(
				imageManager->getListOfModalityNames()[i])->setData(0);
			ChangeBaseImageURMenu.addAction(
				imageManager->getListOfModalityNames()[i])->setData(1);
			ChangeBaseImageLLMenu.addAction(
				imageManager->getListOfModalityNames()[i])->setData(2);
		}

	}
	ui->ULSelectImgBtn->setMenu(&ChangeBaseImageULMenu);
	connect(&ChangeBaseImageULMenu, SIGNAL(triggered(QAction*)), 
		this->m_core, SLOT(slotChangeModality(QAction*)));

	ui->URSelectImgBtn->setMenu(&ChangeBaseImageURMenu);
	connect(&ChangeBaseImageURMenu, SIGNAL(triggered(QAction*)),
		this->m_core, SLOT(slotChangeModality(QAction*)));

	ui->LLSelectImgBtn->setMenu(&ChangeBaseImageLLMenu);
	connect(&ChangeBaseImageLLMenu, SIGNAL(triggered(QAction*)),
		this->m_core, SLOT(slotChangeModality(QAction*)));


	/// Initialize Module widget ui
	//this->m_moduleWidget->UdateTargetImageComboBox();
	return 0;
}

void MainWindow::slotChangeMode(QAction* action)
{
	ui->widgetDockWidget->setEnabled(true);
	if (action == ui->actionContour) {
		m_moduleWidget->setWidget(m_core->Get2DInteractorStyle(
			Core::DEFAULT_IMAGE)->GetPolygonDraw());
	}
	else if (action == ui->actionSeedsPlacer) {
		m_moduleWidget->setWidget(m_core->Get2DInteractorStyle(
			Core::DEFAULT_IMAGE)->GetSeedsPlacer());
	}
	else if (action == ui->actionRuler) {
		m_moduleWidget->setWidget(m_core->Get2DInteractorStyle(
			Core::DEFAULT_IMAGE)->GetRuler());
	}
	else if (action == ui->actionBrush) {
		m_moduleWidget->setWidget(m_core->Get2DInteractorStyle(
			Core::DEFAULT_IMAGE)->GetPaintBrush());
	}
	else if (action == ui->actionROI) {
		m_moduleWidget->setWidget(m_core->Get2DInteractorStyle(
		Core::DEFAULT_IMAGE)->GetROI());
	}
	else {
		m_moduleWidget->setWidget(nullptr);
	}
}

void MainWindow::adjustForCurrentFile(const QString &filePath)
{
	QSettings settings("DIIR","PlaqueQuant");
	QStringList recentFilePaths = settings.value("recentFiles").toStringList();
	
	recentFilePaths.removeAll(filePath);
	recentFilePaths.prepend(filePath);
	while (recentFilePaths.size() > MAX_RECENT_IMAGE)
		recentFilePaths.removeLast();
	settings.setValue("recentFiles", recentFilePaths);

	// see note
	updateRecentActionList();
}

void MainWindow::updateRecentActionList()
{
	QSettings settings("DIIR","PlaqueQuant");
	QStringList recentFilePaths =
		settings.value("recentFiles").toStringList();

	int itEnd = 0;
	if(recentFilePaths.size() <= MAX_RECENT_IMAGE)
		itEnd = recentFilePaths.size();
	else
		itEnd = MAX_RECENT_IMAGE;
    
	for (int i = 0; i < itEnd; i++) 
	{
		recentFileActionList.at(i)->setText(recentFilePaths.at(i));
		recentFileActionList.at(i)->setData(recentFilePaths.at(i));
		recentFileActionList.at(i)->setVisible(true);
	}

	for (int i = itEnd; i < MAX_RECENT_IMAGE; i++)
		recentFileActionList.at(i)->setVisible(false);
}

void MainWindow::slotAbout()
{
	QMessageBox msgBox;
	msgBox.setIconPixmap(QPixmap(":/icons/plaqueQuant.png"));
	msgBox.setText(QString("<h2 align='center'>Plaque Quant - version REPLACE_ME</h2>").replace("REPLACE_ME", this->m_version) +
		QString("<br>This software is a analytical tool designed to diagnose the condition of plaque and blood vessels."));
	msgBox.setWindowTitle("About");
	msgBox.exec();
}

void MainWindow::slotHelp()
{
	QString msg = "";
	msg += QString("<h3>Instructions</h3>") +
		QString("<table>") +
		QString("<tr><td><b>Step 1  </b></td><td>Load images.</td></tr>") +
		QString("<tr><td><b>Step 2  </b></td><td>Segment images</td></tr>") +
		QString("<tr><td><b>Step 3  </b></td><td>Press 'Update' function to view 3D. Adjust opacity of layer with the widget at the bottom right corner.</td></tr>") +
		QString("<tr><td><b>Step 4  </b></td><td>Point the cursor and press 'Enter' to select stenosis reference point and blockage point. Press 'Delete' to clear selections.</td></tr>") +
		QString("<tr><td><b>Step 5  </b></td><td>Follow the instructions on the control panel at the right-hand-side.</td></tr>") +
		QString("</table>");
	QMessageBox::information(NULL,
		"Help",
		msg);
}

void MainWindow::slotImage(int image)
{
	ui->image1frame->setHidden(true);
	ui->image2frame->setHidden(true);
	ui->image3frame->setHidden(true);
	ui->image4frame->setHidden(true);
	switch (image)
	{
	case 0:
		ui->ULBtn->setHidden(false);
		ui->URBtn->setHidden(false);
		ui->LLBtn->setHidden(false);
		ui->LRBtn->setHidden(false);
		ui->ULBtn2->setHidden(true);
		ui->URBtn2->setHidden(true);
		ui->LLBtn2->setHidden(true);
		ui->LRBtn2->setHidden(true);
		ui->image1frame->setHidden(false);
		ui->image2frame->setHidden(false);
		ui->image3frame->setHidden(false);
		ui->image4frame->setHidden(false);
		break;
	case 1:
		ui->ULBtn->setHidden(true);
		ui->ULBtn2->setHidden(false);
		ui->image1frame->setHidden(false);
		ui->image2frame->setHidden(true);
		ui->image3frame->setHidden(true);
		ui->image4frame->setHidden(true);
		break;
	case 2:
		ui->URBtn->setHidden(true);
		ui->URBtn2->setHidden(false);
		ui->image1frame->setHidden(true);
		ui->image2frame->setHidden(false);
		ui->image3frame->setHidden(true);
		ui->image4frame->setHidden(true);
		break;
	case 3:
		ui->LLBtn->setHidden(true);
		ui->LLBtn2->setHidden(false);
		ui->image1frame->setHidden(true);
		ui->image2frame->setHidden(true);
		ui->image3frame->setHidden(false);
		ui->image4frame->setHidden(true);
		break;
	case 4:
		ui->LRBtn->setHidden(true);
		ui->LRBtn2->setHidden(false);
		ui->image1frame->setHidden(true);
		ui->image2frame->setHidden(true);
		ui->image3frame->setHidden(true);
		ui->image4frame->setHidden(false);
	default:
		break;
	}
	m_core->RenderAllViewer();
}

void MainWindow::resizeEvent( QResizeEvent * event )
{
	QMainWindow::resizeEvent(event);
	m_core->RenderAllViewer();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *ev)
{
	ev->accept();
}

void MainWindow::dropEvent( QDropEvent *ev )
{
    const QMimeData *mimeData = ev->mimeData();
	QList<QUrl> urls = mimeData->urls();
	foreach(QUrl url, urls)
	{
		QString folder = url.toLocalFile();
		folder.replace("\\","/");
		
		ioManager->slotOpenWithWizard(folder);

	}
}


void MainWindow::slotShowProgressDialog( int value, QString text )
{
	//m_progressDialog->setValue(value);
	//m_progressDialog->setLabelText(text);
	//m_progressDialog->show();
}




vtkRenderWindow * MainWindow::GetRenderWindow(int i)
{
	switch (i)
	{
	case 1:
		return ui->image1View->GetRenderWindow();
	case 2:
		return ui->image2View->GetRenderWindow();
	case 3:
		return ui->image3View->GetRenderWindow();
	case 4:
		return ui->image4View->GetRenderWindow();
	default:
		break;
	}
	return nullptr;
}

ModuleWidget* MainWindow::GetModuleWidget()
{
	return this->m_moduleWidget;
}

void MainWindow::SetVersion(QString version)
{
	this->m_version = version;
}

void MainWindow::slotChangeBaseImageUL()
{
	ChangeBaseImageULMenu.show();
}

void MainWindow::slotChangeBaseImageUR()
{
	ChangeBaseImageURMenu.show();
}

void MainWindow::slotChangeBaseImageLL()
{
	ChangeBaseImageLLMenu.show();
}

void MainWindow::UpdateStenosisValue(double val)
{
	this->m_measurementWidget->ui.stenosisSpinBox->setValue(val);
}

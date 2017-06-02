#include "MainWindow.h"

#include "ui_MainWindow.h"
#include "ui_ViewerWidget.h"
#include "ui_ModuleWidget.h"
#include "ModuleWidget.h"
#include "ViewerWidget.h"
#include "MeasurementWidget.h"
#include "LabelWidget.h"

#include <qdebug.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QVTKInteractor.h>
#include <qmessagebox.h>


#include <vtkRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include "RegistrationWizard.h"

MainWindow::MainWindow(QWidget *parent)
	:QMainWindow(parent) 
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	this->moduleWiget = new ModuleWidget(this);
	ui->moduleWidgetDockWidget->setWidget(this->moduleWiget);

	this->labelWidget = new LabelWidget(this);
	this->moduleWiget->getUi()->verticalLayoutModule->addWidget(this->labelWidget);

	this->measurementWidget = new MeasurementWidget(this);
	ui->measurementDockWidget->setWidget(measurementWidget);

	this->tabifyDockWidget(ui->measurementDockWidget, ui->moduleWidgetDockWidget);

	QMainWindow* centralWidget = new QMainWindow(this);
	centralWidget->setDockNestingEnabled(true);
	ui->verticalLayoutCentralWidget->insertWidget(0, centralWidget);

	for (int i = NUM_OF_VIEWERS - 1; i > -1; --i) {
		this->viewerWidgets[i] = new ViewerWidget(this);
		if (i % 2) {
			centralWidget->addDockWidget(Qt::BottomDockWidgetArea, this->viewerWidgets[i]);
		}
		else {
			centralWidget->addDockWidget(Qt::TopDockWidgetArea, this->viewerWidgets[i]);
		}
		this->selectImgMenus[i] = new QMenu(this);
		this->viewerWidgets[i]->getUi()->pushButtonSelectImage->setMenu(this->selectImgMenus[i]);
		this->viewerWidgets[i]->setWindowTitle(QString("Viewer ") + QString::number(i));
	}
	settings = new QSettings("Setting.ini", QSettings::IniFormat, this);

	connect(viewerWidgets[0]->getUi()->pushButtonRestore, SIGNAL(toggled(bool)),
		ui->actionImage1, SLOT(setChecked(bool)));
	connect(viewerWidgets[1]->getUi()->pushButtonRestore, SIGNAL(toggled(bool)),
		ui->actionImage2, SLOT(setChecked(bool)));
	connect(viewerWidgets[2]->getUi()->pushButtonRestore, SIGNAL(toggled(bool)),
		ui->actionImage3, SLOT(setChecked(bool)));
	connect(viewerWidgets[3]->getUi()->pushButtonRestore, SIGNAL(toggled(bool)),
		ui->actionImage4, SLOT(setChecked(bool)));


	QActionGroup* actionGroupActionImage = new QActionGroup(this);
	actionGroupActionImage->addAction(ui->actionImage1);
	actionGroupActionImage->addAction(ui->actionImage2);
	actionGroupActionImage->addAction(ui->actionImage3);
	actionGroupActionImage->addAction(ui->actionImage4);
	actionGroupActionImage->addAction(ui->actionFourViews);
	actionGroupActionImage->setExclusive(true);

	connect(ui->actionImage1, SIGNAL(toggled(bool)), this, SLOT(slotImage(bool)));
	connect(ui->actionImage2, SIGNAL(toggled(bool)), this, SLOT(slotImage(bool)));
	connect(ui->actionImage3, SIGNAL(toggled(bool)), this, SLOT(slotImage(bool)));
	connect(ui->actionImage4, SIGNAL(toggled(bool)), this, SLOT(slotImage(bool)));
	connect(ui->actionFourViews, SIGNAL(toggled(bool)), this, SLOT(slotImage(bool)));

	QActionGroup* actionGroupView = new QActionGroup(this);
	actionGroupView->addAction(ui->actionAll_axial_view);
	actionGroupView->addAction(ui->actionMulti_planar_view);
	//actionGroupView->addAction(ui->actionCurved_view);
	actionGroupView->setExclusive(true);


	QActionGroup* actionGroupImage = new QActionGroup(this);
	actionGroupImage->addAction(ui->actionNavigation);
	actionGroupImage->addAction(ui->actionWindow_level);
	actionGroupImage->addAction(ui->actionWindow_level_threshold);
	actionGroupImage->addAction(ui->actionThreshold);
	actionGroupImage->addAction(ui->acitonVOI_selection);
	actionGroupImage->addAction(ui->actionPaint_brush);
	actionGroupImage->addAction(ui->actionSeeds_placer);
	actionGroupImage->addAction(ui->actionVBD_Smoker);
	actionGroupImage->addAction(ui->actionTubular_VOI);
	actionGroupImage->addAction(ui->actionDistance_measure);
	actionGroupImage->addAction(ui->actionMaximum_wall_thickness);
	actionGroupImage->addAction(ui->actionPolygon_draw);
	actionGroupImage->addAction(ui->actionPolygon_draw_series);
	actionGroupImage->addAction(ui->actionVessel_segmentation);
	actionGroupImage->addAction(ui->actionTesting);
	actionGroupImage->setExclusive(true);


	QActionGroup* actionGroupSurface = new QActionGroup(this);
	actionGroupSurface->addAction(ui->actionTraceball_camera);
	actionGroupSurface->addAction(ui->actionCenter_line);
	actionGroupSurface->addAction(ui->actionFind_maximum_radius);
	actionGroupSurface->addAction(ui->actionCurved_navigation);
	actionGroupSurface->addAction(ui->actionPerpendicular_measurement);
	actionGroupSurface->addAction(ui->actionWay_point_centerline);
	actionGroupSurface->addAction(ui->actionStenosis);
	
	actionGroupSurface->setExclusive(true);


	// Connection
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionImport_images, SIGNAL(triggered()), this, SLOT(slotOpenNewImage()));
	connect(ui->actionImport_segmentation, SIGNAL(triggered()),
		this, SLOT(slotOpenOverlay()));
	connect(ui->actionExport_segmentation, SIGNAL(triggered()), 
		this, SLOT(slotSaveOverlay()));
	connect(ui->actionExport_Report, SIGNAL(triggered()),
		this, SLOT(slotExportReport()));
	connect(ui->actionAbout, SIGNAL(triggered()),
		this, SLOT(slotAbout()));
	createRecentImageActions();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slotOpenRecentImage()
{
	QCoreApplication::processEvents();

	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		imageImport(action->data().toString());
	}
}

void MainWindow::slotOpenNewImage()
{
	imageImport("");
}

void MainWindow::slotOpenOverlay()
{
		QString path = QFileDialog::getOpenFileName((this), 
			QString(tr("Import Segmentation")), ".", tr("NIFTI Images (*.nii)"));
		if (path.isEmpty())	return;
		emit signalOverlayImportLoad(path);
}

void MainWindow::slotSaveOverlay()
{
	QString path = QFileDialog::getSaveFileName((this),
		QString(tr("Export Segmentation")), ".", tr("NIFTI Images (*.nii)"));
	if (path.isEmpty())	return;
	emit signalOverlayExportSave(path);
}

void MainWindow::slotExportReport()
{
	QString path = QFileDialog::getSaveFileName((this),
		QString(tr("Export Report")), ".", tr("Report (*.pdf)"));
	if (path.isEmpty())	return;
	emit signalReportExport(path);

	measurementWidget->GenerateReport(path);
}

void MainWindow::slotImage(bool flag)
{
	QAction *action = qobject_cast<QAction *>(sender());
	//qDebug() << actionGroupActionImage->checkedAction();
	if (!flag) {
		if (ui->actionImage1->isChecked() ||
			ui->actionImage2->isChecked() ||
			ui->actionImage3->isChecked() ||
			ui->actionImage4->isChecked()) {
			return;
		}
		else {
			ui->actionFourViews->setChecked(true);
			return;
		}
	}
	for (int i = 0; i < NUM_OF_VIEWERS; ++i) {
		if (!this->viewerWidgets[i]->isFloating()) {
			this->viewerWidgets[i]->setHidden(true);
		}
	}

	if (action == ui->actionFourViews) {
		for (int i = 0; i < NUM_OF_VIEWERS; ++i) {
			this->viewerWidgets[i]->setHidden(false);
			this->viewerWidgets[i]->getUi()->pushButtonRestore->setChecked(false);
		}
	}
	else if (action == ui->actionImage1) {
		this->viewerWidgets[0]->setHidden(false);
		this->viewerWidgets[0]->getUi()->pushButtonRestore->setChecked(true);

	}
	else if (action == ui->actionImage2) {
		this->viewerWidgets[1]->setHidden(false);
		this->viewerWidgets[1]->getUi()->pushButtonRestore->setChecked(true);

	}
	else if (action == ui->actionImage3) {
		this->viewerWidgets[2]->setHidden(false);
		this->viewerWidgets[2]->getUi()->pushButtonRestore->setChecked(true);
	}
	else if (action == ui->actionImage4)
	{
		this->viewerWidgets[3]->setHidden(false);
		this->viewerWidgets[3]->getUi()->pushButtonRestore->setChecked(true);
	}
	//m_core->RenderAllViewer();
}

void MainWindow::slotAbout()
{
	QMessageBox msgBox;
	msgBox.setIconPixmap(QPixmap(":/icons/plaqueQuant.png"));
	msgBox.setText(QString("<h2 align='center'>Plaque Quant - version REPLACE_ME</h2>").replace("REPLACE_ME", "3.0") +
		QString("<br>This software is a analytical tool designed to diagnose the condition of plaque and blood vessels."));
	msgBox.setWindowTitle("About");
	msgBox.exec();
}

void MainWindow::imageImport(QString path)
{
	RegistrationWizard rw(path, modalityNames.size(), this);
	for (int i = 0; i < modalityNames.size(); ++i) {
		rw.setImageModalityNames(i, modalityNames[i]);
	}
	QList<QStringList> _listOfFileNames;

	if (QWizard::Accepted == rw.exec()) {

		for (int i = 0; i < modalityNames.size(); ++i) {
			if (rw.getFileNames(i)) {
				qDebug() << *rw.getFileNames(i);
				_listOfFileNames << *rw.getFileNames(i);
			}
		}

		emit signalImageImportLoad(&_listOfFileNames);

		qDebug() << rw.getDirectory();

		adjustForCurrentFile(rw.getDirectory());

	}
}

void MainWindow::initialization()
{
	this->setEnabled(true);

	ui->centralwidget->setEnabled(true);

	ui->ActionToolBar->setEnabled(true);

	ui->menuImage->setEnabled(true);


	ui->actionImport_segmentation->setEnabled(true);

	ui->actionNew_segmentation->setEnabled(true);

	ui->actionExport_Report->setEnabled(true);

	ui->actionExport_segmentation->setEnabled(true);

	ui->updateBtn->setEnabled(true);

	ui->menuOrientation->setEnabled(true);

	ui->actionMulti_planar_view->trigger();
	ui->actionNavigation->trigger();

}

void MainWindow::enableInteractor(bool flag)
{

}

void MainWindow::addModalityNames(QString name)
{
	modalityNames << name;
	for (int i = 0; i < NUM_OF_2D_VIEWERS; ++i) {
			selectImgMenus[i]->addAction(new QAction(name, selectImgMenus[i]));
	}
}

void MainWindow::setSelectImgMenuVisible(unsigned int num, bool flag)
{
	for (int i = 0; i < NUM_OF_2D_VIEWERS; ++i) {
		QList<QAction*> actions = selectImgMenus[i]->actions();
		actions[num]->setVisible(flag);
	}
}

void MainWindow::clearModalityNames()
{
	modalityNames.clear();
	for (int i = 0; i < NUM_OF_2D_VIEWERS; ++i) {
		selectImgMenus[i]->clear();
	}
}

Ui::MainWindow * MainWindow::getUi()
{
	return this->ui;
}

//QMainWindow * MainWindow::getCentralWidget()
//{
//	return &this->centralWidget;
//}

ModuleWidget * MainWindow::getModuleWidget()
{
	return this->moduleWiget;
}

ViewerWidget * MainWindow::getViewerWidget(unsigned int num)
{
	return this->viewerWidgets[num];
}

MeasurementWidget * MainWindow::getMeasurementWidget()
{
	return this->measurementWidget;
}

LabelWidget * MainWindow::getLabelWidget()
{
	return this->labelWidget;
}

QMenu * MainWindow::getSelectImgMenu(unsigned int i)
{
	return selectImgMenus[i];
}

void MainWindow::setEnabled(bool flag)
{
	ui->actionAbout->setEnabled(true);

}

void MainWindow::createRecentImageActions()
{
	QAction* recentFileAction = 0;
	for (int i = 0; i < MAX_RECENT_IMAGE; i++) {
		recentFileAction = new QAction(this);
		recentFileAction->setVisible(false);
		connect(recentFileAction, SIGNAL(triggered()), this, SLOT(slotOpenRecentImage()));

		recentFileActionList.append(recentFileAction);
		ui->menuRecentImage->addAction(recentFileAction);
	}

	updateRecentActionList();
}

void MainWindow::adjustForCurrentFile(const QString &filePath)
{
	QStringList recentFilePaths = settings->value("recentFiles").toStringList();

	recentFilePaths.removeAll(filePath);
	recentFilePaths.prepend(filePath);
	while (recentFilePaths.size() > MAX_RECENT_IMAGE)
		recentFilePaths.removeLast();
	settings->setValue("recentFiles", recentFilePaths);

	// see note
	updateRecentActionList();
}

void MainWindow::updateRecentActionList()
{
	QStringList recentFilePaths =
		settings->value("recentFiles").toStringList();

	int itEnd = 0;
	if (recentFilePaths.size() <= MAX_RECENT_IMAGE)
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
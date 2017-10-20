#include "MainWindow.h"

#include "ui_MainWindow.h"
#include "ui_ViewerWidget.h"
#include "ui_Switch2DWidget.h"
#include "Switch2DWidget.h"
#include "ui_Switch3DWidget.h"
#include "Switch3DWidget.h"
#include "ViewerWidget.h"
#include "MeasurementWidget.h"
#include "IADEMeasurementWidget.h"
#include "LabelWidget.h"

#include <qdebug.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QVTKInteractor.h>
#include <qmessagebox.h>
#include <qevent.h>
#include <qmimedata.h>

#include <vtkRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include "RegistrationWizard.h"

MainWindow::MainWindow(QWidget *parent)
	:QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	this->switch2DWidget = new Switch2DWidget(this);
	ui->dockWidget2D->setWidget(this->switch2DWidget);

	this->switch3DWidget = new Switch3DWidget(this);
	ui->dockWidget3D->setWidget(this->switch3DWidget);

	this->labelWidget = new LabelWidget(this);
	this->switch2DWidget->getUi()->verticalLayoutModule->addWidget(this->labelWidget);

	this->measurementWidget = new MEASUREMENT_WIDGET(this);
	ui->dockWidgetMeasurement->setWidget(measurementWidget);

	this->tabifyDockWidget(ui->dockWidgetMeasurement, ui->dockWidget3D);
	this->tabifyDockWidget(ui->dockWidgetMeasurement, ui->dockWidget2D);

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
	actionGroupView->setExclusive(true);


	QActionGroup* actionGroupImage = new QActionGroup(this);
	actionGroupImage->addAction(ui->actionNavigation);
	actionGroupImage->addAction(ui->actionWindow_level);
	actionGroupImage->addAction(ui->actionWindow_level_threshold);
	actionGroupImage->addAction(ui->actionThreshold);
	actionGroupImage->addAction(ui->acitonVOI_selection);
	actionGroupImage->addAction(ui->actionPaint_brush);
	actionGroupImage->addAction(ui->actionSeeds_placer);
	actionGroupImage->addAction(ui->actionVBD_Smoker_seed);
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
	actionGroupSurface->addAction(ui->actionVBD_Smoker_BA_diameter);
	actionGroupSurface->addAction(ui->actionICDA_diameter);
	actionGroupSurface->addAction(ui->actionCurved_navigation);
	actionGroupSurface->addAction(ui->actionPerpendicular_measurement);
	actionGroupSurface->addAction(ui->actionWay_point_centerline);
	actionGroupSurface->addAction(ui->actionStenosis);
	actionGroupSurface->setExclusive(true);

	connect(ui->actionICDA_standard, SIGNAL(triggered()),
		ui->actionNavigation, SIGNAL(triggered()));
	connect(ui->actionICDA_standard, SIGNAL(triggered()),
		ui->actionICDA_diameter, SIGNAL(triggered()));
	connect(ui->actionSmoker_standard, SIGNAL(triggered()),
		ui->actionVBD_Smoker_seed, SIGNAL(triggered()));
	connect(ui->actionSmoker_standard, SIGNAL(triggered()),
		ui->actionVBD_Smoker_BA_diameter, SIGNAL(triggered()));
	connect(ui->actionUbogu_standard, SIGNAL(triggered()),
		ui->actionNavigation, SIGNAL(triggered()));
	connect(ui->actionUbogu_standard, SIGNAL(triggered()),
		ui->actionVBD_ubogu_measure, SIGNAL(triggered()));


	QActionGroup* actionGroupDiagnosis = new QActionGroup(this);
	actionGroupDiagnosis->addAction(ui->actionICDA_standard);
	actionGroupDiagnosis->addAction(ui->actionSmoker_standard);
	actionGroupDiagnosis->addAction(ui->actionUbogu_standard);
	actionGroupDiagnosis->setExclusive(true);



	// Connection
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionImport, SIGNAL(triggered()),
		this, SLOT(slotOpen()));
	connect(ui->actionImport_images, SIGNAL(triggered()),
		this, SLOT(slotOpenNewImage()));
	connect(ui->actionExport_curved_images, SIGNAL(triggered()),
		this, SLOT(slotSaveCurvedImage()));
	connect(ui->actionImport_segmentation, SIGNAL(triggered()),
		this, SLOT(slotOpenOverlay()));
	connect(ui->actionExport_segmentation, SIGNAL(triggered()),
		this, SLOT(slotSaveOverlay()));
	connect(ui->actionExport_Report, SIGNAL(triggered()),
		this, SLOT(slotExportReport()));
	connect(ui->actionExport_CSV, SIGNAL(triggered()),
		this, SLOT(slotExportCSV()));
	createRecentImageActions();


	this->ui->menuImage->setEnabled(false);
	this->ui->menuSurface->setEnabled(false);
	this->ui->menuSegmentation->setEnabled(false);
	this->ui->menuDiagnosis->menuAction()->setVisible(false);
	this->ui->menuView->setEnabled(false);
	this->ui->actionNew_segmentation->setEnabled(false);
	this->ui->actionImport_segmentation->setEnabled(false);
	this->ui->actionExport_curved_images->setEnabled(false);
	this->ui->actionExport_segmentation->setEnabled(false);
	this->ui->actionMulti_planar_view->setEnabled(false);
	this->ui->actionAll_axial_view->setEnabled(false);

	this->ui->actionNavigation->setEnabled(false);
	this->ui->actionWindow_level->setEnabled(false);
	this->ui->actionMaximum_wall_thickness->setEnabled(false);
	this->ui->acitonVOI_selection->setEnabled(false);
	this->ui->actionTubular_VOI->setEnabled(false);
	this->ui->actionSeeds_placer->setEnabled(false);
	this->ui->actionVessel_segmentation->setEnabled(false);
	this->ui->actionCurved_view->setEnabled(false);
	this->ui->actionMIP->setEnabled(false);



	this->ui->actionTesting->setVisible(false);
	this->ui->actionWindow_level->setVisible(false);
	this->ui->actionPolygon_draw->setVisible(false);
	this->ui->actionPolygon_draw_series->setVisible(false);
	this->ui->actionDistance_measure->setVisible(false);
	this->ui->actionVBD_Smoker_seed->setVisible(false);
	this->ui->actionPerpendicular_measurement->setVisible(false);
	this->ui->actionFind_maximum_radius->setVisible(false);

	this->ui->actionTraceball_camera->setVisible(false);
	this->ui->actionWay_point_centerline->setVisible(false);
	this->ui->actionICDA_diameter->setVisible(false);
	this->ui->actionVBD_Smoker_BA_diameter->setVisible(false);
	this->ui->actionVBD_ubogu_measure->setVisible(false);


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

void MainWindow::slotOpenNewImage(QString path)
{
	imageImport(path);
}

void MainWindow::slotOpen(QString path)
{
	QMessageBox openMessageBox;
	openMessageBox.setIcon(QMessageBox::Question);
	openMessageBox.setText("Import.");
	openMessageBox.setInformativeText("Import images or segmentation?");
	openMessageBox.setStandardButtons(QMessageBox::Cancel);
	openMessageBox.setDefaultButton(QMessageBox::Cancel);
	QPushButton* imagesButton = openMessageBox.addButton(tr("Images"), QMessageBox::ActionRole);
	QPushButton* segmentationButton = openMessageBox.addButton(tr("Segmentation"), QMessageBox::ActionRole);
	openMessageBox.exec();
	if (openMessageBox.clickedButton() == imagesButton)
	{
		slotOpenNewImage(path);
	}
	else if (openMessageBox.clickedButton() == segmentationButton)
	{
		if (QFileInfo(path).completeSuffix() == "nii" ||
			QFileInfo(path).completeSuffix() == "nii.gz")
		{
			slotOpenOverlay(path);
		}
		else {
			QMessageBox::critical(this, tr("Nifti is needed."), tr("Segmentation file is in Nifti format.\n"
				"Please import \"*.nii\" or \"*.nii.gz\". "), QMessageBox::Ok);
		}
	}
}

void MainWindow::slotSaveCurvedImage(QString path)
{
	if (path.isEmpty()) {
		path = QFileDialog::getSaveFileName((this),
			QString(tr("Export Curved Images")), path, tr("NIFTI Images (*.nii *.nii.gz)"));
	}
	if (path.isEmpty())	return;
	emit signalCurvedImageExport(path);
}

void MainWindow::slotOpenOverlay(QString path)
{
	if (path.isEmpty()) {
		path = QFileDialog::getOpenFileName((this),
			QString(tr("Import Segmentation")), path, tr("NIFTI Images (*.nii *.nii.gz)"));
	}
	if (path.isEmpty())	return;
	emit signalOverlayImportLoad(path);
}

void MainWindow::slotSaveOverlay(QString path)
{
	if (path.isEmpty()) {
		path = QFileDialog::getSaveFileName((this),
			QString(tr("Export Segmentation")), path, tr("NIFTI Images (*.nii *.nii.gz)"));
	}
	if (path.isEmpty())	return;
	emit signalOverlayExportSave(path);
}

void MainWindow::slotExportReport(QString path)
{
	if (path.isEmpty()) {
		path = QFileDialog::getSaveFileName((this),
			QString(tr("Export Report")), path, tr("Report (*.pdf)"));
	}
	if (path.isEmpty())	return;
	emit signalReportExport(path);

	measurementWidget->GenerateReport(path);
}

void MainWindow::slotExportCSV(QString path)
{
		if (path.isEmpty()) {
		path = QFileDialog::getSaveFileName((this),
			QString(tr("Export CSV")), path, tr("Comma-Seperated Values (*.csv)"));
	}
	if (path.isEmpty())	return;
	emit signalReportExport(path);

	measurementWidget->GenerateCSV(path);
}

void MainWindow::slotImage(bool flag)
{
	QAction *action = qobject_cast<QAction *>(sender());
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

void MainWindow::imageImport(QString path)
{
	RegistrationWizard rw(path, modalityNames.size(), this);
	for (int i = 0; i < modalityNames.size(); ++i) {
		rw.setImageModalityNames(i, modalityNames[i]);
	}
	QStringList _listOfFileNames;

	if (QWizard::Accepted == rw.exec()) {

		for (int i = 0; i < modalityNames.size(); ++i) {
			//if (!rw.getFileNames(i).isEmpty()) {
				qDebug() << rw.getFileNames(i);
				_listOfFileNames << rw.getFileNames(i);
			//}
		}

		emit signalImageImportLoad(_listOfFileNames);

		qDebug() << rw.getDirectory();

		adjustForCurrentFile(rw.getDirectory());

	}
}

void MainWindow::initialization()
{
	this->setEnabled(true);

	ui->centralwidget->setEnabled(true);

	ui->ActionToolBar->setEnabled(true);

	ui->actionMulti_planar_view->trigger();
	ui->actionNavigation->trigger();

	measurementWidget->slotUpdateInformation();

	this->ui->menuImage->setEnabled(true);
	this->ui->actionNavigation->setEnabled(true);
	this->ui->actionWindow_level->setEnabled(true);
	this->ui->actionMaximum_wall_thickness->setEnabled(true);
	this->ui->acitonVOI_selection->setEnabled(true);
	this->ui->actionTubular_VOI->setEnabled(true);
	this->ui->actionSeeds_placer->setEnabled(true);
	this->ui->actionVessel_segmentation->setEnabled(true);
	this->ui->actionCurved_view->setEnabled(true);
	this->ui->actionAll_axial_view->setEnabled(true);
	this->ui->actionMulti_planar_view->setEnabled(true);
	this->ui->actionMIP->setEnabled(true);
	this->ui->menuView->setEnabled(true);
	this->ui->actionNew_segmentation->setEnabled(true);
	this->ui->actionImport_segmentation->setEnabled(true);
	this->ui->actionExport_curved_images->setEnabled(true);
	this->ui->actionExport_segmentation->setEnabled(true);
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

Switch2DWidget * MainWindow::getSwitch2DWidget()
{
	return this->switch2DWidget;
}

Switch3DWidget * MainWindow::getSwitch3DWidget()
{
	return this->switch3DWidget;
}

ViewerWidget * MainWindow::getViewerWidget(unsigned int num)
{
	return this->viewerWidgets[num];
}

MEASUREMENT_WIDGET* MainWindow::getMeasurementWidget()
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

void MainWindow::dragEnterEvent(QDragEnterEvent * event)
{
	if (event->mimeData()->hasUrls()) {
		// default only count the first one
		QUrl url = event->mimeData()->urls().first();
		// only accept local file or local directory
		if (url.isLocalFile()) {
			event->acceptProposedAction();
		}
	}
}

void MainWindow::dropEvent(QDropEvent * event)
{
	QUrl url = event->mimeData()->urls().first();
	QFileInfo fileInfo(url.toLocalFile());
	slotOpen(fileInfo.absoluteFilePath());

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

void MainWindow::closeEvent(QCloseEvent *event)
{
	emit signalThisClosing();

	QMainWindow::closeEvent(event);
}

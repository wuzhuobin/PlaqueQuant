#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MyProgressDialog.h"
#include "ImageRegistration.h"
#include "SurfaceCreator.h"
#include "VesselSegmentation.h"
#include "ExtractCenterline.h"
#include "ModuleWidget.h"
#include "MeasurementWidget.hpp"
#include "GPUVolumeRenderingFilter.h"
#include "MeasurementFor3D.h"
#include "Centerline.h"

#include <algorithm>
#include <array>


#include <QUrl>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFuture>
#include <QMimeData>
#include <QSignalMapper>

#include <vtkClipPolyData.h>
#include <vtkAxesActor.h>
#include <vtkProp.h>
#include <vtkPolyDataMapper.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkDistanceWidget.h>
#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>
#include <vtkExtractVOI.h>
#include <vtkCamera.h>
#include <vtkImageThreshold.h>


//#include <itkImageFileWriter.h>

MainWindow::MainWindow() 
	:widgetGroup(this), viewerGroup(this), viewGroup(this),
	m_core(this), imageManager(m_core.GetMyImageManager()), ioManager(m_core.GetIOManager())
{
	this->ui = new Ui::MainWindow;
	this->ui->setupUi(this);
	
	//Initialize
	m_moduleWidget = new ModuleWidget(this);
	ui->widgetDockWidget->setWidget(m_moduleWidget);
	m_measurementWidget = new MeasurementWidget(this);
	ui->measurementDockWidget->setWidget(m_measurementWidget);
	ui->widgetDockWidget->setWidget(m_moduleWidget);
	this->tabifyDockWidget(ui->widgetDockWidget, ui->measurementDockWidget);
	ui->widgetDockWidget->raise();
	ui->widgetDockWidget->show();
	setActionsEnable(false);

	
	// Open Image
	connect(ui->actionOpenImage, SIGNAL(triggered()), ioManager, SLOT(slotOpenWithWizard()));
	connect(&m_core, SIGNAL(signalVisualizeAllViewers()), this, SLOT(slotVisualizeImage()));
	// Save Segmentaiton 
	connect(ui->actionSave, SIGNAL(triggered()), ioManager, SLOT(slotSaveSegmentaitonWithDiaglog()));
	// Open Segmentation
	connect(ui->actionOpenSegmentation, SIGNAL(triggered()), ioManager,
		SLOT(slotOpenSegmentationWithDiaglog()));
	// different mode
	widgetGroup.addAction(ui->actionNavigation);
	widgetGroup.addAction(ui->actionWindowLevel);
	widgetGroup.addAction(ui->actionContour);
	widgetGroup.addAction(ui->actionBrush);
	widgetGroup.addAction(ui->actionRuler);
	widgetGroup.addAction(ui->actionROI);
	widgetGroup.setExclusive(true);
	connect(ui->actionNavigation, SIGNAL(triggered()), &m_core, SLOT(slotNavigationMode()));
	connect(ui->actionWindowLevel, SIGNAL(triggered()), &m_core, SLOT(slotWindowLevelMode()));
	connect(ui->actionContour, SIGNAL(triggered()), &m_core, SLOT(slotContourMode()));
	connect(ui->actionBrush, SIGNAL(triggered()), &m_core, SLOT(slotBrushMode()));
	connect(ui->actionRuler, SIGNAL(triggered(bool)), &m_core, SLOT(slotRuler(bool)));
	connect(ui->actionROI, SIGNAL(triggered()), &m_core, SLOT(slotROIMode()));
	// view
	viewGroup.addAction(ui->actionMultiPlanarView);
	viewGroup.addAction(ui->actionAllAxialView);
	viewGroup.setExclusive(true);
	connect(ui->actionMultiPlanarView, SIGNAL(triggered()), &m_core, SLOT(slotMultiPlanarView()));
	connect(ui->actionAllAxialView, SIGNAL(triggered()), &m_core, SLOT(slotSegmentationView()));
	connect(&m_core, SIGNAL(signalMultiPlanarView()), ui->actionMultiPlanarView, SLOT(trigger()));
	connect(&m_core, SIGNAL(signalSegmentationView()), ui->actionAllAxialView, SLOT(trigger()));

	// slice 
	connect(this->ui->xSpinBox, SIGNAL(valueChanged(int)),
		&m_core, SLOT(slotChangeSliceX(int)), Qt::DirectConnection);
	connect(this->ui->ySpinBox, SIGNAL(valueChanged(int)),
		&m_core, SLOT(slotChangeSliceY(int)), Qt::DirectConnection);
	connect(this->ui->zSpinBox, SIGNAL(valueChanged(int)),
		&m_core, SLOT(slotChangeSliceZ(int)), Qt::DirectConnection);
	connect(&m_core, SIGNAL(signalChangeSliceX(int)), 
		ui->xSpinBox, SLOT(setValue(int)), Qt::DirectConnection);
	connect(&m_core, SIGNAL(signalChangeSliceY(int)),
		ui->ySpinBox, SLOT(setValue(int)), Qt::DirectConnection);
	connect(&m_core, SIGNAL(signalChangeSliceZ(int)), 
		ui->zSpinBox, SLOT(setValue(int)), Qt::DirectConnection);

	// UI
	connect(ui->actionExit,	SIGNAL(triggered()), this, SLOT(slotExit()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
	connect(ui->actionHelp,	SIGNAL(triggered()), this, SLOT(slotHelp()));
	
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

	//connect(ioManager, SIGNAL(finishOpenSegmentation()), this, SLOT(addOverlay2ImageViewer()));

	//3D view
	connect(ui->updateBtn, SIGNAL(clicked()), this, SLOT(slot3DUpdate()));

	//Initial Segmentation window
	segmentationView = false;

	//UI Setting
	ui->ULBtn2->setHidden(true);
	ui->URBtn2->setHidden(true);
	ui->LLBtn2->setHidden(true);
	ui->LRBtn2->setHidden(true);
	ui->viewGridLayout->removeWidget(ui->windowlevelwidget);
	ui->windowlevelwidget->setHidden(true);

	//Recent Image
	createRecentImageActions();

	// Initial Orientation
	m_orientation= SLICE_ORIENTATION_YZ;

	//Initial Visible image number
	visibleImageNum = 0;
	
	ui->image1View->SetRenderWindow(m_core.GetRenderWindow(0));
	ui->image2View->SetRenderWindow(m_core.GetRenderWindow(1));
	ui->image3View->SetRenderWindow(m_core.GetRenderWindow(2));
	ui->image4View->SetRenderWindow(m_core.GetRenderWindow(3));

	//ImageAlignment(NULL) = NULL;
	this->m_centerlinePD = NULL;

	m_InfoDialog = NULL;

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


MainWindow::~MainWindow()
{
	ui->image1View->SetRenderWindow(NULL);

	ui->image2View->SetRenderWindow(NULL);

	ui->image3View->SetRenderWindow(NULL);

	if (m_moduleWidget != NULL) {
		delete m_moduleWidget;
		m_moduleWidget = NULL;
	}
	if (m_InfoDialog != NULL) {
		delete m_InfoDialog;
		m_InfoDialog = NULL;
	}


}

void MainWindow::DisplayErrorMessage(std::string str)
{
	m_core.DisplayErrorMessage(str);

}

void MainWindow::setActionsEnable( bool b )
{	
	//switch after open the image
	//ui->actionOpenImage->setEnabled(!b);
	//ui->menuRecentImage->setEnabled(!b);
	ui->actionSave->setEnabled(b);
	ui->actionNavigation->setEnabled(b);
	ui->actionWindowLevel->setEnabled(b);
	ui->actionContour->setEnabled(b);
	ui->actionBrush->setEnabled(b);
	ui->actionRemoveContour->setEnabled(b);
	ui->actionRuler->setEnabled(b);
	ui->actionROI->setEnabled(b);
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

bool MainWindow::slotVisualizeImage()
{	

	//Enable Actions 
	setActionsEnable(true);

	//connected to slotNavigationMode()
	ui->actionNavigation->trigger();
	// update DICOM header imformation
	m_measurementWidget->slotUpdateImformation();

	// initialize image select function 
	ChangeBaseImageULMenu.clear();
	ChangeBaseImageURMenu.clear();
	ChangeBaseImageLLMenu.clear();
	for (int i = 0; i < this->imageManager->getListOfViewerInputImages().size(); i++)
	{
		if (this->imageManager->getListOfViewerInputImages()[i] != NULL) {
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
		&m_core, SLOT(slotChangeModality(QAction*)));

	ui->URSelectImgBtn->setMenu(&ChangeBaseImageURMenu);
	connect(&ChangeBaseImageURMenu, SIGNAL(triggered(QAction*)),
		&m_core, SLOT(slotChangeModality(QAction*)));

	ui->LLSelectImgBtn->setMenu(&ChangeBaseImageLLMenu);
	connect(&ChangeBaseImageLLMenu, SIGNAL(triggered(QAction*)),
		&m_core, SLOT(slotChangeModality(QAction*)));

	return 0;
}

void MainWindow::adjustForCurrentFile(const QString &filePath)
{
	QSettings settings("DIIR","Project");
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
	QSettings settings("DIIR","Project");
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
	QMessageBox::about(	NULL, 
						"About", 
                       QString("It is a program of displaying multi-modality images made by Andy Li (2016)")
					   );
}

void MainWindow::slotHelp()
{
	//Info Dialog
	m_InfoDialog = new Dialog(this);
	m_InfoDialog->show();
	m_InfoDialog->setModal(true);

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
	m_core.RenderAllViewer();
}

void MainWindow::slotChangeROI()
{
	int extent[6];
	m_core.m_style[0]->GetROI()->SelectROI(extent);
	m_moduleWidget->slotChangeROI(extent);
}

void MainWindow::slot3DUpdate()
{

	this->ui->image4View->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
	
	// Extract VOI
	vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	voi->SetInputData(imageManager->getOverlay().GetVTKOutput());
	voi->SetVOI(imageManager->getOverlay().GetDisplayExtent());
	voi->Update();

	// Threshold the iamge
	vtkSmartPointer<vtkImageThreshold> thres = vtkSmartPointer<vtkImageThreshold>::New();
	thres->SetInputData(voi->GetOutput());
	thres->ThresholdBetween(MainWindow::LABEL_LUMEN - 0.1, MainWindow::LABEL_LUMEN + 0.1);
	thres->SetOutValue(0);
	thres->SetInValue(10);
	thres->Update();

	/// Obtain centerline
	SurfaceCreator* surfaceCreator = new SurfaceCreator();
	surfaceCreator->SetInput(thres->GetOutput());
	surfaceCreator->SetValue(10);
	surfaceCreator->SetSmoothIteration(15);
	surfaceCreator->SetDiscrete(false);
	surfaceCreator->SetResamplingFactor(1.0);
	surfaceCreator->Update();

	// Clip at the end of the boundaries
	vtkSmartPointer<vtkPolyData> surface = vtkSmartPointer<vtkPolyData>::New();
	surface->DeepCopy(surfaceCreator->GetOutput());
	vtkSmartPointer<vtkPlane> plane[2];
	plane[0] = vtkSmartPointer<vtkPlane>::New();
	plane[1] = vtkSmartPointer<vtkPlane>::New();
	plane[0]->SetNormal(0, 0, -1);
	plane[1]->SetNormal(0, 0, 1);
	double zCoord1 = (imageManager->getOverlay().GetDisplayExtent()[5] - 1) * 
		this->imageManager->getListOfViewerInputImages()[0]->GetSpacing()[2] + 
		this->imageManager->getListOfViewerInputImages()[0]->GetOrigin()[2];
	double zCoord2 = (imageManager->getOverlay().GetDisplayExtent()[4] + 1) * 
		this->imageManager->getListOfViewerInputImages()[0]->GetSpacing()[2] + 
		this->imageManager->getListOfViewerInputImages()[0]->GetOrigin()[2];
	plane[0]->SetOrigin(0, 0, zCoord1);
	plane[1]->SetOrigin(0, 0, zCoord2);
	for (int i = 0; i < 2;i++)
	{
		vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
		clipper->SetClipFunction(plane[i]);
		clipper->SetInputData(surface);
		clipper->Update();
		surface->DeepCopy(clipper->GetOutput());
	}


	Centerline* cl = new Centerline;
	try {
		cl->SetSurface(surface);
		cl->Update();
	}
	catch (Centerline::ERROR_CODE e) {
		this->DisplayErrorMessage("Centerline calculations failed. Some functions might not be available. Please select an ROI with cylindrical structures. (Branches are allowed)");

		///Volume Render
		GPUVolumeRenderingFilter* volumeRenderingFilter =
			GPUVolumeRenderingFilter::New();

		volumeRenderingFilter->SetInputData(voi->GetOutput());
		volumeRenderingFilter->SetLookUpTable(this->GetMyImageViewer(0)->GetLookupTable());
		volumeRenderingFilter->GetVolume()->SetPickable(1);
		volumeRenderingFilter->Update();

		m_core.m_3DDataRenderer->AddVolume(volumeRenderingFilter->GetVolume());

		this->ui->image4View->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(0.3, 0.3, 0.3);
		this->ui->image4View->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
		this->ui->image4View->GetRenderWindow()->Render();
		m_core.m_style3D->SetInteractorStyleTo3DDistanceWidget();

		return;
	}


	if (m_centerlinePD != NULL) {
		this->m_centerlinePD->Delete();
		this->m_centerlinePD = NULL;
	}
	this->m_centerlinePD = vtkPolyData::New();
	this->m_centerlinePD->DeepCopy(cl->GetCenterline());

	delete cl;

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(this->m_centerlinePD);
	mapper->SetLookupTable(m_core.GetLookupTable());
	mapper->SetScalarRange(0, 6); // Change this too if you change the look up table!
	mapper->Update();
	vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
	//Actor->GetProperty()->SetColor(overlayColor[0][0]/255.0, overlayColor[0][1] / 255.0, overlayColor[0][2] / 255.0);
	Actor->SetMapper(mapper);
	Actor->GetProperty()->SetOpacity(0.9);
	Actor->GetProperty()->SetRepresentationToSurface();
	Actor->SetPickable(1);
	m_core.m_3DDataRenderer->AddActor(Actor);

	//vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper2->SetInputData(surface);
	//mapper2->SetLookupTable(this->LookupTable);
	//mapper2->SetScalarRange(0, 6); // Change this too if you change the look up table!
	//mapper2->Update();
	//vtkSmartPointer<vtkActor> Actor2 = vtkSmartPointer<vtkActor>::New();
	////Actor->GetProperty()->SetColor(overlayColor[0][0]/255.0, overlayColor[0][1] / 255.0, overlayColor[0][2] / 255.0);
	//Actor2->SetMapper(mapper2);
	//Actor2->GetProperty()->SetRepresentationToWireframe();
	//Actor2->SetPickable(1);
	//this->m_3DDataRenderer->AddActor(Actor2);

	///Volume Render
	GPUVolumeRenderingFilter* volumeRenderingFilter =
		GPUVolumeRenderingFilter::New();
	
	volumeRenderingFilter->SetInputData(voi->GetOutput());
	volumeRenderingFilter->SetLookUpTable(this->GetMyImageViewer(0)->GetLookupTable());
	volumeRenderingFilter->GetVolume()->SetPickable(1);
	//volumeRenderingFilter->GetVolumeProperty()->SetInterpolationTypeToLinear();
	volumeRenderingFilter->Update();

	m_core.m_3DDataRenderer->AddVolume(volumeRenderingFilter->GetVolume());
	this->ui->image4View->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(0.3,0.3,0.3);
	this->ui->image4View->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
	this->ui->image4View->GetRenderWindow()->Render();
	m_core.m_style3D->SetInteractorStyleTo3DDistanceWidget();
	//delete surfaceCreator;

	/*vtkRenderWindow* rwin = vtkRenderWindow::New();
	rwin->AddRenderer(vtkRenderer::New());
	rwin->GetRenderers()->GetFirstRenderer()->AddVolume(volumeRenderingFilter->GetVolume());
	vtkRenderWindowInteractor* interacotr = vtkRenderWindowInteractor::New();
	rwin->SetInteractor(interacotr);
	interacotr->Initialize();
	rwin->Render();
	interacotr->Start();*/
}

void MainWindow::resizeEvent( QResizeEvent * event )
{
	QMainWindow::resizeEvent(event);
	m_core.RenderAllViewer();
		//for (int i = 0; i < 3; i++)
		//{
		//	if (m_2DimageViewer[i] != NULL)
		//		m_2DimageViewer[i]->ResizeHeaderAndOrientationText();
		//}
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

MyImageViewer * MainWindow::GetMyImageViewer(int i)
{
	return m_core.m_2DimageViewer[i];
}

int MainWindow::GetVisibleImageNo()
{
	return visibleImageNum;
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

void MainWindow::RenderAllViewer()
{
	m_core.RenderAllViewer();
}

void MainWindow::RenderAll2DViewers()
{
	//QCoreApplication::processEvents();
	for (int i = 0; i < 3; ++i)
	{
		if (segmentationView && i >= visibleImageNum)
			break;
		m_core.m_2DimageViewer[i]->GetRenderWindow()->Render();
	}
}

void MainWindow::UpdateStenosisValue(double val)
{
	this->m_moduleWidget->UpdateStenosisValue(val);
}


InteractorStyleSwitch * MainWindow::GetInteractorStyleImageSwitch(int i)
{
	return m_core.m_style[i];
}

vtkRenderWindowInteractor * MainWindow::GetVtkRenderWindowInteractor(int i)
{
	return m_core.m_interactor[i];
}

vtkLookupTable * MainWindow::GetLookupTable()
{
	return m_core.GetLookupTable();
}

vtkPolyData * MainWindow::GetCenterlinePD()
{
	return this->m_centerlinePD;
}

Ui_MainWindow * MainWindow::GetUI()
{
	return this->ui;
}


//void MainWindow::Set3DRulerEnabled(bool b)
//{
//	MyVtkDistanceRepresentation3D* rep = MyVtkDistanceRepresentation3D::New();
//	rep->SetDistanceWidget(this->m_distance3DWidget);
//	this->m_distance3DWidget->SetInteractor(ui->image4View->GetRenderWindow()->GetInteractor());
//	this->m_distance3DWidget->SetRepresentation(rep);
//	this->m_distance3DWidget->On();
//}

//void MainWindow::slotCenterline()
//{
//	cout << "slotCenterLine" << endl;
//	// segmentation
//	//VesselSegmentation* vesselSegmentation = new VesselSegmentation();
//	//vesselSegmentation->SetT1(this->imageManager->getListOfITKImage()[0]);
//	//vesselSegmentation->SetInputSegmentationImage(SegmentationOverlay->GetITKOutput(itkImage[0]));
//	//vesselSegmentation->SetMPRAGE(itkImage[1]);
//	//vesselSegmentation->Update();
//
//	//FloatImageType::Pointer outputSegmentation = FloatImageType::New();
//	//vesselSegmentation->GetOutputSegmentationImage(outputSegmentation);
//	//itk::ImageFileWriter<FloatImageType>::Pointer imageFileWriter =
//	//	itk::ImageFileWriter<FloatImageType>::New();
//	//imageFileWriter->SetFileName("C:\\Users\\user\\Desktop\\seg.nii");
//	//imageFileWriter->Update();
//
//
//	//this->SegmentationOverlay->SetInputImageData(outputSegmentation);
//
//
//	//vtkSmartPointer<vtkPolyData> lumen = vtkSmartPointer<vtkPolyData>::New();
//	//vesselSegmentation->GetVesselLumenSurface(lumen);
//
//	//vtkSmartPointer<vtkPolyData> plaque = vtkSmartPointer<vtkPolyData>::New();
//	//vesselSegmentation->GetPlaqueSurface(plaque);
//
//	//vtkSmartPointer<vtkPolyData> wall = vtkSmartPointer<vtkPolyData>::New();
//	//vesselSegmentation->GetVesselWallSurface(wall);
//
//	//// calculate centerlines
//	//ExtractCenterline* lumenExtractCenterline = new ExtractCenterline;
//	//lumenExtractCenterline->SetSurface(lumen);
//	//lumenExtractCenterline->Update();
//	//vtkSmartPointer<vtkPolyData> lumenCapped = vtkSmartPointer<vtkPolyData>::New();
//	//lumenExtractCenterline->GetCappedSurface(lumenCapped);
//	//vtkSmartPointer<vtkPolyData> lumenCenterline = vtkSmartPointer<vtkPolyData>::New();
//	//lumenExtractCenterline->GetCenterline(lumenCenterline);
//
//	//ExtractCenterline* wallExtractCenterline = new ExtractCenterline;
//	//wallExtractCenterline->SetSurface(wall);
//	//wallExtractCenterline->Update();
//	//vtkSmartPointer<vtkPolyData> wallCapped = vtkSmartPointer<vtkPolyData>::New();
//	//wallExtractCenterline->GetCappedSurface(wallCapped);
//	//vtkSmartPointer<vtkPolyData> wallCenterline = vtkSmartPointer<vtkPolyData>::New();
//	//wallExtractCenterline->GetCenterline(wallCenterline);
//
//	//vtkSmartPointer<vtkPolyDataMapper> lumenMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	//lumenMapper->SetInputData(lumen);
//
//	//vtkSmartPointer<vtkPolyDataMapper> plaqueMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	//plaqueMapper->SetInputData(plaque);
//
//	//vtkSmartPointer<vtkPolyDataMapper> wallMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	//wallMapper->SetInputData(wall);
//
//	//// Create Lookup Table to match color
//	//vtkSmartPointer<vtkLookupTable> LUT = vtkSmartPointer<vtkLookupTable>::New();
//	//double LUTmin = min(lumenCenterline->GetScalarRange()[0], wallCenterline->GetScalarRange()[0]);
//	//double LUTmax = max(lumenCenterline->GetScalarRange()[1], wallCenterline->GetScalarRange()[1]);
//
//	//LUT->SetTableRange(LUTmin, LUTmax);
//	//LUT->Build();
//	//LUT->SetTableValue(0, 1.0, 0.0, 0.0, 1);  //Red
//	//LUT->SetTableValue(1, 0.0, 1.0, 0.0, 1); // Green
//	//LUT->SetTableValue(2, 0.0, 0.0, 1.0, 1); // Blue
//
//	//										 // scalar bar
//	//vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
//	//scalarBar->SetLookupTable(LUT);
//	//scalarBar->SetTitle("Vessel Radius");
//	//scalarBar->SetNumberOfLabels(4);
//
//	//vtkSmartPointer<vtkPolyDataMapper> lumenCenterlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	//lumenCenterlineMapper->SetInputData(lumenCenterline);
//	//lumenCenterlineMapper->ScalarVisibilityOn();
//	//lumenCenterlineMapper->SetScalarModeToUsePointData();
//	//lumenCenterlineMapper->SetColorModeToMapScalars();
//	//lumenCenterlineMapper->SetLookupTable(LUT);
//	//lumenCenterlineMapper->SetUseLookupTableScalarRange(1);
//
//	//vtkSmartPointer<vtkPolyDataMapper> wallCenterlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	//wallCenterlineMapper->SetInputData(wallCenterline);
//	//wallCenterlineMapper->ScalarVisibilityOn();
//	//wallCenterlineMapper->SetScalarModeToUsePointData();
//	//wallCenterlineMapper->SetColorModeToMapScalars();
//	//wallCenterlineMapper->SetLookupTable(LUT);
//	//wallCenterlineMapper->SetUseLookupTableScalarRange(1);
//
//	//vtkSmartPointer<vtkActor> lumenActor = vtkSmartPointer<vtkActor>::New();
//	//lumenActor->SetMapper(lumenMapper);
//	//lumenActor->GetProperty()->SetColor(1, 0, 0);
//	//lumenActor->GetProperty()->SetOpacity(0.5);
//
//	//vtkSmartPointer<vtkActor> plaqueActor = vtkSmartPointer<vtkActor>::New();
//	//plaqueActor->SetMapper(plaqueMapper);
//	//plaqueActor->GetProperty()->SetColor(0, 1, 0);
//	//plaqueActor->GetProperty()->SetOpacity(0.2);
//
//	//vtkSmartPointer<vtkActor> wallActor = vtkSmartPointer<vtkActor>::New();
//	//wallActor->SetMapper(wallMapper);
//	//wallActor->GetProperty()->SetColor(0, 0, 1);
//	//wallActor->GetProperty()->SetOpacity(0.3);
//
//	//vtkSmartPointer<vtkActor> lumenCenterlineActor = vtkSmartPointer<vtkActor>::New();
//	//lumenCenterlineActor->SetMapper(lumenCenterlineMapper);
//
//	//vtkSmartPointer<vtkActor> wallCenterlineActor = vtkSmartPointer<vtkActor>::New();
//	//wallCenterlineActor->SetMapper(wallCenterlineMapper);
//
//	//// pick point spheres
//	//vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
//	//sphere->SetRadius(0.5);
//	//sphere->SetCenter(lumenCenterline->GetPoint(0));
//
//	//vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	//sphereMapper->SetInputConnection(sphere->GetOutputPort());
//
//	//vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
//	//sphereActor->SetMapper(sphereMapper);
//	//sphereActor->GetProperty()->SetColor(1, 0, 0);
//	//m_3DDataRenderer->RemoveAllViewProps();
//	//m_3DDataRenderer->AddActor(lumenActor);
//	//m_3DDataRenderer->AddActor(wallActor);
//	//m_3DDataRenderer->AddActor(wallCenterlineActor);
//	//m_3DDataRenderer->AddActor(sphereActor);
//	//m_3DDataRenderer->AddActor2D(scalarBar);
//
//	//vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
//	//ren->AddActor(lumenActor);
//	////ren->AddActor(plaqueActor);
//	//ren->AddActor(wallActor);
//	////ren->AddActor(lumenCenterlineActor);
//	//ren->AddActor(wallCenterlineActor);
//	//ren->AddActor(sphereActor);
//	//ren->AddActor2D(scalarBar);
//
//	//vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
//	//renWin->AddRenderer(ren);
//
//	//vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	//vtkSmartPointer<MouseInteractorStylePP> style = vtkSmartPointer<MouseInteractorStylePP>::New();
//	//style->SetSurface(wall);
//	//style->SetCenterline(wallCenterline);
//	//style->SetSphere(sphere);
//	//iren->SetInteractorStyle(style);
//	//iren->SetRenderWindow(renWin);
//
//	m_core.m_3DDataRenderer->Render();
//
//}
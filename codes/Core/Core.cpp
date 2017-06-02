#include "Core.h"

#include "IADEOverlay.h"
#include "ui_MainWindow.h"
#include "ui_ModuleWidget.h"
#include "ui_ViewerWidget.h"
#include "ViewerWidget.h"
#include "ModuleWidget.h"
#include "ui_QAbstractNavigation.h"
#include "MeasurementWidget.h"
#include "LabelWidget.h"


#include <qdebug.h>
#include <QVTKInteractor.h>
#include <qsignalmapper.h>

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLookupTable.h>

Core::Core(QObject * parent)
	:
	imageManager(NUM_OF_IMAGES, parent),
	ioManager(parent),
	dataProcessor(parent),
	QObject(parent)
{
	ioManager.enableRegistration(true);
	//ioManager.enableRegistration(false);

	imageManager.setModalityName(0, "CUBE T1");
	imageManager.setModalityName(1, "CUBE T2");
	imageManager.setModalityName(2, "CUBE T1 + C");
	imageManager.setModalityName(3, "2D DIR/QIR");
	imageManager.setModalityName(4, "MPRAGE");
	mainWindow.addModalityNames("CUBE T1");
	mainWindow.addModalityNames("CUBE T2");
	mainWindow.addModalityNames("CUBE T1 + C");
	mainWindow.addModalityNames("2D DIR/QIR");
	mainWindow.addModalityNames("MPRAGE");


	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		
		imageViewers[i] = ImageViewer::New();
		imageViewers[i]->SetRenderWindow(mainWindow.getViewerWidget(i)->getUi()->qvtkWidget2->GetRenderWindow());
		imageViewers[i]->SetupInteractor(mainWindow.getViewerWidget(i)->getUi()->qvtkWidget2->GetInteractor());
		//imageViewers[i]->EnableDepthPeelingOn();

		// Never use below method to set the interactorsyle
		//imageInteractorStyle[i]->SetInteractor(imageInteractor[i]);
		imageInteractorStyle[i] = StyleSwitch::New();
		imageInteractorStyle[i]->SetImageViewer(imageViewers[i]);
		mainWindow.getViewerWidget(i)->getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(imageInteractorStyle[i]);
		
	}


	//surfaceInteractor = vtkRenderWindowInteractor::New();

	surfaceViewer = CenterlineSurfaceViewer::New();
	surfaceViewer->SetRenderWindow(mainWindow.getViewerWidget(MainWindow::NUM_OF_VIEWERS - MainWindow::NUM_OF_3D_VIEWERS)->getUi()->qvtkWidget2->GetRenderWindow());
	surfaceViewer->SetupInteractor(mainWindow.getViewerWidget(MainWindow::NUM_OF_VIEWERS - MainWindow::NUM_OF_3D_VIEWERS)->getUi()->qvtkWidget2->GetInteractor());
	surfaceViewer->EnableDepthPeelingOn();


	//surfaceViewer->EnableDepthSortingOn();


	// Never use below method to set the interactorsyle
	//surfaceInteractorStyle[i]->SetInteractor(imageInteractor[i]);
	surfaceInteractorStyle = StyleSwitch3D::New();
	surfaceInteractorStyle->SetSurfaceViewer(surfaceViewer);
	mainWindow.getViewerWidget(MainWindow::NUM_OF_VIEWERS - MainWindow::NUM_OF_3D_VIEWERS)->getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(surfaceInteractorStyle);

#ifdef PLAQUEQUANT_VER
	dataProcessor.imageInteractorStyle = imageInteractorStyle;
	dataProcessor.surfaceInteractorStyle = surfaceInteractorStyle;
	dataProcessor.imageManager = &imageManager;
#endif // PLAQUEQUANT_VER

	mainWindow.getUi()->sliceScrollArea->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevelThreshold());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetThreshold());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPaintBrush());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetLumenSeedsPlacer());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVOI());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVBDSmoker());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetTubularVOI());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetRuler());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetMaximumWallThickness());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPolygonDrawSeries());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVesselSegmentation2());
	//imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel()->show();
	//moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
	

	// connect changing mode
	connect(mainWindow.getUi()->actionTesting, SIGNAL(triggered()),
		this, SLOT(slotTesting()));
	connect(mainWindow.getUi()->actionNavigation, SIGNAL(triggered()),
		this, SLOT(slotNavigation()));
	connect(mainWindow.getUi()->actionWindow_level, SIGNAL(triggered()),
		this, SLOT(slotWindowLevel()));
	connect(mainWindow.getUi()->actionThreshold, SIGNAL(triggered()),
		this, SLOT(slotThreshold()));
	connect(mainWindow.getUi()->actionWindow_level_threshold, SIGNAL(triggered()),
		this, SLOT(slotWindowlevelThreshold()));
	connect(mainWindow.getUi()->actionPaint_brush, SIGNAL(triggered()),
		this, SLOT(slotPaintBrush()));
	connect(mainWindow.getUi()->actionSeeds_placer, SIGNAL(triggered()),
		this, SLOT(slotSeedsPlacer()));
	connect(mainWindow.getUi()->acitonVOI_selection, SIGNAL(triggered()),
		this, SLOT(slotVOI()));
	connect(mainWindow.getUi()->actionTubular_VOI, SIGNAL(triggered()),
		this, SLOT(slotTubularVOI()));
	connect(mainWindow.getUi()->actionDistance_measure, SIGNAL(triggered()),
		this, SLOT(slotRuler()));
	connect(mainWindow.getUi()->actionMaximum_wall_thickness, SIGNAL(triggered()),
		this, SLOT(slotMaximumWallThickness()));
	connect(mainWindow.getUi()->actionPolygon_draw, SIGNAL(triggered()),
		this, SLOT(slotPolygonDraw()));
	connect(mainWindow.getUi()->actionPolygon_draw_series, SIGNAL(triggered()),
		this, SLOT(slotPolygonDrawSeries()));
	connect(mainWindow.getUi()->actionVessel_segmentation, SIGNAL(triggered()),
		this, SLOT(slotVesselSegmentation()));
	connect(mainWindow.getUi()->actionVBD_Smoker, SIGNAL(triggered()),
		this, SLOT(slotVBDSmoker()));




	connect(&mainWindow, SIGNAL(signalImageImportLoad(QList<QStringList>*)),
		&ioManager, SLOT(slotAddToListOfFileNamesAndOpen(QList<QStringList>*)));
	
	// find loading images and overlay
	connect(&ioManager, SIGNAL(signalFinishOpenMultiImages()),
		this, SLOT(slotIOManagerToImageManager()));
	connect(&ioManager, SIGNAL(signalFinishOpenOverlay()),
		this, SLOT(slotOverlayToImageManager()));


	// import and export overlay
	connect(&mainWindow, SIGNAL(signalOverlayImportLoad(QString)), 
		&ioManager, SLOT(slotOpenSegmentation(QString)));
	connect(&mainWindow, SIGNAL(signalOverlayExportSave(QString)),
		&ioManager, SLOT(slotSaveSegmentation(QString)));

	// new overlay
	connect(mainWindow.getUi()->actionNew_segmentation, SIGNAL(triggered()),
		&ioManager, SLOT(slotInitializeOverlay()));

	// change view mode
	connect(mainWindow.getUi()->actionCurved_view, SIGNAL(toggled(bool)),
		this, SLOT(slotCurvedView(bool)));
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		QPushButton* buttons[3] = {
			mainWindow.getViewerWidget(i)->getUi()->pushButtonSigitalView,
			mainWindow.getViewerWidget(i)->getUi()->pushButtonCoronalView,
			mainWindow.getViewerWidget(i)->getUi()->pushButtonAxialView };
		connect(mainWindow.getUi()->actionMulti_planar_view, SIGNAL(triggered()),
			buttons[i%3], SLOT(click()));
		connect(mainWindow.getUi()->actionAll_axial_view, SIGNAL(triggered()),
			buttons[2], SLOT(click()));
	}
	//connect(mainWindow.getUi()->actionAll_axial_view, SIGNAL(triggered()),
	//	this, SLOT(slotAllAxialView()));
	//connect(mainWindow.getUi()->actionMulti_planar_view, SIGNAL(triggered()),
	//	this, SLOT(slotMultiPlanarView()));
	// change image
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		connect(mainWindow.getSelectImgMenu(i), SIGNAL(triggered(QAction*)),
			this, SLOT(slotChangeImage(QAction*)));
		connect(mainWindow.getSelectImgMenu(i), SIGNAL(triggered(QAction*)),
			this, SLOT(slotChangeImage(QAction*)));
	}
	// change slice orientation
	QSignalMapper* sliceOrientationMapperA = new QSignalMapper(this);
	QSignalMapper* sliceOrientationMapperS = new QSignalMapper(this);
	QSignalMapper* sliceOrientationMapperC = new QSignalMapper(this);
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		sliceOrientationMapperA->setMapping(mainWindow.getViewerWidget(i)->getUi()->pushButtonAxialView, i);
		sliceOrientationMapperS->setMapping(mainWindow.getViewerWidget(i)->getUi()->pushButtonSigitalView, i);
		sliceOrientationMapperC->setMapping(mainWindow.getViewerWidget(i)->getUi()->pushButtonCoronalView, i);
		connect(mainWindow.getViewerWidget(i)->getUi()->pushButtonAxialView, SIGNAL(clicked()),
			sliceOrientationMapperA, SLOT(map()));
		connect(mainWindow.getViewerWidget(i)->getUi()->pushButtonSigitalView, SIGNAL(clicked()),
			sliceOrientationMapperS, SLOT(map()));
		connect(mainWindow.getViewerWidget(i)->getUi()->pushButtonCoronalView, SIGNAL(clicked()),
			sliceOrientationMapperC, SLOT(map()));
	}
	connect(sliceOrientationMapperA, SIGNAL(mapped(int)),
		this, SLOT(slotChangeSliceOrientationToXY(int)));
	connect(sliceOrientationMapperS, SIGNAL(mapped(int)),
		this, SLOT(slotChangeSliceOrientationToYZ(int)));
	connect(sliceOrientationMapperC, SIGNAL(mapped(int)),
		this, SLOT(slotChangeSliceOrientationToXZ(int)));

	// surface action
	connect(mainWindow.getUi()->actionTraceball_camera, SIGNAL(triggered()),
		this, SLOT(slotTrackballCamera()));
	connect(mainWindow.getUi()->actionCenter_line, SIGNAL(triggered()),
		this, SLOT(slotCenterLine()));
	connect(mainWindow.getUi()->actionFind_maximum_radius, SIGNAL(triggered()),
		this, SLOT(slotFindMaximumRadius()));
	connect(mainWindow.getUi()->actionPerpendicular_measurement, SIGNAL(triggered()),
		this, SLOT(slotPerpendicularMeasurement()));
	connect(mainWindow.getUi()->actionCurved_navigation, SIGNAL(triggered()),
		this, SLOT(slotCurvedNavigation()));
	connect(mainWindow.getUi()->actionWay_point_centerline, SIGNAL(triggered()),
		this, SLOT(slotWaypoint()));
	connect(mainWindow.getUi()->actionStenosis, SIGNAL(triggered()),
		this, SLOT(slotStenosis()));

	// update btn
	connect(mainWindow.getUi()->updateBtn, SIGNAL(clicked()),
		this, SLOT(slotUpdateSurfaceView()));
	connect(mainWindow.getUi()->updateBtn, SIGNAL(clicked()),
		mainWindow.getUi()->actionTraceball_camera, SLOT(trigger()));
	connect(mainWindow.getUi()->actionUpdate_curved_images, SIGNAL(triggered()),
		this, SLOT(slotInitializeCurvedImage()));

	mainWindow.getUi()->actionThreshold->setVisible(false);
	mainWindow.getUi()->actionTesting->setVisible(false);
	mainWindow.getUi()->actionWindow_level_threshold->setVisible(false);
	mainWindow.getUi()->actionPolygon_draw->setVisible(false);
	mainWindow.getUi()->actionPolygon_draw_series->setVisible(false);
	mainWindow.getUi()->actionPolygon_draw_series->setVisible(false);
	mainWindow.getUi()->actionDistance_measure->setVisible(false);
	mainWindow.getUi()->actionVBD_Smoker->setVisible(false);
	mainWindow.getUi()->actionPerpendicular_measurement->setVisible(false);
	mainWindow.getUi()->actionFind_maximum_radius->setVisible(false);
	mainWindow.getUi()->actionWay_point_centerline->setVisible(false);
	mainWindow.getUi()->actionCurved_navigation->setVisible(false);
	mainWindow.setWindowTitle("PlaqueQuant - v3.0");
	mainWindow.show();
}

Core::~Core()
{

	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		
		imageInteractorStyle[i]->Delete();
		imageInteractorStyle[i] = nullptr;

		
		mainWindow.getViewerWidget(i)->getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(nullptr);
		//imageInteractor[i]->Delete();
		//imageInteractor[i] = nullptr;
		
		imageViewers[i]->Delete();
		imageViewers[i] = nullptr;

		//mainWindow.setRenderWindow(i, nullptr);
	}
}

void Core::slotIOManagerToImageManager()
{
	for (int i = 0; i < NUM_OF_IMAGES; ++i) {
			imageManager.setImage(i, ioManager.getListOfImage()[i]);
			imageManager.setDicomIO(i, ioManager.getListOfDicomIOs()[i]);
	}

	// set input to image viewer
	ioManager.slotInitializeOverlay();


	// update selectImgMenus 
	for (int i = 0; i < NUM_OF_IMAGES; ++i) {
		mainWindow.setSelectImgMenuVisible(i, imageManager.getImage(i));
	}
	// reset display extent first, otherwise the slice spin box cannot update right
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageViewers[i]->ResetDisplayExtent();
	}
	// initialization, and trigger the navigation interactorstyle
	mainWindow.getMeasurementWidget()->wind1 = imageViewers[2]->GetRenderWindow();
	mainWindow.getMeasurementWidget()->wind2 = surfaceViewer->GetRenderWindow();
	mainWindow.getMeasurementWidget()->info = imageManager.getDicomIO(0);
	mainWindow.getMeasurementWidget()->slotUpdateImformation();
	mainWindow.initialization();
	const int* extent = imageViewers[DEFAULT_IMAGE]->GetDisplayExtent();
	imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->SetCurrentFocalPointWithImageCoordinate(
		(extent[1] - extent[0])*0.5,
		(extent[3] - extent[2])*0.5,
		(extent[5] - extent[4])*0.5
	);

	// clear the memory later, sometimes it will clear too early
	// make no different, if it has not been clear
	//ioManager.clearListOfItkImages();
	//ioManager.clearListOfDicoms();
}

void Core::slotOverlayToImageManager()
{
	imageManager.setOverlay(ioManager.getOverlay());

	// Opacity

	connect(mainWindow.getLabelWidget(), SIGNAL(signalOpacityRequested(int)),
		imageManager.getOverlay(), SLOT(slotRequestOpacity(int)));
	connect(imageManager.getOverlay(), SIGNAL(signalGetRequestedOpacity(double)),
		mainWindow.getLabelWidget(), SLOT(slotSetOpacity(double)));
	connect(mainWindow.getLabelWidget(), SIGNAL(signalOpacityChanged(int, double)),
		imageManager.getOverlay(), SLOT(slotSetOpacity(int, double)));
	connect(mainWindow.getLabelWidget(), SIGNAL(signalOpacityChanged(int, double)),
		this, SLOT(slotRenderALlViewers()));
	
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		slotUpdateImageViewersToCurrent(i);
	}




	// clear the memory later, sometimes it will clear too early
	// make no different, if it has not been clear
	//ioManager.clearOverlay();
}

void Core::slotUpdateMeasurements()
{
	//IADEOverlay* overlay = qobject_cast<IADEOverlay*>(sender());
	OVERLAY* overlay = dynamic_cast<OVERLAY*>(sender());
	if (overlay && overlay->Measurements2D.contains(overlay->getCurrentSlice())) {
		mainWindow.getMeasurementWidget()->slotUpdate2DMeasurements(overlay->Measurements2D[overlay->getCurrentSlice()].data());
		mainWindow.getMeasurementWidget()->slotUpdate3DMeasurements(overlay->Measurements3D);
	}
	mainWindow.getMeasurementWidget()->slotUpdateStenosis(
		surfaceInteractorStyle->GetStenosis()->GetStenosisValue()
	);
}


void Core::slotNavigation()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToNavigation();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation());
}

void Core::slotWindowLevel()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToWindowLevel();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
}

void Core::slotThreshold()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToThreshold();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetThreshold());

}

void Core::slotWindowlevelThreshold()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToWindowLevelThreshold();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevelThreshold());
}

void Core::slotPaintBrush()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToPaintBrush();
		//imageInteractorStyle[i]->GetPaintBrush()->SetOverlay(imageManager.getOverlay()->getData());
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPaintBrush());

}

void Core::slotTesting()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToInteractorStyleTesting();
	}
	mainWindow.getModuleWidget()->setWidget(nullptr);

}

void Core::slotSeedsPlacer()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToLumenSeedsPlacer();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetLumenSeedsPlacer());

}

void Core::slotVOI()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToVOI();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVOI());

}

void Core::slotTubularVOI()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToTubularVOI();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetTubularVOI());

}

void Core::slotRuler()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToRuler();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetRuler());

}

void Core::slotMaximumWallThickness()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToMaximumWallThickness();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetMaximumWallThickness());
}

void Core::slotPolygonDraw()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToPolygonDraw();
	}
	mainWindow.getModuleWidget()->setWidget(nullptr);
}

void Core::slotPolygonDrawSeries()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToPolygonDrawSeries();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPolygonDrawSeries());

}

void Core::slotVesselSegmentation()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToVesselSegmentation2();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVesselSegmentation2());
}

void Core::slotTrackballCamera()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DTrackballCamera();
}

void Core::slotCenterLine()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DCenterLine();

}

void Core::slotFindMaximumRadius()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DFindMaximumRadius();
}

void Core::slotPerpendicularMeasurement()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DPerpendicularMeasurement();
}

void Core::slotCurvedNavigation()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DCurvedNavigation();
}

void Core::slotWaypoint()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DWaypoint();
}

void Core::slotStenosis()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DStenosis();
}

void Core::slotVBDSmoker()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToVBDSmoker();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVBDSmoker());
}
#include <qmessagebox.h>
#include <vtkPolyData.h>
void Core::slotInitializeCurvedImage()
{
#ifdef PLAQUEQUANT_VER
	if (!surfaceViewer->GetCenterline() || surfaceViewer->GetCenterline()->GetNumberOfPoints() < 2) {
		QMessageBox::critical(&mainWindow, QString("No centerline"),
			QString("Please Generate centerline first !"));
		return;
	}
	dataProcessor.initializeCurved();
#endif // PLAQUEQUANT_VER

}

void Core::slotChangeImage(QAction * action)
{
	QMenu* selectImgMenu = static_cast<QMenu*>(sender());
	unsigned int viewer = 0;
	for (int i = 0; i < MainWindow::NUM_OF_VIEWERS; ++i) {
		if (selectImgMenu == mainWindow.getSelectImgMenu(i)) {
			viewer = i;
			break;
		}
	}
	int image = imageManager.getIndexOfModalityName(action->text());
	slotChangeImage(viewer, image);
}

void Core::slotChangeImage(int viewer, int image)
{
	currentImage[viewer] = image;
	slotUpdateImageViewersToCurrent(viewer);
}

void Core::slotChangeCurved(int viewer, bool curvedFlag)
{
	currentCurved[viewer] = curvedFlag;
	slotUpdateImageViewersToCurrent(viewer);
}

void Core::slotChangeSliceOrientation(int viewer, int sliceOrientation)
{
	currentSliceOrientation[viewer] = sliceOrientation;
	slotUpdateImageViewersToCurrent(viewer);
}

void Core::slotChangeSliceOrientationToYZ(int viewer)
{
	slotChangeSliceOrientation(viewer, ImageViewer::SLICE_ORIENTATION_YZ);
}

void Core::slotChangeSliceOrientationToXZ(int viewer)
{
	slotChangeSliceOrientation(viewer, ImageViewer::SLICE_ORIENTATION_XZ);

}

void Core::slotChangeSliceOrientationToXY(int viewer)
{
	slotChangeSliceOrientation(viewer, ImageViewer::SLICE_ORIENTATION_XY);
}

void Core::slotUpdateImageViewersToCurrent(int viewer)
{

	imageViewers[viewer]->InitializeHeader(imageManager.getModalityName(currentImage[viewer]).toStdString());
	imageViewers[viewer]->SetLookupTable(imageManager.getOverlay()->getLookupTable());
	if (currentCurved[viewer]) {
#ifdef PLAQUEQUANT_VER
		imageViewers[viewer]->SetOverlay(imageManager.getCurvedPlaqueQuantOverlay()->getData());
		imageViewers[viewer]->SetInputData(imageManager.getCurvedImage(currentImage[viewer]));
		// Measurement 
		// tmp fix
		disconnect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			imageManager.getOverlay(), SLOT(setCurrentSlice(int)));
		disconnect(imageManager.getOverlay(), SIGNAL(signalUpdatedOverlay()),
			this, SLOT(slotUpdateMeasurements()));
		connect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			imageManager.getCurvedPlaqueQuantOverlay(), SLOT(setCurrentSlice(int)), static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(imageManager.getCurvedPlaqueQuantOverlay(), SIGNAL(signalUpdatedOverlay()),
			this, SLOT(slotUpdateMeasurements()), static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
#endif // PLAQUEQUANT_VER
	}
	else
	{
		imageViewers[viewer]->SetOverlay(imageManager.getOverlay()->getData());
		imageViewers[viewer]->SetInputData(imageManager.getImage(currentImage[viewer]));
#ifdef PLAQUEQUANT_VER
		// Measurement 
		// tmp fix
		disconnect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			imageManager.getCurvedPlaqueQuantOverlay(), SLOT(setCurrentSlice(int)));
		disconnect(imageManager.getCurvedPlaqueQuantOverlay(), SIGNAL(signalUpdatedOverlay()),
			this, SLOT(slotUpdateMeasurements()));
#endif // PLAQUEQUANT_VER		
		connect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			imageManager.getOverlay(), SLOT(setCurrentSlice(int)), static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(imageManager.getOverlay(), SIGNAL(signalUpdatedOverlay()),
			this, SLOT(slotUpdateMeasurements()), static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

	}
	imageViewers[viewer]->SetSliceOrientation(currentSliceOrientation[viewer]);
	imageViewers[viewer]->Render();

	//mainWindow.getViewerWidget(viewer)->setWindowTitle(imageManager.getModalityName(currentImage[viewer]));

}


void Core::slotCurvedView(bool flag)
{
#ifdef PLAQUEQUANT_VER
	if (!surfaceViewer->GetCenterline() || surfaceViewer->GetCenterline()->GetNumberOfPoints() < 2) {
		QMessageBox::critical(&mainWindow, QString("No centerline"),
			QString("Please Generate centerline first !"));
		return;
	}
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		if (!imageManager.getCurvedPlaqueQuantOverlay()) {
			dataProcessor.initializeCurved();
		}
		currentCurved[i] = flag;
		slotUpdateImageViewersToCurrent(i);

	}
#endif // PLAQUEQUANT_VER
}



void Core::slotUpdateSurfaceView()
{
	// temporary fix for real time updated.
	vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
	if (currentCurved[DEFAULT_IMAGE]) {
#ifdef PLAQUEQUANT_VER
		image->ShallowCopy(imageManager.getCurvedPlaqueQuantOverlay()->getData());
#endif // PLAQUEQUANT_VER
	}
	else {
		image->ShallowCopy(imageManager.getOverlay()->getData());
	}
	//surfaceViewer->SetInputData(imageManager.getOverlay()->getData());
	surfaceViewer->SetInputData(image);
	surfaceViewer->SetLookupTable(imageManager.getOverlay()->getLookupTable());
	surfaceViewer->SetCenterline(static_cast<OVERLAY*>(imageManager.getOverlay())->getCenterLine());
	surfaceViewer->GetRenderer()->ResetCameraClippingRange();
	surfaceViewer->GetRenderer()->ResetCamera();
	surfaceViewer->Render();

	mainWindow.getUi()->menuSurface->setEnabled(true);
	mainWindow.getUi()->menuSegmentation->setEnabled(true);

}

void Core::slotRenderALlViewers()
{
	imageViewers[0]->Render();
	imageViewers[1]->Render();
	imageViewers[2]->Render();
	surfaceViewer->Render();
}


#ifndef __CORE_H__
#define __CORE_H__

//#define PLAQUEQUANT_VER

#ifdef PLAQUEQUANT_VER
#include "PlaqueQuantIOManager.h"
#include "PlaqueQuantImageManager.h"
#include "PlaqueQuantOverlay.h"
#include "PlaqueQuantMeasurement.h"
#define IO_MANAGER PlaqueQuantIOManager
#define IMAGE_MANAGER PlaqueQuantImageManager
#define OVERLAY PlaqueQuantOverlay
#define MEASUREMENT PlaqueQuantMeasurement
#endif // PLAQUEQUANT_VER



#ifdef IADE_VER
#include "IADEIOManager.h"
#include "IADEImageManager.h"
#include "IADEOverlay.h"
#include "IADEMeasurement.h"
#define IO_MANAGER IADEIOManager
#define IMAGE_MANAGER IADEImageManager 
#define OVERLAY IADEOverlay
#define MEASUREMENT IADEMeasurement
#endif // IADE_VER

#include <qobject.h>


#include "MainWindow.h"


#include "StyleSwitch.h"
#include "StyleSwitch3D.h"
#include "CenterlineSurfaceViewer.h"
#include "ImageViewer.h"
#include "DataProcessor.h"
#include "Yolk3DSeries.h"


class Core: public QObject
{
	Q_OBJECT
public:

	const static unsigned short NUM_OF_IMAGES = 5;
	const static unsigned short DEFAULT_IMAGE = 0;

	Core(QObject* parent = nullptr);
	~Core();

private slots:
	

	void slotIOManagerToImageManager();
	void slotOverlayToImageManager();
	void slotSaveCurvedImage(QString fileName);
	void slotMIP(bool flag);
	// Image interactorstyle
	void slotNavigation();
	void slotWindowLevel();
	void slotThreshold();
	void slotWindowlevelThreshold();
	void slotPaintBrush();
	void slotTesting();
	void slotSeedsPlacer();
	void slotVOI();
	void slotTubularVOI();
	void slotRuler();
	void slotMaximumWallThickness();
	void slotPolygonDraw();
	void slotPolygonDrawSeries();
	void slotVesselSegmentation();
	void slotVBDSmokerSeed();



	// Surface interactorStyle
	void slotTrackballCamera();
	void slotCenterLine();
	void slotFindMaximumRadius();
	void slotICDADiameter();
	void slotVBDBADiameter();
	void slotPerpendicularMeasurement();
	void slotCurvedNavigation();
	void slotWaypoint();
	void slotStenosis();
	void slotVBDUboguMeasure();


	// Curved image updates
	bool slotInitializeCurvedImage();

	void slotChangeImage(QAction* action);
	void slotChangeImage(int viewer, int image);
	void slotChangeCurved(int viewer, bool curvedFlag);
	void slotChangeSliceOrientation(int viewer, int sliceOrientation);
	void slotChangeSliceOrientationToYZ(int viewer);
	void slotChangeSliceOrientationToXZ(int viewer);
	void slotChangeSliceOrientationToXY(int viewer);
	void slotUpdateImageViewersToCurrent(int viewer);
	void slotCurvedView(bool flag);

	// Surface viewers
	void slotUpdateSurfaceView();
	void slotChangeOrientationToYZ(int viewer);
	void slotChangeOrientationToXZ(int viewer);
	void slotChangeOrientationToXY(int viewer);
	void slotUpdateSurfaceViewersToCurrent(int viewer);



	void slotRenderALlViewers();


private:
	MainWindow mainWindow;
	IO_MANAGER ioManager;
	IMAGE_MANAGER imageManager;
	MEASUREMENT measurement;
	DataProcessor dataProcessor;
	Yolk3DSeries mipViewer;
	vtkRenderer* mipRenderer[MainWindow::NUM_OF_2D_VIEWERS];
	vtkSmartPointer<vtkPolyDataMapper> mipPolyDataMapper[MainWindow::NUM_OF_2D_VIEWERS];
	

	int currentImage[MainWindow::NUM_OF_2D_VIEWERS] = {DEFAULT_IMAGE};
	bool currentCurved[MainWindow::NUM_OF_2D_VIEWERS] = { false };
	int currentSliceOrientation[MainWindow::NUM_OF_2D_VIEWERS] = {
		ImageViewer::SLICE_ORIENTATION_YZ,
		ImageViewer::SLICE_ORIENTATION_XZ,
		ImageViewer::SLICE_ORIENTATION_XY};
	//int currentMode[MainWindow::NUM_OF_2D_VIEWERS] = { MULTIPLANAR_VIEW };
	ImageViewer* imageViewers[MainWindow::NUM_OF_2D_VIEWERS];
	StyleSwitch* imageInteractorStyle[MainWindow::NUM_OF_2D_VIEWERS];

	int currentOrientation[MainWindow::NUM_OF_3D_VIEWERS] = {
		ImageViewer::SLICE_ORIENTATION_XY };
	CenterlineSurfaceViewer* surfaceViewer[MainWindow::NUM_OF_3D_VIEWERS];
	StyleSwitch3D* surfaceInteractorStyle[MainWindow::NUM_OF_3D_VIEWERS];

};



#endif // !__CORE_H__
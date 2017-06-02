#ifndef __CORE_H__
#define __CORE_H__

//#define PLAQUEQUANT_VER
//#define IADE_VER
#define DEBUG_VER
#ifdef DEBUG_VER
#define PLAQUEQUANT_VER
#endif // DEBUG_VER

#ifdef PLAQUEQUANT_VER
#include "PlaqueQuantIOManager.h"
#include "PlaqueQuantImageManager.h"
#include "PlaqueQuantOverlay.h"
#define IO_MANAGER PlaqueQuantIOManager
#define IMAGE_MANAGER PlaqueQuantImageManager
#define OVERLAY PlaqueQuantOverlay
#endif // PLAQUEQUANT_VER



#ifdef IADE_VER
#include "IADEIOManager.h"
#include "IADEImageManager.h"
#include "IADEOverlay.h"
#define IO_MANAGER IADEIOManagerIO
#define IMAGE_MANAGER IADEImageManager 
#define OVERLAY IADEOverlay
#endif // IADE_VER

#include <qobject.h>


#include "MainWindow.h"


#include "StyleSwitch.h"
#include "StyleSwitch3D.h"
#include "CenterlineSurfaceViewer.h"
#include "ImageViewer.h"
#include "DataProcessor.h"


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
	void slotUpdateMeasurements();
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

	// Surface interactorStyle
	void slotTrackballCamera();
	void slotCenterLine();
	void slotFindMaximumRadius();
	void slotPerpendicularMeasurement();
	void slotCurvedNavigation();
	void slotWaypoint();
	void slotStenosis();

	// IADE specified
	void slotVBDSmoker();

	// Curved image update
	void slotInitializeCurvedImage();

	void slotChangeImage(QAction* action);
	void slotChangeImage(int viewer, int image);
	void slotChangeCurved(int viewer, bool curvedFlag);
	void slotChangeSliceOrientation(int viewer, int sliceOrientation);
	void slotChangeSliceOrientationToYZ(int viewer);
	void slotChangeSliceOrientationToXZ(int viewer);
	void slotChangeSliceOrientationToXY(int viewer);
	void slotUpdateImageViewersToCurrent(int viewer);
	void slotCurvedView(bool flag);

	void slotUpdateSurfaceView();

	void slotRenderALlViewers();


private:
	MainWindow mainWindow;
	IO_MANAGER ioManager;
	IMAGE_MANAGER imageManager;
	DataProcessor dataProcessor;


	int currentImage[MainWindow::NUM_OF_2D_VIEWERS] = {DEFAULT_IMAGE};
	bool currentCurved[MainWindow::NUM_OF_2D_VIEWERS] = { false };
	int currentSliceOrientation[MainWindow::NUM_OF_2D_VIEWERS] = {
		ImageViewer::SLICE_ORIENTATION_YZ,
		ImageViewer::SLICE_ORIENTATION_XZ,
		ImageViewer::SLICE_ORIENTATION_XY};
	//int currentMode[MainWindow::NUM_OF_2D_VIEWERS] = { MULTIPLANAR_VIEW };
	ImageViewer* imageViewers[MainWindow::NUM_OF_2D_VIEWERS];
	StyleSwitch* imageInteractorStyle[MainWindow::NUM_OF_2D_VIEWERS];

	CenterlineSurfaceViewer* surfaceViewer;
	StyleSwitch3D* surfaceInteractorStyle;


	unsigned int m_viewMode = -1;
};



#endif // !__CORE_H__
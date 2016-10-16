#ifndef __CORE_H__
#define __CORE_H__

#include <QWidget>

#include <vtkSmartPointer.h>

#include "MyImageViewer.h"
#include "InteractorStyleSwitch.h"
#include "InteractorStyleSwitch3D.h"
#include "MyImageManager.h"
#include "IOManager.h"
#include "MyWidgetManager.h"


class Core:public QWidget
{
	Q_OBJECT
public:
	Core(QWidget* parent = nullptr);
	~Core();

	void Initialization();

	// Get functions
	vtkSmartPointer<vtkPolyData> GetCenterlinePD();
	vtkLookupTable*		GetLookupTable();
	vtkRenderWindow*	GetRenderWindow(int num);
	IOManager*			GetIOManager();
	MyImageManager*		GetMyImageManager();

	void RenderAllViewer();
	void DisplayErrorMessage(std::string str);
	
	enum VIEW_MODE
	{
		MULTIPLANAR_VIEW = 0,
		SEGMENTATION_VIEW = 1
	};
	
	enum LABEL_MAPPING {
		LABEL_NULL = 0,
		LABEL_LUMEN = 1,
		LABEL_VESSEL_WALL = 2,
		LABEL_CALCIFICATION = 3,
		LABEL_HEMORRHAGE = 4,
		LABEL_IRNC = 5,
		LABEL_LM = 6
	};

	const static int DEFAULT_IMAGE = 0;
	const static int VIEWER_NUM = 3;


public slots:

	// Initialization
	virtual void slotVisualizeAll2DViewers();
	virtual void slotAddOverlayToImageViewer();

	// Modality
	virtual void slotChangeModality(QAction* action);
	virtual void slotChangeModality(QString modalityName, int viewerNum);
	
	// View Modes
	virtual void slotSegmentationView();
	virtual void slotMultiPlanarView();
	virtual void slotChangeView(int viewMode);

	// Viewers's slice position
	virtual void slotChangeSlice(int slice);
	virtual void slotChangeSliceX(int x);
	virtual void slotChangeSliceY(int y);
	virtual void slotChangeSliceZ(int z);
	virtual void slotChangeFocalPointWithImageCoordinate(int i, int j, int k);

	// mode stuff
	virtual void slotNavigationMode();
	virtual void slotWindowLevelMode();
	virtual void slotBrushMode();
	virtual void slotSetBrushSize(int size);
	virtual void slotSetBrushShape(int shape);
	virtual void slotContourMode();

	// Auto Lumen Segmenation 
	virtual void slotFillContour();
	virtual void slotClearContour();
	virtual void slotEnableAutoLumenSegmentation(bool flag);
	virtual void slotSetContourFilterGenerateValues(int generateValues);
	virtual void slotSetLineInterpolatorToSmoothCurve(bool flag);
	virtual void slotSetLineInterpolatorToPolygon(bool flag);

	virtual void slotROIMode();
	virtual void slotChangeROI();
	virtual void slotSelectROI();
	virtual void slotResetROI();
	
	// Ruler
	virtual void slotRulerMode();
	virtual void slotEnableMaximumWallThickneesLabel(bool flag);

	// set layer color
	virtual void slotSetImageLayerColor(int layer);
	virtual void slotSetPaintBrushToEraser(bool flag);

	// Button slots
	virtual void slotGenerateCenterlineBtn();
	virtual void slotUpdate3DLabelBtn();

	// Validation Patient ID and Patien name
	virtual void slotValidatePatientInformation();

	virtual void slotChangeOpacity(int layer, int opacity);


signals:
	void signalVisualizeAllViewers();
	void signalMultiPlanarView();
	void signalSegmentationView();
	void signalChangeSliceX(int);
	void signalChangeSliceY(int);
	void signalChangeSliceZ(int);

private:
	int* ConvertBoundsToExtent(double* bounds);
	double* CovertExtentToBounds(int* extent);


	// viewer
	vtkSmartPointer<MyImageViewer> m_2DimageViewer[VIEWER_NUM];
	vtkSmartPointer<vtkRenderWindowInteractor> m_interactor[VIEWER_NUM];
	vtkSmartPointer<InteractorStyleSwitch> m_style[VIEWER_NUM];
	vtkRenderWindow*			m_3DimageViewer;
	vtkRenderer*				m_3DDataRenderer;
	vtkRenderer*				m_3DAnnotationRenderer;
	vtkRenderWindowInteractor*  m_3Dinteractor;
	InteractorStyleSwitch3D*	m_style3D;

	// widget
	MyWidgetManager* m_widgetManager = NULL;

	// Data
	MyImageManager* m_imageManager;
	IOManager*		m_ioManager;
	vtkSmartPointer<vtkPolyData> m_centerlinePD;


	bool m_firstInitialize = true;
	int m_viewMode = MULTIPLANAR_VIEW;

};

#endif // !__CORE_H__
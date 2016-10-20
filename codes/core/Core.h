#ifndef __CORE_H__
#define __CORE_H__

#include <QWidget>

#include <vtkSmartPointer.h>

#include "MyImageViewer.h"
#include "InteractorStyleSwitch.h"
#include "InteractorStyleSwitch3D.h"
#include "LumenExtraction.h"
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
	vtkSmartPointer<InteractorStyleSwitch> Get2DInteractorStyle(int);
	vtkLookupTable*		GetLookupTable();
	vtkRenderWindow*	GetRenderWindow(int num);
	IOManager*			GetIOManager();
	MyImageManager*		GetMyImageManager();
	MyWidgetManager*	GetMyWidgetManager();

	void RenderAllViewer();
	void DisplayErrorMessage(std::string str);
	
	enum INTERACTION_MODE
	{
		NAVIGATION_MODE = 0,
		BRUSH_MODE = 1,
		WINDOW_LEVEL_MODE = 2,
		POLYGON_CONTOUR_MODE = 3,
		RULER_MODE = 4,
		ROI_MODE = 5,
		SMARTCONTOUR_MODE = 6,
		SMARTCONTOUR2_MODE = 7
	};

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

	int* ConvertBoundsToExtent(double* bounds, bool clamping = true);
	double* ConvertExtentToBounds(int* extent);

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
	virtual void slotChangeSlices(int* ijk);
	virtual void slotChangeSlices(int, int, int);
	virtual void slotChangeSliceX(int x);
	virtual void slotChangeSliceY(int y);
	virtual void slotChangeSliceZ(int z);
	virtual void slotChangeFocalPointWithImageCoordinate(int i, int j, int k);

	// mode stuff
	virtual void slotNavigationMode();
	virtual void slotWindowLevelMode();
	virtual void slotBrushMode();
	virtual void slotContourMode();
	virtual void slotROIMode();
	virtual void slotSmartContourMode();
	virtual void slotSmartContour2Mode();
	virtual void slotRulerMode();

	// Paint Brush 
	virtual void slotSetBrushSize(int size);
	virtual void slotSetBrushShape(int shape);
	virtual void slotSetPaintBrushToEraser(bool flag);
	// set layer color actually both Contour and paintBrush depend on it
	virtual void slotSetImageLayerColor(int layer);

	// Semi-Auto Lumen Segmenation 
	virtual void slotFillContour();
	virtual void slotClearContour();
	virtual void slotEnableAutoLumenSegmentation(bool flag);
	virtual void slotSetContourFilterGenerateValues(int generateValues);
	virtual void slotSetLineInterpolatorToSmoothCurve(bool flag);
	virtual void slotSetLineInterpolatorToPolygon(bool flag);

	// Lumen Extraction
	virtual void slotExtractLumen();
	virtual void slotExtractLumenDilateLabel(vtkImageData*);
	virtual void slotSetExtractLumenSeedList(std::vector<int *>);
	virtual void slotSetExtractLumenInputImage(vtkImageData*);
	virtual void slotSetExtractLumenDilationValue(int val);
	virtual void slotSetExtractLumenInitialNeighborhoodRadius(int val);
	virtual void slotSetExtractLumenMultiplier(double);

	// ROI 
	virtual void slotChangeROI();
	virtual void slotSelectROI();
	virtual void slotResetROI();
	
	// Ruler
	virtual void slotEnableMaximumWallThickneesLabel(bool flag);



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
	void ModeChangeUpdate(INTERACTION_MODE);

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

	// Filter
	LumenExtraction* m_lumenExtractionFilter;


	bool m_firstInitialize = true;
	int m_viewMode = MULTIPLANAR_VIEW;

};

#endif // !__CORE_H__
#ifndef __CORE_H__
#define __CORE_H__

#include <qobject.h>

#include <vtkSmartPointer.h>

#include "MyImageViewer.h"
#include "InteractorStyleSwitch.h"
#include "InteractorStyleSwitch3D.h"
#include "MyImageManager.h"
#include "IOManager.h"


class Core:public QObject
{
	Q_OBJECT
public:
	Core(QWidget* parent = nullptr);
	Core(QObject* parent = nullptr, QWidget* mainWindow = nullptr);
	~Core();

	void Initialization();

	// Get functions
	vtkSmartPointer<vtkPolyData> GetCenterlinePD();
	InteractorStyleSwitch* Get2DInteractorStyle(int);
	vtkLookupTable*		GetLookupTable();
	vtkRenderWindow*	GetRenderWindow(int num);
	IOManager*			GetIOManager();
	MyImageManager*		GetMyImageManager();

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
	const static int NUMBER_OF_2DVIEWERS = 3;

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

	// mode stuff
	virtual void slotNavigationMode();
	virtual void slotWindowLevelMode();
	virtual void slotBrushMode();
	virtual void slotContourMode();
	virtual void slotROIMode();
	virtual void slotSeedsPlacerMode();
	virtual void slotRulerMode();
	virtual void slotObliqueViewMode();

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

private:
	// viewer
	vtkSmartPointer<MyImageViewer> m_2DimageViewer[NUMBER_OF_2DVIEWERS];
	vtkSmartPointer<vtkRenderWindowInteractor> m_interactor[NUMBER_OF_2DVIEWERS];
	vtkSmartPointer<InteractorStyleSwitch> m_style[NUMBER_OF_2DVIEWERS];
	vtkRenderWindow*			m_3DimageViewer;
	vtkRenderer*				m_3DDataRenderer;
	vtkRenderer*				m_3DAnnotationRenderer;
	vtkRenderWindowInteractor*  m_3Dinteractor;
	InteractorStyleSwitch3D*	m_style3D;

	// Data
	MyImageManager* m_imageManager;
	IOManager*		m_ioManager;
	vtkSmartPointer<vtkPolyData> m_centerlinePD;



	bool m_firstInitialize = true;
	int m_viewMode = MULTIPLANAR_VIEW;

};

#endif // !__CORE_H__
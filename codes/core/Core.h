#ifndef __CORE_H__
#define __CORE_H__

#include <QWidget>

#include <vtkSmartPointer.h>

#include "MyImageViewer.h"
#include "InteractorStyleSwitch.h"
#include "InteractorStyleSwitch3D.h"
#include "MyImageManager.h"
#include "IOManager.h"


class Core:public QWidget
{
	Q_OBJECT
public:
	enum VIEW_MODE
	{
		MULTIPLANAR_VIEW = 0,
		SEGMENTATION_VIEW = 1
	};
	const static int VIEWER_NUM = 3;
	const static int DEFAULT_IMAGE = 0;
	Core(QWidget* parent = nullptr);
	~Core();

	vtkLookupTable* GetLookupTable();

	vtkRenderWindow* GetRenderWindow(int num);

	void RenderAllViewer();

	void DisplayErrorMessage(std::string str);

	IOManager* GetIOManager();

	MyImageManager* GetMyImageManager();

public slots:

	virtual void slotAddOverlayToImageViewer();

	virtual void slotVisualizeAll2DViewers();

	virtual void slotChangeModality(QAction* action);

	virtual void slotChangeModality(QString modalityName, int viewerNum);

	virtual void slotSegmentationView();

	virtual void slotMultiPlanarView();
	
	virtual void slotChangeView(int viewMode);

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
	virtual void slotEnableAutoLumenSegmentation(bool flag);
	virtual void slotSetContourFilterGenerateValues(int generateValues);
	virtual void slotSetLineInterpolatorToSmoothCurve(bool flag);
	virtual void slotSetLineInterpolatorToPolygon(bool flag);

	virtual void slotROIMode();
	virtual void slotChangeROI();
	virtual void slotSelectROI();
	virtual void slotResetROI();

	virtual void slotRuler(bool b);
	// set layer color
	virtual void slotSetImageLayerColor(int layer);

signals:
	void signalVisualizeAllViewers();
	void signalMultiPlanarView();
	void signalSegmentationView();
	void signalChangeSliceX(int);
	void signalChangeSliceY(int);
	void signalChangeSliceZ(int);

//private:
public:

	// viewer
	vtkSmartPointer<MyImageViewer> m_2DimageViewer[VIEWER_NUM];
	vtkSmartPointer<vtkRenderWindowInteractor> m_interactor[VIEWER_NUM];
	vtkSmartPointer<InteractorStyleSwitch> m_style[VIEWER_NUM];

	vtkRenderWindow*			m_3DimageViewer;
	vtkRenderer*				m_3DDataRenderer;
	vtkRenderer*				m_3DAnnotationRenderer;
	vtkRenderWindowInteractor*  m_3Dinteractor;
	InteractorStyleSwitch3D*	m_style3D;

	// Data
	MyImageManager m_imageManager;
	IOManager m_ioManager;


	bool m_firstInitialize = true;
	int m_viewMode = MULTIPLANAR_VIEW;

};

#endif // !__CORE_H__
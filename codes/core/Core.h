#ifndef __CORE_H__
#define __CORE_H__

#include <QWidget>

#include <vtkLookupTable.h>
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

	void DisplayErrorMessage(std::string str);

	IOManager* GetIOManager();

	MyImageManager* GetMyImageManager();

public slots:

	virtual void slotAddOverlayToImageViewer();

	virtual void slotVisualizeViewer();

	virtual void slotSegmentationViewer();

	virtual void slotMultiPlanarView();
	
	virtual void slotChangeView(Core::VIEW_MODE viewMode);

	virtual void slotNavigationMode();

	virtual void slotWindowLevelMode();

	virtual void slotBrushMode();

	virtual void slotContourMode();

	virtual void slotROIMode();
	virtual void slotChangeROI();
	virtual void slotSelectROI();
	virtual void slotResetROI();

	virtual void slotRuler(bool b);



signals:
	void signalVisualizeViewer();
	void signalMultiPlanarView();
	void signalSegmentationView();

//private:
public:

	// viewer
	vtkSmartPointer<vtkLookupTable> m_lookupTable;

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


	bool m_firstInitializedFlag = false;
	VIEW_MODE m_viewMode = MULTIPLANAR_VIEW;

};

#endif // !__CORE_H__
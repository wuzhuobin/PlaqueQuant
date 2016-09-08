#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QDragEnterEvent> 
#include <QDropEvent>
#include <QAction>
#include <QActionGroup>
#include <QMenu>


#include "ui_MainWindow.h"

#include <vtkActor.h>
#include <vtkPolyData.h>

//Own Class
#include "MyImageViewer.h"
#include "InteractorStyleSwitch.h"
#include "InteractorStyleSwitch3D.h"
#include "InfoDialog.h"
#include "Distance3DWidget.h"
#include "MyImageManager.h"
#include "IOManager.h"

class ModuleWidget;
class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(); 	
	~MainWindow();
	static MainWindow* GetMainWindow();

	// Get Functions
	vtkRenderWindow* GetRenderWindow(int i);
	MyImageViewer* GetMyImageViewer(int i);
	InteractorStyleSwitch* GetInteractorStyleImageSwitch(int i);
	vtkRenderWindowInteractor* GetVtkRenderWindowInteractor(int i);
	vtkLookupTable* GetLookupTable();

	QString				GetFileName(int);
	int					GetVisibleImageNo();
	int					GetImageLayerNo();
	vtkPolyData*		GetCenterlinePD();
	Ui_MainWindow* GetUI();

	// Set functions
	void SetImageLayerNo(int);

	// Ui Updates
	void RenderAllViewer();
	void RenderAll2DViewers();
	void UpdateStenosisValue(double val);

	
	enum SLICE_ORIENTATION
  {
    SLICE_ORIENTATION_YZ = 0,
    SLICE_ORIENTATION_XZ = 1,
    SLICE_ORIENTATION_XY = 2
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


public slots:
	//GUI
	virtual void resizeEvent(QResizeEvent * event);
	virtual void dragEnterEvent(QDragEnterEvent *ev);
	virtual void dropEvent( QDropEvent *ev );
	
	virtual void slotChangeSlice();
	virtual void slotChangeSlice(int, int, int);
	virtual void slotUpdateSlice();
	virtual void slotChangeWindowLevel();
	virtual void slotChangeWindowLevelUL();
    virtual void slotChangeWindowLevelUR();
    virtual void slotChangeWindowLevelLL();
    virtual void slotChangeWindowLevelLR();

	//Action
	virtual void slotExit();
	//virtual void slotOpenImage(QString dir);
	//virtual void slotOpenImage();
	virtual void slotOpenRecentImage();
	//virtual void slotSaveSegmentation();
	virtual void slotAbout();
	virtual void slotHelp();
	virtual void slot3DUpdate();
	//virtual void slotMeasureCurrentVolumeOfEveryLabel(double* volumes, int numOfVolumes);

	virtual void slotNavigationMode();
	virtual void slotWindowLevelMode();
	virtual void slotBrushMode();
	virtual void slotContourMode();
	virtual void slotROIMode();
	virtual void slotImage(int image);
	virtual void slotMultiPlanarView();

	// Auto Lumen Segmenation 
	virtual void slotEnableAutoLumenSegmentation(bool flag);
	virtual void slotSetContourFilterGenerateValues(int generateValues);
	virtual void slotSetLineInterpolatorToSmoothCurve(bool flag);
	virtual void slotSetLineInterpolatorToPolygon(bool flag);

	//Overlay
	virtual void slotAddExternalOverlay();
    virtual void slotOverlayOpacity(int opacity);
  
	//Centerline
	virtual void slotCenterline();
    //Segmentation
	virtual void slotSegmentationView();
	virtual void slotChangeBaseImageUL();
	virtual void slotChangeBaseImageUR();
	virtual void slotChangeBaseImageLL();
	virtual void slotChangeImageSeq(int image_no, int window_no);
	virtual void slotSelectImageSeq(QAction*);
	//Intensity
	//virtual void slotChangeIntensity();

	//Widget
	virtual void slotRuler(bool b);
	virtual void slotChangeROI();
	virtual void slotSelectROI();
	virtual void slotResetROI();

	//Widget running 
	virtual void slotShowProgressDialog(int value, QString text);

	//Visualization
	//bool loadImage(int, QStringList*list);
	bool slotVisualizeImage();

	//Alogrithm
	void slotMeasureCurrentVolumeOfEveryLabel();

	virtual void addOverlay2ImageViewer();


private:
	//UI
	Ui_MainWindow* ui;
	void setActionsEnable(bool b);

	//menu for changing images
	QMenu ChangeBaseImageULMenu;
	QMenu ChangeBaseImageURMenu;
	QMenu ChangeBaseImageLLMenu;

	//widgetGroup
	QActionGroup widgetGroup;
	//viewerGroup 
	QActionGroup viewerGroup;
	//viewGroup
	QActionGroup viewGroup;


    
	//2D/3D view
	MyImageViewer*				m_2DimageViewer[3];
	vtkRenderWindow*			m_3DimageViewer;
	vtkRenderer*				m_3DDataRenderer;
	vtkRenderer*				m_3DAnnotationRenderer;
	vtkRenderWindowInteractor*  m_3Dinteractor;
	vtkRenderWindowInteractor*  m_interactor[3];
	InteractorStyleSwitch*		m_style[3];
	InteractorStyleSwitch3D*	m_style3D;



    
	//3D ruler
	Distance3DWidget* m_distance3DWidget;
	//Recent File
	const static int MAX_RECENT_IMAGE = 10;
	QList<QAction*> recentFileActionList;
	void createRecentImageActions();
	void adjustForCurrentFile(const QString& filePath);
	void updateRecentActionList();

	// UI related
	void DisplayErrorMessage(std::string);
	
	//Data
	vtkPolyData* m_centerlinePD;
	MyImageManager imageManager;
	IOManager ioManager;



    //Orientation
	int m_orientation;
    
	//Dock Widget
	friend class ModuleWidget;
	ModuleWidget* m_moduleWidget;

	//Navigation
	double m_focalPoint[3];
    
    int visibleImageNum;

	//Information
	Dialog* m_InfoDialog;

	//Segmentation view flag
	bool segmentationView;
	bool INITIALIZED_FLAG;


	//Overlay
	//Overlay* SegmentationOverlay;
	int m_layer_no;		//Main Image
	int m_boundingExtent[6];

	const int* overlayColor[6];

	const int lumen[3] = { 255, 0, 0 };
	const int vesselWall[3] = { 0, 0, 255 };
	const int calcification[3] = { 0,255,0 };
	const int hemorrhage[3] = { 255, 255, 0 };
	const int lrnc[3] = { 0, 255, 255 };
	const int lm[3] = { 255, 0, 255 };

	vtkLookupTable* LookupTable;
/*

	this->LookupTable->SetTableValue(0, 0, 0, 0, 0);
	this->LookupTable->SetTableValue(1, 1, 0, 0, 0.5);
	this->LookupTable->SetTableValue(2, 0, 0, 1, 0.9);
	this->LookupTable->SetTableValue(3, 0, 1, 0, 0.1);
	this->LookupTable->SetTableValue(4, 1, 1, 0, 0.7);
	this->LookupTable->SetTableValue(5, 0, 1, 1, 0.4);
	this->LookupTable->SetTableValue(6, 1, 0, 1, 0.5);*/

};
 
#endif //MAINWINDOW_H

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
#include "Core.h"


class ModuleWidget;
class MeasurementWidget;
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow();
	~MainWindow();
	static MainWindow* GetMainWindow();

	// Get Functions
	Core*						GetCore();
	vtkRenderWindow*			GetRenderWindow(int i);
	MyImageViewer*				GetMyImageViewer(int i);
	InteractorStyleSwitch*		GetInteractorStyleImageSwitch(int i);
	vtkRenderWindowInteractor*	GetVtkRenderWindowInteractor(int i);
	vtkLookupTable*				GetLookupTable();
	//QString				GetFileName(int);
	int					GetVisibleImageNo();
	vtkPolyData*		GetCenterlinePD();
	Ui_MainWindow*		GetUI();
	
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

	public slots:
	//GUI
	virtual void resizeEvent(QResizeEvent * event);
	virtual void dragEnterEvent(QDragEnterEvent *ev);
	virtual void dropEvent(QDropEvent *ev);

	//Action
	virtual void slotExit();

	virtual void slotOpenRecentImage();

	virtual void slotAbout();
	virtual void slotHelp();
	virtual void slot3DUpdate();
	/**
	 * four viewers and maximum
	 */
	virtual void slotImage(int image);

	/**
	 * Show up the menu for changing the modality
	 */
	virtual void slotChangeBaseImageUL();
	virtual void slotChangeBaseImageUR();
	virtual void slotChangeBaseImageLL();

	virtual void slotChangeROI();

	//Widget running 
	virtual void slotShowProgressDialog(int value, QString text);

	//Visualization
	bool slotVisualizeImage();



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
	MyImageManager* imageManager;
	IOManager* ioManager;

	// Core
	Core* m_core;


	//Orientation
	int m_orientation;

	//Dock Widget
	friend class ModuleWidget;
	friend class MeasurementWidget;
	ModuleWidget* m_moduleWidget;
	MeasurementWidget* m_measurementWidget;

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
	int m_boundingExtent[6];

};
 
#endif //MAINWINDOW_H

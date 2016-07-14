#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent> 
#include <QDropEvent>
#include <QAction>
#include <QUrl>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QFuture>
#include <qdebug.h>
#include <qactiongroup.h>
#include <qsignalmapper.h>

#include "vtkCamera.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkSmartPointer.h"
#include "vtkMarchingCubes.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkWindowedSincPolyDataFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkImageAccumulate.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindow.h"
#include "vtkTextMapper.h"
#include "vtkImageProperty.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkPolyData.h"
#include <vtkCellData.h>
#include <vtkSphereSource.h>
#include <vtkVersion.h>
#include <vtkDistanceRepresentation3D.h>
#include <vtkDistanceRepresentation2D.h>
#include "vtkDistanceWidget.h"
#include "vtkPointHandleRepresentation3D.h"
#include <vtkLookupTable.h>


//Own Class
//#include "Define.h"
#include "RegistrationWizard.h"
//#include "MyVtkInteractorStyleImage.h"
#include "MyImageViewer.h"
//#include "MyProgressDialog.h"
#include "ModuleWidget.h"
#include "InfoDialog.h"
//#include "ImageRegistration.h"
//#include "MyPlaneWidget.h"
//#include "Overlay.h"
//#include "SurfaceCreator.h"
//#include "VesselSegmentation.h"
//#include "ExtractCenterline.h"
#include "MultiplanarViewController.h"
#include "MultiWidgetsController.h"
#include "ui_MainWindow.h"


class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	static const int maxRecentImage = 10;
	MainWindow(QMainWindow* parent = 0);
	~MainWindow();

	/**
	* save recent folder to register
	* i have no idea the following 3
	*/
	void adjustForCurrentFile(const QString& filePath);
	void updateRecentActionList();
	void createRecentImageActions();
public:
	public slots:
	void slotAbout();
	void slotHelp();
	void slotOpenImage(QString dir);
	void slotOpenImage();
	void slotOpenRecentImage();
	void slotVisualize2DImage(int i);
	void slotNavigationMode();
	void slotWindowLevelMode();
	void slotContour(bool);
	void slotPaintBrush(bool);
	void slotOtherMode();
	void slotImage(int i);
	void resizeEvent(QResizeEvent * event);
	void slotAllAxialView();
	void slotMultiPlanarView();
	void slotPopupULSelectImgBtn();
	void slotPopupURSelectImgBtn();
	void slotPopupLLSelectImgBtn();
	void slotULSelectImageSeq(QAction*);
	void slotURSelectImageSeq(QAction*);
	void slotLLSelectImageSeq(QAction*);

	
private:
	//UI
	Ui::MainWindow ui;
	ModuleWidget moduleWidget;
	Dialog dialog;
	// for using recent folders
	QList<QAction*> recentFileActionList;
	// for group of actionNavigation, actoinWindowLevel, actionRuler,
	// actionROI, actoinContour, actoinBrush
	MyVtkInteractorStyleImage::Mode mode;
	QActionGroup group1;
	// for group of actionImage1, actionImage2, actionImage3, actionImage4,
	// actionFourViews
	QActionGroup group2;
	// for group of actionAllAxialView, actionMultiplanerView
	QActionGroup group3;

	// all axial view
	bool allAxialView = false;

	//SelectImgBtnMenu
	QMenu ULSelectImgBtnMenu;
	QMenu URSelectImgBtnMenu;
	QMenu LLSelectImgBtnMenu;
	QMenu LRSelectImgBtnMenu;

	//Initialization
	void initializeModule();

	// controller 3 2d viewer
	MultiplanarViewController controller[5];

	// widgets controller 
	MultiWidgetsController wController[5];

	//File Name
	QStringList fileNameList[5];
	// number of images from waizard
	int visibleImageNum;


};
 
#endif //MAINWINDOW_H

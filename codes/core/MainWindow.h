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
	void slotVisualizeImage(int i);

	
private:
	//UI
	Ui::MainWindow ui;
	ModuleWidget moduleWidget;
	Dialog dialog;

	//Initialization
	//void initializeViewers();
	void initializeModule();
	//void initializeOverlay();

	// controller 3 2d viewer
	MultiplanarViewController controller;

	//File Name
	QStringList fileNameList[5];
	// number of images from waizard
	int visibleImageNum;
	// for using recent folders
	QList<QAction*> recentFileActionList;

};
 
#endif //MAINWINDOW_H

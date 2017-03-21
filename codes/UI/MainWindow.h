#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QDragEnterEvent> 
#include <QDropEvent>
#include <QAction>
#include <QActionGroup>
#include <QMenu>

#include <vtkPolyData.h>

//Own Class
#include "InfoDialog.h"
#include "MyImageManager.h"
#include "IOManager.h"
#include "Core.h"

namespace Ui { class MainWindow; }
class ModuleWidget;
class MeasurementWidget;
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow();
	~MainWindow();
	static MainWindow* GetMainWindow();

	MeasurementWidget* GetMeasurementWidget();

	void openOneImage(QString img);

	//// Get Functions
	Core*						GetCore();
	vtkRenderWindow*			GetRenderWindow(int i);
	//ModuleWidget*				GetModuleWidget();

	// Titile
	void SetVersion(QString);

	// to be moved
	void UpdateStenosisValue(double val);

	enum SLICE_ORIENTATION
	{
		SLICE_ORIENTATION_YZ = 0,
		SLICE_ORIENTATION_XZ = 1,
		SLICE_ORIENTATION_XY = 2
	};

	void EnableImageSelection(bool enable);

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

	//Widget running 
	virtual void slotShowProgressDialog(int value, QString text);

	//Visualization
	bool slotVisualizeImage();

	virtual void slotChangeMode(QAction* action);



private:
	//UI
	Ui::MainWindow* ui;
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
	QString m_version;

	// Core
	Core* m_core;

	//Dock Widget
	friend class ModuleWidget;
	friend class MeasurementWidget;
	ModuleWidget* m_moduleWidget;
	MeasurementWidget* m_measurementWidget;


	//Information
	Dialog m_InfoDialog;

	int m_boundingExtent[6];

};
 
#endif //MAINWINDOW_H

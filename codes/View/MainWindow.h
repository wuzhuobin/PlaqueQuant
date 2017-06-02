/**
* MainWindow of 
* API class for using the Vikey USB authentication system. The class is derived from
* QWidget. And this API class can only be used under Windows OS.
*
* Updated Log:
* version 1.0.0
* Original one provided authentication method for having key, soft id, hard id
* product name, expired date time
*
* @class	EncryptionAuthentication
* @author	WUZHUOBIN
* @version	1.1.0
* @since	2017.01.05
*
*/

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QtWidgets/QMainWindow>



namespace Ui { class MainWindow; }
class vtkRenderWindow;
class QMenu;
class QSettings;
class Switch2DWidget;
class Switch3DWidget;
class ViewerWidget;
class MeasurementWidget;
class LabelWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT;


public:

	const static unsigned short NUM_OF_2D_VIEWERS = 3;
	const static unsigned short NUM_OF_3D_VIEWERS = 1;
	const static unsigned short NUM_OF_VIEWERS = NUM_OF_3D_VIEWERS + NUM_OF_2D_VIEWERS;

	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void initialization();
	void enableInteractor(bool flag);

	void addModalityNames(QString name);
	void setSelectImgMenuVisible(unsigned int num, bool flag);
	void clearModalityNames();

	// get ui things for connection 
	Ui::MainWindow* getUi();
	Switch2DWidget* getSwitch2DWidget();
	Switch3DWidget* getSwitch3DWidget();
	ViewerWidget* getViewerWidget(unsigned int num);
	MeasurementWidget* getMeasurementWidget();
	LabelWidget* getLabelWidget();
	QMenu* getSelectImgMenu(unsigned int i);

protected:
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);

signals:

	void signalImageImportLoad(QStringList);
	void signalOverlayImportLoad(QString);
	void signalOverlayExportSave(QString);
	void signalReportExport(QString);


private slots:
	
	void slotOpenRecentImage();
	void slotOpenNewImage(QString path = QString());
	void slotOpen(QString path = QString());
	void slotOpenOverlay(QString path = QString());
	void slotSaveOverlay(QString path = QString());

	void slotExportReport(QString path = QString());
	void slotExportCSV(QString path = QString());
	/**
	* four viewers and maximum
	*/
	void slotImage(bool flag);


private:
	Ui::MainWindow* ui;

	void imageImport(QString path = QString());

	//Recent File
	const static int MAX_RECENT_IMAGE = 10;
	QList<QAction*> recentFileActionList;
	void createRecentImageActions();
	void adjustForCurrentFile(const QString& filePath);
	void updateRecentActionList();
	
	ViewerWidget* viewerWidgets[NUM_OF_VIEWERS] = { nullptr };

	Switch2DWidget* switch2DWidget = nullptr;

	Switch3DWidget* switch3DWidget = nullptr;

	MeasurementWidget* measurementWidget = nullptr;

	LabelWidget* labelWidget = nullptr;

	QMenu* selectImgMenus[NUM_OF_VIEWERS] = { nullptr };

	QStringList modalityNames;

	QSettings* settings = nullptr;
};

#endif // !__MAIN_WINDOW_H__

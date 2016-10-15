#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDir>
#include <QButtonGroup>

#include "MainWindow.h"


namespace Ui {
	class ModuleWidget;
}
class MainWindow;
class ModuleWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ModuleWidget(QWidget *parent = 0);
    ~ModuleWidget();
	void setMainWindow(MainWindow* mainWnd);

	void UpdateStenosisValue(double);
	//QString GetLumenPath();
	//QString GetVesselWallPath();

public slots:
    void slotSegmentationView();
	void slotSelectROI();
	void slotResetROI();
	void slotChangeOpacity(int opactiy);
	void slotChangeROI(int* bound);
	void slotSetPage();
	//void slotAddLumen();
	//void slotAddVesselWall();
	void SetBrushSize();
	void slotEnableAutoLumenSegmentation(bool flag);

	virtual void slotUpdateTableWidget();

signals:
	void finished();
	void busy(int, QString);

protected:
    
private:
    Ui::ModuleWidget *ui;

    //Recent Parameters
    int m_maxParameters;
	QStringList NameList, ParameterList;
    //bool First_Load;
    int m_visible_image_no;

	QButtonGroup m_contourRadioButtonGroup;

	MainWindow* m_mainWnd;
	
	//QString lumen_Path, vesselWall_Path;
};

#endif // MODULEWIDGET_H

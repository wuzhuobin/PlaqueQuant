#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <QProgressDialog>
#include <QWidget>
#include <QSettings>
#include <QProgressBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QSpinBox>
#include <QComboBox>
#include <QHeaderView>
#include <QDebug>
#include <QDesktopServices>
#include <QApplication>
#include <QUrl>
#include <QButtonGroup>

#include <vtkStringArray.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkIntArray.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkActor2D.h>
#include <vtkActor.h>

#include "MyThread.h"
#include "Define.h"
#include "MainWindow.h"
#include "ReportGenerator.h"


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

	void SetVisibleImageNo(int);
	void SetPage(int);
	void UpdateStenosisValue(double);
	//QString GetLumenPath();
	//QString GetVesselWallPath();

	QSpinBox* GetBrushSizeSpinBox();
	QComboBox* GetBrushShapeComBox();
	void GenerateReport();
	void setMainWindow(MainWindow* mainWnd);

public slots:
    void slotSegmentationView();
	void slotSelectROI();
	void slotResetROI();
	void slotChangeOpacity(int opactiy);
	void slotChangeROI(int* bound);
	void slotEnableMWTCalculation(int);
	//void slotAddLumen();
	//void slotAddVesselWall();
	void GenerateReportPushButtonVisible();
	void NextPage();
	void BackPage();
	void SetBrushSize();
	void slotReportGetInput();
	void slotUpdate3DMeasurements();
	void slotUpdate2DMeasurements();
	void slotUpdate2DMeasurements(int slice);
	void slotCalculateMaximumWallThickness();
	void slotEnableAutoLumenSegmentation(bool flag);

	virtual void slotUpdateTableWidget();

signals:
	void finished();
	void busy(int, QString);

protected:
	virtual void InternalUpdate();
    
private:
    Ui::ModuleWidget *ui;

    //Recent Parameters
    int m_maxParameters;
	QStringList NameList, ParameterList;
    //bool First_Load;
    int m_visible_image_no;

	vtkSmartPointer<vtkPolyData>					m_displayPD;
	vtkSmartPointer<vtkActor>						m_lineActor;
	vtkSmartPointer<vtkActor2D>						m_labelActor;
	vtkSmartPointer<vtkStringArray>					m_labelArray;
	vtkSmartPointer<vtkIntArray>					m_sizeArray;
	vtkSmartPointer<vtkPointSetToLabelHierarchy>	m_p2labelfilter;

	QButtonGroup m_contourRadioButtonGroup;

	MainWindow* m_mainWnd;
	
	//QString lumen_Path, vesselWall_Path;
};

#endif // MODULEWIDGET_H

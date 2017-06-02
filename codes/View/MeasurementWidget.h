#pragma once
#ifndef __MEASUREMENTWIDGET_H__
#define __MEASUREMENTWIDGET_H__
#include <QWidget>

//#include "ui_measurementwidget.h"
#include <itkGDCMImageIO.h>

namespace Ui { class MeasurementWidget; }

class MainWindow;
class vtkRenderWindow;

class MeasurementWidget : public QWidget {
	Q_OBJECT

public:
	MeasurementWidget(QWidget * parent = Q_NULLPTR);
	~MeasurementWidget();

	Ui::MeasurementWidget* getUi();
	itk::GDCMImageIO::Pointer info = nullptr;
	vtkRenderWindow* wind1 = nullptr;
	vtkRenderWindow* wind2 = nullptr;;


public slots:
	void slotUpdate3DMeasurements(double* Measurements3D);
	void slotUpdate2DMeasurements(double* Measurements2D);
	void slotUpdate2DMeasurements(int slice);
	void slotUpdateStenosis(double stenosis);
	void slotUpdateImformation();

private:
	Ui::MeasurementWidget* ui = nullptr;

	void GenerateReport(QString	path);

	friend class MainWindow;
	MainWindow* m_mainWnd;


};

#endif // __MEASUREMENTWIDGET_H__
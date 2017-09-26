#pragma once
#ifndef __MEASUREMENTWIDGET_H__
#define __MEASUREMENTWIDGET_H__
#include <QWidget>

//#include "ui_measurementwidget.h"
#include <itkGDCMImageIO.h>

namespace Ui { class MeasurementWidget; }

class vtkRenderWindow;

class MeasurementWidget : public QWidget {
	Q_OBJECT

public:
	MeasurementWidget(QWidget * parent = Q_NULLPTR);
	~MeasurementWidget();

	Ui::MeasurementWidget* getUi();
	itk::GDCMImageIO::Pointer info = nullptr;
	vtkRenderWindow* wind1 = nullptr;
	vtkRenderWindow* wind2 = nullptr;

public slots:
	void slotUpdate3DMeasurements(double* Measurements3D);
	void slotUpdate2DMeasurements(double* Measurements2D);
	void slotUpdateStenosis(double stenosis);
	void slotUpdateInformation();
	void GenerateReport(QString	path);
	void GenerateCSV(QString path);

private:
	Ui::MeasurementWidget* ui = nullptr;

};

#endif // __MEASUREMENTWIDGET_H__
#pragma once
#ifndef MEASUREMENTWIDGET_HPP
#define MEASUREMENTWIDGET_HPP
#include <QWidget>
#include "ui_measurementwidget.h"

class MainWindow;

class MeasurementWidget : public QWidget {
	Q_OBJECT

public:
	MeasurementWidget(QWidget * parent = Q_NULLPTR);
	~MeasurementWidget();

	void setMainWindow(MainWindow* mainWnd);

public slots:
	void slotUpdate3DMeasurements();
	void slotUpdate2DMeasurements();
	void slotUpdate2DMeasurements(int slice);
	void slotUpdateImformation();
	void slotReportGetInput();

private:
	Ui::MeasurementWidget ui;

	void GenerateReport();

	MainWindow* m_mainWnd;


};

#endif // MEASUREMENTWIDGET_HPP
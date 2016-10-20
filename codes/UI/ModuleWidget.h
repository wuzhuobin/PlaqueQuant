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
class ModuleWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ModuleWidget(QWidget *parent = 0);
    ~ModuleWidget();

	void UdateTargetImageComboBox();
	void UpdateSeedListView();

	static std::vector<int*>		SeedIJKList;
	static std::vector<double*>		SeedCoordinatesList;

public slots:
	void slotChangeOpacity(int opactiy);
	void slotChangeROI(int* bound);
	void slotSetPage();
	void slotEnableAutoLumenSegmentation(bool flag);
	void slotUpdateROISpinBoxes(double* bounds);

	// UI Linking
	virtual void slotChangeSliderNeighborhoodRadius();
	virtual void slotChangeSpinBoxNeighborhoodRadius();
	virtual void slotChangeSliderVesselWallThickness();
	virtual void slotChangeSpinBoxVesselWallThickness();
	
	// Seeds Operations
	virtual void slotUpdateCoordinateLabel();
	virtual void slotUpdateSeedListView();
	virtual void slotDeleteCurrentSeed();
	virtual void slotSetExtractLumenTargetImage(QString);
	virtual void slotGenerateContour();
	virtual void slotSnapToSeed(int rowIndex);



signals:
	void finished();
	void busy(int, QString);

protected:
    
private:

    Ui::ModuleWidget *ui;

	QButtonGroup m_contourRadioButtonGroup;
	friend class MainWindow;
	MainWindow* m_mainWnd;
	
};

#endif // MODULEWIDGET_H

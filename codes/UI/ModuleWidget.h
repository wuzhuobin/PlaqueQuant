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
	void UpdateSeedList();

	static std::vector<int*> SeedList;

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
	virtual void slotUpdateCoordinateLabel();


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

#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDir>
#include <QButtonGroup>


class MainWindow;
namespace Ui { class ModuleWidget; }

class ModuleWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ModuleWidget(QWidget *parent = 0);
    ~ModuleWidget();

	void ClearWidget();
	void addWidget(QWidget* style);
	void setWidget(QWidget* style);
	//void UdateTargetImageComboBox();
	//void UpdateSeedListView();

	//static std::vector<int*>		SeedIJKList;
	//static std::vector<double*>		SeedCoordinatesList;

public slots:
	void slotChangeOpacity(int opactiy);
	void slotUpdateOpacity();
	void slotChangeROI(int* bound);
	//void slotSetPage();
	void slotUpdateROISpinBoxes(double* bounds);

	//// UI Linking
	//virtual void slotChangeSliderNeighborhoodRadius();
	//virtual void slotChangeSpinBoxNeighborhoodRadius();
	//virtual void slotChangeSliderVesselWallThickness();
	//virtual void slotChangeSpinBoxVesselWallThickness();
	//
	//// Seeds Operations
	//virtual void slotUpdateCoordinateLabel();
	////virtual void slotUpdateSeedListView();
	//virtual void slotDeleteCurrentSeed();
	//virtual void slotSetExtractLumenTargetImage(QString);
	//virtual void slotGenerateContour();
	//virtual void slotSnapToSeed(int rowIndex);


protected:
    
private:

    Ui::ModuleWidget *ui;

	friend class MainWindow;
	MainWindow* m_mainWnd;
	
};

#endif // MODULEWIDGET_H

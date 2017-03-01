#ifndef __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__
#define __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

#include "InteractorStyleSeedsPlacer.h"
#include "QAbstractNavigation.h"

#include "LumenExtractionFilter.h"

namespace Ui { class QInteractorStyleLumenSeedsPlacer; }
class QInteractorStyleLumenSeedsPlacer: public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleLumenSeedsPlacer);

public:
	vtkTypeMacro(QInteractorStyleLumenSeedsPlacer, InteractorStyleSeedsPlacer);
	static QInteractorStyleLumenSeedsPlacer* New();
	void SetSeedsPlacerEnable(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
	/**
	 * temporary fix
	 */
	void SetTargetImages(
		QList<vtkSmartPointer<vtkImageData>> listOfVtkImages, 
		QList<QString> listOfModalityNames);

public slots:
	void SlotClearAllSeeds();
	void SlotCentreLine();
	void SetFocalSeed(int i);
	void DeleteFocalSeed();
	void SaveWidgetToSeeds();
	void DropSeed();

	void CenterLine();

	void ExtractLumen();
	/**
	 * @deprecated
	 * method has already been moved to InteractorStyleVesselSegmentation
	 */
	void ExtractLumenPolyData();
	void SetMultipier(double value);
	void SetNumberOfIteractions(int value);
	void SetInitialNeighborhoodRadius(int value);

protected:
	QInteractorStyleLumenSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleLumenSeedsPlacer();

	//void uniqueInvoke(bool flag);
	void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);

	void OnKeyPress();

private:
	Ui::QInteractorStyleLumenSeedsPlacer* ui = nullptr;

	int m_numberOfIteractions = 3;
	double m_multiplier = 2.1;
	int m_initialNeighborhoodRadius = 1;

	QList<vtkSmartPointer<vtkImageData>> m_listOfVtkImages;
	QList<QString> m_listOfModalityNames;

};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

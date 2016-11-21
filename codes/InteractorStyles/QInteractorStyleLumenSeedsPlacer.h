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
	virtual void SetSeedsPlacerEnable(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

public slots:
	virtual void SlotClearAllSeeds();
	virtual void SetFocalSeed(int i);
	virtual void DeleteFocalSeed();
	virtual void SaveWidgetToSeeds();
	virtual void DropSeed();

	virtual void ExtractLumen();
	/**
	 * @deprecated
	 */
	virtual void ExtractLumenPolyData();
	virtual void SetMultipier(double value);
	virtual void SetNumberOfIteractions(int value);
	virtual void SetInitialNeighborhoodRadius(int value);

protected:
	QInteractorStyleLumenSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleLumenSeedsPlacer();

	virtual void UniqueEnable(bool flag);
	virtual void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);

	virtual void OnKeyPress();

private:

	Ui::QInteractorStyleLumenSeedsPlacer* ui = nullptr;

	int m_numberOfIteractions = 3;
	double m_multiplier = 2.1;
	int m_initialNeighborhoodRadius = 1;

};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

#ifndef __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__
#define __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

#include "InteractorStyleSeedsPlacer.h"
#include "QAbstractNavigation.h"

#include "LumenExtractionFilter.h"

namespace Ui { class QInteractorStyleLumenSeedsPlacer; }
class LumenExtraction;
class QInteractorStyleLumenSeedsPlacer: public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleLumenSeedsPlacer);
	typedef itk::LumenExtractionFilter<double> LumenExtractionFilter;
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
	virtual void SetMultipier(double value);
	virtual void SetInitialNeighborhoodRadius(int value);

protected:
	QInteractorStyleLumenSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleLumenSeedsPlacer();

	virtual void UniqueEnable(bool flag);
	virtual void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);

	virtual void OnKeyPress();

private:

	Ui::QInteractorStyleLumenSeedsPlacer* ui = nullptr;
	LumenExtraction* m_lumenExtraction = nullptr;
	LumenExtractionFilter::Pointer m_lumenExtractionFilter = nullptr;
};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

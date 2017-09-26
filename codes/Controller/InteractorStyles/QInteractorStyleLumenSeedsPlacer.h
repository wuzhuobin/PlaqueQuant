#ifndef __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__
#define __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

#include "InteractorStyleSeedsPlacer.h"
#include "QAbstractNavigation.h"

class vtkImageData;
class QListWidget;

namespace Ui { class QInteractorStyleLumenSeedsPlacer; }
class QInteractorStyleLumenSeedsPlacer: public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleLumenSeedsPlacer);

public:
	vtkTypeMacro(QInteractorStyleLumenSeedsPlacer, InteractorStyleSeedsPlacer);
	static QInteractorStyleLumenSeedsPlacer* New();

public slots:
	virtual void SetCustomEnabled(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);


	virtual void GenerateWidgetFromSeeds() override;
	virtual void SaveWidgetToSeeds() override;
	virtual void ClearAllSeeds() override;
	virtual void SetFocalSeed(int i) override;
	virtual void DeleteFocalSeed();
	virtual void DropSeed() override;
	virtual void UpdateWidgetToSeeds(
		int* newImagePos,
		int* oldImagePos = nullptr);
	virtual void ExtractLumenByTargetViewer();
	virtual void ExtractLumen();
	void SetMultipier(double value);
	void SetNumberOfIteractions(int value);
	void SetInitialNeighborhoodRadius(int value);

protected:
	QInteractorStyleLumenSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleLumenSeedsPlacer();

	void SetFocalSeed(int i, QList<int*>& seeds);

	void GenerateWidgetFromSeeds(const QList<int*>& seeds);

	void SaveWidgetToSeeds(QList<int*>& seeds);
	void DropSeed(QList<int*>& seeds);

	void UpdateWidgetToSeeds(
		QList<int*>& seeds,
		int* newImagePos,
		int* oldImagePos = nullptr);
	void ClearAllSeeds(QList<int*>& seeds);
	void DeleteFocalSeed(QList<int*>& seeds);

	void ExtractLumen(QList<int*>& seeds);


	virtual void uniqueEnable() override;
	void uniqueInitialization();
	void initialization();
	void UpdateTargetViewer();

	virtual QListWidget* GetListWidget();

	///< container for all lumen seeds
	static QList<int*> m_lumenSeeds;


	int m_numberOfIteractions = 3;
	double m_multiplier = 2.1;
	int m_initialNeighborhoodRadius = 1;

private:
	Ui::QInteractorStyleLumenSeedsPlacer* ui = nullptr;


};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

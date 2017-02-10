#ifndef __QINTERACTOR_STYLE_OBLIQUE_VIEW_SEEDS_PLACER_H__
#define __QINTERACTOR_STYLE_OBLIQUE_VIEW_SEEDS_PLACER_H__

#include "InteractorStyleSeedsPlacer.h"
#include "ui_QInteractorStyleObliqueViewSeedsPlacer.h"
#include "QAbstractNavigation.h"

namespace Ui { class QInteractorStyleObliqueViewSeedsPlacer; }
class QInteractorStyleObliqueViewSeedsPlacer: public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleObliqueViewSeedsPlacer);

public:
	vtkTypeMacro(QInteractorStyleObliqueViewSeedsPlacer, InteractorStyleSeedsPlacer);
	static QInteractorStyleObliqueViewSeedsPlacer* New();
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


protected:
	QInteractorStyleObliqueViewSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleObliqueViewSeedsPlacer();

	//void uniqueInvoke(bool flag);
	void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);

	void OnKeyPress();

private:

	Ui::QInteractorStyleObliqueViewSeedsPlacer* ui = nullptr;

	int m_numberOfIteractions = 3;
	double m_multiplier = 2.1;
	int m_initialNeighborhoodRadius = 1;


	QList<vtkSmartPointer<vtkImageData>> m_listOfVtkImages;
	QList<QString> m_listOfModalityNames;

};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

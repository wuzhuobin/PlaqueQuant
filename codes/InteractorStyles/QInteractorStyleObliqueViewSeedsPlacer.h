#ifndef __QINTERACTOR_STYLE_OBLIQUE_VIEW_SEEDS_PLACER_H__
#define __QINTERACTOR_STYLE_OBLIQUE_VIEW_SEEDS_PLACER_H__

#include <vtkImageResliceMapper.h>
#include <vtkImageReslice.h>

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
	
	void EnableObliqueView(bool);

public slots:
	void SlotClearAllSeeds();
	void slotPushBtnAxialReconstruction();

	void SetFocalSeed(int i);
	void DeleteFocalSeed();
	void SaveWidgetToSeeds();
	void DropSeed();


protected:
	QInteractorStyleObliqueViewSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleObliqueViewSeedsPlacer();

	//void uniqueInvoke(bool flag);
	void CleanAllLists();
	void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);
	void InitializeObliqueView();


	virtual void OnKeyPress();
	virtual void MoveSliceForward();
	virtual void MoveSliceBackward();


private:
	Ui::QInteractorStyleObliqueViewSeedsPlacer* ui = nullptr;

	int m_numberOfIteractions = 3;
	double m_multiplier = 2.1;
	int m_initialNeighborhoodRadius = 1;
	bool m_inReconstructedView = false;

	static QList<vtkSmartPointer<vtkImageData>> m_listOfVtkImages;
	static QList<QString> m_listOfModalityNames;

	// For reslice view
	int m_currentObliqueSlice;
	static QList<double*> m_normalList;
	static QList<double*> m_originList;
	QList<vtkProp*> m_savedActors;
	vtkSmartPointer<vtkImageResliceMapper> m_resliceMapper;
	vtkSmartPointer<vtkImageSlice> m_reslicer;
};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__
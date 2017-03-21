#ifndef __QINTERACTOR_STYLE_OBLIQUE_VIEW_SEEDS_PLACER_H__
#define __QINTERACTOR_STYLE_OBLIQUE_VIEW_SEEDS_PLACER_H__

#include <vtkImageResliceMapper.h>
#include <vtkImageReslice.h>

#include "vtkDistanceWidget.h"
#include "MyResliceMapper.h"
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
	///**
	// * temporary fix
	// */
	//void SetTargetImages(
	//	QList<vtkSmartPointer<vtkImageData>> listOfVtkImages, 
	//	QList<QString> listOfModalityNames);
	
	void EnableObliqueView(bool);
	
	virtual void SetInteractor(vtkRenderWindowInteractor *interactor);


public slots:
	void slotClearAllSeeds();
	void slotPushBtnAxialReconstruction();
	void slotExtractSegmentFromOverlay();
	void slotUpdateSpinBoxExtractRadius();
	void slotUpdateHSliderExtractRadius();

	void SetFocalSeed(int i);
	void DeleteFocalSeed();
	void SaveWidgetToSeeds();
	void DropSeed();


protected:
	QInteractorStyleObliqueViewSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleObliqueViewSeedsPlacer();

	//void uniqueInvoke(bool flag);
	/// Internal functions
	void CleanAllLists();
	void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);
	void UpdateSeedTable();
	void InterpolateWayPointsToPolyline(vtkPolyData* outPD);
	void InterpolatePolyline(vtkPolyData* inPolyline, vtkPolyData* outPD);
	void ExtractSegment(vtkImageData* inImage,
		vtkImageData* outImage, vtkPolyData* inPolydata);
	void InitializeObliqueView();
	void SetImageToViewer(vtkImageData* image, vtkImageData* overlay);
	void CenterCursorPosition();

	virtual void OnKeyPress();

	virtual void MoveSliceForward();
	virtual void MoveSliceBackward();

	void SetObliqueSlice(int sliceIndex);


private:
	Ui::QInteractorStyleObliqueViewSeedsPlacer* ui = nullptr;

	vtkPolyData* m_polyline;

	int m_numberOfIteractions = 3;
	double m_multiplier = 2.1;
	int m_initialNeighborhoodRadius = 1;
	bool m_inObliqueView = false;

	// For reslice view
	int m_currentObliqueSlice;
	static QList<int*>	m_coordList;
	static vtkImageData* s_curvedImage;
	static vtkImageData* s_curvedOverlay;

	static std::vector<vtkImageData*> s_vectImageStore;

	QList<vtkProp*> m_savedActors;
};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

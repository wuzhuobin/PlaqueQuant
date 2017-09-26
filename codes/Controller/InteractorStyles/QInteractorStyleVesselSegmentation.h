//#ifndef __QABSTRACT_INTERACTOR_STYLE_VESSEL_SEGMENTATION_H__
//#define __QABSTRACT_INTERACTOR_STYLE_VESSEL_SEGMENTATION_H__
//#include "QAbstractNavigation.h"
//#include "InteractorStyleVesselSegmentation.h"
//
//namespace Ui {class QInteractorStyleVesselSegmentation;}
//
//class QInteractorStyleVesselSegmentation : public QAbstractNavigation, 
//	public InteractorStyleVesselSegmentation 
//{
//	Q_OBJECT;
//	QSETUP_UI_HEAD(QInteractorStyleVesselSegmentation);
//
//public:
//	vtkTypeMacro(QInteractorStyleVesselSegmentation, InteractorStyleVesselSegmentation);
//	static QInteractorStyleVesselSegmentation* New();
//
//	virtual void SetCustomEnabled(bool flag);
//	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
//
//public slots:
//	virtual void FillPolygon();
//	virtual void SetSmoothCurveEnable(bool flag);
//	virtual void SetPolygonEnable(bool flag);
//	virtual void EnableNoSegmentation(bool flag);
//	virtual void EnableLumenSegmentation(bool flag);
//	virtual void EnableVesselWallSegmentation(bool flag);
//	virtual void SetContourLabel(int label);
//	virtual void SetGenerateValue(int value);
//	virtual void SetDilateValue(double value);
//	virtual void GenerateLumenWallContourWidget();
//	virtual void FillPolygonThroughSlice();
//	
//	void CleanOne();
//	void CleanAll();
//
//protected:
//	QInteractorStyleVesselSegmentation(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
//	~QInteractorStyleVesselSegmentation();
//	//virtual void uniqueInvoke(bool flag);
//
//
//private:
//	Ui::QInteractorStyleVesselSegmentation *ui = nullptr;
//};
//
//#endif // __QABSTRACT_INTERACTOR_STYLE_VESSEL_SEGMENTATION_H__
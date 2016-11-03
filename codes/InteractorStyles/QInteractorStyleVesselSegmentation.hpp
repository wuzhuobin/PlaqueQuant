#pragma once
#include "QAbstractNavigation.hpp"
#include "InteractorStyleVesselSegmentation.h"

namespace Ui {class QInteractorStyleVesselSegmentation;}

class QInteractorStyleVesselSegmentation : public QAbstractNavigation, 
	public InteractorStyleVesselSegmentation 
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleVesselSegmentation);

public:
	vtkTypeMacro(QInteractorStyleVesselSegmentation, InteractorStyleVesselSegmentation);
	static QInteractorStyleVesselSegmentation* New();

	virtual void Initialization();
	virtual void SetPolygonModeEnabled(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

public slots:
	virtual void FillPolygon();
	virtual void SetSmoothCurveEnable(bool flag);
	virtual void SetPolygonEnable(bool flag);
	virtual void EnableNoSegmentation(bool flag);
	virtual void EnableLumenSegmentation(bool flag);
	virtual void EnableVesselWallSegmentation(bool flag);
	virtual void SetContourLabel(int label);
	virtual void SetGenerateValue(int value);
	virtual void GenerateLumenWallContourWidget();

protected:
	QInteractorStyleVesselSegmentation(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	~QInteractorStyleVesselSegmentation();

private:
	Ui::QInteractorStyleVesselSegmentation *ui = nullptr;
};

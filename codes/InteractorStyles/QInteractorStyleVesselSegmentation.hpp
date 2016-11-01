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


protected:
	QInteractorStyleVesselSegmentation(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	~QInteractorStyleVesselSegmentation();

private:
	Ui::QInteractorStyleVesselSegmentation *ui = nullptr;
};

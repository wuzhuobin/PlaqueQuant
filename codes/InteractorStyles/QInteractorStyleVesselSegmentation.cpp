#include "QInteractorStyleVesselSegmentation.hpp"
#include "ui_qinteractorstylevesselsegmentation.h"

vtkStandardNewMacro(QInteractorStyleVesselSegmentation);
QSETUP_UI_SRC(QInteractorStyleVesselSegmentation);
void QInteractorStyleVesselSegmentation::Initialization()
{
	QAbstractNavigation::Initialization();
	this->setEnabled(true);
	ui->vesselFrame->setHidden(true);
}
void QInteractorStyleVesselSegmentation::SetPolygonModeEnabled(bool flag)
{
	InteractorStyleVesselSegmentation::SetPolygonModeEnabled(flag);
	Initialization();
}
void QInteractorStyleVesselSegmentation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleVesselSegmentation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleVesselSegmentation::FillPolygon()
{
	InteractorStylePolygonDraw::FillPolygon();
}

QInteractorStyleVesselSegmentation::QInteractorStyleVesselSegmentation(int uiType, QWidget * parent)
{
	QNEW_UI();
	if (numOfMyself == 1) {
		// Connect for UNIQUE_UI


	}
}

QInteractorStyleVesselSegmentation::~QInteractorStyleVesselSegmentation() 
{
	QDELETE_UI();
}

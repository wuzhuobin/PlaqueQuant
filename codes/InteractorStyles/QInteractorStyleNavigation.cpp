#include "QInteractorStyleNavigation.hpp"
#include "ui_qinteractorstylenavigation.h"


vtkStandardNewMacro(QInteractorStyleNavigation);
QSETUP_UI_SRC(QInteractorStyleNavigation);


void QInteractorStyleNavigation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

QInteractorStyleNavigation::QInteractorStyleNavigation(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleNavigation::~QInteractorStyleNavigation() {
	QDELETE_UI();
}

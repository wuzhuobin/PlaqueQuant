#include "QInteractorStyleWindowLevel.hpp"
#include "ui_qinteractorstylewindowlevel.h"


vtkStandardNewMacro(QInteractorStyleWindowLevel);
QSETUP_UI_SRC(QInteractorStyleWindowLevel);

QInteractorStyleWindowLevel::QInteractorStyleWindowLevel(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleWindowLevel::~QInteractorStyleWindowLevel()
{
	QDELETE_UI();
}

void QInteractorStyleWindowLevel::Initialization()
{
	QAbstractNavigation::Initialization();
	this->setEnabled(true);
}

void QInteractorStyleWindowLevel::SetWindowLevelModeEnabled(bool flag)
{
	InteractorStyleWindowLevel::SetWindowLevelModeEnabled(flag);
	Initialization();
}

void QInteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

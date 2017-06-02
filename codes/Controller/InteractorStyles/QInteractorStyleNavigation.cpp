#include "QInteractorStyleNavigation.h"
#include "ui_qinteractorstylenavigation.h"
#include "ui_QAbstractNavigation.h"
#include "ImageViewer.h"

#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>
//#include <vtkImageData.h>

vtkStandardNewMacro(QInteractorStyleNavigation);
QSETUP_UI_SRC(QInteractorStyleNavigation);



void QInteractorStyleNavigation::SetCustomEnabled(bool flag)
{
	InteractorStyleNavigation::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

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

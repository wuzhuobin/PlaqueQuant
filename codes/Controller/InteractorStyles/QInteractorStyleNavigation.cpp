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
	double* worldPos = this->GetImageViewer()->GetFocalPointWithWorldCoordinate();
	unsigned int orientation = this->GetImageViewer()->GetSliceOrientation();
}

void QInteractorStyleNavigation::SetCurrentSlice(int slice)
{
	AbstractNavigation::SetCurrentSlice(slice);
	double* worldPos = this->GetImageViewer()->GetFocalPointWithWorldCoordinate();
	unsigned int orientation = this->GetImageViewer()->GetSliceOrientation();
	signalWorldPos(worldPos[0], worldPos[1], worldPos[2], orientation);
}

QInteractorStyleNavigation::QInteractorStyleNavigation(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleNavigation::~QInteractorStyleNavigation() {
	QDELETE_UI();
}

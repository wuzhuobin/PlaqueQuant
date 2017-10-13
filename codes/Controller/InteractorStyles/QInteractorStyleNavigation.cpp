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
	//double* imagePos = this->GetImageViewer()->GetFocalPointWithWorldCoordinate();
	//unsigned int orientation = this->GetImageViewer()->GetSliceOrientation();
}

void QInteractorStyleNavigation::SetCurrentSlice(int slice)
{
	AbstractNavigation::SetCurrentSlice(slice);
	double imageCoord[3];
	this->GetImageViewer()->GetFocalPointWithWorldCoordinate(imageCoord);
	unsigned int orientation = this->GetImageViewer()->GetSliceOrientation();
	emit signalImagePos(imageCoord[0], imageCoord[1], imageCoord[2], orientation);
	//emit signalImageCoord(imageCoord[0], imageCoord[1], imageCoord[2], orientation);
}

QInteractorStyleNavigation::QInteractorStyleNavigation(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleNavigation::~QInteractorStyleNavigation() {
	QDELETE_UI();
}

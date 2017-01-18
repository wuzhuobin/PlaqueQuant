#include "QInteractorStyleNavigation.h"
#include "ui_qinteractorstylenavigation.h"
#include "ui_QAbstractNavigation.h"

vtkStandardNewMacro(QInteractorStyleNavigation);
QSETUP_UI_SRC(QInteractorStyleNavigation);



void QInteractorStyleNavigation::SetNavigationModeEnabled(bool flag)
{
	InteractorStyleNavigation::SetNavigationModeEnabled(flag);
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

void QInteractorStyleNavigation::uniqueCall()
{
	// checking whether extent is equal to the old extent
	// if different, update the maximum and minimum of the ui
	const int* extent = GetExtent();
	for (int i = 0; i < 6; ++i) {
		if (extent[i] != m_oldExtent[i]) {
			memcpy(m_oldExtent, extent, sizeof(m_oldExtent));

			QAbstractNavigation::getUi()->sliceSpinBoxX->setMinimum(extent[0]);
			QAbstractNavigation::getUi()->sliceSpinBoxY->setMinimum(extent[2]);
			QAbstractNavigation::getUi()->sliceSpinBoxZ->setMinimum(extent[4]);
			QAbstractNavigation::getUi()->sliceSpinBoxX->setMaximum(extent[1]);
			QAbstractNavigation::getUi()->sliceSpinBoxY->setMaximum(extent[3]);
			QAbstractNavigation::getUi()->sliceSpinBoxZ->setMaximum(extent[5]);

			QAbstractNavigation::getUi()->sliceHorizontalSliderX->setMinimum(extent[0]);
			QAbstractNavigation::getUi()->sliceHorizontalSliderY->setMinimum(extent[2]);
			QAbstractNavigation::getUi()->sliceHorizontalSliderZ->setMinimum(extent[4]);
			QAbstractNavigation::getUi()->sliceHorizontalSliderX->setMaximum(extent[1]);
			QAbstractNavigation::getUi()->sliceHorizontalSliderY->setMaximum(extent[3]);
			QAbstractNavigation::getUi()->sliceHorizontalSliderZ->setMaximum(extent[5]);
			break;
		}
	}
	
}

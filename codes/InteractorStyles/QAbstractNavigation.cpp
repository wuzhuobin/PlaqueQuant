#include "QInteractorStyleNavigation.h"
#include "QAbstractNavigation.h"
#include "ui_QAbstractNavigation.h"

QSETUP_UI_SRC(QAbstractNavigation);

QAbstractNavigation::QAbstractNavigation(int uiType, QWidget * parent)
{
	// The first instance of QAbstractNavigation will have control of the UI widget
	QNEW_UI();
	if (numOfMyself == 1) {

	}
}

QAbstractNavigation::~QAbstractNavigation()
{
	QDELETE_UI();
}

void QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(int * ijk)
{
	SetCurrentFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);
}

void QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	if (ui->sliceSpinBoxX->value() != i) {
		ui->sliceSpinBoxX->setValue(i);
	}
	if (ui->sliceSpinBoxY->value() != j) {
		ui->sliceSpinBoxY->setValue(j);
	}
	if (ui->sliceSpinBoxZ->value() != k) {
		ui->sliceSpinBoxZ->setValue(k);
	}
}
void QAbstractNavigation::UniqueEnable(bool flag)
{
	QAbstractInteractorStyle::UniqueEnable(flag);
	// run any way for once
	// I don't know why i need to do this
	if (flag != initializationFlag) {

	}
}

void QAbstractNavigation::slotChangeSlice()
{
	SetCurrentFocalPointWithImageCoordinate(ui->sliceSpinBoxX->value(),
		ui->sliceSpinBoxY->value(),
		ui->sliceSpinBoxZ->value());
}



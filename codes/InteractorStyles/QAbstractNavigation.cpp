#include "QInteractorStyleNavigation.h"
#include "QAbstractNavigation.h"
#include "ui_QAbstractNavigation.h"

QSETUP_UI_SRC(QAbstractNavigation);

QAbstractNavigation::QAbstractNavigation(int uiType, QWidget * parent)
{
	// The first instance of QAbstractNavigation will have control of the UI widget
	QNEW_UI();
	if (numOfMyself == 1) {
		uniqueInitialization();
	}
	initialization();
}

QAbstractNavigation::~QAbstractNavigation()
{
	QDELETE_UI();
}

void QAbstractNavigation::uniqueEnable()
{
	connect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
		this, SLOT(slotChangeSlice()),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
	connect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
		this, SLOT(slotChangeSlice()),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
	connect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
		this, SLOT(slotChangeSlice()),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

}

void QAbstractNavigation::uniqueDisable()
{
	disconnect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
		this, SLOT(slotChangeSlice()));
	disconnect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
		this, SLOT(slotChangeSlice()));
	disconnect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
		this, SLOT(slotChangeSlice()));
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

void QAbstractNavigation::slotChangeSlice()
{
	SetCurrentFocalPointWithImageCoordinate(ui->sliceSpinBoxX->value(),
		ui->sliceSpinBoxY->value(),
		ui->sliceSpinBoxZ->value());
}



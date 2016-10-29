#include "QInteractorStyleNavigation.hpp"
#include "QAbstractNavigation.hpp"

QSETUP_UI_SRC(QAbstractNavigation);

QAbstractNavigation::QAbstractNavigation(int uiType, QWidget * parent)
	:m_uiType(uiType)
{
	// The first instance of QAbstractNavigation will have control of the UI widget
	QNEW_UI();
	if (numOfMyself == 1) {
		connect(ui->sliceSpinBoxX, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(ui->sliceSpinBoxY, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(ui->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
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

void QAbstractNavigation::Initialization()
{
	this->setEnabled(true);
}

void QAbstractNavigation::slotChangeSlice()
{
	SetCurrentFocalPointWithImageCoordinate(ui->sliceSpinBoxX->value(),
		ui->sliceSpinBoxY->value(),
		ui->sliceSpinBoxZ->value());
}



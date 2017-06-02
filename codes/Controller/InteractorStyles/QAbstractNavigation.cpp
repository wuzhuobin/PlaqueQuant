#include "QInteractorStyleNavigation.h"
#include "QAbstractNavigation.h"
#include "ui_QAbstractNavigation.h"

// although it is not right, but anyway
#include "AbstractNavigation.h"
#include "ImageViewer.h"

QSETUP_UI_SRC(QAbstractNavigation);

QAbstractNavigation::QAbstractNavigation(int uiType, QWidget * parent)
{
	// The first instance of QAbstractNavigation will have control of the UI widget
	QNEW_UI();
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

	// although it is not right, but anyway
	AbstractNavigation* _style = dynamic_cast<AbstractNavigation*>(this);
	if (_style) {
		SetExtentRange(_style->GetImageViewer()->GetDisplayExtent());
	}
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

void QAbstractNavigation::SetExtentRange(int * extent)
{
	// checking whether extent is equal to the old extent
	// if different, update the maximum and minimum of the ui
	//const int* extent = GetImageViewer()->GetDisplayExtent();
	if (!std::equal(m_oldExtent, m_oldExtent+5, extent)) {
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
	}
}

void QAbstractNavigation::slotChangeSlice()
{
	SetCurrentFocalPointWithImageCoordinate(ui->sliceSpinBoxX->value(),
		ui->sliceSpinBoxY->value(),
		ui->sliceSpinBoxZ->value());
}



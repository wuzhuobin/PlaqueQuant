#include "ui_QInteractorStyleRuler.h"
#include "ui_QAbstractNavigation.h"


#include "QInteractorStyleRuler.h"

vtkStandardNewMacro(QInteractorStyleRuler);
QSETUP_UI_SRC(QInteractorStyleRuler);

void QInteractorStyleRuler::uniqueInvoke(bool flag)
{
	QAbstractNavigation::uniqueInvoke(flag);
	if (flag && flag != initializationFlag) {
		// turn on codes

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
	// turn off
	if (!flag && flag != initializationFlag) {
		// turn off codes
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));

	}
	if (flag != initializationFlag) {

	}
	initializationFlag = flag;
}

void QInteractorStyleRuler::SetDistanceWidgetEnabled(bool flag)
{
	InteractorStyleRuler::SetDistanceWidgetEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleRuler::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleRuler::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}


void QInteractorStyleRuler::EnableMaximumWallThickneesLabel(bool flag) {
	InteractorStyleRuler::EnableMaximumWallThickneesLabel(flag);
}

QInteractorStyleRuler::QInteractorStyleRuler(int uiType, QWidget * parent)
{
	QNEW_UI();
	if (numOfMyself == 1) {

	}
	connect(ui->maximumWallThicknessChkBox, SIGNAL(toggled(bool)),
		this, SLOT(EnableMaximumWallThickneesLabel(bool)));
}

QInteractorStyleRuler::~QInteractorStyleRuler()
{
	QDELETE_UI();
}

#include "QInteractorStyleWindowLevel.h"
#include "ui_qinteractorstylewindowlevel.h"
#include "ui_QAbstractNavigation.h"


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

void QInteractorStyleWindowLevel::uniqueInvoke(bool flag)
{
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
	initializationFlag = flag;

}

void QInteractorStyleWindowLevel::SetWindowLevelModeEnabled(bool flag)
{
	InteractorStyleWindowLevel::SetWindowLevelModeEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

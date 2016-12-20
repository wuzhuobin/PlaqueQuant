#include "QAbstractInteractorStyle.h"
#include "ui_qabstractinteractorstyle.h"

QSETUP_UI_SRC(QAbstractInteractorStyle);

void QAbstractInteractorStyle::uniqueInvoke(bool flag)
{
	// turn on for once
	if (flag && flag != initializationFlag) {
		uniqueEnable();
	}
	// turn off for once
	if (!flag && flag != initializationFlag) {
		uniqueDisable();
	}
	// run any way for once
	if (flag != initializationFlag) {
		uniqueCall();
	}
	initializationFlag = flag;
}

QAbstractInteractorStyle::QAbstractInteractorStyle(int uiType, QWidget * parent)
	:QWidget(parent) {
	QNEW_UI();
	if (numOfMyself == 1) {
		// this will only run in the first class's construction
		uniqueInitialization();
	}
	initialization();

}

QAbstractInteractorStyle::~QAbstractInteractorStyle() {
	QDELETE_UI();
}

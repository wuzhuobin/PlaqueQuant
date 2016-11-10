#include "QAbstractInteractorStyle.h"
#include "ui_qabstractinteractorstyle.h"

QSETUP_UI_SRC(QAbstractInteractorStyle);

void QAbstractInteractorStyle::UniqueEnable(bool flag)
{
	// turn on for once
	if (flag && flag != initializationFlag) {
	}
	// turn off for once
	if (!flag && flag != initializationFlag) {
	}
	// run any way for once
	if (flag != initializationFlag) {

	}


	initializationFlag = flag;
}

QAbstractInteractorStyle::QAbstractInteractorStyle(int uiType, QWidget * parent)
	:QWidget(parent) {
	QNEW_UI();
	if (numOfMyself == 1) {
		// this will only run in the first class's construction
	}


}

QAbstractInteractorStyle::~QAbstractInteractorStyle() {
	QDELETE_UI();
}

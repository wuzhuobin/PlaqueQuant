#include "QAbstractInteractorStyle.h"
#include "ui_qabstractinteractorstyle.h"

QSETUP_UI_SRC(QAbstractInteractorStyle);

void QAbstractInteractorStyle::uniqueInvoke(bool flag)
{
	// turn on for once
	if (flag && flag != getInitializationFlag()) {
		uniqueEnable();
	}
	// turn off for once
	if (!flag && flag != getInitializationFlag()) {
		uniqueDisable();
	}
	// run any way for once
	if (flag != getInitializationFlag()) {
		uniqueCall();
	}
	setInitializationFlag(flag);
}

QAbstractInteractorStyle::QAbstractInteractorStyle(int uiType, QWidget * parent)
	:QWidget(parent) {
	QNEW_UI();
}

QAbstractInteractorStyle::~QAbstractInteractorStyle() {
	QDELETE_UI();
}

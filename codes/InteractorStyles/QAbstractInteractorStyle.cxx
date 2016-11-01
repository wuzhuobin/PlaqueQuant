#include "QAbstractInteractorStyle.h"
#include "ui_qabstractinteractorstyle.h"

QSETUP_UI_SRC(QAbstractInteractorStyle);

QAbstractInteractorStyle::QAbstractInteractorStyle(int uiType, QWidget * parent)
	:QWidget(parent) {
	QNEW_UI();
	if (numOfMyself == 1) {
		// Connect for UNIQUE_UI
	}
	else if (numOfMyself > 1){
		// connect for MULTIPLE_UI
	}
	else {
		// connect fot NO_UI
	}


}

QAbstractInteractorStyle::~QAbstractInteractorStyle() {
	QDELETE_UI();
}

#include "QAbstractInteractorStyle.h"

SETUP_UI_LIST(QAbstractInteractorStyle);

QAbstractInteractorStyle::QAbstractInteractorStyle(int uiType, QWidget * parent)
	:m_uiType(uiType), QWidget(parent) {

}

QAbstractInteractorStyle::~QAbstractInteractorStyle() {
	DELETE_UI();
}

void QAbstractInteractorStyle::setUiType(int uiType)
{
	m_uiType = uiType;
}

int QAbstractInteractorStyle::getUiType()
{
	return m_uiType;
}

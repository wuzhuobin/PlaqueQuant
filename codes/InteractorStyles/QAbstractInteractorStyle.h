#pragma once
#include <QWidget>
#include <qlist.h>
#include "ui_qabstractinteractorstyle.h"

namespace Ui {class QAbstractInteractorStyle;}

#ifndef QSETUP_UI(STYLE_NAME)
#define QSETUP_UI(STYLE_NAME) \
protected:\
static QList<Ui::##STYLE_NAME*> m_uis; \
virtual void setupUi()\
{\
	switch (m_uiType)\
	{\
	case UNIQUE_UI:\
		if (m_uis.size() < 1) {\
			ui = new Ui::##STYLE_NAME;\
			ui->setupUi(this);\
			m_uis.push_back(ui);\
		}\
		else {\
			ui = m_uis.first();\
		}\
		break;\
	case MULTIPLE_UI:\
		ui = new Ui::##STYLE_NAME;\
		ui->setupUi(this);\
		m_uis.push_back(ui);\
		break;\
	case NO_UI:\
		ui = nullptr;\
		break;\
	}\
}

#endif // !QSETUP_UI(STYLE_NAME)

#ifndef SETUP_UI_LIST(STYLE_NAME)
#define SETUP_UI_LIST(STYLE_NAME) \
QList<Ui::##STYLE_NAME*> ##STYLE_NAME::m_uis;
#endif // !SETUP_UI_LIST(STYLE_NAME)

#ifndef DELETE_UI()
#define DELETE_UI() \
if (m_uis.removeOne(ui)) {\
	if (ui != nullptr) {\
		delete ui;\
	}\
}
#endif // !DELETE_UI

class QAbstractInteractorStyle : public QWidget {
	Q_OBJECT
		
public:
	enum UI_TYPE
	{
		NO_UI = -1,
		UNIQUE_UI = 0,
		MULTIPLE_UI = 1
	};
	QSETUP_UI(QAbstractInteractorStyle);
//protected:
//	static QList<Ui::QAbstractInteractorStyle*> m_uis;
//
//	virtual void setupUi()
//	{
//		switch (m_uiType)
//		{
//		case UNIQUE_UI:
//			if (m_uis.size() < 1) {
//				ui = new Ui::QAbstractInteractorStyle;
//				ui->setupUi(this);
//				m_uis.push_back(ui);
//			}
//			else {
//				ui = m_uis.first();
//			}
//			break;
//		case MULTIPLE_UI:
//			ui = new Ui::QAbstractInteractorStyle;
//			ui->setupUi(this);
//			m_uis.push_back(ui);
//			break;
//		}
//	}
protected:
	QAbstractInteractorStyle(int uiType = 0, QWidget * parent = Q_NULLPTR);
	~QAbstractInteractorStyle();



	virtual void setUiType(int uiType);
	virtual int getUiType();



	int m_uiType = 0;

private:
	Ui::QAbstractInteractorStyle* ui = nullptr;


};

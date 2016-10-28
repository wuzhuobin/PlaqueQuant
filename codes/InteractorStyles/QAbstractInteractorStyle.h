#pragma once
#include <QWidget>
#include <qlist.h>

namespace Ui { class QAbstractInteractorStyle; }

#ifndef QSETUP_UI_HEAD(STYLE_NAME)
#define QSETUP_UI_HEAD(STYLE_NAME) \
public:\
Ui::##STYLE_NAME* getUi();\
protected:\
static QList<Ui::##STYLE_NAME*> m_uis; \
void setupUi();\
void setUiType(int uiType);\
int getUiType();\
int m_uiType = -1;\
private:\
static int numOfMyself;
#endif // !QSETUP_UI_HEAD(STYLE_NAME)

#ifndef QSETUP_UI_SRC(STYLE_NAME)
#define QSETUP_UI_SRC(STYLE_NAME) \
int STYLE_NAME::numOfMyself = 0;\
QList<Ui::##STYLE_NAME*> STYLE_NAME::m_uis;\
void STYLE_NAME::setupUi()\
{\
	switch (m_uiType)\
	{\
		case UNIQUE_UI:\
			if (m_uis.size() < 1) {\
				ui = new Ui::##STYLE_NAME; \
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
		++numOfMyself;\
}\
void STYLE_NAME::setUiType(int uiType){\
	this->m_uiType = m_uiType;\
}\
int STYLE_NAME::getUiType(){\
	return this->m_uiType;\
}\
Ui::##STYLE_NAME* STYLE_NAME::getUi(){\
	return ui;\
}
#endif // !QSETUP_UI_SRC(STYLE_NAME)

#ifndef QNEW_UI()
#define QNEW_UI()\
this->setupUi();this->setEnabled(false);
#endif // !QNEW_UI()

#ifndef QDELETE_UI()
#define QDELETE_UI() \
--numOfMyself;\
switch (m_uiType)\
{\
	case UNIQUE_UI:\
		if(numOfMyself == 0){\
			if (m_uis.removeOne(ui)) {\
				if (ui != nullptr) {\
					delete ui; \
					ui = nullptr;\
				}\
			}\
		}\
		break;\
	case MULTIPLE_UI:\
		if (m_uis.removeOne(ui)) {\
			if (ui != nullptr) {\
				delete ui; \
				ui = nullptr;\
			}\
		}\
		break;\
}
#endif // !QDELETE_UI()

class QAbstractInteractorStyle : public QWidget {
	Q_OBJECT;
	QSETUP_UI_HEAD(QAbstractInteractorStyle);
public:
	enum UI_TYPE
	{
		NO_UI = -1,
		UNIQUE_UI = 0,
		MULTIPLE_UI = 1
	};
	virtual void Initialization() = 0;
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
//virtual void setUiType(int uiType);
//virtual int getUiType();
//int m_uiType = -1;
protected:
	QAbstractInteractorStyle(int uiType = -1, QWidget * parent = Q_NULLPTR);
	~QAbstractInteractorStyle();





private:
	Ui::QAbstractInteractorStyle* ui = nullptr;


};

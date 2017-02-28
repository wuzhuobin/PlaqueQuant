/**
 * @file	QAbstractInteractorStyle.h
 * @brief	whatever
 * @authour	wuzhuobin
 * @date	20/12/2016
 * @version	v1.0.0
 * 
 * <pre>copyright:	authors have all right reserved. <pre/>
 * <pre>email:		jiejin2022@163.com. <pre/>
 * <pre>company:	CUHK. <pre/>
 */

#ifndef __QABSTRACT_INTERACTOR_STYLE_H__
#define __QABSTRACT_INTERACTOR_STYLE_H__

#include <qwidget.h>
#include <qlist.h>
#include <qdebug.h>

namespace Ui { class QAbstractInteractorStyle; }

//#ifndef QSETUP_UI_HEAD(STYLE_NAME)
#define QSETUP_UI_HEAD(STYLE_NAME) \
public:\
Ui::##STYLE_NAME* getUi();\
protected:\
static QList<Ui::##STYLE_NAME*> m_uis; \
void setupUi();\
int uiType = NO_UI;\
static int numOfMyself;\
static bool initializationFlag;
//#endif // !QSETUP_UI_HEAD(STYLE_NAME)


//#ifndef QSETUP_UI_SRC(STYLE_NAME)
#define QSETUP_UI_SRC(STYLE_NAME) \
int STYLE_NAME::numOfMyself = 0;\
bool STYLE_NAME::initializationFlag = false;\
QList<Ui::##STYLE_NAME*> STYLE_NAME::m_uis;\
void STYLE_NAME::setupUi()\
{\
	switch (uiType)\
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
Ui::##STYLE_NAME* STYLE_NAME::getUi(){\
	return ui;\
}
//#endif // !QSETUP_UI_SRC(STYLE_NAME)


//#ifndef QNEW_UI()
#define QNEW_UI()\
this->uiType = uiType;this->setupUi();
//#endif // !QNEW_UI()

//#ifndef QDELETE_UI()
#define QDELETE_UI() \
--numOfMyself;\
switch (this->uiType)\
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
//#endif // !QDELETE_UI()

/**
 * @class	QAbstractInteractorStyle 
 * @brief	An predefined interface for all QInteractorStyle class.
 * Detail description.
 * 
 */
class QAbstractInteractorStyle : public QWidget {
	Q_OBJECT;
	QSETUP_UI_HEAD(QAbstractInteractorStyle);
public:
	/**
	 * The type of the ui. 
	 * Defined enumeration type using in initialization the QInteractorStyle's ui
	 */
	enum UI_TYPE
	{
		///< no ui instance 
		NO_UI = 0X00, 
		///< Only one ui instance will be created by the firest instance 
		UNIQUE_UI = 0X01, 
		///< every instance has a ui instance
		MULTIPLE_UI = 0X02 
	};
protected:
	/**
	 * @brief	Constructor.
	 * @see		#UI_TYPE
	 * @param	uiType	to define the number of the ui instances
	 * @param	parent	QWidget's parent
	 */
	QAbstractInteractorStyle(int uiType = NO_UI, QWidget * parent = Q_NULLPTR);
	/**
	 * @brief	Destructor.
	 */
	virtual ~QAbstractInteractorStyle();
	/**
	* @brief	function be invoked once in constructor.
	* This function only be invoked in the first instance.
	*/
	virtual void uniqueInitialization() {};
	/**
	* @brief	function be invokded in constructor.
	*/
	virtual void initialization() {};
	/**
	 * @brief	function be invoked once requirement. 
	 * @see	#uniqueEnable()
	 * @see	#uniqueDisable()
	 * @see	#uniqueCall()
	 * @param	flag	if true, #uniqueEnable() and #uniqueCall() will be called
	 *					otherwise, #uniqueDisable() and #uniqueCall() will be called
	 * 
	 * The #uniqueInvoke() method can provide a template which can enable, disable 
	 * or call anyway, only once in all its instances.
	 * so some connection or initialization for UNIQUE_UI which should only run once
	 * should place in it in case for multiple connections or initialization.
	 */
	virtual void uniqueInvoke(bool flag);
	/**
	 * @brief	abstract funtion to enable once.
	 * @see	#uniqueInvoke()
	 * 
	 * The #uniqueEnable() function used to guarantee only enable once.
	 */
	virtual void uniqueEnable() = 0;
	/**
	 * @brief	abstract funtion to disable once.
	 * @see #uniqueInvoke()
	 *
	 * The #uniqueDisable() function used to guarantee only disable once.
	 */
	virtual void uniqueDisable() = 0;
	/**
	 * @brief	abstract funtion to call once.
	 * @see #uniqueInvoke()
	 *
	 * The #uniqueCall() function used to guarantee only call once.
	 */
	virtual void uniqueCall() = 0;

private:
	///< ui thing. For integrity.
	Ui::QAbstractInteractorStyle* ui = nullptr;

};


#endif // !__QABSTRACT_INTERACTOR_STYLE_H__

#ifndef __QINTERACTOR_STYLE_NAVIGATION_H__
#define __QINTERACTOR_STYLE_NAVIGATION_H__
/**
* @file	QInteractorStyleNavigation.h
* @brief	whatever
* @authour	wuzhuobin
* @date	20/12/2016
* @version	v1.0.0
*
* <pre>copyright:	authors have all right reserved. <pre/>
* <pre>email:		jiejin2022@163.com. <pre/>
* <pre>company:	CUHK. <pre/>
*/
#include "QAbstractNavigation.h"
#include "InteractorStyleNavigation.h"

namespace Ui { class QInteractorStyleNavigation;}
/**
 * @class	QInteractorStyleNavigation 
 * @brief	Navigation.
 * using cursor to navigate. 
 */
class QInteractorStyleNavigation : public QAbstractNavigation, 
	public InteractorStyleNavigation
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleNavigation);

public:
	vtkTypeMacro(QInteractorStyleNavigation, InteractorStyleNavigation);
	static QInteractorStyleNavigation* New();
	virtual void SetNavigationModeEnabled(bool flag);

	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

protected:
	QInteractorStyleNavigation(int uiType = NO_UI, QWidget * parent = Q_NULLPTR);
	~QInteractorStyleNavigation();
	virtual void uniqueInvoke(bool flag);

private:
	Ui::QInteractorStyleNavigation* ui = nullptr;
	int m_oldExtent[6] = { -1 };
};

#endif // !__QINTERACTOR_STYLE_NAVIGATION_H__

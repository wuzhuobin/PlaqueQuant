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
#ifndef __QINTERACTOR_STYLE_NAVIGATION_H__
#define __QINTERACTOR_STYLE_NAVIGATION_H__
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
	/**
	* @brief	Enabled/Disable this InteractorStyle.
	* @param	flag	true, enable. false, disable. 
	*/
	virtual void SetNavigationModeEnabled(bool flag);
	/**
	* @override
	* @brief	function to set the focal point.
	* @param	ijk	int[3] array to set the focal point
	* @param	i, j, k	to set the focal point
	*/
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

protected:
	/**
	 * @brief	Constructor.
	 * @param	uiType	NO_UI, this navigation style using UI of QAbstractNavigation
	 * @param	parent	QWidget's parent 
	 */
	QInteractorStyleNavigation(int uiType = NO_UI, QWidget * parent = Q_NULLPTR);
	/**
	 * @brief	Destructor.
	 */
	virtual ~QInteractorStyleNavigation();
	/**
	* @override
	* @brief	abstract funtion to call once.
	* @see #uniqueInvoke()
	*
	* The #uniqueCall() function used to guarantee only call once.
	* For updating the spinbox's boundaries.
	*/
	virtual void uniqueCall();

private:
	///< ui pointer.
	Ui::QInteractorStyleNavigation* ui = nullptr;
	///< for using in whether to update the spinbox. 
	int m_oldExtent[6] = { -1 };
};

#endif // !__QINTERACTOR_STYLE_NAVIGATION_H__

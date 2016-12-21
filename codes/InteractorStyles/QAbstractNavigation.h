/**
 * @file	QAbstractNavigation.h
 * @brief	whatever
 * @authour	wuzhuobin
 * @date	20/12/2016
 * @version	v1.0.0
 *
 * <pre>copyright:	authors have all right reserved. <pre/>
 * <pre>email:		jiejin2022@163.com. <pre/>
 * <pre>company:	CUHK. <pre/>
 */
#ifndef __QABSTRACT_NAVIGATION_H__
#define __QABSTRACT_NAVIGATION_H__
#include "QAbstractInteractorStyle.h"

namespace Ui { class QAbstractNavigation;}
/**
 * @class	QAbstractNavigation 
 * @brief	Navigation for all class. 
 * Since all QInteractorStyle will use navigation, other class should inherit it
 */
class QAbstractNavigation : public QAbstractInteractorStyle
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QAbstractNavigation);
public:
	/**
	 * @brief	function to set the focal point.
	 * @param	ijk	int[3] array to set the focal point
	 * @param	i, j, k	to set the focal point
	 */
	virtual void SetCurrentFocalPointWithImageCoordinate(int* ijk);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
public slots:
	/**
	 * @slot
	 * @brief	function to update the current focal point with the spinBox. 
	 */
	virtual void slotChangeSlice();

protected:
	/**
	 * @brief	Constructor.
	 * @see	#UI_TYPE
	 * @param	uiType	to define the number of the ui instances
	 * @param	parent	QWidget's parent
	 */
	QAbstractNavigation(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	/**
	* @brief	Destructor.
	*/
	virtual ~QAbstractNavigation();
	/**
	* @override
	* @brief	abstract funtion to enable once.
	* @see #uniqueInvoke()
	*
	* The #uniqueEnable() function used to guarantee only enable once.
	* For the connection of the spinbox
	*/
	virtual void uniqueEnable();
	/**
	* @override
	* @brief	abstract funtion to disable once.
	* @see #uniqueInvoke()
	*
	* The #uniqueDisable() function used to guarantee only disable once
	* For the disconnection of the spinbox
	*/
	virtual void uniqueDisable();
	/**
	* @override
	* @brief	abstract funtion to call once.
	* @see #uniqueInvoke()
	*
	* The #uniqueCall() function used to guarantee only call once.
	*/
	virtual void uniqueCall() {};
	

private:
	///< the pointer of the navigation ui.
	Ui::QAbstractNavigation* ui = nullptr;
};

#endif // !__QABSTRACT_NAVIGATION_H__

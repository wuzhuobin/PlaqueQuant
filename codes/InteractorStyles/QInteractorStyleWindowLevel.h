#ifndef __QINTERACTOR_STYLE_WINDOW_LEVEL_H__
#define __QINTERACTOR_STYLE_WINDOW_LEVEL_H__
/**
* @file	QInteractorStyleWindowLevel.h
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
#include "InteractorStyleWindowLevel.h"

namespace Ui {class QInteractorStyleWindowLevel;}
/**
* @class	QInteractorStyleWindowLevel
* @brief	window level.
* using cursor to change window level.
*/
class QInteractorStyleWindowLevel : public QAbstractNavigation,
	public InteractorStyleWindowLevel
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleWindowLevel);

public:
	vtkTypeMacro(QInteractorStyleWindowLevel, InteractorStyleWindowLevel)
	static QInteractorStyleWindowLevel* New();
	/**
	* @brief	Enabled/Disable this InteractorStyle.
	* @param	flag	true, enable. false, disable.
	*/
	virtual void SetWindowLevelModeEnabled(bool flag);
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
	* @param	uiType	NO_UI
	* @param	parent	QWidget's parent
	*/
	QInteractorStyleWindowLevel(int uiType = NO_UI, QWidget * parent = Q_NULLPTR);
	/**
	 * @brief	Destructor. 
	 */
	virtual ~QInteractorStyleWindowLevel();

private:
	///< the pointer of the navigation ui.
	Ui::QInteractorStyleWindowLevel *ui = nullptr;
};


#endif // !__QINTERACTOR_STYLE_WINDOW_LEVEL_H__

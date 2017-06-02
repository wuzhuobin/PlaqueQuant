/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract interactor class used in medical viewers.
This class is the ultimate parent of all interactor classes.


Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef ABSTRACT_INTERACTOR_STYLE_H
#define ABSTRACT_INTERACTOR_STYLE_H

#include <list>
class vtkObject;
class vtkCallbackCommand;
/**
 * Macro for iterate specified @param STYLE_NAME class to run its @param METHOD
 * saving human labor to type this iterate again!
 */

//#ifndef DYNAMIC_CAST_CONSTITERATOR(STYLE_NAME, METHOD)
//#define DYNAMIC_CAST_CONSTITERATOR(STYLE_NAME, METHOD) \
//for(std::list<AbstractInteractorStyle*>::const_iterator cit = \
//	m_abstractInteractorStyles.cbegin(); cit != m_abstractInteractorStyles.cend(); ++cit){\
//	STYLE_NAME* _style = STYLE_NAME::SafeDownCast(*cit); \
//	if (_style != nullptr && _style->GetCustomEnabled()) { \
//		_style->##METHOD; \
//	} \
//}
//#endif // !DYNAMIC_CAST_CONSTITERATOR(STYLE_NAME, METHOD)

#ifndef DYNAMIC_CAST_CONSTITERATOR
#define DYNAMIC_CAST_CONSTITERATOR(STYLE_NAME, METHOD) \
for(std::list<AbstractInteractorStyle*>::const_iterator cit = \
	m_abstractInteractorStyles.cbegin(); cit != m_abstractInteractorStyles.cend(); ++cit){\
	STYLE_NAME* _style = dynamic_cast<##STYLE_NAME*>(*cit); \
	if (_style != nullptr && _style->GetCustomEnabled()) { \
		_style->##METHOD; \
	} \
}
#endif // !DYNAMIC_CAST_CONSTITERATOR



class AbstractInteractorStyle
{
public:

	virtual void SetCustomEnabled(bool flag);
	virtual bool GetCustomEnabled();
	virtual void CustomEnabledOn();
	virtual void CustomEnabledOff();

	virtual void SetViewer(vtkObject* viewer);
	virtual vtkObject* GetViewer();
protected:
	AbstractInteractorStyle();
	virtual ~AbstractInteractorStyle();

	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnLeftDoubleClick();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnRightDoubleClick();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();
	virtual void OnMiddleDoubleClick();


	bool m_customFlag = false;
	bool m_rightFunctioning = false;
	bool m_leftFunctioning = false;
	bool m_middleFunctioning = false;

	unsigned short m_numberOfLeftClicks = 0;
	unsigned short m_numberOfRightClicks = 0;
	unsigned short m_numberOfMiddleClicks = 0;

	static std::list<AbstractInteractorStyle*> m_abstractInteractorStyles;

	vtkObject* m_viewer = nullptr;
	///< callback for re-eanbled when Viewer invoke vtkCommand#UpdateDataEvent
	vtkCallbackCommand* callback = nullptr;
};

#endif //ABSTRACT_INTERACTOR_STYLE_H
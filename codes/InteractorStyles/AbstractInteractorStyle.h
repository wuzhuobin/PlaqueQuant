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
/**
 * Macro for iterate specified @param STYLE_NAME class to run its @param METHOD
 * saving human labor to type this iterate again!
 */
#ifndef STYLE_DOWN_CAST_CONSTITERATOR(STYLE_NAME, METHOD)
#define STYLE_DOWN_CAST_CONSTITERATOR(STYLE_NAME, METHOD) \
for(std::list<AbstractInteractorStyle*>::const_iterator cit = \
	m_abstractInteractorStyles.cbegin(); cit != m_abstractInteractorStyles.cend(); ++cit){\
	STYLE_NAME* _style = dynamic_cast<##STYLE_NAME*>(*cit); \
	if (_style != nullptr) { \
		_style->##METHOD; \
	} \
}
#endif // !STYLE_DOWN_CAST_CONSTITERATOR(STYLE_NAME, METHOD)

class AbstractInteractorStyle
{
public:

protected:
	AbstractInteractorStyle();
	virtual ~AbstractInteractorStyle();

	virtual void OnMouseWheelForward() = 0;
	virtual void OnMouseWheelBackward() = 0;
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnLeftDoubleClick() = 0;
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnRightDoubleClick() = 0;
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();
	virtual void OnMiddleDoubleClick() = 0;
	virtual void OnMouseMove() = 0;
	virtual void OnChar() = 0;
	virtual void OnKeyPress() = 0;



	bool m_rightFunctioning = false;
	bool m_leftFunctioning = false;
	bool m_middleFunctioning = false;

	unsigned short NumberOfLeftClicks = 0;
	unsigned short NumberOfRightClicks = 0;
	unsigned short NumberOfMiddleClicks = 0;

	static std::list<AbstractInteractorStyle*> m_abstractInteractorStyles;


};

#endif //ABSTRACT_INTERACTOR_STYLE_H
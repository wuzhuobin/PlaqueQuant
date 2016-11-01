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


//#include <vtkActor.h>
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
	if (_style != nullptr && _style != this) { \
		_style->##METHOD; \
	} \
}
#endif // !STYLE_DOWN_CAST_CONSTITERATOR(STYLE_NAME, METHOD)

class AbstractInteractorStyle
{
public:
	//virtual void SetOrientation(int);
	//
	//int GetOrientation();

	//vtkActor* PickActor(int x, int y);

protected:
	AbstractInteractorStyle();
	~AbstractInteractorStyle();

	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnMiddleButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonUp();
	virtual void OnMiddleButtonUp();
	
	//int			m_orientation;

	bool m_rightFunctioning = false;
	bool m_leftFunctioning = false;
	bool m_middleFunctioning = false;
	bool m_rightDoubleClick = false;
	bool m_leftDoubleClick = false;
	bool m_middleDoubleClick = false;


	unsigned short NumberOfLeftClicks = 0;
	unsigned short NumberOfRightClicks = 0;
	unsigned short NumberOfMiddleClicks = 0;

	static std::list<AbstractInteractorStyle*> m_abstractInteractorStyles;


};

#endif //ABSTRACT_INTERACTOR_STYLE_H
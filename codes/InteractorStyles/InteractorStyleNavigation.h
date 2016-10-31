/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant

Author:		Lok, Ka Hei Jason
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
M.Phil Student

This class allows interaction related to reslice position and m_orientation on images.

Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#ifndef INTERACTOR_STYLE_NAVIGATION_H
#define INTERACTOR_STYLE_NAVIGATION_H

#include "AbstractNavigation.h"


class InteractorStyleNavigation : public AbstractNavigation
{
public:
	vtkTypeMacro(InteractorStyleNavigation, AbstractNavigation);
	static InteractorStyleNavigation* New();
	void SetNavigationModeEnabled(bool flag);
protected:
	InteractorStyleNavigation();
	~InteractorStyleNavigation();

	virtual void OnMouseMove();
	virtual void OnLeftButtonUp();
	virtual void OnChar();
private:
	void CalculateIndex();
	int m_index[3];
	bool m_navigationModeEnableFlag = false;
};


#endif // INTERACTOR_STYLE_NAVIGATION_H
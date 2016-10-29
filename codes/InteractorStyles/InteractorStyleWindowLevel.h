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

This class allows interactive modification to window and level.


Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#ifndef INTERACTOR_STYLE_WINDOW_LEVEL_H
#define INTERACTOR_STYLE_WINDOW_LEVEL_H
#include <vtkInteractorStyleImage.h>
#include "AbstractNavigation.h"


class InteractorStyleWindowLevel : public AbstractNavigation
{
public:
	vtkTypeMacro(InteractorStyleWindowLevel, AbstractNavigation);
	static InteractorStyleWindowLevel* New();
	//void SetWindowLevelSpinBox(QDoubleSpinBox* w, QDoubleSpinBox* l);

	virtual void SetWindowLevelModeEnabled(bool flag);

	virtual void SetWindow(double window);

	virtual void SetLevel(double level);
	virtual void WindowLevel();
	virtual void SetWindowLevel(double window, double level);

protected:
	InteractorStyleWindowLevel();
	~InteractorStyleWindowLevel();

	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnKeyPress();
	virtual void OnChar();

private:
	double m_window;
	double m_level;
	
	bool m_windowLevelEnabledFlag = false;
	//QDoubleSpinBox* m_wlDoubleSpinBox[2];
	vtkImageProperty* propertyTMP = nullptr;

};



#endif


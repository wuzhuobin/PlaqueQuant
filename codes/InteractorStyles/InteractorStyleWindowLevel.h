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
#include "AbstractInteractorStyleImage.h"


class InteractorStyleWindowLevel : public AbstractInteractorStyleImage
{
public:
	vtkTypeMacro(InteractorStyleWindowLevel, AbstractInteractorStyleImage);
	static InteractorStyleWindowLevel* New();
	//void SetWindowLevelSpinBox(QDoubleSpinBox* w, QDoubleSpinBox* l);

	virtual void SetWindow(double window);
	virtual void SetLevel(double level);
	virtual void SetWindowLevel(double window, double level);

protected:
	InteractorStyleWindowLevel();
	~InteractorStyleWindowLevel();

	virtual void OnMouseMove();
	virtual void OnKeyPress();

private:
	void WindowLevel();
	double m_window;
	double m_level;
	//QDoubleSpinBox* m_wlDoubleSpinBox[2];


};



#endif


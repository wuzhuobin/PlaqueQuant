/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor modified from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef INTERACTOR_STYLE_3D_NAVIGATION_H
#define INTERACTOR_STYLE_3D_NAVIGATION_H

#include <vtkObjectFactory.h>
#include <QTime>
#include "AbstractInteractorStyle3D.h"

class InteractorStyle3DNavigation : public AbstractInteractorStyle3D
{
public:
	vtkTypeMacro(InteractorStyle3DNavigation, AbstractInteractorStyle3D);
	static InteractorStyle3DNavigation* New();

protected:
	InteractorStyle3DNavigation();
	~InteractorStyle3DNavigation();

	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnChar();

private:
	bool CalculateIndex();

	QTime m_clickTimer;
};


#endif // !INTERACTOR_STYLE_3D_NAVIGATION_H

/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract interactor class used in medical viewers.
This class is the ultimate parent of all interactor classes related to 3D.


Wong Matthew Lun
Copyright (C) 2016
*/
#ifndef ABSTRACT_INTERACTOR_STYLE_3D_H
#define ABSTRACT_INTERACTOR_STYLE_3D_H

/// VTK
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>

#include "AbstractInteractorStyle.h"


class AbstractInteractorStyle3D : public vtkInteractorStyleTrackballCamera, public AbstractInteractorStyle
{
public:
	vtkTypeMacro(AbstractInteractorStyle3D, vtkInteractorStyleTrackballCamera);
	static AbstractInteractorStyle3D *New();


	double* GetCurrentPos();
	int*	GetPreviousPosition();



protected:
	AbstractInteractorStyle3D();
	~AbstractInteractorStyle3D();
	
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();

	double	m_currentPos[3];
	int 	m_previousPosition[2];
};

#endif //ABSTRACT_INTERACTOR_STYLE_3D_H
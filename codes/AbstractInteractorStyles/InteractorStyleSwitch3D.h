/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is based on vtkInteractorStyleSwitch, written to allow easy
switching between 3D interactors.

Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef INTERACTOR_STYLE_SWITCH_3D_H
#define INTERACTOR_STYLE_SWITCH_3D_H

#include <string>
#include <vtkCornerAnnotation.h>
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleSwitchBase.h>
#include "AbstractInteractorStyle.h"
#include "InteractorStyle3DNavigation.h"
#include "InteractorStyle3DDistanceWidget.h"
#include "StyleSwitchMacro.h"



class InteractorStyleSwitch3D : public vtkInteractorStyleSwitchBase, public AbstractInteractorStyle
{
public:
	vtkTypeMacro(InteractorStyleSwitch3D, vtkInteractorStyleSwitchBase);
	static InteractorStyleSwitch3D* New();

	vtkGetMacro(Navigation, InteractorStyle3DNavigation*);
	vtkGetMacro(DistanceWidget, InteractorStyle3DDistanceWidget*);

	void SetInteractor(vtkRenderWindowInteractor *iren);

	SetInteractorStyle3DMacro(Navigation);
	SetInteractorStyle3DMacro(DistanceWidget);

	CurrentStyle3DMacro(Navigation);
	CurrentStyle3DMacro(DistanceWidget);

	virtual void SetDefaultRenderer(vtkRenderer* renderer);
	virtual void SetCurrentRenderer(vtkRenderer* renderer);
	virtual void OnChar();

	//void Change3DInteractionMode(vtkObject*, long unsigned int, void*);


protected:
	InteractorStyleSwitch3D();
	~InteractorStyleSwitch3D();

	void InternalUpdate();
	void SetAutoAdjustCameraClippingRange(int value);

	//void Set3DMode(int);

	vtkCornerAnnotation*						m_textActor;
	vtkInteractorStyle*					CurrentStyle;
	InteractorStyle3DNavigation*		Navigation;
	InteractorStyle3DDistanceWidget*	DistanceWidget;

};



#endif
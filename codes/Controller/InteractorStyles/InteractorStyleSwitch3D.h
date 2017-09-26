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


#include <vtkSetGet.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleSwitchBase.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <list>



#define SetInteractorStyle3DMacro(style) \
void SetInteractorStyleTo3D##style () { \
	if (CurrentStyle != ##style) { \
		if (this->CurrentStyle) { \
			this->InternalUpdate(false);\
			this->CurrentStyle->SetInteractor(0); \
	} \
		this->CurrentStyle = this->##style;\
	} \
	if (this->CurrentStyle) { \
		this->CurrentStyle->SetInteractor(this->Interactor);\
		this->CurrentStyle->SetTDxStyle(this->TDxStyle);\
		this->InternalUpdate(true);\
	} \
}

#define CurrentStyle3DMacro(style) \
bool CurrentStyleIs3D##style () {\
	return CurrentStyle == ##style; \
}

class SurfaceViewer;

class InteractorStyleSwitch3D : public vtkInteractorStyleSwitchBase
{
public:
	vtkTypeMacro(InteractorStyleSwitch3D, vtkInteractorStyleSwitchBase);
	static InteractorStyleSwitch3D* New();

	vtkGetMacro(TrackballCamera, vtkInteractorStyleTrackballCamera*);
	SetInteractorStyle3DMacro(TrackballCamera);
	CurrentStyle3DMacro(TrackballCamera);

	void SetInteractor(vtkRenderWindowInteractor *iren);

	virtual void SetSurfaceViewer(SurfaceViewer* surfaceViewer);
	virtual void SetDefaultRenderer(vtkRenderer* renderer);
	virtual void SetCurrentRenderer(vtkRenderer* renderer);
	virtual void OnChar();


	virtual void SetEnabled(int i);
	virtual void SetEnabledOn();
	virtual void SetEnabledOff();


protected:
	InteractorStyleSwitch3D();
	virtual ~InteractorStyleSwitch3D();

	void InternalUpdate(bool flag);
	void SetAutoAdjustCameraClippingRange(int value);

	vtkInteractorStyle*	CurrentStyle;
	vtkInteractorStyleTrackballCamera* TrackballCamera;
	std::list<vtkInteractorStyle*> allStyles;


};



#endif
/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant
This class is based on vtkInteractorStyleSwitch, written to allow easy
switching between 2D interactors.
Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef INTERACTOR_STYLE_SWITCH_H
#define INTERACTOR_STYLE_SWITCH_H

#include <vtkSetGet.h>
#include <vtkInteractorStyleSwitchBase.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <list>

#define SetInteractorStyleMacro(style) \
void SetInteractorStyleTo##style () { \
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

#define CurrentStyleMacro(style) \
bool CurrentStyleIs##style () {\
	return CurrentStyle == ##style; \
}
#define SET_CURRENT_STYLE_TO_MACRO
#ifndef SET_CURRENT_STYLE_TO_MACRO
#define SET_CURRENT_STYLE_TO_MACRO_H(style) \
void SetCurrentStyleTo##style();
#define SET_CURRENT_STYLE_TO_MACRO_CPP(style) \
void InteractorStyleSwitch::SetCurrentStyleTo##style(){ \
	if (this->CurrentStyle != ##style) { \
		if (this->CurrentStyle) { \
			this->CurrentStyle->SetInteractor(0); \
	} \
		this->CurrentStyle = this->##style;\
	} \
	if (this->CurrentStyle) { \
		this->CurrentStyle->SetInteractor(this->Interactor);\
		this->CurrentStyle->SetTDxStyle(this->TDxStyle);\
	} \
	this->InternalUpdate();\
}
#endif // !SET_CURRENT_STYLE_TO_MACRO


class ImageViewer;

class InteractorStyleSwitch : public vtkInteractorStyleSwitchBase
{
public:
	static InteractorStyleSwitch* New();
	vtkTypeMacro(InteractorStyleSwitch, vtkInteractorStyleSwitchBase);
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	vtkGetMacro(InteractorStyleTesting, vtkInteractorStyleTrackballCamera*);
	SetInteractorStyleMacro(InteractorStyleTesting);
	CurrentStyleMacro(InteractorStyleTesting);

	void SetInteractor(vtkRenderWindowInteractor *iren);



	virtual void SetDefaultRenderer(vtkRenderer* renderer);
	virtual void SetCurrentRenderer(vtkRenderer* renderer);
	virtual void SetImageViewer(ImageViewer* m_imageViewer);


	virtual void SetEnabled(int);
	virtual void SetEnabledOn();
	virtual void SetEnabledOff();

protected:
	InteractorStyleSwitch();
	virtual ~InteractorStyleSwitch();

	void InternalUpdate(bool flag);
	void SetAutoAdjustCameraClippingRange(int value);


	vtkInteractorStyleTrackballCamera* InteractorStyleTesting;
	vtkInteractorStyle*	CurrentStyle;
	std::list<vtkInteractorStyle*> allStyles;
};



#endif
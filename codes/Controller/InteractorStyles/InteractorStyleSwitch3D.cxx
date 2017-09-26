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


#include <vtkCallbackCommand.h>
#include <vtkObjectFactory.h>
#include "InteractorStyleSwitch3D.h"
#include "AbstractInteractorStyle.h"
#include "vtkRenderWindowInteractor.h"
#include "AbstractInteractorStyleSurface.h"
#include "SurfaceViewer.h"
//#include "MainWindow.h"

//class MyViewer;

vtkStandardNewMacro(InteractorStyleSwitch3D);

InteractorStyleSwitch3D::InteractorStyleSwitch3D()
{
	//MainWindow* mainwnd = MainWindow::GetMainWindow();

	this->TrackballCamera = vtkInteractorStyleTrackballCamera::New();
	allStyles.push_back(TrackballCamera);
	//this->DistanceWidget = InteractorStyle3DDistanceWidget::New();
	this->CurrentStyle = 0;

}

InteractorStyleSwitch3D::~InteractorStyleSwitch3D()
{
	/* Do not modifiy the lower code£¡ */
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		if ((*it) != NULL) {
			(*it)->Delete();
			(*it) = NULL;
		}
	}
	allStyles.clear();
}

void InteractorStyleSwitch3D::InternalUpdate(bool flag)
{

	// using enable update to call #EnabledOn() and #EnabledOff()
	AbstractInteractorStyleSurface* _style = AbstractInteractorStyleSurface::SafeDownCast(this->CurrentStyle);
	if (_style) {
		_style->SetCustomEnabled(flag);
	}

}

void InteractorStyleSwitch3D::SetAutoAdjustCameraClippingRange(int value)
{
	if (value == this->AutoAdjustCameraClippingRange)
	{
		return;
	}

	if (value < 0 || value > 1)
	{
		vtkErrorMacro("Value must be between 0 and 1 for" <<
			" SetAutoAdjustCameraClippingRange");
		return;
	}

	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		(*it)->SetAutoAdjustCameraClippingRange(value);
	}
	this->Modified();
}
//
//void InteractorStyleSwitch3D::Set3DMode(int m)
//{
//	switch (m)
//	{
//	case 1:
//		this->SetInteractorStyleTo3DTrackBallCamera();
//		break;
//	case 2:
//		this->SetInteractorStyleTo3DNavigation();
//		break;
//	case 3:
//		this->SetInteractorStyleTo3DPickSeed();
//		break;
//	default:
//		this->SetInteractorStyleTo3DTrackBallCamera();
//		break;
//	}
//}

void InteractorStyleSwitch3D::SetSurfaceViewer(SurfaceViewer * surfaceViewer)
{
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		AbstractInteractorStyleSurface* _style =
			AbstractInteractorStyleSurface::SafeDownCast(*cit);
		if (_style != NULL) {
			_style->SetViewer(surfaceViewer);
		}
	}
}

void InteractorStyleSwitch3D::SetDefaultRenderer(vtkRenderer* renderer)
{
	Superclass::SetDefaultRenderer(renderer);
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetDefaultRenderer(renderer);
	}

}

void InteractorStyleSwitch3D::SetCurrentRenderer(vtkRenderer* renderer)
{
	Superclass::SetCurrentRenderer(renderer);
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetCurrentRenderer(renderer);
	}

}


void InteractorStyleSwitch3D::OnChar()
{
}
void InteractorStyleSwitch3D::SetEnabled(int i)
{
	Superclass::SetEnabled(i);
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetEnabled(i);
	}
}
void InteractorStyleSwitch3D::SetEnabledOn()
{
	this->SetEnabled(1);
}
void InteractorStyleSwitch3D::SetEnabledOff()
{
	this->SetEnabled(0);
}
//
//void InteractorStyleSwitch3D::Change3DInteractionMode(vtkObject *, long unsigned int, void *)
//{
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	MyViewer* viewer = mainWnd->GetViewers(3);
//	int state = viewer->GetButtonRep()->GetState() + 1;
//
//	this->Set3DMode(state);
//}

void InteractorStyleSwitch3D::SetInteractor(vtkRenderWindowInteractor *iren)
{
	if (iren == this->Interactor)
	{
		return;
	}
	// if we already have an Interactor then stop observing it
	if (this->Interactor)
	{
		this->Interactor->RemoveObserver(this->EventCallbackCommand);
	}
	this->Interactor = iren;
	// add observers for each of the events handled in ProcessEvents
	if (iren)
	{
		iren->AddObserver(vtkCommand::CharEvent,
			this->EventCallbackCommand,
			this->Priority);

		iren->AddObserver(vtkCommand::DeleteEvent,
			this->EventCallbackCommand,
			this->Priority);
	}
	//this->SetInteractorStyleTo3DTrackBallCamera();
}
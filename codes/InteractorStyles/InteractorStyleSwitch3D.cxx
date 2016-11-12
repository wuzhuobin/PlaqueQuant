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
#include "InteractorStyleSwitch3D.h"
#include "MainWindow.h"

class MyViewer;

vtkStandardNewMacro(InteractorStyleSwitch3D);

InteractorStyleSwitch3D::InteractorStyleSwitch3D()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();

	this->Navigation = InteractorStyle3DNavigation::New();
	this->DistanceWidget = InteractorStyle3DDistanceWidget::New();
	this->CurrentStyle = 0;

	this->SetInteractorStyleTo3DDistanceWidget();
}

InteractorStyleSwitch3D::~InteractorStyleSwitch3D()
{
	this->Navigation->Delete();
	this->Navigation = NULL;

	this->DistanceWidget->Delete();
	this->DistanceWidget = NULL;
}

void InteractorStyleSwitch3D::InternalUpdate()
{
	// Don't render text before any text is loaded
	if (this->Interactor != NULL)
		this->Interactor->Render();
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

	this->AutoAdjustCameraClippingRange = value;
	this->Navigation->SetAutoAdjustCameraClippingRange(value);
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

void InteractorStyleSwitch3D::SetDefaultRenderer(vtkRenderer* renderer)
{
	Superclass::SetDefaultRenderer(renderer);
	this->Navigation->SetDefaultRenderer(renderer);
	this->DistanceWidget->SetDefaultRenderer(renderer);
}

void InteractorStyleSwitch3D::SetCurrentRenderer(vtkRenderer* renderer)
{
	Superclass::SetCurrentRenderer(renderer);
	this->Navigation->SetCurrentRenderer(renderer);
	this->DistanceWidget->SetCurrentRenderer(renderer);

}


void InteractorStyleSwitch3D::OnChar()
{
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
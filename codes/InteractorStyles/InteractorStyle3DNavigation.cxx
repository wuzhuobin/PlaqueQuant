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

#include <vtkSmartPointer.h>
#include "InteractorStyle3DNavigation.h"
#include "MainWindow.h"

vtkStandardNewMacro(InteractorStyle3DNavigation);

InteractorStyle3DNavigation::InteractorStyle3DNavigation()
{
}

InteractorStyle3DNavigation::~InteractorStyle3DNavigation()
{
}

void InteractorStyle3DNavigation::OnLeftButtonDown()
{
	this->m_clickTimer.restart();
	AbstractInteractorStyle3D::OnLeftButtonDown();
}

void InteractorStyle3DNavigation::OnLeftButtonUp()
{
	if (this->m_clickTimer.elapsed() < 150) {
		this->CalculateIndex();
	}
	AbstractInteractorStyle3D::OnLeftButtonUp();
}

void InteractorStyle3DNavigation::OnChar()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();

	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	char key = rwi->GetKeyCode();

	if (key == 'r') {
		if(true) 
		{
			
		}
		else {
			AbstractInteractorStyle3D::OnChar();
		}
	}
	else {
		AbstractInteractorStyle3D::OnChar();
	}
}

bool InteractorStyle3DNavigation::CalculateIndex()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();


	return true;
}


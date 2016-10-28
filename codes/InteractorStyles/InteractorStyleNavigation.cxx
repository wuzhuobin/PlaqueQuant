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

This class allows interaction related to reslice position and m_orientation on images.

Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#include "InteractorStyleNavigation.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>


vtkStandardNewMacro(InteractorStyleNavigation);

InteractorStyleNavigation::InteractorStyleNavigation()
	:AbstractNavigation()
{
}

InteractorStyleNavigation::~InteractorStyleNavigation()
{
}

void InteractorStyleNavigation::OnMouseMove()
{
	//if (m_rightFunctioning) {
	//	this->SynchronalZooming();
	//}
	if (m_leftFunctioning) {
		this->CalculateIndex();
		return;
	}
	AbstractNavigation::OnMouseMove();

	if (m_rightFunctioning) {
		SynchronizedZooming();
	}
}


void InteractorStyleNavigation::OnLeftButtonUp()
{
	this->CalculateIndex();
	AbstractNavigation::OnLeftButtonUp();
}

void InteractorStyleNavigation::OnChar()
{
	switch (this->Interactor->GetKeyCode())
	{
	case 'R':
	case 'r':
		AbstractNavigation::OnChar();
		SynchronizedZooming();
		break;
	default:
		break;
	}
}

void InteractorStyleNavigation::SynchronizedZooming()
{
	AbstractNavigation::SynchronalZooming();
}

void InteractorStyleNavigation::CalculateIndex()
{
	//Pick
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		m_imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	double index[3];
	if (m_imageViewer->GetInput() != NULL) {
		picked[GetSliceOrientation()] = GetOrigin()[GetSliceOrientation()] +
			GetSlice() * GetSpacing()[GetSliceOrientation()];
		for (int i = 0; i < 3; i++)
		{
			index[i] = (picked[i] - GetOrigin()[i]) / GetSpacing()[i];
		}
		SetCurrentFocalPointWithImageCoordinate(index[0], index[1], index[2]);
	}

}


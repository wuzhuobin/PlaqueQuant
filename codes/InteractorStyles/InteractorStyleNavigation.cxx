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

void InteractorStyleNavigation::SetNavigationModeEnabled(bool flag)
{
	m_navigationModeEnableFlag = flag;
}

InteractorStyleNavigation::InteractorStyleNavigation()
	:AbstractNavigation()
{
}

InteractorStyleNavigation::~InteractorStyleNavigation()
{
}

void InteractorStyleNavigation::OnMouseMove()
{
	AbstractNavigation::OnMouseMove();
	if (m_leftFunctioning) {
		this->CalculateIndex();
		SetCurrentFocalPointWithImageCoordinate(m_index[0], m_index[1], m_index[2]);
	}
	if (m_rightFunctioning) {
		SynchronalZooming();
	}
}


void InteractorStyleNavigation::OnLeftButtonUp()
{
	AbstractInteractorStyle::OnLeftButtonUp();
	this->CalculateIndex();
	SetCurrentFocalPointWithImageCoordinate(m_index[0], m_index[1], m_index[2]);
}

void InteractorStyleNavigation::OnChar()
{
	switch (this->Interactor->GetKeyCode())
	{
	case 'R':
	case 'r':
		AbstractNavigation::OnChar();
		SynchronalZooming();
		break;
	default:
		AbstractNavigation::OnChar();
		break;
	}
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
	if (m_imageViewer->GetInput() != NULL) {
		picked[GetSliceOrientation()] = GetOrigin()[GetSliceOrientation()] +
			GetSlice() * GetSpacing()[GetSliceOrientation()];
		for (int i = 0; i < 3; i++)
		{
			m_index[i] = (picked[i] - GetOrigin()[i]) / GetSpacing()[i];
		}
	}

}


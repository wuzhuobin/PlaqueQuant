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
#include <vtkCamera.h>
#include <vtkAbstractPicker.h>


vtkStandardNewMacro(InteractorStyleNavigation);

InteractorStyleNavigation::InteractorStyleNavigation()
	:AbstractInteractorStyleImage()
{
}

InteractorStyleNavigation::~InteractorStyleNavigation()
{
}

void InteractorStyleNavigation::OnMouseMove()
{
	//if (m_rightFunctioning) {
	//	this->SynchronizedZooming();
	//}
	if (m_leftFunctioning) {
		this->CalculateIndex();
		return;
	}
	AbstractInteractorStyleImage::OnMouseMove();

	if (m_rightFunctioning) {
		SynchronizedZooming();
	}
}


void InteractorStyleNavigation::OnLeftButtonUp()
{
	this->CalculateIndex();
	AbstractInteractorStyleImage::OnLeftButtonUp();
}



void InteractorStyleNavigation::SynchronizedZooming()
{
	double scale = m_imageViewer->GetRenderer()->GetActiveCamera()->GetParallelScale();
	
	for (std::list<MyImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
			(*it)->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
			(*it)->Render();
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
	double index[3];
	if (m_imageViewer->GetInput() != NULL) {
		picked[m_orientation] = m_origin[m_orientation] + m_slice * m_spacing[m_orientation];
		for (int i = 0; i < 3; i++)
		{
			index[i] = (picked[i] - m_origin[i]) / m_spacing[i];
		}
		for (std::list<MyImageViewer*>::iterator it = m_synchronalViewers.begin();
			it != m_synchronalViewers.end(); ++it) {
			(*it)->SetSlice(index[(*it)->GetSliceOrientation()]);
		}
		m_imageViewer->SetFocalPointWithImageCoordinate(index[0] + 0.5, 
			index[1] + 0.5, index[2] + 0.5);
	}

}


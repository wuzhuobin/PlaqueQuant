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
#include "MainWindow.h"

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
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	MyImageViewer* viewer = NULL;
	double scale = m_imageViewer->GetRenderer()->GetActiveCamera()->GetParallelScale();
	
	for (int i = 0; i < 3; ++i) {
		viewer = mainWnd->GetMyImageViewer(i);
		if (viewer != NULL) {
			viewer->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
			viewer->Render();
		}
	}


}

void InteractorStyleNavigation::CalculateIndex()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
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
		//m_imageViewer->SetFocalPointWithWorldCoordinate(picked[0], picked[1], picked[2]);
		//m_imageViewer->SetFocalPointWithImageCoordinate((int)(index[0] + 0.5), (int)(index[1] + 0.5), (int)(index[2] + 0.5));
		mainWnd->slotChangeSlice((int)(index[0] + 0.5), (int)(index[1] + 0.5), (int)(index[2] + 0.5));
	}

}


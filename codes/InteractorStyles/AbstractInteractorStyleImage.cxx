/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract interactor class used in medical viewers.
This class contains methods of image related processes, including changing slice position,
zoomming, dragging...etc.


Wong Matthew Lun
Copyright (C) 2016
*/

#include "AbstractInteractorStyleImage.h"

#include <vtkCamera.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>


vtkStandardNewMacro(AbstractInteractorStyleImage);
std::list<vtkImageViewer2*> AbstractInteractorStyleImage::m_synchronalViewers;

AbstractInteractorStyleImage::AbstractInteractorStyleImage() : vtkInteractorStyleImage()
{
	//m_rightFunctioning = false;
	//m_leftFunctioning = false;
	//m_middleFunctioning = false;
}

AbstractInteractorStyleImage::~AbstractInteractorStyleImage()
{
}

void AbstractInteractorStyleImage::SetImageViewer(vtkImageViewer2 * imageViewer)
{
	this->m_imageViewer = imageViewer;
	AddSynchronalViewer(imageViewer);
}

void AbstractInteractorStyleImage::AddSynchronalViewer(vtkImageViewer2 * imageViewer)
{
	if (std::find(m_synchronalViewers.cbegin(), m_synchronalViewers.cend(), imageViewer)
		== m_synchronalViewers.cend()) {
		m_synchronalViewers.push_back(imageViewer);
	}
}

void AbstractInteractorStyleImage::EnableSynchronalZooming(bool flag)
{
	this->m_synchronalZoomingFlag = flag;
	SynchronalZooming();
}

void AbstractInteractorStyleImage::SynchronalZooming()
{
	if (!m_synchronalZoomingFlag)
		return;
	double scale = m_imageViewer->GetRenderer()->GetActiveCamera()->GetParallelScale();
	for (std::list<vtkImageViewer2*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		(*it)->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
		(*it)->Render();
	}
}

//vtkActor * AbstractInteractorStyleImage::PickActor(int x, int y)
//{
//	vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
//	if (this->m_imageViewer->GetRenderer()) {
//		picker->Pick(x, y, 0, this->m_imageViewer->GetRenderer());
//	}
//	if (picker->GetActor()) {
//		return picker->GetActor();
//	}
//	else {
//		return nullptr;
//	}
//}

int AbstractInteractorStyleImage::GetSlice()
{
	return m_imageViewer->GetSlice();
}

int AbstractInteractorStyleImage::GetMinSlice()
{
	return m_imageViewer->GetSliceMin();
}

int AbstractInteractorStyleImage::GetMaxSlice()
{
	return m_imageViewer->GetSliceMax();
}

int AbstractInteractorStyleImage::GetSliceOrientation()
{
	return m_imageViewer->GetSliceOrientation();
}

double * AbstractInteractorStyleImage::GetOrigin()
{
	return m_imageViewer->GetInput()->GetOrigin();
}

double * AbstractInteractorStyleImage::GetSpacing()
{
	return m_imageViewer->GetInput()->GetSpacing();
}

int * AbstractInteractorStyleImage::GetExtent()
{
	return m_imageViewer->GetInput()->GetExtent();
}

void AbstractInteractorStyleImage::OnMouseWheelForward()
{
	vtkInteractorStyleImage::OnMouseWheelForward();
}

void AbstractInteractorStyleImage::OnMouseWheelBackward()
{
	vtkInteractorStyleImage::OnMouseWheelBackward();
}

void AbstractInteractorStyleImage::OnLeftButtonDown()
{
	AbstractInteractorStyle::OnLeftButtonDown();
	//vtkInteractorStyleImage::OnLeftButtonDown();
}

void AbstractInteractorStyleImage::OnLeftButtonUp()
{
	AbstractInteractorStyle::OnLeftButtonUp();
	//vtkInteractorStyleImage::OnLeftButtonUp();
}

void AbstractInteractorStyleImage::OnRightButtonDown()
{
	AbstractInteractorStyle::OnRightButtonDown();
	vtkInteractorStyleImage::OnRightButtonDown();
}

void AbstractInteractorStyleImage::OnRightButtonUp()
{
	AbstractInteractorStyle::OnRightButtonUp();
	vtkInteractorStyleImage::OnRightButtonUp();
}

void AbstractInteractorStyleImage::OnMiddleButtonDown()
{
	AbstractInteractorStyle::OnMiddleButtonDown();
	vtkInteractorStyleImage::OnMiddleButtonDown();
}

void AbstractInteractorStyleImage::OnMiddleButtonUp()
{
	AbstractInteractorStyle::OnMiddleButtonUp();
	vtkInteractorStyleImage::OnMiddleButtonUp();
}

void AbstractInteractorStyleImage::OnMouseMove()
{
	vtkInteractorStyleImage::OnMouseMove();
}

void AbstractInteractorStyleImage::OnChar()
{
	vtkInteractorStyleImage::OnChar();
}

void AbstractInteractorStyleImage::OnKeyPress()
{
	vtkInteractorStyleImage::OnKeyPress();
}


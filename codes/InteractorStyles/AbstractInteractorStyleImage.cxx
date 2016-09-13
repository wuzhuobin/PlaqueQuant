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

#include <vtkPropPicker.h>

vtkStandardNewMacro(AbstractInteractorStyleImage);

AbstractInteractorStyleImage::AbstractInteractorStyleImage() : vtkInteractorStyleImage()
{
	m_rightFunctioning = false;
	m_leftFunctioning = false;
	m_middleFunctioning = false;
}

AbstractInteractorStyleImage::~AbstractInteractorStyleImage()
{
}

void AbstractInteractorStyleImage::SetImageViewer(MyImageViewer * imageViewer)
{
	this->m_imageViewer = imageViewer;
	m_minSlice = imageViewer->GetSliceMin();
	m_maxSlice = imageViewer->GetSliceMax();
	m_slice = imageViewer->GetSlice();
	m_orientation = imageViewer->GetSliceOrientation();

	imageViewer->GetInput()->GetExtent(m_extent);
	imageViewer->GetInput()->GetSpacing(m_spacing);
	imageViewer->GetInput()->GetOrigin(m_origin);
	AddSynchronalViewer(imageViewer);

}

void AbstractInteractorStyleImage::AddSynchronalViewer(MyImageViewer * imageViewer)
{
	m_synchronalViewers.push_back( imageViewer);
}

void AbstractInteractorStyleImage::SetSynchronalViewers(std::list<MyImageViewer*> synchronalViewers)
{
	m_synchronalViewers = synchronalViewers;
}

//void AbstractInteractorStyleImage::SetSliceSpinBox(QSpinBox * x, QSpinBox * y, QSpinBox * z)
//{
//	m_sliceSplinBox[0] = x;
//	m_sliceSplinBox[1] = y;
//	m_sliceSplinBox[2] = z;
//}

//void AbstractInteractorStyleImage::SetOrientation(int m_orientation)
//{
//	this->m_orientation = m_orientation;
//	this->imageViewer->SetSliceOrientation(m_orientation);
//}

//int AbstractInteractorStyleImage::GetOrientation()
//{
//	return m_orientation;
//}

vtkActor * AbstractInteractorStyleImage::PickActor(int x, int y)
{
	vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
	if (this->m_imageViewer->GetRenderer()) {
		picker->Pick(x, y, 0, this->m_imageViewer->GetRenderer());
	}
	if (picker->GetActor()) {
		return picker->GetActor();
	}
	else {
		return nullptr;
	}
}

void AbstractInteractorStyleImage::SetCurrentSlice(int slice)
{
	m_slice = slice;
	for (std::list<MyImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		int ijk[3];
		(*it)->GetFocalPointWithImageCoordinate(ijk);
		ijk[m_orientation] = m_slice;
		(*it)->SetFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);
		if ((*it)->GetSliceOrientation() == m_orientation) {
			(*it)->SetSlice(m_slice);
		}
	}
}

void AbstractInteractorStyleImage::OnMouseWheelForward()
{
	this->MoveSliceForward();
	//vtkInteractorStyleImage::OnMouseWheelForward();
}

void AbstractInteractorStyleImage::OnMouseWheelBackward()
{
	this->MoveSliceBackward();
	//vtkInteractorStyleImage::OnMouseWheelBackward();
}

void AbstractInteractorStyleImage::OnLeftButtonDown()
{
	m_leftFunctioning = true;
	vtkInteractorStyleImage::OnLeftButtonDown();
}

void AbstractInteractorStyleImage::OnLeftButtonUp()
{
	m_leftFunctioning = false;
	vtkInteractorStyleImage::OnLeftButtonUp();
}

void AbstractInteractorStyleImage::OnRightButtonDown()
{
	m_rightFunctioning = true;
	vtkInteractorStyleImage::OnRightButtonDown();
}

void AbstractInteractorStyleImage::OnRightButtonUp()
{
	m_rightFunctioning = false;
	vtkInteractorStyleImage::OnRightButtonUp();
}

void AbstractInteractorStyleImage::OnMiddleButtonDown()
{
	m_middleFunctioning = true;
	vtkInteractorStyleImage::OnMiddleButtonDown();
}

void AbstractInteractorStyleImage::OnMiddleButtonUp()
{
	m_middleFunctioning = false;
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

void AbstractInteractorStyleImage::OnKeyPressed()
{
	vtkInteractorStyleImage::OnKeyPress();
}

void AbstractInteractorStyleImage::MoveSliceForward()
{
	if (m_slice < m_maxSlice)
	{
		m_slice += 1;
		SetCurrentSlice(m_slice);
	}
}

void AbstractInteractorStyleImage::MoveSliceBackward()
{
	if (m_slice > m_minSlice)
	{
		m_slice -= 1;
		SetCurrentSlice(m_slice);
	}
}


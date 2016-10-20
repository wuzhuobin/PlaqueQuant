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
#include <vtkRenderWindowInteractor.h>

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
	//m_slice = imageViewer->GetSlice();
	//m_minSlice = imageViewer->GetSliceMin();
	//m_maxSlice = imageViewer->GetSliceMax();
	//m_orientation = imageViewer->GetSliceOrientation();

	//imageViewer->GetInput()->GetExtent(m_extent);
	//imageViewer->GetInput()->GetSpacing(m_spacing);
	//imageViewer->GetInput()->GetOrigin(m_origin);
	AddSynchronalViewer(imageViewer);

	//SetCurrentSlice((m_maxSlice + m_minSlice) * 0.5);

}

void AbstractInteractorStyleImage::AddSynchronalViewer(MyImageViewer * imageViewer)
{
	if (std::find(m_synchronalViewers.cbegin(), m_synchronalViewers.cend(), imageViewer)
		== m_synchronalViewers.cend()) {
		m_synchronalViewers.push_back(imageViewer);
	}
}

void AbstractInteractorStyleImage::SetSynchronalViewers(std::list<MyImageViewer*> synchronalViewers)
{
	m_synchronalViewers = synchronalViewers;
}

void AbstractInteractorStyleImage::SynchronizedZooming()
{
	double scale = m_imageViewer->GetRenderer()->GetActiveCamera()->GetParallelScale();

	for (std::list<MyImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		(*it)->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
		(*it)->Render();
	}
}

//void AbstractInteractorStyleImage::SetSliceSpinBox(QSpinBox * x, QSpinBox * y, QSpinBox * z)
//{
//	GetSlice()SplinBox[0] = x;
//	GetSlice()SplinBox[1] = y;
//	GetSlice()SplinBox[2] = z;
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

void AbstractInteractorStyleImage::SetCurrentSlice(int slice)
{
	int ijk[3];
	m_imageViewer->GetFocalPointWithImageCoordinate(ijk);
	ijk[GetSliceOrientation()] = slice;
	SetCurrentFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);
}

void AbstractInteractorStyleImage::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	for (std::list<MyImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		(*it)->SetFocalPointWithImageCoordinate(i, j, k);
		(*it)->Render();
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

void AbstractInteractorStyleImage::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	int coordinate[3];
	m_imageViewer->GetFocalPointWithImageCoordinate(coordinate);

	// quite ugly here
	if (key == "Prior") {
		MoveSliceForward();
	}
	else if (key == "Next") {
		MoveSliceBackward();
	}
	else if (key == "Up") {
		switch (GetSliceOrientation())
		{
		case vtkImageViewer2::SLICE_ORIENTATION_YZ:
			SetCurrentFocalPointWithImageCoordinate(
				coordinate[0], coordinate[1], ++coordinate[2]);
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XZ:
			SetCurrentFocalPointWithImageCoordinate(
				coordinate[0], coordinate[1], ++coordinate[2]);
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XY:
			SetCurrentFocalPointWithImageCoordinate(
				coordinate[0], --coordinate[1], coordinate[2]);
			break;
		}
	}
	else if (key == "Down") {
		switch (GetSliceOrientation())
		{
		case vtkImageViewer2::SLICE_ORIENTATION_YZ:
			SetCurrentFocalPointWithImageCoordinate(
				coordinate[0], coordinate[1], --coordinate[2]);
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XZ:
			SetCurrentFocalPointWithImageCoordinate(
				coordinate[0], coordinate[1], --coordinate[2]);
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XY:
			SetCurrentFocalPointWithImageCoordinate(
				coordinate[0], ++coordinate[1], coordinate[2]);
			break;
		}
	}
	else if (key == "Left") {
		switch (GetSliceOrientation())
		{
		case vtkImageViewer2::SLICE_ORIENTATION_YZ:
			SetCurrentFocalPointWithImageCoordinate(
				coordinate[0], --coordinate[1], coordinate[2]);
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XZ:
			SetCurrentFocalPointWithImageCoordinate(
				--coordinate[0], coordinate[1], coordinate[2]);
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XY:
			SetCurrentFocalPointWithImageCoordinate(
				--coordinate[0], coordinate[1], coordinate[2]);
			break;
		}
	}
	else if (key == "Right") {
		switch (GetSliceOrientation())
		{
		case vtkImageViewer2::SLICE_ORIENTATION_YZ:
			SetCurrentFocalPointWithImageCoordinate(
				coordinate[0], ++coordinate[1], coordinate[2]);
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XZ:
			SetCurrentFocalPointWithImageCoordinate(
				++coordinate[0], coordinate[1], coordinate[2]);
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XY:
			SetCurrentFocalPointWithImageCoordinate(
				++coordinate[0], coordinate[1], coordinate[2]);
			break;
		}
	}
	//vtkInteractorStyleImage::OnKeyPress();
}

void AbstractInteractorStyleImage::MoveSliceForward()
{
	if (GetSlice() < GetMaxSlice())
	{
		SetCurrentSlice(GetSlice() + 1);
	}
}

void AbstractInteractorStyleImage::MoveSliceBackward()
{
	if (GetSlice() > GetMinSlice())
	{
		SetCurrentSlice(GetSlice() - 1);
	}
}


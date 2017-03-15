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

#include "AbstractNavigation.h"

#include <vtkCamera.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(AbstractNavigation);
std::list<MyImageViewer*> AbstractNavigation::m_synchronalViewers;


AbstractNavigation::AbstractNavigation() 
{
}

AbstractNavigation::~AbstractNavigation()
{
}

//void AbstractNavigation::SynchronalZooming()
//{
//	double scale = m_imageViewer->GetRenderer()->GetActiveCamera()->GetParallelScale();
//	for (std::list<vtkImageViewer2*>::iterator it = m_synchronalViewers.begin();
//		it != m_synchronalViewers.end(); ++it) {
//		(*it)->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
//		(*it)->Render();
//	}
//}

void AbstractNavigation::SetImageViewer(MyImageViewer * imageViewer)
{
	m_imageViewer = imageViewer;
	AddSynchronalViewer(imageViewer);
	AbstractInteractorStyleImage::SetImageViewer(imageViewer);
}

void AbstractNavigation::AddSynchronalViewer(MyImageViewer * imageViewer)
{
	if (std::find(m_synchronalViewers.cbegin(), m_synchronalViewers.cend(), imageViewer)
		== m_synchronalViewers.cend()) {
		m_synchronalViewers.push_back(imageViewer);
	}
	AbstractInteractorStyleImage::AddSynchronalViewer(imageViewer);
}

//vtkActor * AbstractNavigation::PickActor(int x, int y)
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

void AbstractNavigation::SetCurrentSlice(int slice)
{
	int ijk[3];
	MyImageViewer* viewer = MyImageViewer::SafeDownCast(m_imageViewer);
	if (viewer != nullptr) {
		viewer->GetFocalPointWithImageCoordinate(ijk);
		ijk[GetSliceOrientation()] = slice;
		SetCurrentFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);
	}
	else {
		AbstractInteractorStyleImage::SetCurrentSlice(slice);
	}
}

void AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(int * ijk)
{
	SetCurrentFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);
}

void AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	int ijk[3];
	m_imageViewer->GetFocalPointWithImageCoordinate(ijk);
	if (i != ijk[0] || j != ijk[1] || k != ijk[2]) {
		STYLE_DOWN_CAST_CONSTITERATOR(AbstractNavigation,
			SetCurrentFocalPointWithImageCoordinateByViewer(i, j, k));
	}
	
}

void AbstractNavigation::OnMouseWheelForward()
{
	this->MoveSliceForward();
	//vtkInteractorStyleImage::OnMouseWheelForward();
}

void AbstractNavigation::OnMouseWheelBackward()
{
	this->MoveSliceBackward();
	//vtkInteractorStyleImage::OnMouseWheelBackward();
}

void AbstractNavigation::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	int coordinate[3];
	MyImageViewer* viewer = MyImageViewer::SafeDownCast(m_imageViewer);
	if (viewer != nullptr) {
		viewer->GetFocalPointWithImageCoordinate(coordinate);

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
	}
	else {
		vtkInteractorStyleImage::OnKeyPress();
	}


}

void AbstractNavigation::MoveSliceForward()
{
	if (GetSlice() < GetMaxSlice())
	{
		SetCurrentSlice(GetSlice() + 1);
	}
}

void AbstractNavigation::MoveSliceBackward()
{
	if (GetSlice() > 0)
	{
		cout << "GetSlice: " << GetSlice() << " SetSlice: " << GetSlice() - 1;

		SetCurrentSlice(GetSlice() - 1);
		cout << " New Slice: " << GetSlice() << endl;
	}
}

void AbstractNavigation::SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k)
{
	int ijk[3];
	m_imageViewer->GetFocalPointWithImageCoordinate(ijk);
	if (i != ijk[0] || j != ijk[1] || k != ijk[2]) {
		m_imageViewer->SetFocalPointWithImageCoordinate(i, j, k);
	}
}


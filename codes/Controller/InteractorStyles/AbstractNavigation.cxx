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

#include "ImageViewer.h"

#include <vtkCamera.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(AbstractNavigation);

AbstractNavigation::AbstractNavigation() 
{
}

AbstractNavigation::~AbstractNavigation()
{
}

ImageViewer* AbstractNavigation::GetImageViewer()
{
	return ImageViewer::SafeDownCast(m_viewer);
}

void AbstractNavigation::SetCurrentSlice(int slice)
{
	int ijk[3];
	if (GetImageViewer()) {
		GetImageViewer()->GetFocalPointWithImageCoordinate(ijk);
		ijk[GetSliceOrientation()] = slice;
		SetCurrentFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);
	}
	else if(GetVtkImageViewer2()) {
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
	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk);
	if (i != ijk[0] || j != ijk[1] || k != ijk[2]) {
		SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(AbstractNavigation,
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

	// quite ugly here
	if (key == "Prior") {
		MoveSliceForward();
	}
	else if (key == "Next") {
		MoveSliceBackward();
	}

	if (GetImageViewer()) {
		int coordinate[3];

		GetImageViewer()->GetFocalPointWithImageCoordinate(coordinate);

		if (key == "Up") {
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
		AbstractInteractorStyleImage::OnKeyPress();
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
	if (GetSlice() > GetMinSlice())
	{
		SetCurrentSlice(GetSlice() - 1);
	}
}

void AbstractNavigation::SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k)
{
	int ijk[3];
	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk);
	if (i != ijk[0] || j != ijk[1] || k != ijk[2]) {
		GetImageViewer()->SetFocalPointWithImageCoordinate(i, j, k);
	}
}


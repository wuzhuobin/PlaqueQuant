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
#include <vtkImageViewer2.h>

#include <vtkWorldPointPicker.h>

//vtkStandardNewMacro(AbstractInteractorStyleImage);
std::list<AbstractInteractorStyleImage*> AbstractInteractorStyleImage::m_imageStyles;

AbstractInteractorStyleImage::AbstractInteractorStyleImage() : vtkInteractorStyleTrackballCamera()
{
	m_imageStyles.push_back(this);
}

AbstractInteractorStyleImage::~AbstractInteractorStyleImage()
{
	m_imageStyles.remove(this);
}

vtkImageViewer2 * AbstractInteractorStyleImage::GetVtkImageViewer2()
{
	return vtkImageViewer2::SafeDownCast(this->m_viewer);
}

void AbstractInteractorStyleImage::SetCurrentSlice(int slice)
{
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(AbstractInteractorStyleImage, GetVtkImageViewer2()->SetSlice(slice));

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
	double scale = GetVtkImageViewer2()->GetRenderer()->GetActiveCamera()->GetParallelScale();
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(AbstractInteractorStyleImage, GetVtkImageViewer2()->GetRenderer()->GetActiveCamera()->SetParallelScale(scale));
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(AbstractInteractorStyleImage, GetVtkImageViewer2()->Render());

}

int AbstractInteractorStyleImage::GetWindow()
{
	return GetVtkImageViewer2()->GetColorWindow();
}

int AbstractInteractorStyleImage::GetLevel()
{
	return GetVtkImageViewer2()->GetColorLevel();
}

int AbstractInteractorStyleImage::GetSlice()
{
	return GetVtkImageViewer2()->GetSlice();
}

int AbstractInteractorStyleImage::GetMinSlice()
{
	return GetVtkImageViewer2()->GetSliceMin();
}

int AbstractInteractorStyleImage::GetMaxSlice()
{
	return GetVtkImageViewer2()->GetSliceMax();
}

int AbstractInteractorStyleImage::GetSliceOrientation()
{
	return GetVtkImageViewer2()->GetSliceOrientation();
}

double * AbstractInteractorStyleImage::GetOrigin()
{
	return GetVtkImageViewer2()->GetInput()->GetOrigin();
}

double * AbstractInteractorStyleImage::GetSpacing()
{
	return GetVtkImageViewer2()->GetInput()->GetSpacing();
}

int * AbstractInteractorStyleImage::GetExtent()
{
	return GetVtkImageViewer2()->GetInput()->GetExtent();
}

void AbstractInteractorStyleImage::OnMouseWheelForward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
}

void AbstractInteractorStyleImage::OnMouseWheelBackward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
}

void AbstractInteractorStyleImage::OnLeftButtonDown()
{
	this->m_numberOfLeftClicks++;

	// Reset numClicks - If mouse moved further than resetPixelDistance
	if (CheckMoveDistance())
	{
		this->m_numberOfLeftClicks = 1;
	}
	if (this->m_numberOfLeftClicks == 2)
	{
		OnLeftDoubleClick();
		this->m_numberOfLeftClicks = 0;
	}
	AbstractInteractorStyle::OnLeftButtonDown();
	//vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void AbstractInteractorStyleImage::OnLeftButtonUp()
{
	AbstractInteractorStyle::OnLeftButtonUp();
	//vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void AbstractInteractorStyleImage::OnLeftDoubleClick()
{
}

void AbstractInteractorStyleImage::OnRightButtonDown()
{
	this->m_numberOfRightClicks++;

	// Reset numClicks - If mouse moved further than resetPixelDistance
	if (CheckMoveDistance())
	{
		this->m_numberOfRightClicks = 1;
	}


	if (this->m_numberOfRightClicks == 2)
	{
		OnRightDoubleClick();
		this->m_numberOfRightClicks = 0;
	}

	AbstractInteractorStyle::OnRightButtonDown();
	vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}

void AbstractInteractorStyleImage::OnRightButtonUp()
{
	AbstractInteractorStyle::OnRightButtonUp();
	vtkInteractorStyleTrackballCamera::OnRightButtonUp();
}

void AbstractInteractorStyleImage::OnRightDoubleClick()
{

}

void AbstractInteractorStyleImage::OnMiddleButtonDown()
{
	this->m_numberOfMiddleClicks++;

	// Reset numClicks - If mouse moved further than resetPixelDistance
	if (CheckMoveDistance())
	{
		this->m_numberOfMiddleClicks = 1;
	}


	if (this->m_numberOfMiddleClicks == 2)
	{
		OnMiddleDoubleClick();
		this->m_numberOfMiddleClicks = 0;
	}
	AbstractInteractorStyle::OnMiddleButtonDown();
	vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}

void AbstractInteractorStyleImage::OnMiddleButtonUp()
{
	AbstractInteractorStyle::OnMiddleButtonUp();
	vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}

void AbstractInteractorStyleImage::OnMiddleDoubleClick()
{
}

void AbstractInteractorStyleImage::OnMouseMove()
{
	vtkInteractorStyleTrackballCamera::OnMouseMove();
	if (m_rightFunctioning) {
		SynchronalZooming();
	}
}

void AbstractInteractorStyleImage::OnChar()
{
	vtkInteractorStyleTrackballCamera::OnChar();
}

void AbstractInteractorStyleImage::OnKeyPress()
{
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}

bool AbstractInteractorStyleImage::CheckMoveDistance()
{
	int pickPosition[2];
	this->GetInteractor()->GetEventPosition(pickPosition);

	int xdist = pickPosition[0] - this->PreviousPosition[0];
	int ydist = pickPosition[1] - this->PreviousPosition[1];

	this->PreviousPosition[0] = pickPosition[0];
	this->PreviousPosition[1] = pickPosition[1];

	int moveDistance = (int)sqrt((double)(xdist*xdist + ydist*ydist));

	return (moveDistance <= RESET_PIXEL_DISTANCE);
}
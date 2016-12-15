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

void AbstractInteractorStyleImage::SetCurrentSlice(int slice)
{
	VIEWER_CONSTITERATOR(SetSlice(slice));
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
	VIEWER_CONSTITERATOR(GetRenderer()->GetActiveCamera()->SetParallelScale(scale));
	VIEWER_CONSTITERATOR(Render());

	//for (std::list<vtkImageViewer2*>::iterator it = m_synchronalViewers.begin();
	//	it != m_synchronalViewers.end(); ++it) {
	//	(*it)->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
	//	(*it)->Render();
	//}
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
	//vtkInteractorStyleImage::OnLeftButtonDown();
}

void AbstractInteractorStyleImage::OnLeftButtonUp()
{
	AbstractInteractorStyle::OnLeftButtonUp();
	//vtkInteractorStyleImage::OnLeftButtonUp();
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
	vtkInteractorStyleImage::OnRightButtonDown();
}

void AbstractInteractorStyleImage::OnRightButtonUp()
{
	AbstractInteractorStyle::OnRightButtonUp();
	vtkInteractorStyleImage::OnRightButtonUp();
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
	vtkInteractorStyleImage::OnMiddleButtonDown();
}

void AbstractInteractorStyleImage::OnMiddleButtonUp()
{
	AbstractInteractorStyle::OnMiddleButtonUp();
	vtkInteractorStyleImage::OnMiddleButtonUp();
}

void AbstractInteractorStyleImage::OnMiddleDoubleClick()
{
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
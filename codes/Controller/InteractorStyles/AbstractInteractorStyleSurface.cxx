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

#include "AbstractInteractorStyleSurface.h"

#include <vtkCamera.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>

#include "SurfaceViewer.h"


//vtkStandardNewMacro(AbstractInteractorStyleSurface);
std::list<AbstractInteractorStyleSurface*> AbstractInteractorStyleSurface::m_surfaceStyles;

SurfaceViewer * AbstractInteractorStyleSurface::GetSurfaceViewer()
{
	return reinterpret_cast<SurfaceViewer*>(m_viewer);
}

AbstractInteractorStyleSurface::AbstractInteractorStyleSurface() : vtkInteractorStyleTrackballCamera()
{
	m_surfaceStyles.push_back(this);
}

AbstractInteractorStyleSurface::~AbstractInteractorStyleSurface()
{
	m_surfaceStyles.remove(this);
}

double * AbstractInteractorStyleSurface::GetOrigin()
{
	return GetSurfaceViewer()->GetInput()->GetOrigin();
}

double * AbstractInteractorStyleSurface::GetSpacing()
{
	return GetSurfaceViewer()->GetInput()->GetSpacing();
}

int * AbstractInteractorStyleSurface::GetExtent()
{
	return GetSurfaceViewer()->GetInput()->GetExtent();
}

void AbstractInteractorStyleSurface::OnMouseWheelForward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
}

void AbstractInteractorStyleSurface::OnMouseWheelBackward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
}

void AbstractInteractorStyleSurface::OnLeftButtonDown()
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
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void AbstractInteractorStyleSurface::OnLeftButtonUp()
{
	AbstractInteractorStyle::OnLeftButtonUp();
	vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void AbstractInteractorStyleSurface::OnLeftDoubleClick()
{
}

void AbstractInteractorStyleSurface::OnRightButtonDown()
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

void AbstractInteractorStyleSurface::OnRightButtonUp()
{
	AbstractInteractorStyle::OnRightButtonUp();
	vtkInteractorStyleTrackballCamera::OnRightButtonUp();
}

void AbstractInteractorStyleSurface::OnRightDoubleClick()
{
}

void AbstractInteractorStyleSurface::OnMiddleButtonDown()
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

void AbstractInteractorStyleSurface::OnMiddleButtonUp()
{
	AbstractInteractorStyle::OnMiddleButtonUp();
	vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}

void AbstractInteractorStyleSurface::OnMiddleDoubleClick()
{
}

void AbstractInteractorStyleSurface::OnMouseMove()
{
	vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void AbstractInteractorStyleSurface::OnChar()
{
	vtkInteractorStyleTrackballCamera::OnChar();
}

void AbstractInteractorStyleSurface::OnKeyPress()
{
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}

bool AbstractInteractorStyleSurface::CheckMoveDistance()
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
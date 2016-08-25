/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract interactor class used in medical viewers.
This class is the ultimate parent of all interactor classes related to 3D.


Wong Matthew Lun
Copyright (C) 2016
*/

#include "AbstractInteractorStyle3D.h"
#include "MainWindow.h"

vtkStandardNewMacro(AbstractInteractorStyle3D);

AbstractInteractorStyle3D::AbstractInteractorStyle3D() : AbstractInteractorStyle(), vtkInteractorStyleTrackballCamera()
{
	double defVal[] = { 0,0,0 };
	memcpy(m_currentPos, defVal, sizeof(double) * 3);
	memcpy(m_previousPosition, defVal, sizeof(int) * 2);
}

AbstractInteractorStyle3D::~AbstractInteractorStyle3D()
{
}

double * AbstractInteractorStyle3D::GetCurrentPos()
{
	return m_currentPos;
}

int * AbstractInteractorStyle3D::GetPreviousPosition()
{
	return m_previousPosition;
}

void AbstractInteractorStyle3D::OnLeftButtonDown()
{
	AbstractInteractorStyle::OnLeftButtonDown();
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void AbstractInteractorStyle3D::OnLeftButtonUp()
{
	AbstractInteractorStyle::OnLeftButtonUp();
	vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void AbstractInteractorStyle3D::OnRightButtonDown()
{
	AbstractInteractorStyle::OnRightButtonDown();
	vtkInteractorStyleTrackballCamera::OnRightButtonDown();

}

void AbstractInteractorStyle3D::OnRightButtonUp()
{
	AbstractInteractorStyle::OnRightButtonUp();
	vtkInteractorStyleTrackballCamera::OnRightButtonUp();
}

void AbstractInteractorStyle3D::OnMiddleButtonDown()
{
	AbstractInteractorStyle::OnMiddleButtonDown();
	vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}

void AbstractInteractorStyle3D::OnMiddleButtonUp()
{
	AbstractInteractorStyle::OnMiddleButtonUp();
	vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}


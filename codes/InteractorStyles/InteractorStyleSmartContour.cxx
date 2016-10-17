/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor modified from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/
#include "InteractorStyleSmartContour.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkSeedRepresentation.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkProperty.h>

#include <vtkHandleWidget.h>

using namespace std;
vtkStandardNewMacro(InteractorStyleSmartContour);

//vtkSmartPointer<vtkPoints> InteractorStyleSmartContour::m_seeds = 
//vtkSmartPointer<vtkPoints>::New();

list<int*> InteractorStyleSmartContour::m_seeds;


void InteractorStyleSmartContour::SetSmartContourEnable(bool flag)
{
	if (flag) {
		// SetInteractor CANNOT place in the constructor
		m_seedWidget->SetInteractor(this->Interactor);
		vtkImageActorPointPlacer* pointPlacter =
			vtkImageActorPointPlacer::SafeDownCast(
				m_seedWidget->GetSeedRepresentation()->GetHandleRepresentation()->
				GetPointPlacer());
		if (pointPlacter != NULL) {
			pointPlacter->SetImageActor(m_imageViewer->GetImageActor());
		}
		m_seedWidget->EnabledOn();
		m_seedWidget->On();
	}
	else {
		m_seedWidget->EnabledOff();
		m_seedWidget->Off();
	}
}

InteractorStyleSmartContour::InteractorStyleSmartContour()
{
	m_seedWidget = vtkSmartPointer<vtkSeedWidget>::New();
	m_seedWidget->CreateDefaultRepresentation();
	m_seedWidget->GetSeedRepresentation()->SetHandleRepresentation(
		vtkSmartPointer<vtkPointHandleRepresentation3D>::New());
	m_seedWidget->GetSeedRepresentation()->GetHandleRepresentation()->SetPointPlacer(
		vtkSmartPointer<vtkImageActorPointPlacer>::New());
}

InteractorStyleSmartContour::~InteractorStyleSmartContour()
{
}

void InteractorStyleSmartContour::OnMouseMove()
{
	AbstractInteractorStyleImage::OnMouseMove();
}

void InteractorStyleSmartContour::OnLeftButtonDown()
{
	CalculateIndex();
	cout << m_world[0] << ' ' << m_world[1] << ' ' << m_world[2] << endl;
	double pos[3];
	m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(0, pos);
	cout << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
	m_imageViewer->Render();

	//Check if valid pick
	if (m_world[0] == 0 &&
		m_world[1] == 0 &&
		m_world[2] == 0) {
		return;
	}

	//AbstractInteractorStyleImage::OnLeftButtonDown();
}

void InteractorStyleSmartContour::OnLeftButtonUp()
{
	AbstractInteractorStyleImage::OnLeftButtonUp();
}

void InteractorStyleSmartContour::OnRightButtonDown()
{
	CalculateIndex();
	double e[2] = { 10 ,10 };

	//vtkHandleWidget *currentHandle = m_seedWidget->CreateNewHandle();
	//currentHandle->EnabledOn();

	// Create a circle
	vtkSmartPointer<vtkRegularPolygonSource> polygonSource =
		vtkSmartPointer<vtkRegularPolygonSource>::New();
	polygonSource->SetNumberOfSides(50);
	polygonSource->SetRadius(1);
	polygonSource->SetCenter(m_world[0], m_world[1], m_world[2]);
	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(polygonSource->GetOutputPort());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(1, 0, 0);

	m_imageViewer->GetAnnotationRenderer()->AddActor(actor);
	m_seedActors.push_back(actor);
	m_imageViewer->Render();
}

void InteractorStyleSmartContour::OnChar()
{
	char key = this->Interactor->GetKeyCode();
	cout << __func__ << ' '<< key << endl;
	AbstractInteractorStyleImage::OnChar();
	switch (key)
	{
	case'D':
		ClearAllSeeds();
		break;
	default:
		AbstractInteractorStyleImage::OnChar();
		break;
	}
}

void InteractorStyleSmartContour::OnKeyPress()
{
	string key = this->Interactor->GetKeySym();
	cout << __func__ << ' ' << key << endl;
	if (key == "D") {

	}
	else {
		AbstractInteractorStyleImage::OnKeyPress();
	}
}

void InteractorStyleSmartContour::CalculateIndex()
{
	//Pick
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		m_imageViewer->GetRenderer());

	this->GetInteractor()->GetPicker()->GetPickPosition(m_world);

	if (m_imageViewer->GetInput() != NULL) {
		m_world[GetSliceOrientation()] = GetOrigin()[GetSliceOrientation()] +
			GetSlice() * GetSpacing()[GetSliceOrientation()];
	}

}

void InteractorStyleSmartContour::UpdateSeedWidgetBefore()
{
	for (int i = 0; i < m_seedWidget->GetSeedRepresentation()->GetNumberOfSeeds(); ++i) {
		double worldPos[3];
		m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(i, worldPos);
		int* imagePos = new int[3];
		for (int pos = 0; pos < 3; ++pos) {
			imagePos[pos] = (worldPos[pos] - GetOrigin()[pos]) / GetSpacing()[pos];
		}
		if (find_if(m_seeds.cbegin(), m_seeds.cend(), [&imagePos](int* i) -> bool {
			return (i[0] == imagePos[0] && i[1] == imagePos[1] && i[2] == imagePos[2]);
		}) == m_seeds.cend()) {
			m_seeds.push_back(imagePos);
		}
		m_seedWidget->DeleteSeed(i);
	}
	
}

void InteractorStyleSmartContour::UpdateSeedWidgetAfter()
{
	
}

void InteractorStyleSmartContour::ClearAllSeeds()
{
	cout << __FUNCTION__ << endl;
	cout << m_seedActors.size() << endl;
	for (list<MyImageViewer*>::const_iterator cit1 = m_synchronalViewers.cbegin();
		cit1 != m_synchronalViewers.cend(); ++cit1) {
		for (std::list<vtkSmartPointer<vtkActor>>::const_iterator cit2 = m_seedActors.cbegin();
			cit2 != m_seedActors.cend(); ++cit2) {
			if ((*cit1)->GetAnnotationRenderer()->HasViewProp((*cit2))) {
				(*cit1)->GetAnnotationRenderer()->RemoveActor((*cit2));
			}
		}
		(*cit1)->Render();
	}
	m_seedActors.clear();
}


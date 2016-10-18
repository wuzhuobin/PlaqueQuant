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
#include <vtkProperty.h>

#include <vtkHandleWidget.h>

using namespace std;
vtkStandardNewMacro(InteractorStyleSmartContour);

list<int*> InteractorStyleSmartContour::m_seeds;

void InteractorStyleSmartContour::SetSmartContourEnable(bool flag)
{
	if (flag) {
		// SetInteractor CANNOT place in the constructor
		m_seedWidget->SetInteractor(this->Interactor);
		m_seedRep->GetImagePointPlacer()->SetImageActor(m_imageViewer->GetImageActor());
		m_seedWidget->EnabledOn();
		m_seedWidget->On();
		m_seedWidget->CompleteInteraction();
	}
	else {
		m_seedWidget->EnabledOff();
		m_seedWidget->Off();
	}
}

void InteractorStyleSmartContour::SetFocalSeed(int i)
{
	if (i >= m_seedRep->GetNumberOfSeeds() || i == m_focalSeed) {
		return;
	}
	m_focalSeed = i;

}

InteractorStyleSmartContour::InteractorStyleSmartContour()
{
	m_seedWidget = vtkSmartPointer<MySeedWidget>::New();
	m_seedRep = vtkSmartPointer<MySeedRepresentation>::New();
	m_seedWidget->SetRepresentation(m_seedRep);
}

InteractorStyleSmartContour::~InteractorStyleSmartContour()
{
}

void InteractorStyleSmartContour::OnMouseMove()
{
	if (!m_renderingFlag) {
		m_renderingFlag = true;
		UpdateSeedWidgetBefore();
	}

	AbstractInteractorStyleImage::OnMouseMove();
}

void InteractorStyleSmartContour::OnLeftButtonDown()
{
	//CalculateIndex();
	//cout << m_world[0] << ' ' << m_world[1] << ' ' << m_world[2] << endl;
	//double pos[3];
	//m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(0, pos);
	//cout << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
	//m_imageViewer->Render();

	////Check if valid pick
	//if (m_world[0] == 0 &&
	//	m_world[1] == 0 &&
	//	m_world[2] == 0) {
	//	return;
	//}

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

	m_imageViewer->Render();
}

void InteractorStyleSmartContour::OnChar()
{
	char key = this->Interactor->GetKeyCode();
	cout << __func__ << ' '<< key << endl;
	AbstractInteractorStyleImage::OnChar();
	switch (key)
	{
	case 'A':
		m_seedWidget->RestartInteraction();
		break;
	case 'P':
		for (int i = 0; i < m_seedRep->GetNumberOfSeeds(); ++i) {
			double pos[3];
			m_seedRep->GetSeedWorldPosition(i, pos);
			cout << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
		}
		break;
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

void InteractorStyleSmartContour::OnLeave()
{
	cout << __FUNCTION__ << endl;
	if (m_renderingFlag) {
		m_renderingFlag = false;
		UpdateSeedWidgetAfter();
	}
	AbstractInteractorStyleImage::OnLeave();
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
	while (!m_seeds.empty()) {

		int* imagePos = m_seeds.back();
		double worldPos[3];
		for (int pos = 0; pos < 3; ++pos) {
			worldPos[pos] = (imagePos[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
		}
		vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
		newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
		newSeed->EnabledOn();
		m_seeds.pop_back();
		delete imagePos;
	}
	UpdateFocalSeed();
	m_imageViewer->Render();
}

void InteractorStyleSmartContour::UpdateSeedWidgetAfter()
{
	for (int i = m_seedRep->GetNumberOfSeeds() - 1; i >= 0; --i) {
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
	m_imageViewer->Render();
}

void InteractorStyleSmartContour::UpdateFocalSeed()
{
	//if (m_seedRep->GetNumberOfSeeds() <= m_focalSeed) {
	//	return;
	//}
	//double color[3] = { 1,1,1 };
	//for (int i = 0; i < m_seedRep->GetNumberOfSeeds(); ++i) {
	//	if (i == m_focalSeed) {
	//		color[0] = 0; color[2] = 0;
	//		vtkPointHandleRepresentation3D::SafeDownCast(
	//			m_seedWidget->GetSeed(i)->GetHandleRepresentation()
	//		)->GetProperty()->SetColor(color);
	//	}
	//	else {
	//		color[0] = 1; color[2] = 1;
	//		vtkPointHandleRepresentation3D::SafeDownCast(
	//			m_seedWidget->GetSeed(i)->GetHandleRepresentation()
	//		)->GetProperty()->SetColor(color);
	//	}

	//}

}

void InteractorStyleSmartContour::ClearAllSeeds()
{
	for (int i = m_seedRep->GetNumberOfSeeds() - 1; i >= 0; --i) {
		m_seedWidget->DeleteSeed(i);
	}
	//cout << __FUNCTION__ << endl;
	//cout << m_seedActors.size() << endl;
	//for (list<MyImageViewer*>::const_iterator cit1 = m_synchronalViewers.cbegin();
	//	cit1 != m_synchronalViewers.cend(); ++cit1) {
	//	for (std::list<vtkSmartPointer<vtkActor>>::const_iterator cit2 = m_seedActors.cbegin();
	//		cit2 != m_seedActors.cend(); ++cit2) {
	//		if ((*cit1)->GetAnnotationRenderer()->HasViewProp((*cit2))) {
	//			(*cit1)->GetAnnotationRenderer()->RemoveActor((*cit2));
	//		}
	//	}
	//	(*cit1)->Render();
	//}
	//m_seedActors.clear();
}


/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract interactor class used in medical viewers.
This class is the ultimate parent of all interactor classes.


Wong Matthew Lun
Copyright (C) 2016
*/

#include "AbstractInteractorStyle.h"

#include <vtkCallbackCommand.h>

std::list<AbstractInteractorStyle*> AbstractInteractorStyle::m_abstractInteractorStyles;



void AbstractInteractorStyle::SetCustomEnabled(bool flag)
{
	m_customFlag = flag;
	if (flag && m_viewer) {
		m_viewer->AddObserver(vtkCommand::UpdateDataEvent, callback);
	}
	else if(m_viewer){
		m_viewer->RemoveObserver(callback);
	}
}

bool AbstractInteractorStyle::GetCustomEnabled()
{
	return m_customFlag;
}

void AbstractInteractorStyle::CustomEnabledOn()
{
	SetCustomEnabled(true);
}

void AbstractInteractorStyle::CustomEnabledOff()
{
	SetCustomEnabled(false);
}

void AbstractInteractorStyle::SetViewer(vtkObject * viewer)
{
	m_viewer = viewer;
}

vtkObject * AbstractInteractorStyle::GetViewer()
{
	return m_viewer;
}

AbstractInteractorStyle::AbstractInteractorStyle() {
	m_abstractInteractorStyles.push_back(this);

	callback = vtkCallbackCommand::New();
	callback->SetClientData(this);
	callback->SetCallback([](vtkObject* caller, unsigned long id, void* clientData, void*) {
		reinterpret_cast<AbstractInteractorStyle*>(clientData)->CustomEnabledOff();
		reinterpret_cast<AbstractInteractorStyle*>(clientData)->CustomEnabledOn();
	});
}

AbstractInteractorStyle::~AbstractInteractorStyle() {
	m_viewer = nullptr;
	m_abstractInteractorStyles.remove(this);
	callback->Delete();
	callback = nullptr;
}

void AbstractInteractorStyle::OnLeftButtonDown()
{
	m_leftFunctioning = true;
}

void AbstractInteractorStyle::OnRightButtonDown()
{
	m_rightFunctioning = true;
}

void AbstractInteractorStyle::OnMiddleButtonDown()
{
	m_middleFunctioning = true;
}

void AbstractInteractorStyle::OnLeftButtonUp()
{
	m_leftFunctioning = false;
}

void AbstractInteractorStyle::OnLeftDoubleClick()
{
}

void AbstractInteractorStyle::OnRightButtonUp()
{
	m_rightFunctioning = false;
}

void AbstractInteractorStyle::OnRightDoubleClick()
{
}

void AbstractInteractorStyle::OnMiddleButtonUp()
{
	m_middleFunctioning = false;
}

void AbstractInteractorStyle::OnMiddleDoubleClick()
{
}

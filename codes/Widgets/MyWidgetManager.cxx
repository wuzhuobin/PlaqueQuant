#include "MyWidgetManager.h"
#include "MyImageViewer.h"
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>


MyWidgetManager::MyWidgetManager(QObject* parent)
	:QObject(parent)
{
	this->m_roi = vtkSmartPointer<vtkROIWidget>::New();
	
	m_widgets += this->m_roi;
}

MyWidgetManager::~MyWidgetManager()
{
	m_widgets.clear();
}

void MyWidgetManager::SetInteractor(vtkRenderWindowInteractor * interactor)
{
}

/* Set interactor before enabling */
void MyWidgetManager::EnableROIWidget(
	vtkSmartPointer<MyImageViewer> viewer2D[3], 
	vtkRenderWindow* viewer3D, 
	double* placeBounds, 
	double* cursorPos)
{
	if (this->m_roi->GetEnabled())
		return;
	
	for (int i = 0; i < 3; i++)
	{
		this->m_roi->SetBorderWidgetsInteractor(i, viewer2D[i]->GetRenderWindow()->GetInteractor());
	}
	this->m_roi->SetInteractor(viewer3D->GetInteractor());
	this->m_roi->GetRepresentation()->PlaceWidget(placeBounds);
	this->m_roi->SetPositionPointer(cursorPos);
	this->m_roi->GetRepresentation()->SetPlaceFactor(1);
	this->m_roi->EnabledOn();
}

void MyWidgetManager::EnableROIWidget()
{
	this->m_roi->EnabledOn();
}

void MyWidgetManager::DisableROIWidget()
{
	this->m_roi->EnabledOff();
}

vtkROIWidget* MyWidgetManager::GetROIWidget()
{
	return this->m_roi;
}

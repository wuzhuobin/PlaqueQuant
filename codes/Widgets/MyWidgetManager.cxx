#include "MyWidgetManager.h"


MyWidgetManager::MyWidgetManager(QObject* parent)
	:QObject(parent)
{
	m_roiWidget = vtkSmartPointer<vtkROIWidget>::New();
	
	
	
	
	m_widgets += m_roiWidget;
}

MyWidgetManager::~MyWidgetManager()
{
	m_widgets.clear();
}

void MyWidgetManager::SetInteractor(vtkRenderWindowInteractor * interactor)
{
	m_interactor = interactor;
	for (int i = 0; i < m_widgets.size(); ++i) {
		m_widgets[i]->SetInteractor(interactor);
	}
}

void MyWidgetManager::EnableROIWidget()
{
	for (int i = 0; i < m_widgets.size(); ++i) {
		m_widgets[i]->EnabledOff();
	}
	m_roiWidget->EnabledOn();
}

vtkROIWidget * MyWidgetManager::GetROIWidget()
{
	return m_roiWidget;
}

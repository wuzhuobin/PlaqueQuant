#include "MyWidgetManager.h"


MyWidgetManager::MyWidgetManager(QObject* parent)
	:QObject(parent)
{
	m_roiBorderWidget = vtkSmartPointer<vtkROIBorderWidget>::New();
	
	
	
	
	m_widgets += m_roiBorderWidget;
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

void MyWidgetManager::EnableROIBorderWidget()
{
	for (int i = 0; i < m_widgets.size(); ++i) {
		m_widgets[i]->EnabledOff();
	}
	m_roiBorderWidget->EnabledOn();
}

vtkROIBorderWidget * MyWidgetManager::GetROIBorderWidget()
{
	return m_roiBorderWidget;
}

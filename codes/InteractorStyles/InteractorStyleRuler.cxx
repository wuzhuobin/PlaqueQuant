#include "InteractorStyleRuler.h"

#include <vtkAxisActor2D.h>
#include <vtkProperty2D.h>


vtkStandardNewMacro(InteractorStyleRuler);

void InteractorStyleRuler::SetDistanceWidgetEnabled(bool flag)
{
	if (flag)
	{
		this->m_distanceWidgetEnabledFlag = true;
		if (m_distanceWidget != NULL)
			m_distanceWidget->Delete();

		m_distanceWidget = vtkDistanceWidget::New();
		m_distanceWidget->SetInteractor(this->Interactor);
		//m_distanceWidget->SetPriority(this->GetPriority() + 0.1);

		//m_pointHandleRepresentation2D = vtkPointHandleRepresentation2D::New();
		//m_distanceRepresentation2D = vtkDistanceRepresentation2D::New();
		//m_distanceRepresentation2D->SetHandleRepresentation(m_pointHandleRepresentation2D);
		//m_distanceWidget->SetRepresentation(m_distanceRepresentation2D);
		m_distanceWidget->CreateDefaultRepresentation();
		m_distanceWidget->GetDistanceRepresentation()->SetLabelFormat("%-#11.2f mm");
		m_distanceWidget->GetDistanceRepresentation()->InstantiateHandleRepresentation();
		m_distanceWidget->On();
	}
	else
	{
		m_distanceWidget->Off();
		this->m_distanceWidgetEnabledFlag = false;

	}
}

InteractorStyleRuler::InteractorStyleRuler()
{
	//m_pointHandleRepresentation2D = vtkPointHandleRepresentation2D::New();
	//m_distanceRepresentation2D = vtkDistanceRepresentation2D::New();
	m_distanceWidget = vtkDistanceWidget::New();

}

InteractorStyleRuler::~InteractorStyleRuler()
{
	//if (this->m_pointHandleRepresentation2D != NULL) {
	//	this->m_pointHandleRepresentation2D->Delete();
	//	this->m_pointHandleRepresentation2D = NULL;
	//}
	//if (this->m_distanceRepresentation2D != NULL) {
	//	this->m_distanceRepresentation2D->Delete();
	//	this->m_distanceRepresentation2D = NULL;

	//}
	if (this->m_distanceWidget != NULL) {
		this->m_distanceWidget->Delete();
		this->m_distanceWidget = NULL;
	}
}

void InteractorStyleRuler::OnLeftButtonDown()
{
	if (this->m_distanceWidget->GetWidgetState() > -1) {
		return;
	}
	InteractorStyleNavigation::OnLeftButtonDown();
}

void InteractorStyleRuler::OnRightButtonDown()
{
	if (this->m_distanceWidget->GetWidgetState() > -1) {
		return;
	}
	InteractorStyleNavigation::OnRightButtonDown();

}

void InteractorStyleRuler::OnMouseWheelBackward()
{
	if (this->m_distanceWidget->GetWidgetState() > -1) {
		return;
	}
	InteractorStyleNavigation::OnMouseWheelBackward();
}

void InteractorStyleRuler::OnMouseWheelForward()
{
	if (this->m_distanceWidget->GetWidgetState() > -1) {
		return;
	}
	InteractorStyleNavigation::OnMouseWheelForward();
}

void InteractorStyleRuler::OnMiddleButtonDown()
{
	if (this->m_distanceWidget->GetWidgetState() > -1) {
		return;
	}
	InteractorStyleNavigation::OnMiddleButtonDown();
}


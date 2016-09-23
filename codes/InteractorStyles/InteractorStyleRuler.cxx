#include "InteractorStyleRuler.h"

#include <vtkAxisActor2D.h>
#include <vtkProperty2D.h>
//#include <vtkPointHandleRepresentation2D.h>
//#include <vtkDistanceRepresentation2D.h>



vtkStandardNewMacro(InteractorStyleRuler);

void InteractorStyleRuler::SetDistanceWidgetEnabled(bool flag)
{
	if (flag)
	{
		this->m_distanceWidgetEnabledFlag = true;
		if(m_distanceRepresentation3D != NULL)
			m_distanceRepresentation3D->Delete();
		m_distanceRepresentation3D = vtkDistanceRepresentation3D::New();
		if (m_distanceWidget != NULL)
			m_distanceWidget->Delete();
		m_distanceWidget = vtkDistanceWidget::New();
		m_distanceWidget->SetInteractor(this->Interactor);
		//m_distanceWidget->SetRepresentation(m_distanceRepresentation3D);
		m_distanceWidget->CreateDefaultRepresentation();
		//m_distanceWidget->SetPriority(this->GetPriority() + 0.1);

		//m_pointHandleRepresentation2D = vtkPointHandleRepresentation2D::New();

		//m_distanceRepresentation2D->SetHandleRepresentation(m_pointHandleRepresentation2D);
		//m_distanceWidget->SetRepresentation(m_distanceRepresentation2D);
		m_distanceWidget->GetDistanceRepresentation()->SetLabelFormat("%-#11.2f mm");
		//m_distanceWidget->GetDistanceRepresentation()->InstantiateHandleRepresentation();
		m_distanceWidget->On();
	}
	else
	{
		if (m_distanceWidget != NULL) {
			//m_distanceWidget->SetInteractor(NULL);
			m_distanceWidget->Off();
			m_distanceWidget->Delete();
			m_distanceWidget = NULL;
		}
		this->m_distanceWidgetEnabledFlag = false;

	}
}

InteractorStyleRuler::InteractorStyleRuler()
{
	//m_pointHandleRepresentation2D = vtkPointHandleRepresentation2D::New();
	//m_distanceRepresentation2D = vtkDistanceRepresentation2D::New();
	m_distanceRepresentation3D = NULL;
	m_distanceWidget = NULL;
}

InteractorStyleRuler::~InteractorStyleRuler()
{
	//if (this->m_pointHandleRepresentation2D != NULL) {
	//	this->m_pointHandleRepresentation2D->Delete();
	//	this->m_pointHandleRepresentation2D = NULL;
	//}
	if (this->m_distanceRepresentation3D != NULL) {
		this->m_distanceRepresentation3D->Delete();
		this->m_distanceRepresentation3D = NULL;

	}
	if (this->m_distanceWidget != NULL) {
		this->m_distanceWidget->Delete();
		this->m_distanceWidget = NULL;
	}
}


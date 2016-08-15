#include <vtkPointHandleRepresentation3D.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkAbstractPicker.h>
#include <vtkRendererCollection.h>

#include "Distance3DWidget.h"
#include "MainWindow.h"


void Distance3DWidget::Set3DRulerEnabled(bool b)
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	if (b)
	{
		//this->m_distanceWidget = vtkDistanceWidget::New();
		this->m_distanceWidget->SetInteractor(mainWnd->GetRenderWindow(4)->GetInteractor());
		this->m_distanceWidget->SetPriority(mainWnd->GetRenderWindow(4)->GetInteractor()->GetInteractorStyle()->GetPriority() + 0.1);
		
		vtkSmartPointer< vtkPointHandleRepresentation3D > rulerHandleRep3D = vtkSmartPointer< vtkPointHandleRepresentation3D >::New();
	
		vtkSmartPointer< vtkDistanceRepresentation2D > distanceRep3D = vtkSmartPointer< vtkDistanceRepresentation2D >::New();
		distanceRep3D->SetHandleRepresentation(rulerHandleRep3D);
		this->m_distanceWidget->SetRepresentation(distanceRep3D);
		distanceRep3D->InstantiateHandleRepresentation();
		distanceRep3D->SetLabelFormat("%-#11.2f mm");
		//distanceRep3D->GetAxis()->GetProperty()->SetColor(0, 1, 0);

		this->m_distanceWidget->CreateDefaultRepresentation();

		this->m_distanceWidget->On();
	}
	else
	{
		this->m_distanceWidget->Off();

	}

	mainWnd->GetRenderWindow(4)->Render();
}

void Distance3DWidget::SetInteractor(vtkRenderWindowInteractor *interactor)
{
	this->m_distanceWidget->SetInteractor(interactor);
}

vtkRenderWindowInteractor * Distance3DWidget::GetInteractor()
{
	return m_distanceWidget->GetInteractor();
}

bool Distance3DWidget::CalculateIndex()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();

	//Pick
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		mainWnd->GetRenderWindow(4)->GetRenderers()->GetFirstRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0 && picked[2] == 0.0)
		return false;

	//Update current pos
	m_currentPos[0] = picked[0];
	m_currentPos[1] = picked[1];
	m_currentPos[2] = picked[2];

	////Set Spin Box value
	//mainWnd->slotChangeCursorPosition(m_currentPos[0], m_currentPos[1], m_currentPos[2]);
	
	
	return true;
}


Distance3DWidget::Distance3DWidget()
{
	this->m_distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
}

Distance3DWidget::~Distance3DWidget()
{
}
#include "InteractorStyleRuler.h"

#include <vtkAxisActor2D.h>
#include <vtkCellArray.h>
#include <vtkProperty2D.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <InteractorStyleSwitch.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkStringArray.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <vtkProperty.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkImageActorPointPlacer.h>



vtkStandardNewMacro(InteractorStyleRuler);

void InteractorStyleRuler::SetCustomEnabled(bool flag)
{
	InteractorStyleNavigation::SetCustomEnabled(flag);
	if (flag)
	{
		vtkImageActorPointPlacer* imageActorPointPlacer =
			vtkImageActorPointPlacer::New();
		imageActorPointPlacer->SetImageActor(GetVtkImageViewer2()->GetImageActor());

		vtkPointHandleRepresentation3D* pointHandleRepresentation3D = 
			vtkPointHandleRepresentation3D::New();
		pointHandleRepresentation3D->SetPointPlacer(imageActorPointPlacer);
		imageActorPointPlacer->Delete();

		vtkDistanceRepresentation2D* distanceRepresentation2D = 
			vtkDistanceRepresentation2D::New();
		distanceRepresentation2D->SetLabelFormat("%-#11.2f mm");
		distanceRepresentation2D->SetHandleRepresentation(pointHandleRepresentation3D);
		pointHandleRepresentation3D->Delete();

		if (!m_distanceWidget) {
			m_distanceWidget = vtkDistanceWidget::New();
		}
		m_distanceWidget->SetRepresentation(distanceRepresentation2D);
		m_distanceWidget->SetInteractor(this->Interactor);
		m_distanceWidget->EnabledOn();
		distanceRepresentation2D->Delete();

	}
	else
	{
		if (m_distanceWidget != nullptr) {
			m_distanceWidget->EnabledOff();
			m_distanceWidget->Delete();
			m_distanceWidget = nullptr;
		}

		GetVtkImageViewer2()->Render();
	}
}

void InteractorStyleRuler::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	//SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStyleRuler, UpdateMaximumWallThicknessLabel());
}

InteractorStyleRuler::~InteractorStyleRuler()
{
	if (this->m_distanceWidget != nullptr) {
		this->m_distanceWidget->Delete();
		this->m_distanceWidget = nullptr;
	}
}


InteractorStyleRuler::InteractorStyleRuler()
{
	m_distanceWidget = nullptr;
}


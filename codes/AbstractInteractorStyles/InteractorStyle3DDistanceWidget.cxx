/*
Author:		Wong, Matthew Lun
Date:		29th, August 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant



Wong Matthew Lun
Copyright (C) 2016
*/

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPointData.h>
#include <vtkKdTree.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkCenterOfMass.h>
#include <vtkCellPicker.h>
#include "InteractorStyle3DDistanceWidget.h"
#include "MainWindow.h"

vtkStandardNewMacro(InteractorStyle3DDistanceWidget);

InteractorStyle3DDistanceWidget::InteractorStyle3DDistanceWidget()
{
}

InteractorStyle3DDistanceWidget::~InteractorStyle3DDistanceWidget()
{
}


void InteractorStyle3DDistanceWidget::OnMiddleButtonDown()
{
	AbstractInteractorStyle3D::OnMiddleButtonDown();
}

void InteractorStyle3DDistanceWidget::OnKeyPress()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();
	

	if (mainwnd->GetCenterlinePD() == NULL)
		Superclass::OnKeyPress();
	else {
		vtkSmartPointer<vtkKdTree> kdtree = vtkSmartPointer<vtkKdTree>::New();
		kdtree->BuildLocatorFromPoints(mainwnd->GetCenterlinePD());

		std::string key = this->Interactor->GetKeySym();

		if (key == "Return") {
			if (this->m_ids.size() >= 2) {
				this->UpdateStenosisValue();
				return;
			}

			int* pos = this->Interactor->GetEventPosition();

			vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
			picker->SetVolumeOpacityIsovalue(0.2); // #LookupTable
			picker->SetTolerance(0.0005);
			picker->Pick(pos[0], pos[1], 0, this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

			double* worldPosition = picker->GetPickPosition();

			// snap the point to the centerline
			double dist;
			int id = kdtree->FindClosestPoint(worldPosition, dist);
			worldPosition = mainwnd->GetCenterlinePD()->GetPoint(id);

			this->m_ids.push_back(id);

			vtkSmartPointer<vtkSphereSource> source = vtkSmartPointer<vtkSphereSource>::New();
			source->SetCenter(worldPosition);
			source->Update();

			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper->SetInputConnection(source->GetOutputPort());
			mapper->Update();

			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			actor->SetMapper(mapper);
			if (this->m_ids.size() == 1) {
				actor->GetProperty()->SetColor(1, 0, 1);
			}
			else {
				actor->GetProperty()->SetColor(0, 1, 0);
			}
			this->m_ABActors.push_back(actor);

			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
			this->Interactor->Render();

			if (this->m_ids.size() >= 2) {
				this->UpdateStenosisValue();
			}
		}
		else if (key == "Delete") {
			// if there are nothing to delete, skip
			if (this->m_ids.size() <= 0) {
				return;
			}

			for (int i = 0; i < this->m_ids.size(); i++)
			{
				this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(this->m_ABActors.at(i));
			}

			this->m_ABActors.clear();
			this->m_ids.clear();

			this->Interactor->Render();
		}
	}
}

bool InteractorStyle3DDistanceWidget::CalculateIndex()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	

	return true;
}

void InteractorStyle3DDistanceWidget::UpdateStenosisValue()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();

	vtkSmartPointer<vtkPolyData> cl = mainwnd->GetCenterlinePD();
	vtkDataArray* radiusArray = cl->GetPointData()->GetArray("Radius");

	double A = cl->GetPointData()->GetArray("Radius")->GetComponent(this->m_ids.at(0), 0);
	double B = cl->GetPointData()->GetArray("Radius")->GetComponent(this->m_ids.at(1), 0);

	double stenosisVal = { A > B ? 1. - B / A : 1 - A / B };
	mainwnd->UpdateStenosisValue(stenosisVal);
}


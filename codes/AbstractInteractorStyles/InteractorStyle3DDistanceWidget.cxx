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

#include <vtkSmartPointer.h>
#include <vtkCenterOfMass.h>
#include "InteractorStyle3DDistanceWidget.h"
#include "MainWindow.h"

vtkStandardNewMacro(InteractorStyle3DDistanceWidget);

InteractorStyle3DDistanceWidget::InteractorStyle3DDistanceWidget()
{
}

InteractorStyle3DDistanceWidget::~InteractorStyle3DDistanceWidget()
{
}


void InteractorStyle3DDistanceWidget::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;

	if (key == "n") {
		// if there are already two widgets, return
		if (this->m_distanceWidgetList.size() >= 2)
			return;

		// create a new distance widget and push it to the list
		Distance3DWidget* l_dwidget = Distance3DWidget::New();
		this->m_distanceWidgetList.push_back(l_dwidget);
		l_dwidget->GetRepresentation()->BuildRepresentation();
		l_dwidget->SetInteractor(this->Interactor);
		l_dwidget->On();

		this->Interactor->Render();
	}
	else if (key == "Delete") {
		// if there are nothing to delete, skip
		if (this->m_distanceWidgetList.size() <= 0) {
			return;
		}

		Distance3DWidget* l_dwidget = this->m_distanceWidgetList.back();
		this->m_distanceWidgetList.pop_back();
		l_dwidget->Off();
		l_dwidget->SetInteractor(NULL);
		l_dwidget->Delete();

		this->Interactor->Render();
	}
}

bool InteractorStyle3DDistanceWidget::CalculateIndex()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();


	return true;
}


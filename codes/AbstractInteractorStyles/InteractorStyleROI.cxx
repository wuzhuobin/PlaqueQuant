#include "InteractorStyleROI.h"
#include "MainWindow.h"

vtkStandardNewMacro(InteractorStyleROI);

void InteractorStyleROI::SetPlaneWidgetEnabled(bool flag)
{
	MainWindow *mainWnd = MainWindow::GetMainWindow();

	if (flag) {
		planeWidget->initializeCustomFunction();
		planeWidget->SetInputData(imageViewer->GetInput());
		planeWidget->SetImageViewer(imageViewer);
		planeWidget->SetDefaultBound(imageViewer->GetBound());
		planeWidget->SetInteractor(mainWnd->GetVtkRenderWindowInteractor(orientation));
		qDebug() << "planeWidget:" << planeWidget;
		qDebug() << "planeWidgetCallback" << planeWidgetCallback;
		planeWidget->AddObserver(vtkCommand::InteractionEvent, planeWidgetCallback);
		planeWidget->AddObserver(vtkCommand::EndInteractionEvent, planeWidgetCallback);

		switch (orientation)
		{
		case 0:
			planeWidgetCallback->SetPlaneWidget(
				planeWidget,
				mainWnd->GetInteractorStyleImageSwitch(1)->GetROI()->GetPlaneWidget(),
				mainWnd->GetInteractorStyleImageSwitch(2)->GetROI()->GetPlaneWidget());
			planeWidget->NormalToXAxisOn();
			break;
		case 1:
			planeWidgetCallback->SetPlaneWidget(
				mainWnd->GetInteractorStyleImageSwitch(0)->GetROI()->GetPlaneWidget(),
				planeWidget,
				mainWnd->GetInteractorStyleImageSwitch(2)->GetROI()->GetPlaneWidget());
			planeWidget->NormalToYAxisOn();
			break;
		case 2:
			planeWidgetCallback->SetPlaneWidget(
				mainWnd->GetInteractorStyleImageSwitch(0)->GetROI()->GetPlaneWidget(),
				mainWnd->GetInteractorStyleImageSwitch(1)->GetROI()->GetPlaneWidget(),
				planeWidget);
			planeWidget->NormalToZAxisOn();
			break;
		default:
			break;
		}
		double* bound = imageViewer->GetBound();
		double displayBound[6];
		double	m_currentBound[6];
		double m_focalPoint[3];
		//Calculate the cursor focal point
		m_focalPoint[0] = origin[0] + m_sliceSplinBox[0]->value() * spacing[0];
		m_focalPoint[1] = origin[1] + m_sliceSplinBox[1]->value() * spacing[1];
		m_focalPoint[2] = origin[2] + m_sliceSplinBox[2]->value() * spacing[2];

		//Set Current Bound
		for (int j = 0; j < 3; j++)
		{
			double roiHalfSize[3];
			roiHalfSize[j] = (bound[j * 2 + 1] - bound[j * 2])*0.25;

			m_currentBound[j * 2] = m_focalPoint[j] - roiHalfSize[j] > bound[j * 2] ? m_focalPoint[j] - roiHalfSize[j] : bound[j * 2];
			m_currentBound[j * 2 + 1] = m_focalPoint[j] + roiHalfSize[j] < bound[j * 2 + 1] ? m_focalPoint[j] + roiHalfSize[j] : bound[j * 2 + 1];

			displayBound[j * 2] = m_focalPoint[j] - roiHalfSize[j] > bound[j * 2] ? m_focalPoint[j] - roiHalfSize[j] : bound[j * 2];
			displayBound[j * 2 + 1] = m_focalPoint[j] + roiHalfSize[j] < bound[j * 2 + 1] ? m_focalPoint[j] + roiHalfSize[j] : bound[j * 2 + 1];

			//Restrict display bound on the plane
			if (orientation == j)
			{
				displayBound[j * 2] = m_focalPoint[j];
				displayBound[j * 2 + 1] = m_focalPoint[j];
			}
		}
		qDebug() << "orientation: " << orientation;
		qDebug() << m_currentBound[0] << '\t' << m_currentBound[1] << '\t' << m_currentBound[2];
		qDebug() << m_currentBound[3] << '\t' << m_currentBound[4] << '\t' << m_currentBound[5];
		planeWidget->SetCurrentBound(m_currentBound);
		planeWidget->PlaceWidget(displayBound);
		planeWidget->On();
		imageViewer->Render();
	}
	else {
		planeWidget->Off();
		imageViewer->Render();
	}
}

void InteractorStyleROI::OnMouseMove()
{
	InteractorStyleNavigation::OnMouseMove();
	UpdateAllWidget();
}

void InteractorStyleROI::OnLeftButtonDown()
{
	InteractorStyleNavigation::OnLeftButtonDown();
	UpdateAllWidget();
}

void InteractorStyleROI::OnLeftButtonUp()
{
	InteractorStyleNavigation::OnLeftButtonUp();
	UpdateAllWidget();
}

void InteractorStyleROI::UpdateAllWidget()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	MyPlaneWidget* m_planeWidget[3] = { 
		mainWnd->GetInteractorStyleImageSwitch(0)->GetROI()->GetPlaneWidget(),
		mainWnd->GetInteractorStyleImageSwitch(1)->GetROI()->GetPlaneWidget(),
		mainWnd->GetInteractorStyleImageSwitch(2)->GetROI()->GetPlaneWidget() };
	double* currentBound;
	//Update all PlaneWidget
	for (int i = 0; i < 3; i++)
	{
		//Current bound
		currentBound = m_planeWidget[i]->GetCurrentBound();

		//Display bound
		double displayBound[6];
		double* focalPoint = m_planeWidget[i]->GetImageViewer()->GetFocalPoint();

		//Decide if it can be displayed
		if (focalPoint[i] >= currentBound[i * 2] && focalPoint[i] <= currentBound[i * 2 + 1])
			m_planeWidget[i]->SetVisibility(true);
		else
			m_planeWidget[i]->SetVisibility(false);
		m_planeWidget[i]->GetInteractor()->GetRenderWindow()->Render();
	}
}

MyPlaneWidget * InteractorStyleROI::GetPlaneWidget()
{
	return this->planeWidget;
}

InteractorStyleROI::InteractorStyleROI()
{
	planeWidget = MyPlaneWidget::New();
	planeWidgetCallback = MyPlaneWidgetCallback::New();
}


InteractorStyleROI::~InteractorStyleROI()
{
	if (planeWidget != NULL) {
		planeWidget->Delete();
	}
	if (planeWidgetCallback != NULL) {
		planeWidgetCallback->Delete();
	}
}

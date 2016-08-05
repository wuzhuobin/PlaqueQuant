#include "InteractorStyleROI.h"
#include "MainWindow.h"

vtkStandardNewMacro(InteractorStyleROI);

void InteractorStyleROI::SetImageViewer(MyImageViewer * imageViewer)
{
	AbstractInteractorStyleImage::SetImageViewer(imageViewer);

	planeWidget->initializeCustomFunction();
	planeWidget->SetInputData(imageViewer->GetInput());
	planeWidget->SetImageViewer(imageViewer);
	planeWidget->SetDefaultBound(imageViewer->GetBound());
	planeWidget->SetInteractor(this->Interactor);
	planeWidget->AddObserver(vtkCommand::InteractionEvent, planeWidgetCallback);
	planeWidget->AddObserver(vtkCommand::EndInteractionEvent, planeWidgetCallback);

}

void InteractorStyleROI::SetPlaneWidgetEnabled(bool b)
{
	MainWindow *mainWnd = MainWindow::GetMainWindow();

	switch (orientation)
	{
	case 0:
		planeWidgetCallback->SetPlaneWidget(
			planeWidget,
			mainWnd->GetInteractorStyleImageSwitch(1)->GetROI()->GetPlaneWidget(),
			mainWnd->GetInteractorStyleImageSwitch(2)->GetROI()->GetPlaneWidget());
		planeWidget->SetNormalToXAxis(true);
		break;
	case 1:
		planeWidgetCallback->SetPlaneWidget(
			mainWnd->GetInteractorStyleImageSwitch(0)->GetROI()->GetPlaneWidget(),
			planeWidget,
			mainWnd->GetInteractorStyleImageSwitch(2)->GetROI()->GetPlaneWidget());
		planeWidget->SetNormalToYAxis(true);
		break;
	case 2:
		planeWidgetCallback->SetPlaneWidget(
			mainWnd->GetInteractorStyleImageSwitch(0)->GetROI()->GetPlaneWidget(),
			mainWnd->GetInteractorStyleImageSwitch(1)->GetROI()->GetPlaneWidget(),
			planeWidget);
		planeWidget->SetNormalToZAxis(true);
	default:
		break;
	}
	double* bound = imageViewer->GetBound();
	double displayBound[6];
	double	m_currentBound[6];
	double m_focalPoint[3];


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
	planeWidget->On();
	imageViewer->Render();
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

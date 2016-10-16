#include "InteractorStyleROI.h"

vtkStandardNewMacro(InteractorStyleROI);
using namespace std;
void InteractorStyleROI::SetPlaneWidgetEnabled(bool flag)
{
	if (flag && rois.size() == 3) {
		///
		planeWidget->initializeCustomFunction();
		planeWidget->SetInputData(m_imageViewer->GetInput());
		planeWidget->SetImageViewer(m_imageViewer);
		planeWidget->SetDefaultBound(m_imageViewer->GetBound());
		planeWidget->SetInteractor(this->Interactor);
		planeWidget->AddObserver(vtkCommand::InteractionEvent, planeWidgetCallback);
		planeWidget->AddObserver(vtkCommand::EndInteractionEvent, planeWidgetCallback);
		planeWidgetCallback->SetPlaneWidget(
			rois[0]->GetPlaneWidget(),
			rois[1]->GetPlaneWidget(),
			rois[2]->GetPlaneWidget()
		);

		switch (GetSliceOrientation())
		{
		case vtkImageViewer2::SLICE_ORIENTATION_YZ:
			planeWidget->NormalToXAxisOn();
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XZ:
			planeWidget->NormalToYAxisOn();
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XY:
			planeWidget->NormalToZAxisOn();
			break;
		default:
			break;
		}
		double* bound = m_imageViewer->GetBound();
		//double displayBound[6];
		double	m_currentBound[6];
		double* m_focalPoint = planeWidget->GetImageViewer()->GetFocalPointWithWorldCoordinate();

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
			if (GetSliceOrientation() == j)
			{
				displayBound[j * 2] = m_focalPoint[j];
				displayBound[j * 2 + 1] = m_focalPoint[j];
			}
		}
		//qDebug() << "m_orientation: " << m_orientation;
		//qDebug() << m_currentBound[0] << '\t' << m_currentBound[1] << '\t' << m_currentBound[2];
		//qDebug() << m_currentBound[3] << '\t' << m_currentBound[4] << '\t' << m_currentBound[5];
		planeWidget->SetCurrentBound(m_currentBound);
		planeWidget->ReplaceWidget(displayBound);
		planeWidget->On();
		m_imageViewer->Render();
	}
	else {
		if (m_imageViewer) {
			planeWidget->Off();
			m_imageViewer->Render();
		}
	}
}

void InteractorStyleROI::OnMouseMove()
{
	InteractorStyleNavigation::OnMouseMove();
	if (m_leftFunctioning) {
		UpdateAllWidget();
	}

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

void InteractorStyleROI::SelectROI(int* newExtent)
{
	const double* bound = planeWidget->GetCurrentBound();
	
	newExtent[0] = (bound[0] - GetOrigin()[0]) / GetSpacing()[0];
	newExtent[1] = (bound[1] - GetOrigin()[0]) / GetSpacing()[0];
	newExtent[2] = (bound[2] - GetOrigin()[1]) / GetSpacing()[1];
	newExtent[3] = (bound[3] - GetOrigin()[1]) / GetSpacing()[1];
	newExtent[4] = (bound[4] - GetOrigin()[2]) / GetSpacing()[2];
	newExtent[5] = (bound[5] - GetOrigin()[2]) / GetSpacing()[2];
	




	//for (int i = 0; i < 3; ++i) {
	//	qDebug() << i;
	//	qDebug() << "extent:" << extent[i * 2] << '\t' << extent[i * 2 + 1];
	//	qDebug() << "m_spacing:" << m_spacing[i];
	//	qDebug() << "bound:" << bound[i * 2] << '\t' << bound[i * 2 + 1];
	//	qDebug() << "m_origin:" << m_origin[i];
	//	qDebug() << "newExtent:" << newExtent[i * 2] << newExtent[2 * i + 1];
	//}
}

void InteractorStyleROI::UpdateAllWidget()
{
	double* bound = planeWidget->GetCurrentBound();
	UpdateAllWidget(bound);
}

void InteractorStyleROI::UpdateAllWidget(double * bound)
{
	if (rois.size() != 3)
		return;
	MyPlaneWidget* m_planeWidget[3] = {
		rois[0]->GetPlaneWidget(),
		rois[1]->GetPlaneWidget(),
		rois[2]->GetPlaneWidget() };
	double* currentBound;
	//Update all PlaneWidget
	for (int i = 0; i < 3; i++)
	{
		//Current bound
		currentBound = m_planeWidget[i]->GetCurrentBound();
		double* focalPoint = m_planeWidget[i]->GetImageViewer()->GetFocalPointWithWorldCoordinate();

		if (bound[0] != currentBound[0] || bound[1] != currentBound[1] ||
			bound[2] != currentBound[2] || bound[3] != currentBound[3] ||
			bound[4] != currentBound[4] || bound[5] != currentBound[5]) {
			//Display bound
			double displayBound[6];
			for (int j = 0; j < 3; j++)
			{
				displayBound[j * 2] = currentBound[j * 2];
				displayBound[j * 2 + 1] = currentBound[j * 2 + 1];

				if (i == j)
				{
					displayBound[j * 2] = focalPoint[j];
					displayBound[j * 2 + 1] = focalPoint[j];
				}
			}
			m_planeWidget[i]->ReplaceWidget(displayBound);

		}


		//Decide if it can be displayed
		if (focalPoint[i] >= currentBound[i * 2] && focalPoint[i] <= currentBound[i * 2 + 1])
			m_planeWidget[i]->SetVisibility(true);
		else
			m_planeWidget[i]->SetVisibility(false);
		m_planeWidget[i]->GetInteractor()->GetRenderWindow()->Render();
	}
}

void InteractorStyleROI::AddSynchronalROI(InteractorStyleROI * roi)
{
	if (std::find(rois.cbegin(), rois.cend(), roi) == rois.cend()) {
		rois.push_back(roi);
	}
}

MyPlaneWidget * InteractorStyleROI::GetPlaneWidget()
{
	return this->planeWidget;
}

InteractorStyleROI::InteractorStyleROI()
	:InteractorStyleNavigation()
{
	//this->m_imageViewer = NULL;

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

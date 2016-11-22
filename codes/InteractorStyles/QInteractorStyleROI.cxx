#include "QInteractorStyleROI.h"
#include "ui_QInteractorStyleROI.h"

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(QInteractorStyleROI);
QSETUP_UI_SRC(QInteractorStyleROI);
vtkSmartPointer<vtkROIWidget> QInteractorStyleROI::m_roi = nullptr;
vtkSmartPointer<vtkRenderWindow> QInteractorStyleROI::m_renderWindow = nullptr;


void QInteractorStyleROI::SetROIWidgetEnabled(bool flag)
{
	InteractorStyleNavigation::SetNavigationModeEnabled(flag);
	if (flag) {
		m_roi->SetBorderWidgetsInteractor(m_uniqueROIId, this->Interactor);
		if (m_uniqueROIId == 2) {
			m_roi->GetRepresentation()->PlaceWidget(
				m_imageViewer->GetInput()->GetBounds());
			m_roi->SetPositionPointer(
				m_imageViewer->GetFocalPointWithWorldCoordinate());
			m_roi->EnabledOn();
		}
	}

	UniqueEnable(flag);
}
//
//void QInteractorStyleROI::OnMouseMove()
//{
//	InteractorStyleNavigation::OnMouseMove();
//	if (m_leftFunctioning) {
//		UpdateAllWidget();
//	}
//
//}
//
//void QInteractorStyleROI::OnLeftButtonDown()
//{
//	InteractorStyleNavigation::OnLeftButtonDown();
//	UpdateAllWidget();
//}
//
//void QInteractorStyleROI::OnLeftButtonUp()
//{
//	InteractorStyleNavigation::OnLeftButtonUp();
//	UpdateAllWidget();
//}
//
//void QInteractorStyleROI::SelectROI(int* newExtent)
//{
//	const double* bound = planeWidget->GetCurrentBound();
//	
//	newExtent[0] = (bound[0] - GetOrigin()[0]) / GetSpacing()[0];
//	newExtent[1] = (bound[1] - GetOrigin()[0]) / GetSpacing()[0];
//	newExtent[2] = (bound[2] - GetOrigin()[1]) / GetSpacing()[1];
//	newExtent[3] = (bound[3] - GetOrigin()[1]) / GetSpacing()[1];
//	newExtent[4] = (bound[4] - GetOrigin()[2]) / GetSpacing()[2];
//	newExtent[5] = (bound[5] - GetOrigin()[2]) / GetSpacing()[2];
//	
//
//
//
//
//	//for (int i = 0; i < 3; ++i) {
//	//	qDebug() << i;
//	//	qDebug() << "extent:" << extent[i * 2] << '\t' << extent[i * 2 + 1];
//	//	qDebug() << "m_spacing:" << m_spacing[i];
//	//	qDebug() << "bound:" << bound[i * 2] << '\t' << bound[i * 2 + 1];
//	//	qDebug() << "m_origin:" << m_origin[i];
//	//	qDebug() << "newExtent:" << newExtent[i * 2] << newExtent[2 * i + 1];
//	//}
//}
//
//void QInteractorStyleROI::UpdateAllWidget()
//{
//	double* bound = planeWidget->GetCurrentBound();
//	UpdateAllWidget(bound);
//}
//
//void QInteractorStyleROI::UpdateAllWidget(double * bound)
//{
//	if (rois.size() != 3)
//		return;
//	MyPlaneWidget* m_planeWidget[3] = {
//		rois[0]->GetPlaneWidget(),
//		rois[1]->GetPlaneWidget(),
//		rois[2]->GetPlaneWidget() };
//	double* currentBound;
//	//Update all PlaneWidget
//	for (int i = 0; i < 3; i++)
//	{
//		//Current bound
//		currentBound = m_planeWidget[i]->GetCurrentBound();
//		double* focalPoint = m_planeWidget[i]->GetImageViewer()->GetFocalPointWithWorldCoordinate();
//
//		if (bound[0] != currentBound[0] || bound[1] != currentBound[1] ||
//			bound[2] != currentBound[2] || bound[3] != currentBound[3] ||
//			bound[4] != currentBound[4] || bound[5] != currentBound[5]) {
//			//Display bound
//			double displayBound[6];
//			for (int j = 0; j < 3; j++)
//			{
//				displayBound[j * 2] = currentBound[j * 2];
//				displayBound[j * 2 + 1] = currentBound[j * 2 + 1];
//
//				if (i == j)
//				{
//					displayBound[j * 2] = focalPoint[j];
//					displayBound[j * 2 + 1] = focalPoint[j];
//				}
//			}
//			m_planeWidget[i]->ReplaceWidget(displayBound);
//
//		}
//
//
//		//Decide if it can be displayed
//		if (focalPoint[i] >= currentBound[i * 2] && focalPoint[i] <= currentBound[i * 2 + 1])
//			m_planeWidget[i]->SetVisibility(true);
//		else
//			m_planeWidget[i]->SetVisibility(false);
//		m_planeWidget[i]->GetInteractor()->GetRenderWindow()->Render();
//	}
//}
//

QInteractorStyleROI::QInteractorStyleROI(int uiType, QWidget * parent)
{
	QNEW_UI();
	if (m_roi == nullptr) {
		m_roi = vtkSmartPointer<vtkROIWidget>::New();
		m_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		m_renderWindow->OffScreenRenderingOn();
		m_renderWindow->AddRenderer(vtkSmartPointer<vtkRenderer>::New());
		vtkSmartPointer<vtkRenderWindowInteractor> interactor = 
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		m_renderWindow->SetInteractor(interactor);
		interactor->Start();
		m_roi->SetInteractor(interactor);
		m_roi->GetRepresentation()->SetPlaceFactor(1);
	}
	m_uniqueROIId = numOfMyself;
	if (numOfMyself == 1) {
		
	}

}

QInteractorStyleROI::~QInteractorStyleROI()
{
	QDELETE_UI();
	m_renderWindow = nullptr;
	m_roi = nullptr;
}

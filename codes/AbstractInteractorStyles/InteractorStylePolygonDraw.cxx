///*
//Author:		Wong, Matthew Lun
//Date:		16th, June 2016
//Occupation:	Chinese University of Hong Kong,
//			Department of Imaging and Inteventional Radiology,
//			Junior Research Assistant
//
//Author:		Lok, Ka Hei Jason
//Date:		16th, June 2016
//Occupation:	Chinese University of Hong Kong,
//			Department of Imaging and Inteventional Radiology,
//			M.Phil Student
//
//This class allows interactive segmentation on images with contour widget.
//
//Wong Matthew Lun, Lok Ka Hei
//Copyright (C) 2016
//*/
//
//#include <vtkInteractorStyleImage.h>
//#include <vtkPolygon.h>
//#include <vtkCallbackCommand.h>
//#include <vtkCommand.h>
//#include "InteractorStylePolygonDraw.h"
//#include "MainWindow.h"
//
//vtkStandardNewMacro(InteractorStylePolygonDraw);
//
//InteractorStylePolygonDraw::InteractorStylePolygonDraw()
//{
//	DOUBLE_CLICKED_FLAG = false;
//	CONTOUR_IS_ON_FLAG = false;
//	m_timer.start();
//	m_firstClickTimeStamp = m_timer.elapsed();
//	m_contourWidget = NULL;
//	m_contourRep = NULL;
//}
//
//InteractorStylePolygonDraw::~InteractorStylePolygonDraw()
//{
//	if (m_contourRep) {
//		m_contourRep->Delete();
//		m_contourRep = NULL;
//	}
//
//	if (m_contourWidget) {
//		m_contourWidget->Delete();
//		m_contourWidget = NULL;
//	}
//}
//
//void InteractorStylePolygonDraw::OnLeftButtonDown()
//{
//	if (this->CheckDoubleClicked() && CONTOUR_IS_ON_FLAG) {
//		this->FillPolygon();
//		this->SetPolygonModeEnabled(false);
//		this->CONTOUR_IS_ON_FLAG = false;
//	}
//	else if (!CONTOUR_IS_ON_FLAG){
//		this->SetPolygonModeEnabled(true);
//		this->CONTOUR_IS_ON_FLAG = true;
//	}
//
//	AbstractInteractorStyleImage::OnLeftButtonDown();
//}
//
//void InteractorStylePolygonDraw::OnRightButtonDown()
//{
//	if (CONTOUR_IS_ON_FLAG) {
//		this->FillPolygon();
//		this->SetPolygonModeEnabled(false);
//		this->CONTOUR_IS_ON_FLAG = false;
//	}
//
//	AbstractInteractorStyleImage::OnRightButtonDown();
//}
//
//void InteractorStylePolygonDraw::OnMouseMove()
//{
//	if (!this->CONTOUR_IS_ON_FLAG) {
//		this->SetPolygonModeEnabled(true);
//		this->CONTOUR_IS_ON_FLAG = true;
//	}
//}
//
//void InteractorStylePolygonDraw::OnKeyPress()
//{
//	std::string key = this->Interactor->GetKeySym();
//
//	std::cout << key << endl;
//
//	if (key == "Escape") {
//		this->SetPolygonModeEnabled(false);
//	}
//	if (key == "Return" && m_contourWidget) {
//		if (CONTOUR_IS_ON_FLAG)
//			this->FillPolygon();
//	}
//	AbstractInteractorStyleImage::OnKeyPress();
//}
//
//bool InteractorStylePolygonDraw::CheckDoubleClicked()
//{
//	int t = m_timer.elapsed() - m_firstClickTimeStamp;
//
//	if (t < 200 && !DOUBLE_CLICKED_FLAG) {
//		DOUBLE_CLICKED_FLAG = true;
//		m_firstClickTimeStamp = m_timer.elapsed();
//		return true;
//	}
//	else {
//		DOUBLE_CLICKED_FLAG = false;
//		m_firstClickTimeStamp = m_timer.elapsed();
//		return false;
//	}
//}
//
//void InteractorStylePolygonDraw::SetPolygonModeEnabled(bool b)
//{
//	MainWindow* mainwnd = MainWindow::GetMainWindow();
//
//	if (m_contourWidget) {
//		m_contourWidget->Off();
//		m_contourWidget->SetRepresentation(NULL);
//		m_contourWidget->EnabledOff();
//		m_contourWidget->Delete();
//		m_contourWidget = NULL;
//	}
//
//
//	if (m_contourRep) {
//		m_contourRep->Delete();
//		m_contourRep = NULL;
//	}
//
//	// Check if the parent is deleted
//	//if (mainwnd->GetViewers(m_orientation)->GetRenderWindow()) {
//		//return;
//	//}
//
//	//mainwnd->GetViewers(m_orientation)->GetRenderWindow()->Render();
//
//	if (b)
//	{
//		m_contourWidget = vtkContourWidget::New();
//		m_contourWidget->SetInteractor(this->Interactor);
//		m_contourWidget->SetCurrentRenderer(mainwnd->GetViewers(m_orientation)->GetAnnotationRenderer());
//		m_contourWidget->SetDefaultRenderer(mainwnd->GetViewers(m_orientation)->GetAnnotationRenderer());
//
//		m_contourRep = vtkOrientedGlyphContourRepresentation::New();
//		m_contourRep->SetRenderer(mainwnd->GetViewers(m_orientation)->GetAnnotationRenderer());
//		m_contourRep->SetNeedToRender(true);
//		m_contourRep->GetLinesProperty()->SetColor(255, 255, 0);
//		m_contourRep->SetLineInterpolator(NULL);
//		m_contourRep->AlwaysOnTopOn();
//
//
//		vtkPolyData* cursorpolyData = m_contourRep->GetActiveCursorShape();
//		vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
//		if (m_orientation == 0) {
//			translation->RotateX(90);
//			translation->RotateZ(90);
//		}
//		else if (m_orientation == 1) {
//			translation->RotateX(90);
//			translation->RotateY(90);
//		}
//
//		vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//		transformFilter->SetInputData(cursorpolyData);
//		transformFilter->SetTransform(translation);
//		transformFilter->Update();
//
//		cursorpolyData->DeepCopy(transformFilter->GetOutput());
//		m_contourRep->BuildRepresentation();
//		m_contourWidget->SetRepresentation(m_contourRep);
//		m_contourWidget->FollowCursorOn();
//		m_contourWidget->ContinuousDrawOn();
//		//m_contourWidget->CreateDefaultRepresentation();
//		m_contourWidget->On();
//		m_contourWidget->EnabledOn();
//
//		mainwnd->ResetAllViewerCameraClippingRange();
//		mainwnd->RenderAllViewer();
//		this->CONTOUR_IS_ON_FLAG = true;
//	} else{
//		this->CONTOUR_IS_ON_FLAG = false;
//	}
//}
//
//void InteractorStylePolygonDraw::DisplayPolygon(vtkObject* caller, long unsigned vtkNotUsed(eventId), void* vtkNotUsed(clientData))
//{
//	MainWindow* mainwnd = MainWindow::GetMainWindow();
//
//	mainwnd->GetViewers(this->m_orientation)->GetDataRenderer()->ResetCameraClippingRange();
//	mainwnd->GetViewers(this->m_orientation)->Render();
//}
//
//void InteractorStylePolygonDraw::FillPolygon()
//{
//	//Get Main window
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	BrushProperty brushProperty = mainWnd->GetImageSegmentationWidget()->GetBrushProperty();
//
//	Image* labelImage = mainWnd->GetPlanner()->GetImageByUniqueName("Label");
//
//	if (m_contourRep == NULL) return;
//	if (m_contourWidget == NULL) return;
//	if (labelImage == NULL) {
//		this->SetPolygonModeEnabled(false);
//		return;
//	}
//	//Get most updated current value
//	mainWnd->GetCursorPosition(m_currentPos);
//
//	m_contourWidget->CloseLoop();
//
//	vtkPolyData* polydata = m_contourRep->GetContourRepresentationAsPolyData();
//
//	// Check if contour is drawn
//	if (polydata->GetNumberOfPoints() == 0)
//		return;
//
//	vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
//	int numOfPoints = polydata->GetNumberOfPoints();
//
//	// Get the coordinates of the contour data points
//	double origin[3];
//	double spacing[3];
//	labelImage->GetImageData()->GetOrigin(origin);
//	labelImage->GetImageData()->GetSpacing(spacing);
//
//	double lastPoint[3] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MAX, VTK_DOUBLE_MAX };
//	for (vtkIdType i = 0; i < numOfPoints; i++)
//	{
//		double p[3];
//		polydata->GetPoint(i, p);
//
//		// Project point to slice plane
//		labelImage->GetReslicer()->ProjectWorldPointToReslicePlane(p, p, this->m_orientation);
//
//
//		// if the points is too close to the previous point, skip it to avoid error in PointInPolygon algorithm
//		double d = vtkMath::Distance2BetweenPoints(lastPoint, p);
//		if (d < 1E-5)
//			continue;
//		
//		polygon->GetPoints()->InsertNextPoint(p);
//		memcpy(lastPoint, p, sizeof(double) * 3);
//	}
//
//	//Test whether the points are inside the polygon or not
//	double n[3];
//	polygon->ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(), static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), n);
//
//	double polygon_bounds[6];
//
//	int curIndex[3];
//	labelImage->GetReslicer()->GetITKIndexFromVTKImageActorPoint(m_currentPos, curIndex, m_orientation);
//
//	polygon->GetPoints()->GetBounds(polygon_bounds);
//
//	int bound_extent[6];
//	labelImage->GetImageData()->GetExtent(bound_extent);
//
//	bound_extent[m_orientation * 2] = 0;
//	bound_extent[m_orientation * 2 + 1] = 1;
//
//	for (int i = bound_extent[0]; i < bound_extent[1]; i++)
//	{
//		for (int j = bound_extent[2]; j < bound_extent[3]; j++)
//		{
//			for (int k = bound_extent[4]; k < bound_extent[5]; k++)
//			{
//				int vtkIndex[3];
//				vtkIndex[0] = i;
//				vtkIndex[1] = j;
//				vtkIndex[2] = k;
//
//				// obtain world coordinate projected on to the reslice plane
//				double vtkPoint[3];
//				labelImage->GetReslicer()->GetVTKPointFromITKIndex(vtkIndex, vtkPoint, this->m_orientation);
//
//				// project point to polygon's plane
//				vtkPlane::GeneralizedProjectPoint(vtkPoint, polygon->GetPoints()->GetPoint(0), n, vtkPoint);
//
//
//				// if point outof polygon
//				if ((polygon->PointInPolygon(vtkPoint, polygon->GetPoints()->GetNumberOfPoints(), static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), polygon_bounds, n)) == 0) {
//					continue;
//				}
//				
//				//cout << i << " " << j << " " << k << "\n";
//				ImageType::IndexType itkIndex;
//				itkIndex[0] = i;
//				itkIndex[1] = j;
//				itkIndex[2] = k;
//				itkIndex[m_orientation] = curIndex[m_orientation];
//
//				labelImage->GetItkImage()->SetPixel(itkIndex, brushProperty.labelID);
//			}
//
//		}
//
//	}
//
//	labelImage->GetItkImage()->Update();
//
//	for (int i = 0; i < 3; i++)
//	{
//		labelImage->GetReslicer()->GetReslicer(i)->Modified();
//		labelImage->GetReslicer()->GetReslicer(i)->Update();
//	}
//
//	SetPolygonModeEnabled(false);
//	SetPolygonModeEnabled(true);
//	mainWnd->RenderAllViewer();
//
//}

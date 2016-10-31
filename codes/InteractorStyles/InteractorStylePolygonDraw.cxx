/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			Junior Research Assistant

Author:		Lok, Ka Hei Jason
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

This class allows interactive segmentation on images with contour widget.

Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#include <vtkContourWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPolygon.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkMath.h>
#include <vtkLinearContourLineInterpolator.h>
#include <vtkBezierContourLineInterpolator.h>
#include <vtkProperty.h>
#include <vtkDoubleArray.h>

#include "InteractorStylePolygonDraw.h"
#include "LumenSegmentation.h"
#include "MyImageViewer.h"

using namespace std;
vtkStandardNewMacro(InteractorStylePolygonDraw);

InteractorStylePolygonDraw::InteractorStylePolygonDraw()
	:AbstractNavigation()
{
	DOUBLE_CLICKED_FLAG = false;
	CONTOUR_IS_ON_FLAG = false;
	AUTO_LUMEN_SEGMENTATION_ENABLE_FLAG = false;

	//m_timer.start();
	//m_firstClickTimeStamp = m_timer.elapsed();
	this->m_interpolator = 
		vtkSmartPointer<vtkBezierContourLineInterpolator>::New();
	vesselWallLabel = 1;
	this->m_generateValue = 60;
}

InteractorStylePolygonDraw::~InteractorStylePolygonDraw()
{
	if (m_vesselWallContourRepresentation) {
		m_vesselWallContourRepresentation->Delete();
		m_vesselWallContourRepresentation = NULL;
	}

	if (m_vesselWallContourWidget) {
		m_vesselWallContourWidget->Delete();
		m_vesselWallContourWidget = NULL;
	}
	if (m_lumenWallContourRepresentation) {
		m_lumenWallContourRepresentation->Delete();
		m_lumenWallContourRepresentation = NULL;
	}

	if (m_lumenWallContourWidget) {
		m_lumenWallContourWidget->Delete();
		m_lumenWallContourWidget = NULL;
	}
}

void InteractorStylePolygonDraw::OnLeftButtonDown()
{
	//if (this->CheckDoubleClicked() && CONTOUR_IS_ON_FLAG) {
	//	this->m_vesselWallContourWidget->CloseLoop();
	//	this->FillPolygon();
	//	this->SetPolygonModeEnabled(false);
	//	this->CONTOUR_IS_ON_FLAG = false;
	//}
	//else if (!CONTOUR_IS_ON_FLAG){
	//	this->SetPolygonModeEnabled(true);
	//	this->CONTOUR_IS_ON_FLAG = true;
	//}
	AbstractNavigation::OnLeftButtonDown();
}

void InteractorStylePolygonDraw::OnRightButtonDown()
{
	if (CONTOUR_IS_ON_FLAG && AUTO_LUMEN_SEGMENTATION_ENABLE_FLAG) {
		this->GenerateLumenWallContourWidget();
	}

	AbstractNavigation::OnRightButtonDown();
}

void InteractorStylePolygonDraw::OnMouseMove()
{
	//if (!this->CONTOUR_IS_ON_FLAG) {
	//	this->SetPolygonModeEnabled(true);
	//	this->CONTOUR_IS_ON_FLAG = true;
	//}
	if (m_polygonDrawEnabledFlag) {
		SetPolygonModeEnabled(m_polygonDrawEnabledFlag);
	}
}

void InteractorStylePolygonDraw::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	
	if (key == "Escape") {
		//this->SetPolygonModeEnabled(false);
	}
	if (key == "Return" && m_vesselWallContourWidget) {
		if (CONTOUR_IS_ON_FLAG)
			this->FillPolygon();
	}
	AbstractNavigation::OnKeyPress();
}

bool InteractorStylePolygonDraw::CheckDoubleClicked()
{
	//int t = m_timer.elapsed() - m_firstClickTimeStamp;
	int t = 100;

	if (t < 200 && !DOUBLE_CLICKED_FLAG) {
		DOUBLE_CLICKED_FLAG = true;
		//m_firstClickTimeStamp = m_timer.elapsed();
		return true;
	}
	else {
		DOUBLE_CLICKED_FLAG = false;
		//m_firstClickTimeStamp = m_timer.elapsed();
		return false;
	}
}

void InteractorStylePolygonDraw::SetPolygonModeEnabled(bool b)
{
	m_polygonDrawEnabledFlag = b;
	if (!m_polygonDrawEnabledFlag) {
		ClearAllConoturs();
		m_currentContour = nullptr;
	}
	else if (m_currentContour == nullptr ||
		m_currentContour->GetWidgetState() == vtkContourWidget::Manipulate) {
		m_currentContour->CloseLoop();
		switch (m_contourType)
		{
		case NO_SEG:
			m_noSegmentation.push_back(vtkSmartPointer<vtkContourWidget>::New());
			m_currentContour = m_noSegmentation.back();
			m_currentContour->SetRepresentation(
				vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New());

			m_currentContourRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(
				m_currentContour->GetContourRepresentation());
			m_currentContourRep->GetLinesProperty()->SetColor(0, 255, 0);
			break;
		case VESSEL_WALL:
			m_noSegmentation.push_back(vtkSmartPointer<vtkContourWidget>::New());
			m_currentContour = m_noSegmentation.back();
			m_currentContour->SetRepresentation(
				vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New());

			m_currentContourRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(
				m_currentContour->GetContourRepresentation());
			m_currentContourRep->GetLinesProperty()->SetColor(0, 255, 0);
			break;
		default:
			break;
		}
		m_currentContour->SetInteractor(this->Interactor);
		m_currentContour->ContinuousDrawOn();
		m_currentContour->FollowCursorOn();

		m_currentContourRep->SetLineInterpolator(m_interpolator);
		m_currentContourRep->AlwaysOnTopOn();
		m_currentContour->EnabledOn();

	}
	else {
		/*if (m_vesselWallContourWidget) {
			m_vesselWallContourWidget->Off();
			m_vesselWallContourWidget->SetRepresentation(NULL);
			m_vesselWallContourWidget->EnabledOff();
			m_vesselWallContourWidget->Delete();
			m_vesselWallContourWidget = NULL;
		}


		if (m_vesselWallContourRepresentation) {
			m_vesselWallContourRepresentation->Delete();
			m_vesselWallContourRepresentation = NULL;
		}
		if (m_lumenWallContourWidget) {
			m_lumenWallContourWidget->Off();
			m_lumenWallContourWidget->SetRepresentation(NULL);
			m_lumenWallContourWidget->EnabledOff();
			m_lumenWallContourWidget->Delete();
			m_lumenWallContourWidget = NULL;
		}


		if (m_lumenWallContourRepresentation) {
			m_lumenWallContourRepresentation->Delete();
			m_lumenWallContourRepresentation = NULL;
		}

		if (b)
		{
			m_vesselWallContourRepresentation = vtkOrientedGlyphContourRepresentation::New();

			MyImageViewer* viewer = dynamic_cast<MyImageViewer*>(m_imageViewer);
			if (viewer != NULL) {
				cout << "viewer" << endl;
				m_vesselWallContourRepresentation->SetRenderer(viewer->GetAnnotationRenderer());
			}
			else {
				m_vesselWallContourRepresentation->SetRenderer(m_imageViewer->GetRenderer());
			}

			m_vesselWallContourRepresentation->SetNeedToRender(true);
			m_vesselWallContourRepresentation->GetLinesProperty()->SetColor(0, 0, 255);
			m_vesselWallContourRepresentation->SetLineInterpolator(this->m_interpolator);
			m_vesselWallContourRepresentation->AlwaysOnTopOn();
			m_vesselWallContourRepresentation->BuildRepresentation();

			m_vesselWallContourWidget = vtkContourWidget::New();
			m_vesselWallContourWidget->SetInteractor(this->Interactor);
			m_vesselWallContourWidget->SetRepresentation(m_vesselWallContourRepresentation);
			if (viewer != NULL) {
				cout << "viewer" << endl;

				m_vesselWallContourWidget->SetDefaultRenderer(viewer->GetAnnotationRenderer());
			}
			else {
				m_vesselWallContourWidget->SetDefaultRenderer(m_imageViewer->GetRenderer());
			}
			m_vesselWallContourWidget->FollowCursorOn();
			m_vesselWallContourWidget->ContinuousDrawOn();
			m_vesselWallContourWidget->On();
			m_vesselWallContourWidget->EnabledOn();



			this->m_imageViewer->Render();
			this->CONTOUR_IS_ON_FLAG = true;
		}
		else {
			this->CONTOUR_IS_ON_FLAG = false;
		}*/
	}



}

void InteractorStylePolygonDraw::SetVesselWallLabel(int vesselWallLabel)
{
	this->vesselWallLabel = vesselWallLabel;
}

void InteractorStylePolygonDraw::SetLumenWallLabel(int lumenWallLabel)
{
	this->lumenWallLabel = lumenWallLabel;
}

void InteractorStylePolygonDraw::SetNoSegLabel(int noSegLabel)
{
	this->noSegLabel = noSegLabel;
}

void InteractorStylePolygonDraw::EnableAutoLumenSegmentation(bool flag)
{
	this->AUTO_LUMEN_SEGMENTATION_ENABLE_FLAG = flag;
}

void InteractorStylePolygonDraw::SetContourFilterGenerateValues(int generateValues)
{
	this->m_generateValue = generateValues;
}

void InteractorStylePolygonDraw::DisplayPolygon(vtkObject* caller, long unsigned vtkNotUsed(eventId), void* vtkNotUsed(clientData))
{

	m_imageViewer->Render();
}

void InteractorStylePolygonDraw::ClearCurrentContour()
{
	list<vtkSmartPointer<vtkContourWidget>>* _contours[] = {
		&m_noSegmentation,
		&m_vesselWallContourWidgets,
		&m_noSegmentation
	};
	_contours[m_contourType]->back()->EnabledOff();
	_contours[m_contourType]->pop_back();

}

void InteractorStylePolygonDraw::ClearAllConoturs()
{
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend();
		++cit) {

		(*cit)->EnabledOff();
	}
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_lumenWallContourWidgets.cbegin(); cit != m_lumenWallContourWidgets.cend();
		++cit) {

		(*cit)->EnabledOff();

	}
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_noSegmentation.cbegin(); cit != m_noSegmentation.cend();
		++cit) {

		(*cit)->EnabledOff();

	}
	m_noSegmentation.clear();
	m_vesselWallContourWidgets.clear();
	m_lumenWallContourWidgets.clear();
}

void InteractorStylePolygonDraw::GenerateLumenWallContourWidget()
{
	if (m_vesselWallContourRepresentation == NULL || m_vesselWallContourWidget == NULL)
		return;
	
	// if there are not points to close #Issue7
	if (this->m_vesselWallContourRepresentation->GetNumberOfNodes() < 1)
		return;

	this->m_vesselWallContourWidget->CloseLoop();

	if (m_lumenWallContourWidget) {
		m_lumenWallContourWidget->Off();
		m_lumenWallContourWidget->SetRepresentation(NULL);
		m_lumenWallContourWidget->EnabledOff();
		m_lumenWallContourWidget->Delete();
		m_lumenWallContourWidget = NULL;
	}


	if (m_lumenWallContourRepresentation) {
		m_lumenWallContourRepresentation->Delete();
		m_lumenWallContourRepresentation = NULL;
	}
	if (this->CONTOUR_IS_ON_FLAG) {
		m_lumenWallContourRepresentation = vtkOrientedGlyphContourRepresentation::New();
		MyImageViewer* viewer2 = dynamic_cast<MyImageViewer*>(m_imageViewer);
		if (viewer2 != NULL) {
			m_lumenWallContourRepresentation->SetRenderer(viewer2->GetAnnotationRenderer());
		}
		else {
			m_lumenWallContourRepresentation->SetRenderer(m_imageViewer->GetRenderer());
		}
		m_lumenWallContourRepresentation->SetNeedToRender(true);
		m_lumenWallContourRepresentation->GetLinesProperty()->SetColor(255, 0, 0);
		m_lumenWallContourRepresentation->SetLineInterpolator(this->m_interpolator);
		m_lumenWallContourRepresentation->AlwaysOnTopOn();
		m_lumenWallContourRepresentation->BuildRepresentation();

		m_lumenWallContourWidget = vtkContourWidget::New();
		m_lumenWallContourWidget->SetInteractor(this->Interactor);
		m_lumenWallContourWidget->SetRepresentation(m_lumenWallContourRepresentation);
		if (viewer2 != NULL) {
			m_lumenWallContourWidget->SetDefaultRenderer(viewer2->GetAnnotationRenderer());
		}
		else {
			m_lumenWallContourWidget->SetDefaultRenderer(m_imageViewer->GetRenderer());
		}
		m_lumenWallContourWidget->FollowCursorOn();
		m_lumenWallContourWidget->ContinuousDrawOn();
		m_lumenWallContourWidget->On();
		m_lumenWallContourWidget->EnabledOn();

		vtkSmartPointer<LumenSegmentaiton> ls =
			vtkSmartPointer<LumenSegmentaiton>::New();
		ls->SetInputData(m_imageViewer->GetInput());
		ls->SetSlice(GetSlice());
		ls->SetGenerateValues(1, m_generateValue, m_generateValue);
		ls->SetVesselWallContourRepresentation(this->m_vesselWallContourRepresentation);
		ls->Update();

		if (ls->GetOutput() == nullptr)
			return;

		// This prevent crash when a loop is not found #Issue7
		if (ls->GetOutput()->GetNumberOfPoints() == 0)
			return;

		m_lumenWallContourWidget->Initialize(ls->GetOutput());
		m_lumenWallContourWidget->CloseLoop();
		m_imageViewer->Render();
	}
}

void InteractorStylePolygonDraw::SetLineInterpolator(int i)
{
	if (this->m_interpolator != NULL) {
		this->m_interpolator->Delete();
		this->m_interpolator = NULL;
	}
	switch (i)
	{
	case 0:
		this->m_interpolator = vtkBezierContourLineInterpolator::New();
		break;
	case 1:
		this->m_interpolator = vtkLinearContourLineInterpolator::New();
		break;
	}
	SetPolygonModeEnabled(false);
	SetPolygonModeEnabled(true);
}

void InteractorStylePolygonDraw::FillPolygon()
{

	vtkContourWidget* contourWidget[2] = 
		{m_vesselWallContourWidget, m_lumenWallContourWidget};
	vtkContourRepresentation* contourRepresentation[2] =
		{m_vesselWallContourRepresentation, m_lumenWallContourRepresentation};
	int label[2] = { vesselWallLabel , lumenWallLabel };
	for (int i = 0; i < 2; ++i) {
		if (contourWidget[i] == NULL) continue;
		if (contourRepresentation[i] == NULL) continue;

		contourWidget[i]->CloseLoop();

		vtkPolyData* polydata = contourRepresentation[i]->GetContourRepresentationAsPolyData();

		// Check if contour is drawn
		if (polydata->GetNumberOfPoints() == 0)
			return;

		vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
		int numOfPoints = polydata->GetNumberOfPoints();

		// Get the coordinates of the contour data points

		double lastPoint[3] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MAX, VTK_DOUBLE_MAX };
		for (vtkIdType i = 0; i < numOfPoints; i++)
		{
			double worldCoordinate[3];
			double displayCoordinate[3];
			polydata->GetPoint(i, worldCoordinate);

			//Take one image data 1 to be reference
			displayCoordinate[0] = (worldCoordinate[0] - GetOrigin()[0]) / GetSpacing()[0];
			displayCoordinate[1] = (worldCoordinate[1] - GetOrigin()[1]) / GetSpacing()[1];
			displayCoordinate[2] = (worldCoordinate[2] - GetOrigin()[2]) / GetSpacing()[2];
			//cout << s[0] << " " << s[1] << " " << s[2] << endl;
			//Test whether the points are inside the polygon or not
			// if the points is too close to the previous point, skip it to avoid error in PointInPolygon algorithm
			double d = vtkMath::Distance2BetweenPoints(lastPoint, displayCoordinate);
			if (d < 1E-5)
				continue;
			memcpy(lastPoint, displayCoordinate, sizeof(double) * 3);
			displayCoordinate[GetSliceOrientation()] = 0.0;
			polygon->GetPoints()->InsertNextPoint(displayCoordinate);
			//switch (GetSliceOrientation())
			//{
			//case 0:
			//	polygon->GetPoints()->InsertNextPoint(0.0, displayCoordinate[1], displayCoordinate[2]);
			//	break;
			//case 1:
			//	polygon->GetPoints()->InsertNextPoint(displayCoordinate[0], 0.0, displayCoordinate[2]);
			//	break;
			//case 2:
			//	polygon->GetPoints()->InsertNextPoint(displayCoordinate[0], displayCoordinate[1], 0.0);
			//	break;
			//default:
			//	break;
			//}
		}
		//Test whether the points are inside the polygon or not
		double n[3];
		polygon->ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(),
			static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), n);
		double bounds[6];
		int bounds_int[6];

		polygon->GetPoints()->GetBounds(bounds);

		bounds_int[0] = floor(bounds[0]) - 3;
		bounds_int[1] = ceil(bounds[1]) + 3;
		bounds_int[2] = floor(bounds[2]) - 3;
		bounds_int[3] = ceil(bounds[3]) + 3;
		bounds_int[4] = floor(bounds[4]) - 3;
		bounds_int[5] = ceil(bounds[5]) + 3;

		// Clamp values to within extent specified 
		bounds_int[0] = { bounds_int[0] < this->GetExtent()[0] ? this->GetExtent()[0] : bounds_int[0] };
		bounds_int[1] = { bounds_int[1] > this->GetExtent()[1] ? this->GetExtent()[1] : bounds_int[1] };
		bounds_int[2] = { bounds_int[2] < this->GetExtent()[2] ? this->GetExtent()[2] : bounds_int[2] };
		bounds_int[3] = { bounds_int[3] > this->GetExtent()[3] ? this->GetExtent()[3] : bounds_int[3] };
		bounds_int[4] = { bounds_int[4] < this->GetExtent()[4] ? this->GetExtent()[4] : bounds_int[4] };
		bounds_int[5] = { bounds_int[5] > this->GetExtent()[5] ? this->GetExtent()[5] : bounds_int[5] };

		// for using overlay::SetPixels()


		vtkSmartPointer<vtkPoints> points =
			vtkSmartPointer<vtkPoints>::New();
		bounds_int[GetSliceOrientation() * 2] = 0;
		bounds_int[GetSliceOrientation() * 2 + 1] = 0;

		for (int x = bounds_int[0]; x <= bounds_int[1]; x++) {
			for (int y = bounds_int[2]; y <= bounds_int[3]; y++) {
				for (int z = bounds_int[4]; z <= bounds_int[5]; z++) {
					double p[3] = { x, y, z };
					if (polygon->PointInPolygon(p, polygon->GetPoints()->GetNumberOfPoints(),
						static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, n)) {
						p[GetSliceOrientation()] = m_imageViewer->GetSlice();
						points->InsertNextPoint(p[0], p[1], p[2]);
					}
				}
			}
		}

		m_imageViewer->GetOverlay()->SetPixels(points, label[i]);

	}

	SetPolygonModeEnabled(false);
	SetPolygonModeEnabled(true);
	m_imageViewer->GetInputLayer()->Modified();
	for (std::list<MyImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		(*it)->Render();
	}


}


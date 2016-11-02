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
#include <vtkPolygon.h>
#include <vtkMath.h>
#include <vtkLinearContourLineInterpolator.h>
#include <vtkBezierContourLineInterpolator.h>
#include <vtkProperty.h>

#include "InteractorStylePolygonDraw.h"
#include "MyImageViewer.h"
#include "InteractorStyleVesselSegmentation.h"
#include "LumenSegmentation.h"

using namespace std;
vtkStandardNewMacro(InteractorStyleVesselSegmentation);

InteractorStyleVesselSegmentation::InteractorStyleVesselSegmentation()
	:InteractorStylePolygonDraw()
{
}

InteractorStyleVesselSegmentation::~InteractorStyleVesselSegmentation()
{
}

void InteractorStyleVesselSegmentation::OnLeftButtonUp()
{
	InteractorStylePolygonDraw::OnLeftButtonUp();
}

void InteractorStyleVesselSegmentation::OnMouseMove()
{
	WriteToPolydata();
	InteractorStylePolygonDraw::OnMouseMove();
}

void InteractorStyleVesselSegmentation::OnKeyPress()
{
	string key = this->Interactor->GetKeySym();
	if (key == "b") {
		ReadFromPolydata();
	}
	else {
		InteractorStylePolygonDraw::OnKeyPress();
	}
}

void InteractorStyleVesselSegmentation::SetPolygonModeEnabled(bool b)
{
	InteractorStylePolygonDraw::SetPolygonModeEnabled(b);
}

void InteractorStyleVesselSegmentation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	ReadFromPolydata();
}

void InteractorStyleVesselSegmentation::NewContour()
{
	switch (m_contourType)
	{
	case CONTOUR:
		InteractorStylePolygonDraw::NewContour();
		break;
	case VESSEL_WALL:
		// close loop the last contour
		if (m_vesselWallContourWidgets.size() != 0) {
			m_vesselWallContourWidgets.back()->CloseLoop();
		}
		m_vesselWallContourWidgets.push_back(vtkSmartPointer<vtkContourWidget>::New());
		m_currentContour = m_vesselWallContourWidgets.back();
		m_currentContour->SetRepresentation(
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New());

		m_currentContourRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(
			m_currentContour->GetContourRepresentation());
		m_currentContourRep->GetLinesProperty()->SetColor(0, 0, 255);

		m_currentContour->SetInteractor(this->Interactor);
		m_currentContour->SetCurrentRenderer(m_imageViewer->GetAnnotationRenderer());
		m_currentContour->ContinuousDrawOn();
		m_currentContour->FollowCursorOn();

		m_currentContourRep->SetLineInterpolator(m_interpolator);
		m_currentContourRep->AlwaysOnTopOn();
		m_currentContour->EnabledOn();


		break;
	case LUMEN:
		break;
	default:
		break;
	}
}

void InteractorStyleVesselSegmentation::NewContour(int type, QList<vtkSmartPointer<vtkPolyData>>* list)
{

	for (QList<vtkSmartPointer<vtkPolyData>>::const_iterator cit = list->cbegin();
		cit != list->cend(); ++cit) {

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> _contourRep =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
		_contourRep->SetLineInterpolator(m_interpolator);
		_contourRep->AlwaysOnTopOn();

		vtkSmartPointer<vtkContourWidget> _contour =
			vtkSmartPointer<vtkContourWidget>::New();
		switch (type)
		{
		case CONTOUR:
			_contourRep->GetLinesProperty()->SetColor(255, 0, 0);
			m_contours.push_back(_contour);
			break;
		case VESSEL_WALL:
			_contourRep->GetLinesProperty()->SetColor(0, 0, 255);
			m_vesselWallContourWidgets.push_back(_contour);
			break;
		case LUMEN:
			_contourRep->GetLinesProperty()->SetColor(255, 0, 0);
			m_lumenWallContourWidgets.push_back(_contour);
			break;
		default:
			break;
		}


		_contour->SetRepresentation(_contourRep);
		_contour->SetInteractor(this->Interactor);
		// Must be set before initialize();
		_contour->GetContourRepresentation()->
			SetRenderer(m_imageViewer->GetAnnotationRenderer());
		_contour->ContinuousDrawOn();
		_contour->FollowCursorOn();
		_contour->Initialize((*cit));
		_contour->EnabledOn();
		_contour->CloseLoop();


	}

}

void InteractorStyleVesselSegmentation::ReadFromPolydata()
{
	if (m_contourType == CONTOUR) {
		return;
	}
	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY)
		return;
	ClearAllConoturs();
	Overlay* overlay = m_imageViewer->GetOverlay();
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* maps[2] = {
		overlay->GetVesselWallPolyData(),
		overlay->GetLumenPolyData()
	};
	for (int i = 0; i < 2; ++i) {
		if (maps[i]->contains(GetSlice())) {
			QList<vtkSmartPointer<vtkPolyData>>* list = maps[i]->value(GetSlice());
			NewContour(i + 1, list);
		}
	}
	m_imageViewer->Render();
}

void InteractorStyleVesselSegmentation::WriteToPolydata()
{
	if (m_contourType == CONTOUR) {
		return;
	}
	if (m_currentContour != nullptr &&
		m_currentContour->GetWidgetState() != vtkContourWidget::Manipulate) {
		return;
	}
	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY)
		return;

	Overlay* overlay = m_imageViewer->GetOverlay();
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* maps[2] = {
		overlay->GetVesselWallPolyData(),
		overlay->GetLumenPolyData()
	};
	list<vtkSmartPointer<vtkContourWidget>>* lists[2] = {
		&m_vesselWallContourWidgets,
		&m_lumenWallContourWidgets 
	};
	for (int i = 0; i < 2; ++i) {
		if (maps[i]->contains(GetSlice())) {
			delete maps[i]->value(GetSlice());
		}
		QList<vtkSmartPointer<vtkPolyData>>* _qlist = 
			new QList<vtkSmartPointer<vtkPolyData>>;
		(*maps[i])[GetSlice()] = _qlist;
		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit = lists[i]->cbegin();
			cit != lists[i]->cend(); ++cit) {
			vtkSmartPointer<vtkPolyData> _nodePolyData =
				vtkSmartPointer<vtkPolyData>::New();
			(*cit)->GetContourRepresentation()->GetNodePolyData(_nodePolyData);
			*_qlist += _nodePolyData;
		}

	}
}

void InteractorStyleVesselSegmentation::SetVesselWallLabel(int vesselWallLabel)
{
	this->m_vesselWallLabel = vesselWallLabel;
}

void InteractorStyleVesselSegmentation::SetLumenWallLabel(int lumenWallLabel)
{
	this->m_lumenWallLabel = lumenWallLabel;
}

void InteractorStyleVesselSegmentation::SetSegmentationMode(int i)
{
	if (i < 0 || i > 2) {
		return;
	}
	m_contourType = i;
	SetPolygonModeEnabled(false);
	SetPolygonModeEnabled(true);
}

void InteractorStyleVesselSegmentation::EnableNoSegmentation()
{
	SetSegmentationMode(0);
}

void InteractorStyleVesselSegmentation::EnableLumenSegmentation()
{
	SetSegmentationMode(1);
}

void InteractorStyleVesselSegmentation::EnableVesselWallSegmentation()
{
	SetSegmentationMode(2);
}

void InteractorStyleVesselSegmentation::SetContourFilterGenerateValues(int generateValues)
{
	this->m_generateValue = generateValues;
}

void InteractorStyleVesselSegmentation::ClearAllConoturs()
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
	m_vesselWallContourWidgets.clear();
	m_lumenWallContourWidgets.clear();
	InteractorStylePolygonDraw::ClearAllConoturs();
	m_imageViewer->Render();
}

void InteractorStyleVesselSegmentation::GenerateLumenWallContourWidget()
{
	//if (m_vesselWallContourRepresentation == NULL || m_vesselWallContourWidget == NULL)
	//	return;

	//// if there are not points to close #Issue7
	//if (this->m_vesselWallContourRepresentation->GetNumberOfNodes() < 1)
	//	return;

	//this->m_vesselWallContourWidget->CloseLoop();

	//if (m_lumenWallContourWidget) {
	//	m_lumenWallContourWidget->Off();
	//	m_lumenWallContourWidget->SetRepresentation(NULL);
	//	m_lumenWallContourWidget->EnabledOff();
	//	m_lumenWallContourWidget->Delete();
	//	m_lumenWallContourWidget = NULL;
	//}


	//if (m_lumenWallContourRepresentation) {
	//	m_lumenWallContourRepresentation->Delete();
	//	m_lumenWallContourRepresentation = NULL;
	//}
	//if (this->CONTOUR_IS_ON_FLAG) {
	//	m_lumenWallContourRepresentation = vtkOrientedGlyphContourRepresentation::New();
	//	MyImageViewer* viewer2 = dynamic_cast<MyImageViewer*>(m_imageViewer);
	//	if (viewer2 != NULL) {
	//		m_lumenWallContourRepresentation->SetRenderer(viewer2->GetAnnotationRenderer());
	//	}
	//	else {
	//		m_lumenWallContourRepresentation->SetRenderer(m_imageViewer->GetRenderer());
	//	}
	//	m_lumenWallContourRepresentation->SetNeedToRender(true);
	//	m_lumenWallContourRepresentation->GetLinesProperty()->SetColor(255, 0, 0);
	//	m_lumenWallContourRepresentation->SetLineInterpolator(this->m_interpolator);
	//	m_lumenWallContourRepresentation->AlwaysOnTopOn();
	//	m_lumenWallContourRepresentation->BuildRepresentation();

	//	m_lumenWallContourWidget = vtkContourWidget::New();
	//	m_lumenWallContourWidget->SetInteractor(this->Interactor);
	//	m_lumenWallContourWidget->SetRepresentation(m_lumenWallContourRepresentation);
	//	if (viewer2 != NULL) {
	//		m_lumenWallContourWidget->SetDefaultRenderer(viewer2->GetAnnotationRenderer());
	//	}
	//	else {
	//		m_lumenWallContourWidget->SetDefaultRenderer(m_imageViewer->GetRenderer());
	//	}
	//	m_lumenWallContourWidget->FollowCursorOn();
	//	m_lumenWallContourWidget->ContinuousDrawOn();
	//	m_lumenWallContourWidget->On();
	//	m_lumenWallContourWidget->EnabledOn();

	//	vtkSmartPointer<LumenSegmentaiton> ls =
	//		vtkSmartPointer<LumenSegmentaiton>::New();
	//	ls->SetInputData(m_imageViewer->GetInput());
	//	ls->SetSlice(GetSlice());
	//	ls->SetGenerateValues(1, m_generateValue, m_generateValue);
	//	ls->SetVesselWallContourRepresentation(this->m_vesselWallContourRepresentation);
	//	ls->Update();

	//	if (ls->GetOutput() == nullptr)
	//		return;

	//	// This prevent crash when a loop is not found #Issue7
	//	if (ls->GetOutput()->GetNumberOfPoints() == 0)
	//		return;

	//	m_lumenWallContourWidget->Initialize(ls->GetOutput());
	//	m_lumenWallContourWidget->CloseLoop();
	//	m_imageViewer->Render();
	//}
}

void InteractorStyleVesselSegmentation::FillPolygon()
{
	InteractorStylePolygonDraw::FillPolygon(
		&m_lumenWallContourWidgets, m_lumenWallLabel);
	InteractorStylePolygonDraw::FillPolygon(
		&m_vesselWallContourWidgets, m_vesselWallLabel);
	InteractorStylePolygonDraw::FillPolygon();

}


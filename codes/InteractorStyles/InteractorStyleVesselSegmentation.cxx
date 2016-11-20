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
#include "LumenSegmentationFilter2.h"
//#include "LumenSegmentation.h"

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
	InteractorStylePolygonDraw::OnMouseMove();
	// save the polydata all the time
	WriteToPolydata();
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
	ReadFromPolydata();
}

void InteractorStyleVesselSegmentation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	ReadFromPolydata();
}

void InteractorStyleVesselSegmentation::NewContour()
{
	// if user is manipulate a contour widget, the interactorstyle will not 
	// generate new contour widget
	if (m_currentContour != nullptr &&
		m_currentContour->GetWidgetState() != vtkContourWidget::Manipulate) {
		return;
	}
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
		// close loop the last contour
		if (m_lumenWallContourWidgets.size() != 0) {
			m_lumenWallContourWidgets.back()->CloseLoop();
		}
		m_lumenWallContourWidgets.push_back(vtkSmartPointer<vtkContourWidget>::New());
		m_currentContour = m_lumenWallContourWidgets.back();
		m_currentContour->SetRepresentation(
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New());

		m_currentContourRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(
			m_currentContour->GetContourRepresentation());
		m_currentContourRep->GetLinesProperty()->SetColor(255, 0, 0);

		m_currentContour->SetInteractor(this->Interactor);
		m_currentContour->SetCurrentRenderer(m_imageViewer->GetAnnotationRenderer());
		m_currentContour->ContinuousDrawOn();
		m_currentContour->FollowCursorOn();

		m_currentContourRep->SetLineInterpolator(m_interpolator);
		m_currentContourRep->AlwaysOnTopOn();
		m_currentContour->EnabledOn();
		break;
	default:
		break;
	}
}

void InteractorStyleVesselSegmentation::NewContour(int type, QList<vtkSmartPointer<vtkPolyData>>* list)
{

	for (QList<vtkSmartPointer<vtkPolyData>>::const_iterator cit = list->cbegin();
		cit != list->cend(); ++cit) {
		// skip strange polydate
		if ((*cit) != nullptr && (*cit)->GetNumberOfPoints() < 1) {
			continue;
		}
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
	ReadFromPolydata(1);
	ReadFromPolydata(2);
	m_currentContour = nullptr;
	m_imageViewer->Render();
}

void InteractorStyleVesselSegmentation::ReadFromPolydata(int type)
{
	if (m_contourType == CONTOUR) {
		return;
	}
	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY)
		return;
	CleanConoturs(type);
	Overlay* overlay = m_imageViewer->GetOverlay();
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* maps[2] = {
		overlay->GetVesselWallPolyData(),
		overlay->GetLumenPolyData()
	};

	if (maps[type - 1]->contains(GetSlice())) {
		QList<vtkSmartPointer<vtkPolyData>>* list = maps[type - 1]->value(GetSlice());
		NewContour(type, list);
	}
}

void InteractorStyleVesselSegmentation::WriteToPolydata()
{
	if (m_contourType == CONTOUR) {
		return;
	}
	//if (m_currentContour != nullptr &&
	//	m_currentContour->GetWidgetState() != vtkContourWidget::Manipulate) {
	//	return;
	//}
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

void InteractorStyleVesselSegmentation::SetGenerateValue(int value)
{
	this->m_generateValue = value;
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

void InteractorStyleVesselSegmentation::CleanAllConoturs()
{
	CleanConoturs(0);
	CleanConoturs(1);
	CleanConoturs(2);
	m_imageViewer->Render();

}

void InteractorStyleVesselSegmentation::CleanConoturs(int type)
{
	switch (type)
	{
	case CONTOUR:
		InteractorStylePolygonDraw::CleanAllConoturs();
		break;
	case VESSEL_WALL:
		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
			= m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend();
			++cit) {
			(*cit)->EnabledOff();
		}
		m_vesselWallContourWidgets.clear();
		break;
	case LUMEN:
		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
			= m_lumenWallContourWidgets.cbegin(); cit != m_lumenWallContourWidgets.cend();
			++cit) {
			(*cit)->EnabledOff();

		}
		m_lumenWallContourWidgets.clear();
		break;
	}

}

void InteractorStyleVesselSegmentation::GenerateLumenPolydata()
{
	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY) {
		return;
	}
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* lumenMap = 
		m_imageViewer->GetOverlay()->GetLumenPolyData();
	if (lumenMap->contains(GetSlice())) {
		delete  lumenMap->value(GetSlice());
	}
	(*lumenMap)[GetSlice()] = new QList<vtkSmartPointer<vtkPolyData>>;
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit =
		m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend(); ++cit) {
		if ((*cit)->GetContourRepresentation()->GetContourRepresentationAsPolyData()->GetNumberOfPoints() < 3) {
			continue;
		}
		vtkSmartPointer<LumenSegmentationFilter2> lsFilter =
			vtkSmartPointer<LumenSegmentationFilter2>::New();
		lsFilter->SetInputData(m_imageViewer->GetInput());
		lsFilter->SetGenerateValues(1,m_generateValue, m_generateValue);
		lsFilter->SetVesselWallContourRepresentation(
			vtkOrientedGlyphContourRepresentation::SafeDownCast((*cit)->GetContourRepresentation())
		);
		lsFilter->SetSlice(GetSlice());
		lsFilter->Update();
		(*(*lumenMap)[GetSlice()]) += lsFilter->GetOutput();
	}
	// Since the lumen polydata was wirten to the ovelay
	// Reload the polydata from the overlay and genenrate new contour widgets
	ReadFromPolydata(LUMEN);
	m_imageViewer->Render();
}

void InteractorStyleVesselSegmentation::FillPolygon()
{
	InteractorStylePolygonDraw::FillPolygon(
		&m_vesselWallContourWidgets, m_vesselWallLabel);
	InteractorStylePolygonDraw::FillPolygon(
		&m_lumenWallContourWidgets, m_lumenWallLabel);
	InteractorStylePolygonDraw::FillPolygon();

}


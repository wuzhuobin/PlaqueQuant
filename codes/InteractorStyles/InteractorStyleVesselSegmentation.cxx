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
#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCenterOfMass.h>
#include <vtkImageThreshold.h>

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
	if (b) {
		ReadFromPolydata();
	}
}

void InteractorStyleVesselSegmentation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	// in case for double re-reading (generate the same contour twice)
	// which made the contour blink and performance down
	int ijk[3];
	m_imageViewer->GetFocalPointWithImageCoordinate(ijk);
	if (i == ijk[0] && j == ijk[1] && k == ijk[2]) {
		return;
	}
	AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	STYLE_DOWN_CAST_CONSTITERATOR(InteractorStyleVesselSegmentation, ReadFromPolydata());
	//ReadFromPolydata();
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
	CleanContours(1);
	CleanContours(2);
	ReadFromPolydata(1);
	ReadFromPolydata(2);
	SetAllContoursEnabled(true);
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
	if (m_currentContour != nullptr &&
		m_currentContour->GetWidgetState() != vtkContourWidget::Manipulate) {
		CleanCurrentContour();
	}
	m_currentContour = nullptr;
	ReadFromPolydata();
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

void InteractorStyleVesselSegmentation::SetDilateValue(double value)
{
	m_dilateValue = value;
}

void InteractorStyleVesselSegmentation::CleanCurrentContour()
{

	list<vtkSmartPointer<vtkContourWidget>>* _contourss[] =
		{&m_contours, &m_vesselWallContourWidgets, &m_lumenWallContourWidgets};
	int i = 0;
	for (; i < 3; ++i) {
		if (_contourss[i]->size() > 0 &&
			_contourss[i]->back() == m_currentContour) {
			_contourss[i]->back()->EnabledOff();
			_contourss[i]->pop_back();
			break;
		}
	}
	if (_contourss[i]->size() == 0) {
		m_currentContour = nullptr;
	}
	else {
		m_currentContour = _contourss[i]->back();
	}
	m_imageViewer->Render();
}

void InteractorStyleVesselSegmentation::CleanAllContours()
{
	CleanContours(0);
	CleanContours(1);
	CleanContours(2);
	m_imageViewer->Render();

}

void InteractorStyleVesselSegmentation::CleanContours(int type)
{
	switch (type)
	{
	case CONTOUR:
		InteractorStylePolygonDraw::CleanAllContours();
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

void InteractorStyleVesselSegmentation::SetAllContoursEnabled(int flag)
{
	SetAllContoursEnabled(CONTOUR, flag);
	SetAllContoursEnabled(VESSEL_WALL, flag);
	SetAllContoursEnabled(LUMEN, flag);
}

void InteractorStyleVesselSegmentation::SetAllContoursEnabled(int type, int flag)
{
	switch (type)
	{
	case CONTOUR:
		InteractorStylePolygonDraw::SetAllContoursEnabled(flag);
		break;
	case VESSEL_WALL:
		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
			= m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend();
			++cit) {
			(*cit)->SetEnabled(flag);
		}
		break;
	case LUMEN:
		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
			= m_lumenWallContourWidgets.cbegin(); cit != m_lumenWallContourWidgets.cend();
			++cit) {
			(*cit)->SetEnabled(flag);
		}
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

		vtkSmartPointer<vtkPolyData> _lumenPolyData =
			vtkSmartPointer<vtkPolyData>::New();
		_lumenPolyData->ShallowCopy(lsFilter->GetOutput());

		(*(*lumenMap)[GetSlice()]) += _lumenPolyData;
	}
	// Since the lumen polydata was wirten to the ovelay
	// Reload the polydata from the overlay and genenrate new contour widgets
	CleanContours(LUMEN);
	ReadFromPolydata(LUMEN);
	m_imageViewer->Render();
}

void InteractorStyleVesselSegmentation::GenerateVesselWallPolyData()
{
	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY) {
		return;
	}
	const int* extent = m_imageViewer->GetOverlay()->GetOutput()->GetExtent();
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* vesselWallMap =
		m_imageViewer->GetOverlay()->GetVesselWallPolyData();
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* lumenMap =
		m_imageViewer->GetOverlay()->GetLumenPolyData();

	// since the upper range of the vtkMarchingSquare don't work at all
	// here it do manually thresholding for remove 
	vtkSmartPointer<vtkImageThreshold> imageThreshold =
		vtkSmartPointer<vtkImageThreshold>::New();
	//imageThreshold->ThresholdBetween(0.9, 1.1);
	imageThreshold->ThresholdByLower(1);
	imageThreshold->SetInputData(m_imageViewer->GetOverlay()->GetOutput());
	imageThreshold->ReplaceOutOn();
	imageThreshold->SetOutValue(0);
	imageThreshold->Update();

	for (int i = extent[4]; i <= extent[5]; ++i) {

		if (lumenMap->contains(i)) {
			delete  lumenMap->value(i);
		}
		if (vesselWallMap->contains(i)) {
			delete  vesselWallMap->value(i);
		}
		(*lumenMap)[i] = new QList<vtkSmartPointer<vtkPolyData>>;
		(*vesselWallMap)[i] = new QList<vtkSmartPointer<vtkPolyData>>;


		vtkSmartPointer<vtkMarchingSquares> marchingSquares =
			vtkSmartPointer<vtkMarchingSquares>::New();
		marchingSquares->SetInputConnection(imageThreshold->GetOutputPort());
		marchingSquares->CreateDefaultLocator();
		marchingSquares->SetImageRange(extent[0], extent[1], extent[2], extent[3], i, i);
		marchingSquares->SetValue(1, 1.00);
		marchingSquares->Update();

		vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
		connectivityFilter->SetExtractionModeToAllRegions();
		connectivityFilter->Update();

		for (int j = 0; j < connectivityFilter->GetNumberOfExtractedRegions(); ++j) {
			// extract lumen contour polydata
			vtkSmartPointer<vtkPolyDataConnectivityFilter> _connectivityFilter =
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
			_connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
			_connectivityFilter->AddSpecifiedRegion(j);
			_connectivityFilter->SetExtractionModeToSpecifiedRegions();
			_connectivityFilter->Update();

			LumenSegmentationFilter2::ReorderPolyData(_connectivityFilter->GetOutput());

			double toleranceInitial = 1;
			int loopBreaker = 0;

			vtkSmartPointer<vtkCleanPolyData> clearPolyData =
				vtkSmartPointer<vtkCleanPolyData>::New();
			clearPolyData->SetInputConnection(_connectivityFilter->GetOutputPort());
			clearPolyData->ToleranceIsAbsoluteOn();
			clearPolyData->SetAbsoluteTolerance(toleranceInitial);
			clearPolyData->PointMergingOn();
			clearPolyData->Update();
			while (clearPolyData->GetOutput()->GetNumberOfPoints() < 3 && loopBreaker < 10) {
				toleranceInitial *= 0.75;
				clearPolyData->SetAbsoluteTolerance(toleranceInitial);
				clearPolyData->Update();
				loopBreaker += 1;
			}

			vtkSmartPointer<vtkPolyData> _lumenPolyData =
				vtkSmartPointer<vtkPolyData>::New();
			_lumenPolyData->ShallowCopy(clearPolyData->GetOutput());
			(*(*lumenMap)[i]) += _lumenPolyData;


			// Dilate part
			// generate polydata 
			double center[3];
			vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
				vtkSmartPointer<vtkCenterOfMass>::New();
			centerOfMassFilter->SetInputData(_lumenPolyData);
			centerOfMassFilter->SetUseScalarsAsWeights(false);
			centerOfMassFilter->Update();
			centerOfMassFilter->GetCenter(center);

			vtkSmartPointer<vtkTransform> trans1 =
				vtkSmartPointer<vtkTransform>::New();
			trans1->Translate(-center[0], -center[1], -center[2]);
			vtkSmartPointer<vtkTransform> trans2 =
				vtkSmartPointer<vtkTransform>::New();
			trans2->Scale(1 + m_dilateValue, 1 + m_dilateValue, 1);
			trans2->Concatenate(trans1);
			vtkSmartPointer<vtkTransform> trans3 =
				vtkSmartPointer<vtkTransform>::New();
			trans3->Translate(center);
			trans3->Concatenate(trans2);
			trans3->Update();

			vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyDataFilter =
				vtkSmartPointer<vtkTransformPolyDataFilter>::New();
			transformPolyDataFilter->SetTransform(trans3);
			transformPolyDataFilter->SetInputData(_lumenPolyData);
			transformPolyDataFilter->Update();

			vtkSmartPointer<vtkPolyData> _vesselWalll =
				vtkSmartPointer<vtkPolyData>::New();
			_vesselWalll->ShallowCopy(transformPolyDataFilter->GetOutput());
			*(*vesselWallMap)[i] += _vesselWalll;
		}

	}
	CleanContours(LUMEN);
	CleanContours(VESSEL_WALL);

	ReadFromPolydata(LUMEN);
	ReadFromPolydata(VESSEL_WALL);
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


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
#include <vtkDijkstraImageContourLineInterpolator.h>	
#include <vtkProperty.h>
#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
//#include <vtkIncrementalOctreePointLocator.h>

#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkLookupTable.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkCommand.h>

#include <algorithm>

#include "ContourWidgetSeries.h"
#include "ContourWidgetSeriesOrientedGlyphRepresentation.h"
#include "MagneticContourLineInterpolator.h"
#include "InteractorStylePolygonDrawSeries.h"
#include "ImageViewer.h"

using namespace std;
vtkStandardNewMacro(InteractorStylePolygonDrawSeries);

InteractorStylePolygonDrawSeries::InteractorStylePolygonDrawSeries()
{
	m_contourSeriesRep = vtkSmartPointer<ContourWidgetSeriesOrientedGlyphRepresentation>::New();
	m_contourSeries = vtkSmartPointer<ContourWidgetSeries>::New();
	m_contourSeries->SetRepresentation(m_contourSeriesRep);
	m_contourSeries->ContinuousDrawOn();
	m_contourSeries->FollowCursorOn();
}

InteractorStylePolygonDrawSeries::~InteractorStylePolygonDrawSeries()
{

}

void InteractorStylePolygonDrawSeries::OnLeftButtonDown()
{
	AbstractNavigation::OnLeftButtonDown();
}

void InteractorStylePolygonDrawSeries::OnRightButtonDown()
{
	AbstractNavigation::OnRightButtonDown();
	//if (m_contourSeriesRep->GetActiveContour() != -1) {
	//	m_contourSeries->GetContour(m_contourSeriesRep->GetActiveContour())->CloseLoop();
	//}
}

void InteractorStylePolygonDrawSeries::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	
	if (key == "Return" ) {
		this->m_contourSeries->CloseAllLoops();
		this->FillContours();
	}
	else if (key == "l") {
		SetLinearInterpolator();
	}
	else if (key == "b") {
		SetBezierInterpolator();
	}
	else if (key == "d") {
		SetMagneticInterpolator();
	}
	else {
		AbstractNavigation::OnKeyPress();
	}
}


void InteractorStylePolygonDrawSeries::SetCustomEnabled(bool b)
{
	AbstractNavigation::SetCustomEnabled(b);
	if (b) {

		vtkSmartPointer<vtkImageActorPointPlacer> imagePointPlacer =
			vtkSmartPointer<vtkImageActorPointPlacer>::New();
		imagePointPlacer->SetImageActor(GetImageViewer()->GetImageActor());

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRep =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
		contourRep->SetPointPlacer(imagePointPlacer);
		//contourRep->ClosedLoopOn();

		m_contourSeriesRep->SetContourRepresentation(contourRep);
		SetContourLabel(m_contourLabel);
		SetInterpolator(m_interpolator);



		m_contourSeries->SetInteractor(this->Interactor);
		m_contourSeries->EnabledOn();



	}
	else {
		m_contourSeries->ResetContours();
		m_contourSeries->EnabledOff();
	}
}

void InteractorStylePolygonDrawSeries::SetContourLabel(int noSegLabel)
{
	this->m_contourLabel = noSegLabel;
	if (m_contourSeriesRep) {
		double colour[3] = { 0 };
		GetImageViewer()->GetLookupTable()->GetColor(static_cast<double>(noSegLabel), colour);
		
		vtkOrientedGlyphContourRepresentation::SafeDownCast(
			m_contourSeriesRep->ContourWidgetSeriesRepresentation::GetContourRepresentation())->GetLinesProperty()->SetColor(
				colour[0], 
				colour[1], 
				colour[2]);
	}
}

void InteractorStylePolygonDrawSeries::SetInterpolator(int interpolator)
{
	vtkSmartPointer<vtkContourLineInterpolator> lineInterpolator = nullptr;
	switch (interpolator)
	{
	case LINEAR:
		lineInterpolator = vtkSmartPointer<vtkLinearContourLineInterpolator>::New();
		break;
	case BEZIER:
		lineInterpolator = vtkSmartPointer<vtkBezierContourLineInterpolator>::New();
		break;
	case MAGNETIC:
		lineInterpolator = vtkSmartPointer<MagneticContourLineInterpolator>::New();
		//vtkDijkstraImageContourLineInterpolator::SafeDownCast(lineInterpolator)->GetDijkstraImageGeodesicPath()->SetCurvatureWeight(1);
		//vtkDijkstraImageContourLineInterpolator::SafeDownCast(lineInterpolator)->GetDijkstraImageGeodesicPath()->SetEdgeLengthWeight(1);
		//vtkDijkstraImageContourLineInterpolator::SafeDownCast(lineInterpolator)->GetDijkstraImageGeodesicPath()->SetImageWeight(0.001);
		break;
	default:
		break;
	}
	
	if (lineInterpolator) {
		m_contourSeriesRep->ContourWidgetSeriesRepresentation::GetContourRepresentation()->SetLineInterpolator(lineInterpolator);
	}
	m_interpolator = interpolator;
}

void InteractorStylePolygonDrawSeries::FillContours()
{
	FillContours(m_contourSeriesRep);
	//for (int i = 0; i < m_contourSeriesRep->GetNumberOfContours(); ++i) {
	//	
	//	vtkPolyData* _contour = m_contourSeriesRep->GetContourRepresentation(i)->GetContourRepresentationAsPolyData();
	//	double* colour = vtkOrientedGlyphContourRepresentation::SafeDownCast(m_contourSeriesRep->GetContourRepresentation(i))->
	//		GetLinesProperty()->GetColor();
	//	
	//	int index = -1;
	//	double lColour[3];
	//	do
	//	{
	//		++index;
	//		GetImageViewer()->GetLookupTable()->GetColor(index, lColour);
	//	} while (!std::equal(colour, colour + 3, lColour) && 
	//		index < GetImageViewer()->GetLookupTable()->GetNumberOfColors());

	//	vtkSmartPointer<vtkTransform> transform =
	//		vtkSmartPointer<vtkTransform>::New();
	//	transform->Scale(
	//		1 / GetSpacing()[0],
	//		1 / GetSpacing()[1],
	//		1 / GetSpacing()[2]
	//	);
	//	transform->Translate(
	//		-GetOrigin()[0],
	//		-GetOrigin()[1],
	//		-GetOrigin()[2]
	//	);

	//	double spacing[3];
	//	memcpy(spacing, GetSpacing(), sizeof(spacing));
	//	spacing[GetSliceOrientation()] = 0;

	//	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
	//		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	//	transformFilter->SetInputData(_contour);
	//	transformFilter->SetTransform(transform);
	//	transformFilter->Update();

	//	// in case the points are too cloese together
	//	// if it is too close, the PointInPolygon will go wrong
	//	vtkSmartPointer<vtkCleanPolyData> cleanPolyData =
	//		vtkSmartPointer<vtkCleanPolyData>::New();
	//	cleanPolyData->SetInputConnection(transformFilter->GetOutputPort());
	//	cleanPolyData->SetAbsoluteTolerance(sqrt(
	//		spacing[0] * spacing[0] + 
	//		spacing[1] * spacing[1] + 
	//		spacing[2] * spacing[2]
	//	));
	//	cleanPolyData->Update();

	//	// please only use vtkPoints#InsertNextPoint() or DeepCopy but not ShallowCopy
	//	vtkSmartPointer<vtkPolygon> polygon =
	//		vtkSmartPointer<vtkPolygon>::New();
	//	polygon->GetPoints()->DeepCopy(cleanPolyData->GetOutput()->GetPoints());
	//	//for (vtkIdType id = 0; id < cleanPolyData->GetOutput()->GetPoints()->GetNumberOfPoints(); ++id) {
	//	//	polygon->GetPoints()->InsertNextPoint(cleanPolyData->GetOutput()->GetPoints()->GetPoint(id));
	//	//}
	//	FillPolygon(polygon, index);

	//}
	GetImageViewer()->GetOverlay()->Modified();
	GetImageViewer()->GetOverlay()->InvokeEvent(vtkCommand::UpdateDataEvent);
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStylePolygonDrawSeries, GetImageViewer()->Render());
}

void InteractorStylePolygonDrawSeries::FillContours(ContourWidgetSeriesOrientedGlyphRepresentation * rep)
{
	for (int i = 0; i < rep->GetNumberOfContours(); ++i) {

		vtkPolyData* _contour = rep->GetContourRepresentation(i)->GetContourRepresentationAsPolyData();
		double* colour = vtkOrientedGlyphContourRepresentation::SafeDownCast(rep->GetContourRepresentation(i))->
			GetLinesProperty()->GetColor();

		int index = -1;
		double lColour[3];
		do
		{
			++index;
			GetImageViewer()->GetLookupTable()->GetColor(index, lColour);
		} while (!std::equal(colour, colour + 3, lColour) &&
			index < GetImageViewer()->GetLookupTable()->GetNumberOfColors());

		vtkSmartPointer<vtkTransform> transform =
			vtkSmartPointer<vtkTransform>::New();
		transform->Scale(
			1 / GetSpacing()[0],
			1 / GetSpacing()[1],
			1 / GetSpacing()[2]
		);
		transform->Translate(
			-GetOrigin()[0],
			-GetOrigin()[1],
			-GetOrigin()[2]
		);

		double spacing[3];
		memcpy(spacing, GetSpacing(), sizeof(spacing));
		spacing[GetSliceOrientation()] = 0;

		vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
			vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformFilter->SetInputData(_contour);
		transformFilter->SetTransform(transform);
		transformFilter->Update();

		// in case the points are too cloese together
		// if it is too close, the PointInPolygon will go wrong
		vtkSmartPointer<vtkCleanPolyData> cleanPolyData =
			vtkSmartPointer<vtkCleanPolyData>::New();
		cleanPolyData->SetInputConnection(transformFilter->GetOutputPort());
		cleanPolyData->SetAbsoluteTolerance(sqrt(
			spacing[0] * spacing[0] +
			spacing[1] * spacing[1] +
			spacing[2] * spacing[2]
		));
		cleanPolyData->Update();

		// please only use vtkPoints#InsertNextPoint() or DeepCopy but not ShallowCopy
		vtkSmartPointer<vtkPolygon> polygon =
			vtkSmartPointer<vtkPolygon>::New();
		polygon->GetPoints()->DeepCopy(cleanPolyData->GetOutput()->GetPoints());
		//for (vtkIdType id = 0; id < cleanPolyData->GetOutput()->GetPoints()->GetNumberOfPoints(); ++id) {
		//	polygon->GetPoints()->InsertNextPoint(cleanPolyData->GetOutput()->GetPoints()->GetPoint(id));
		//}
		FillPolygon(polygon, index);

	}
}

void InteractorStylePolygonDrawSeries::FillPolygon(vtkPolygon * polygon, int colour)
{
	int bounds[6] = { 0 };
	int orientation = 0;
	// doing clamping
	// since it is a 2D polygon in double but without tolerance setting
	// it need to find out polygon's orientation to avoid too accurate precision
	for (int boundIndex = 0; boundIndex < 3; ++boundIndex) {
		bounds[2 * boundIndex] =
			max(static_cast<int>(polygon->GetPoints()->GetBounds()[2 * boundIndex] + 0.5),
				GetImageViewer()->GetDisplayExtent()[2 * boundIndex]);
		bounds[2 * boundIndex + 1] =
			min(static_cast<int>(polygon->GetPoints()->GetBounds()[2 * boundIndex + 1] + 0.5),
				GetImageViewer()->GetDisplayExtent()[2 * boundIndex + 1]);
		if (bounds[2 * boundIndex + 1] == bounds[2 * boundIndex]) {
			orientation = boundIndex;
		}
	}

	double n[3];
	vtkPolygon::ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(),
		static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), n);
	
	// searching inside bounds
	for (int x = bounds[0]; x <= bounds[1]; x++) {
		for (int y = bounds[2]; y <= bounds[3]; y++) {
			for (int z = bounds[4]; z <= bounds[5]; z++) {
				double p[3] = {
					static_cast<double>(x),
					static_cast<double>(y),
					static_cast<double>(z) };
				// directly set its coordinate to polygon's bound to avoid over precision
				p[orientation] = polygon->GetPoints()->GetBounds()[2 * orientation];
				
				int inPolygon = vtkPolygon::PointInPolygon(p,
					polygon->GetPoints()->GetNumberOfPoints(), static_cast<double*>(
						polygon->GetPoints()->GetData()->GetVoidPointer(0)),
					polygon->GetPoints()->GetBounds(), n);

				if (inPolygon) {
					unsigned char * pixel = 
						static_cast<unsigned char *>(GetImageViewer()->GetOverlay()->GetScalarPointer(x, y, z));
					*pixel = colour;
				}
			}
		}
	}
}

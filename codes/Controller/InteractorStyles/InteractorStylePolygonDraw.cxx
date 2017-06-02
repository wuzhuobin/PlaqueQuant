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
#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkIncrementalOctreePointLocator.h>
#include <vtkObjectFactory.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>

#include <algorithm>

#include "InteractorStylePolygonDraw.h"
#include "ImageViewer.h"

using namespace std;
vtkStandardNewMacro(InteractorStylePolygonDraw);

InteractorStylePolygonDraw::InteractorStylePolygonDraw()
	:AbstractNavigation()
{
	this->m_interpolator = 
		vtkSmartPointer<vtkBezierContourLineInterpolator>::New();
}

InteractorStylePolygonDraw::~InteractorStylePolygonDraw()
{

}

void InteractorStylePolygonDraw::OnLeftButtonDown()
{
	AbstractNavigation::OnLeftButtonDown();
}

void InteractorStylePolygonDraw::OnRightButtonDown()
{
	AbstractNavigation::OnRightButtonDown();
	if (m_currentContour != nullptr) {
		m_currentContour->CloseLoop();
	}
}

void InteractorStylePolygonDraw::OnMouseMove()
{
	if (GetCustomEnabled()) {
		NewContour();
		m_currentContour->EnabledOn();
	}
}

void InteractorStylePolygonDraw::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	
	if (key == "Escape") {
		CleanAllContours();
	}
	else if (key == "Return" ) {
		this->FillPolygon();
	}
	else {
		AbstractNavigation::OnKeyPress();
	}
}


void InteractorStylePolygonDraw::SetCustomEnabled(bool b)
{
	AbstractNavigation::SetCustomEnabled(b);
	if (!b) {
		CleanAllContours();
		m_currentContour = nullptr;
	}
}

void InteractorStylePolygonDraw::NewContour()
{
	// if user is manipulate a contour widget, the interactorstyle will not 
	// generate new contour widget
	if (m_currentContour != nullptr &&
		m_currentContour->GetWidgetState() != vtkContourWidget::Manipulate) {
		return;
	}
	// close loop the last contour
	if (m_contours.size() != 0) {
		m_contours.back()->CloseLoop();
	}
	m_contours.push_back(vtkSmartPointer<vtkContourWidget>::New());
	m_currentContour = m_contours.back();
	m_currentContour->SetRepresentation(
		vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New());


	m_currentContour->SetInteractor(this->Interactor);
	m_currentContour->SetCurrentRenderer(GetImageViewer()->GetRenderer());
	m_currentContour->ContinuousDrawOn();
	m_currentContour->FollowCursorOn();

	m_currentContourRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(
		m_currentContour->GetContourRepresentation());
	m_currentContourRep->GetLinesProperty()->SetColor(0, 255, 0);
	m_currentContourRep->SetLineInterpolator(m_interpolator);
	m_currentContourRep->AlwaysOnTopOn();

	vtkSmartPointer<vtkImageActorPointPlacer> pointPlacer =
		vtkSmartPointer<vtkImageActorPointPlacer>::New();
	pointPlacer->SetImageActor(GetImageViewer()->GetImageActor());
	m_currentContourRep->SetPointPlacer(pointPlacer);
}


void InteractorStylePolygonDraw::SetContourLabel(unsigned char noSegLabel)
{
	this->m_contourLabel = noSegLabel;
}

void InteractorStylePolygonDraw::CleanCurrentContour()
{
	if (m_contours.size() > 0) {
		m_contours.back()->EnabledOff();
		m_contours.pop_back();
	}

	// none contours left, just set m_currentContour = nullptr
	if (m_contours.size() == 0) {
		m_currentContour = nullptr;
	}
	else {
		m_currentContour = m_contours.back();
	}
	GetImageViewer()->Render();

}

void InteractorStylePolygonDraw::CleanAllContours()
{

	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_contours.cbegin(); cit != m_contours.cend();
		++cit) {

		(*cit)->EnabledOff();

	}
	m_contours.clear();
	m_currentContour = nullptr;
	GetImageViewer()->Render();
}

void InteractorStylePolygonDraw::SetAllContoursEnabled(int flag)
{
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_contours.cbegin(); cit != m_contours.cend();
		++cit) {
		(*cit)->SetEnabled(flag);
	}
}

void InteractorStylePolygonDraw::SetSmoothCurveEnable()
{
	SetLineInterpolator(0);
}

void InteractorStylePolygonDraw::SetPolygonEnable()
{
	SetLineInterpolator(1);
}

void InteractorStylePolygonDraw::SetLineInterpolator(int i)
{
	switch (i)
	{
	case 0:
		this->m_interpolator = vtkSmartPointer<vtkBezierContourLineInterpolator>::New();
		break;
	case 1:
		this->m_interpolator = vtkSmartPointer<vtkLinearContourLineInterpolator>::New();
		break;
	}
	if (m_currentContour != nullptr &&
		m_currentContour->GetWidgetState() != vtkContourWidget::Manipulate) {
		CleanCurrentContour();
	}
}

void InteractorStylePolygonDraw::FillPolygon()
{
	FillPolygon(&m_contours, m_contourLabel);
}

void InteractorStylePolygonDraw::FillPolygon(
	std::list<vtkSmartPointer<vtkContourWidget>> * contour, unsigned char label)
{

	vtkSmartPointer<vtkPoints> fillPoints =
		vtkSmartPointer<vtkPoints>::New();

	list<vtkSmartPointer<vtkPolygon>> contourPolygons;

	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit = contour->cbegin();
		cit != contour->cend(); ++cit) {
		vtkPolyData* _polyData;
		if ((*cit) == nullptr) {
			continue;
		}
		if ((*cit)->GetContourRepresentation() == nullptr ||
			(*cit)->GetContourRepresentation()->GetNumberOfNodes() < 3) {
			continue;
		}
		// Check if contour is drawn correctly
		_polyData = (*cit)->GetContourRepresentation()->GetContourRepresentationAsPolyData();
		if (_polyData == nullptr || _polyData->GetNumberOfPoints() < 3) {
			vtkErrorMacro( << "_polyData is nullptr");
			continue;
		}
		(*cit)->CloseLoop();

		contourPolygons.push_back(vtkSmartPointer<vtkPolygon>::New());

		// Get the coordinates of the contour data points
		double lastPoint[3] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MAX, VTK_DOUBLE_MAX };
		for (vtkIdType i = 0; i < _polyData->GetNumberOfPoints(); i++)
		{
			double displayCoordinate[3];
			const double* worldCoordinate = _polyData->GetPoint(i);

			//Take one image data 1 to be reference
			displayCoordinate[0] = ((worldCoordinate[0] - GetOrigin()[0]) / GetSpacing()[0] );
			displayCoordinate[1] = ((worldCoordinate[1] - GetOrigin()[1]) / GetSpacing()[1] );
			displayCoordinate[2] = ((worldCoordinate[2] - GetOrigin()[2]) / GetSpacing()[2] );
			displayCoordinate[GetSliceOrientation()] = GetSlice();
			//cout << s[0] << " " << s[1] << " " << s[2] << endl;
			//Test whether the points are inside the polygon or not
			// if the points is too close to the previous point, skip it to avoid error in PointInPolygon algorithm
			double d = vtkMath::Distance2BetweenPoints(lastPoint, displayCoordinate);
			if (d < 1E-5)
				continue;
			memcpy(lastPoint, displayCoordinate, sizeof(displayCoordinate));
			// Because the index of the SliceOrientation is wrong in double
			// it need to be set manually
			contourPolygons.back()->GetPoints()->InsertNextPoint(displayCoordinate);

		}
	}

	FillPolygon(&contourPolygons, label );
}

void InteractorStylePolygonDraw::FillPolygon(
	std::list<vtkSmartPointer<vtkPolygon>>* contourPolygon, unsigned char label) {

	vtkSmartPointer<vtkPoints> fillPoints =
		vtkSmartPointer<vtkPoints>::New();

	for (list<vtkSmartPointer<vtkPolygon>>::const_iterator cit = contourPolygon->cbegin();
		cit != contourPolygon->cend(); ++cit) {
		if ((*cit) == nullptr || (*cit)->GetPoints()->GetNumberOfPoints() < 3) {
			continue;
		}
		//Test whether the points are inside the polygon or not
		double normalVector[3];
		(*cit)->ComputeNormal((*cit)->GetPoints()->GetNumberOfPoints(),
			static_cast<double*>((*cit)->GetPoints()->GetData()->GetVoidPointer(0)), normalVector);
		int bounds[6];
		std::copy((*cit)->GetPoints()->GetBounds(), (*cit)->GetPoints()->GetBounds() + 6, bounds);
			
		// doing clamping
		for (int boundIndex = 0; boundIndex < 3; ++boundIndex) {
			bounds[2 * boundIndex] =
				max(bounds[2 * boundIndex] - 3, GetExtent()[2 * boundIndex]);
			bounds[2 * boundIndex + 1] =
				min(bounds[2 * boundIndex + 1] + 3, GetExtent()[2 * boundIndex + 1]);
		}

		for (int x = bounds[0]; x <= bounds[1]; x++) {
			for (int y = bounds[2]; y <= bounds[3]; y++) {
				for (int z = bounds[4]; z <= bounds[5]; z++) {
					double p[3] = { 
						static_cast<double>(x), 
						static_cast<double>(y), 
						static_cast<double>(z) };
					if (vtkPolygon::PointInPolygon(p, (*cit)->GetPoints()->GetNumberOfPoints(),
						static_cast<double*>(
						(*cit)->GetPoints()->GetData()->GetVoidPointer(0)),
							(*cit)->GetPoints()->GetBounds(), normalVector)) {
						fillPoints->InsertNextPoint(p);
						unsigned char * pixel = static_cast<unsigned char *>(GetImageViewer()->GetOverlay()->GetScalarPointer(p[0], p[1], p[2]));
						*pixel = label;
					}
				}
			}
		}

	}

	if (fillPoints->GetNumberOfPoints() < 1)
		return;
	//m_imageViewer->GetOverlay()->SetPixels(fillPoints, (unsigned char)label);
	GetImageViewer()->GetOverlay()->Modified();

	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStylePolygonDraw, GetImageViewer()->Render());
}

void InteractorStylePolygonDraw::FillPolygon(
	std::list<vtkSmartPointer<vtkPolygon>>* contourPolygon, unsigned char label, int slice)
{
	vtkSmartPointer<vtkPoints> fillPoints =
		vtkSmartPointer<vtkPoints>::New();

	for (list<vtkSmartPointer<vtkPolygon>>::const_iterator cit = contourPolygon->cbegin();
		cit != contourPolygon->cend(); ++cit) {
		if ((*cit) == nullptr || (*cit)->GetPoints()->GetNumberOfPoints() < 3) {
			continue;
		}

		const double* lastPoint =
			(*cit)->GetPoints()->GetPoint((*cit)->GetPoints()->GetNumberOfPoints() - 1);
		const double* firstPoint = (*cit)->GetPoints()->GetPoint(0);
		// remove duplicate last point
		// without this, pointsInPolygon may be wrong
		while (std::equal(lastPoint, lastPoint + 3, firstPoint)) {
			(*cit)->GetPoints()->SetNumberOfPoints((*cit)->GetPoints()->GetNumberOfPoints() - 1);
		}
		//Test whether the points are inside the polygon or not
		double normalVector[3];
		(*cit)->ComputeNormal((*cit)->GetPoints()->GetNumberOfPoints(),
			static_cast<double*>((*cit)->GetPoints()->GetData()->GetVoidPointer(0)), normalVector);
		int bounds[6];

		std::copy((*cit)->GetBounds(), (*cit)->GetBounds() + 6, bounds);


		// doing clamping
		for (int boundIndex = 0; boundIndex < 3; ++boundIndex) {
			bounds[2 * boundIndex] =
				max(bounds[2 * boundIndex] - 3, GetExtent()[2 * boundIndex]);
			bounds[2 * boundIndex + 1] =
				min(bounds[2 * boundIndex + 1] + 3, GetExtent()[2 * boundIndex + 1]);
		}

		for (int x = bounds[0]; x <= bounds[1]; x++) {
			for (int y = bounds[2]; y <= bounds[3]; y++) {
				for (int z = bounds[4]; z <= bounds[5]; z++) {
					double p[3] = { 
						static_cast<double>(x), 
						static_cast<double>(y), 
						static_cast<double>(z) };
					if (vtkPolygon::PointInPolygon(p, (*cit)->GetPoints()->GetNumberOfPoints(),
						static_cast<double*>(
						(*cit)->GetPoints()->GetData()->GetVoidPointer(0)),
							(*cit)->GetPoints()->GetBounds(), normalVector)) {
						p[GetSliceOrientation()] = slice;
						fillPoints->InsertNextPoint(p[0], p[1], p[2]);
						unsigned char * pixel = static_cast<unsigned char *>(GetImageViewer()->GetOverlay()->GetScalarPointer(p[0], p[1], p[2]));
						*pixel = label;
					}
				}
			}
		}
	}
	if (fillPoints->GetNumberOfPoints() < 1)
		return;
	//m_imageViewer->GetOverlay()->SetPixels(fillPoints, (unsigned char)label);
	GetImageViewer()->GetOverlay()->Modified();
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStylePolygonDraw, GetImageViewer()->Render());

}

//void InteractorStylePolygonDraw::FillPolygon(
//	std::list<vtkSmartPointer<vtkContourWidget>> * contour, unsigned char label)
//{
//	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit = contour->cbegin();
//		cit != contour->cend(); ++cit) {
//		vtkPolyData* _polyData;
//		if ((*cit) == nullptr) {
//			continue;
//		}
//		if ((*cit)->GetContourRepresentation() == nullptr) {
//			continue;
//		}
//		_polyData = (*cit)->GetContourRepresentation()->GetContourRepresentationAsPolyData();
//		if (_polyData == nullptr || _polyData->GetNumberOfPoints() < 3) {
//			continue;
//		}
//		(*cit)->CloseAllLoops();
//
//		// Check if contour is drawn
//		if (_polyData->GetNumberOfPoints() == 0)
//			return;
//
//		vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
//		int numOfPoints = _polyData->GetNumberOfPoints();
//
//		// Get the coordinates of the contour data points
//
//		double lastPoint[3] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MAX, VTK_DOUBLE_MAX };
//		for (vtkIdType i = 0; i < numOfPoints; i++)
//		{
//			double worldCoordinate[3];
//			double displayCoordinate[3];
//			_polyData->GetPoint(i, worldCoordinate);
//
//			//Take one image data 1 to be reference
//			displayCoordinate[0] = (worldCoordinate[0] - GetOrigin()[0]) / GetSpacing()[0];
//			displayCoordinate[1] = (worldCoordinate[1] - GetOrigin()[1]) / GetSpacing()[1];
//			displayCoordinate[2] = (worldCoordinate[2] - GetOrigin()[2]) / GetSpacing()[2];
//			//cout << s[0] << " " << s[1] << " " << s[2] << endl;
//			//Test whether the points are inside the polygon or not
//			// if the points is too close to the previous point, skip it to avoid error in PointInPolygon algorithm
//			double d = vtkMath::Distance2BetweenPoints(lastPoint, displayCoordinate);
//			if (d < 1E-5)
//				continue;
//			memcpy(lastPoint, displayCoordinate, sizeof(double) * 3);
//			displayCoordinate[GetSliceOrientation()] = 0.0;
//			polygon->GetPoints()->InsertNextPoint(displayCoordinate);
//
//		}
//		//Test whether the points are inside the polygon or not
//		double n[3];
//		polygon->ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(),
//			static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), n);
//		double bounds[6];
//		int bounds_int[6];
//
//		polygon->GetPoints()->GetBounds(bounds);
//
//		bounds_int[0] = floor(bounds[0]) - 3;
//		bounds_int[1] = ceil(bounds[1]) + 3;
//		bounds_int[2] = floor(bounds[2]) - 3;
//		bounds_int[3] = ceil(bounds[3]) + 3;
//		bounds_int[4] = floor(bounds[4]) - 3;
//		bounds_int[5] = ceil(bounds[5]) + 3;
//
//		// Clamp values to within extent specified 
//		bounds_int[0] = { bounds_int[0] < this->GetExtent()[0] ? this->GetExtent()[0] : bounds_int[0] };
//		bounds_int[1] = { bounds_int[1] > this->GetExtent()[1] ? this->GetExtent()[1] : bounds_int[1] };
//		bounds_int[2] = { bounds_int[2] < this->GetExtent()[2] ? this->GetExtent()[2] : bounds_int[2] };
//		bounds_int[3] = { bounds_int[3] > this->GetExtent()[3] ? this->GetExtent()[3] : bounds_int[3] };
//		bounds_int[4] = { bounds_int[4] < this->GetExtent()[4] ? this->GetExtent()[4] : bounds_int[4] };
//		bounds_int[5] = { bounds_int[5] > this->GetExtent()[5] ? this->GetExtent()[5] : bounds_int[5] };
//
//		// for using overlay::SetPixels()
//
//
//		vtkSmartPointer<vtkPoints> fillPoints =
//			vtkSmartPointer<vtkPoints>::New();
//		bounds_int[GetSliceOrientation() * 2] = 0;
//		bounds_int[GetSliceOrientation() * 2 + 1] = 0;
//
//		for (int x = bounds_int[0]; x <= bounds_int[1]; x++) {
//			for (int y = bounds_int[2]; y <= bounds_int[3]; y++) {
//				for (int z = bounds_int[4]; z <= bounds_int[5]; z++) {
//					double p[3] = { x, y, z };
//					if (vtkPolygon::PointInPolygon(p, polygon->GetPoints()->GetNumberOfPoints(),
//						static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, n)) {
//						p[GetSliceOrientation()] = m_imageViewer->GetSlice();
//						fillPoints->InsertNextPoint(p[0], p[1], p[2]);
//					}
//				}
//			}
//		}
//
//		m_imageViewer->GetOverlay()->SetPixels(fillPoints, (unsigned char)label);
//
//	}
//
//
//	m_imageViewer->GetOverlay()->Modified();
//	for (std::list<MyImageViewer*>::iterator it = m_synchronalViewers.begin();
//		it != m_synchronalViewers.end(); ++it) {
//		(*it)->Render();
//	}
//
//
//}
//
//

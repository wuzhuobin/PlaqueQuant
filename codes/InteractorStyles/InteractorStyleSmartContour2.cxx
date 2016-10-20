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
#include <vtkBezierContourLineInterpolator.h>
#include <vtkProperty.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkCleanPolyData.h>

#include "InteractorStyleSmartContour2.h"
#include "MyImageViewer.h"

vtkStandardNewMacro(InteractorStyleSmartContour2);
using namespace std;

InteractorStyleSmartContour2::InteractorStyleSmartContour2()
	:AbstractInteractorStyleImage()
{

	m_timer.start();
	m_firstClickTimeStamp = m_timer.elapsed();



	//vtkSmartPointer<vtkNIFTIImageReader> reader =
	//	vtkSmartPointer<vtkNIFTIImageReader>::New();
	//reader->SetFileName("C:\\Users\\user\\Desktop\\test_data\\test_data\\lumen.nii");
	//reader->Update();
	//m_lumenImage = vtkImageData::New();
	//m_lumenImage->DeepCopy(reader->GetOutput());

	//reader->SetFileName("C:\\Users\\user\\Desktop\\test_data\\test_data\\lumen_dilated.nii");
	//reader->Update();
	//m_vesselWallImage = vtkImageData::New();
	//m_vesselWallImage->DeepCopy(reader->GetOutput());
}

InteractorStyleSmartContour2::~InteractorStyleSmartContour2()
{
	ClearAllContour();
	m_imageViewer->Render();
}

void InteractorStyleSmartContour2::OnLeftButtonDown()
{
	if (this->CheckDoubleClicked() && m_ContourIsOnFlag) {
	}
	else if (!m_ContourIsOnFlag){
	}
	AbstractInteractorStyleImage::OnLeftButtonDown();
}

void InteractorStyleSmartContour2::OnRightButtonDown()
{
	if (m_ContourIsOnFlag ) {

	}

	AbstractInteractorStyleImage::OnRightButtonDown();
}

void InteractorStyleSmartContour2::OnMouseMove()
{
	if (!this->m_ContourIsOnFlag) {
		this->SetPolygonModeEnabled(true);
		this->m_ContourIsOnFlag = true;
	}
}

void InteractorStyleSmartContour2::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	
	if (key == "Escape") {
		this->ClearAllContour();
	}
	else if (key == "A") {
		GenerateContourWidget();
	}
	else if (key == "Return" ) {
		if (m_ContourIsOnFlag)
			this->FillPolygon();
	}
	AbstractInteractorStyleImage::OnKeyPress();
}

bool InteractorStyleSmartContour2::CheckDoubleClicked()
{
	int t = m_timer.elapsed() - m_firstClickTimeStamp;

	if (t < 200 && !m_doubleClickedFlag) {
		m_doubleClickedFlag = true;
		m_firstClickTimeStamp = m_timer.elapsed();
		return true;
	}
	else {
		m_doubleClickedFlag = false;
		m_firstClickTimeStamp = m_timer.elapsed();
		return false;
	}
}

void InteractorStyleSmartContour2::SetLumenImage(vtkImageData * lumen)
{
	this->m_lumenImage = lumen;
}

void InteractorStyleSmartContour2::SetVesselWallImage(vtkImageData * vesslWall)
{
	this->m_vesselWallImage = vesslWall;
}

void InteractorStyleSmartContour2::SetPolygonModeEnabled(bool b)
{
	cout << m_lumenWallContourWidgets.size() << endl;
	cout << m_vesselWallContourWidgets.size() << endl;
	m_ContourIsOnFlag = b;
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit1
		= m_lumenWallContourWidgets.cbegin(); cit1 != m_lumenWallContourWidgets.cend();
		++cit1) {
		if (b) {
			(*cit1)->SetInteractor(this->Interactor);
		}
		(*cit1)->SetEnabled(b);

	}

	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit2
		= m_vesselWallContourWidgets.cbegin(); cit2 != m_vesselWallContourWidgets.cend();
		++cit2) {
		if (b) {
			(*cit2)->SetInteractor(this->Interactor);
		}
		(*cit2)->SetEnabled(b);
	}
	m_imageViewer->Render();
}

void InteractorStyleSmartContour2::SetVesselWallLabel(int vesselWallLabel)
{
	this->vesselWallLabel = vesselWallLabel;
}

void InteractorStyleSmartContour2::SetLumenWallLabel(int lumenWallLabel)
{
	this->lumenWallLabel = lumenWallLabel;
}

void InteractorStyleSmartContour2::GenerateContourWidget()
{
	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY) {
		return;
	}
	//m_lumenImage = m_imageViewer->GetInputLayer();
	m_vesselWallImage = m_imageViewer->GetInputLayer();

	vtkImageData* images[2] = { m_lumenImage, m_vesselWallImage };
	list<vtkSmartPointer<vtkContourWidget>>* lists[2] =
	{ &m_lumenWallContourWidgets, &m_vesselWallContourWidgets };

	for (int i = 0; i < 2; ++i)	{
		if (images[i] == nullptr) {
			continue;
		}
		int extent[6];
		images[i]->GetExtent(extent);
		extent[GetSliceOrientation() * 2] = GetSlice();
		extent[GetSliceOrientation() * 2 + 1] = GetSlice();

		vtkSmartPointer<vtkExtractVOI> extractVOI =
			vtkSmartPointer<vtkExtractVOI>::New();
		extractVOI->SetInputData(images[i]);
		extractVOI->SetVOI(extent);
		extractVOI->Update();

		vtkSmartPointer<vtkContourFilter> contourFilter =
			vtkSmartPointer<vtkContourFilter>::New();
		contourFilter->SetInputConnection(extractVOI->GetOutputPort());
		contourFilter->GenerateValues(1, 1, 1);
		contourFilter->Update();

		vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivity = 
			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		connectivity->SetInputConnection(contourFilter->GetOutputPort());
		connectivity->SetExtractionModeToAllRegions();
		connectivity->Update();
		for (int j = 0; j < connectivity->GetNumberOfExtractedRegions(); ++j) {
			vtkSmartPointer<vtkPolyDataConnectivityFilter> _connectivity =
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
			_connectivity->SetInputConnection(contourFilter->GetOutputPort());
			_connectivity->SetExtractionModeToSpecifiedRegions();
			_connectivity->InitializeSpecifiedRegionList();
			_connectivity->AddSpecifiedRegion(j);
			_connectivity->Update();

			vtkSmartPointer<vtkCleanPolyData> clearPolyData =
				vtkSmartPointer<vtkCleanPolyData>::New();
			clearPolyData->SetInputConnection(_connectivity->GetOutputPort());
			//clearPolyData->SetTolerance(0.1);
			clearPolyData->PointMergingOn();
			clearPolyData->Update();

			vtkSmartPointer<vtkContourWidget> newWidget = MyContourWidgetFactory(i);
			MyImageViewer* viewer = dynamic_cast<MyImageViewer*>(m_imageViewer);
			if (viewer != NULL) {
				cout << "viewer" << endl;
				newWidget->SetCurrentRenderer(viewer->GetAnnotationRenderer());
				newWidget->GetContourRepresentation()->
					SetRenderer(viewer->GetAnnotationRenderer());
			}
			else {
				newWidget->SetCurrentRenderer(m_imageViewer->GetRenderer());
				newWidget->GetContourRepresentation()->
					SetRenderer(m_imageViewer->GetRenderer());
			}

			newWidget->Initialize(clearPolyData->GetOutput());
			lists[i]->push_back(newWidget);
		}

	}
	SetPolygonModeEnabled(true);
}

void InteractorStyleSmartContour2::ClearAllContour()
{
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit1
		= m_lumenWallContourWidgets.cbegin(); cit1 != m_lumenWallContourWidgets.cend();
		++cit1) {

		(*cit1)->EnabledOff();

	}

	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit2
		= m_vesselWallContourWidgets.cbegin(); cit2 != m_vesselWallContourWidgets.cend();
		++cit2) {

		(*cit2)->EnabledOff();
	}

	m_lumenWallContourWidgets.clear();
	m_vesselWallContourWidgets.clear();
	m_imageViewer->Render();
}

vtkSmartPointer<vtkContourWidget> InteractorStyleSmartContour2::MyContourWidgetFactory(int type)
{
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
		vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	contourRepresentation->AlwaysOnTopOn();
	switch (type)
	{
	case LUMEN:
		contourRepresentation->GetLinesProperty()->SetColor(255, 0, 0);
		break;
	case VESSEL_WALL:
		contourRepresentation->GetLinesProperty()->SetColor(0, 0, 255);
		break;
	}

	vtkSmartPointer<vtkContourWidget> newWidget =
		vtkSmartPointer<vtkContourWidget>::New();
	newWidget->SetRepresentation(contourRepresentation);
	newWidget->ContinuousDrawOn();
	//MyImageViewer* viewer = dynamic_cast<MyImageViewer*>(m_imageViewer);
	//if (viewer != NULL) {
	//	cout << "viewer" << endl;
	//	newWidget->GetContourRepresentation()->SetRenderer(viewer->GetAnnotationRenderer());
	//}
	//else {
	//	newWidget->GetContourRepresentation()->SetRenderer(m_imageViewer->GetRenderer());
	//}

	contourRepresentation->SetLineInterpolator(
		vtkSmartPointer<vtkBezierContourLineInterpolator>::New());
	return newWidget;
}

void InteractorStyleSmartContour2::FillPolygon()
{
	list<vtkSmartPointer<vtkContourWidget>>* lists[2] =
	{ &m_lumenWallContourWidgets, &m_vesselWallContourWidgets };
	int label[2] = { lumenWallLabel , vesselWallLabel };
	for (int i = 0; i < 2; ++i) {
		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit = lists[i]->cbegin();
			cit != lists[i]->cend(); ++cit) {
			
			(*cit)->CloseLoop();

			vtkPolyData* polydata = (*cit)->GetContourRepresentation()
				->GetContourRepresentationAsPolyData();

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
		
		

	}

	m_imageViewer->GetInputLayer()->Modified();
	for (std::list<MyImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		(*it)->Render();
	}


}


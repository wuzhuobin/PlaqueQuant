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
#include <vtkCenterOfMass.h>
#include <vtkKdtree.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkLine.h>

#include "InteractorStyleSmartContour2.h"
#include "MyImageViewer.h"

vtkStandardNewMacro(InteractorStyleSmartContour2);
using namespace std;

InteractorStyleSmartContour2::InteractorStyleSmartContour2()
	:AbstractInteractorStyleImage()
{
	this->m_lastSlice = -1;
}

InteractorStyleSmartContour2::~InteractorStyleSmartContour2()
{
	ClearPolyDataList();
}

void InteractorStyleSmartContour2::OnLeftButtonDown()
{
	AbstractInteractorStyleImage::OnLeftButtonDown();
}

void InteractorStyleSmartContour2::OnRightButtonDown()
{
	AbstractInteractorStyleImage::OnRightButtonDown();
}

void InteractorStyleSmartContour2::OnMouseMove()
{
	//if (!this->m_ContourIsOnFlag) {
	//	this->SetSmartContour2Enable(true);
	//	this->m_ContourIsOnFlag = true;
	//}
	Superclass::OnMouseMove();
}

void InteractorStyleSmartContour2::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	
	if (key == "Escape") {
		this->ClearAllContour();
	}
	else if (key == "A") {
		GeneratePolyData();
	}
	else if (key == "C") {
		SetSmartContour2Enable(false);
	}
	else if (key == "Return" ) {
		if (m_ContourIsOnFlag)
			this->FillPolygon();
	}
	AbstractInteractorStyleImage::OnKeyPress();
}

void InteractorStyleSmartContour2::SetLumenImage(vtkSmartPointer<vtkImageData> lumen)
{
	this->m_lumenImage = lumen;
}

void InteractorStyleSmartContour2::SetVesselWallImage(vtkSmartPointer<vtkImageData> vesslWall)
{
	this->m_vesselWallImage = vesslWall;
}

void InteractorStyleSmartContour2::SetSmartContour2Enable(bool b)
{
	m_ContourIsOnFlag = b;
	if (b) {
		ClearAllContour();
		GeneratePolyData();
		GenerateContour();
	}
	else {
		//for (vector<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		//	= m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend();
		//	++cit) {

		//	(*cit)->EnabledOff();
		//}
		//for (vector<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		//	= m_lumenWallContourWidgets.cbegin(); cit != m_lumenWallContourWidgets.cend();
		//	++cit) {

		//	(*cit)->EnabledOff();

		//}

		ClearAllContour();

	}
	m_imageViewer->Render();
}

void InteractorStyleSmartContour2::SetVesselWallLabel(int vesselWallLabel)
{
	this->vesselWallLabel = vesselWallLabel;
}

void InteractorStyleSmartContour2::SetLumenWallLabel(int lumenWallLabel)
{
	this->lumenLabel = lumenWallLabel;
}

void InteractorStyleSmartContour2::ClearPolyDataList()
{

	for (vector<vector<vtkSmartPointer<vtkPolyData>>*>::iterator it1 =
		m_vesselWallPolyData.begin(); it1 != m_vesselWallPolyData.end(); ++it1) {

		(*it1)->clear();
		delete (*it1);

		//for (vector<vtkSmartPointer<vtkPolyData>>::iterator it2 =
		//	(*it1)->begin(); it2 != (*it1)->end(); ++it2) {

		//}
	}
	m_vesselWallPolyData.clear();

	for (vector<vector<vtkSmartPointer<vtkPolyData>>*>::iterator it1 =
		m_lumenPolyData.begin(); it1 != m_lumenPolyData.end(); ++it1) {

		(*it1)->clear();
		delete (*it1);

		//for (vector<vtkSmartPointer<vtkPolyData>>::iterator it2 =
		//	(*it1)->begin(); it2 != (*it1)->end(); ++it2) {

		//}
	}
	m_lumenPolyData.clear();
}

void InteractorStyleSmartContour2::GeneratePolyData()
{
	ClearPolyDataList();
	int extent[6];
	m_lumenImage->GetExtent(extent);
	for (int i = extent[4]; i <= extent[5]; i++) {
		GeneratePolyData(i);
	}
}

void InteractorStyleSmartContour2::GeneratePolyData(int slice)
{
	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY) {
		return;
	}

	vtkImageData* images[2] = { m_vesselWallImage, m_lumenImage };
	vector<vtkSmartPointer<vtkContourWidget>>* lists[2] =
	{ &m_vesselWallContourWidgets, &m_lumenWallContourWidgets };
	vector<vector<vtkSmartPointer<vtkPolyData>>*>* polydataList[2] =
	{&m_vesselWallPolyData, &m_lumenPolyData};

	for (int i = 0; i < 2; ++i)	{
		if (images[i] == nullptr) {
			continue;
		}
		int extent[6];
		images[i]->GetExtent(extent);

		vtkSmartPointer<vtkExtractVOI> extractVOI =
			vtkSmartPointer<vtkExtractVOI>::New();
		extractVOI->SetInputData(images[i]);
		extractVOI->SetVOI(extent[0], extent[1], extent[2], extent[3], slice, slice);
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

		// slice index of vector must start from 0
		int realSlice = slice - extent[4];
		// Make Sure there are enough vectors for all slices
		while (polydataList[i]->size() < (realSlice + 1)){
			polydataList[i]->push_back(new vector<vtkSmartPointer<vtkPolyData>>);
		}

		for (int j = 0; j < connectivity->GetNumberOfExtractedRegions(); ++j) {
			vtkSmartPointer<vtkPolyDataConnectivityFilter> _connectivity =
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
			_connectivity->SetInputConnection(contourFilter->GetOutputPort());
			_connectivity->SetExtractionModeToSpecifiedRegions();
			_connectivity->InitializeSpecifiedRegionList();
			_connectivity->AddSpecifiedRegion(j);
			_connectivity->Update();


			ResequenceLumenWallPolyData(_connectivity->GetOutput());

			double toleranceInitial = 1;
			int loopBreaker = 0;
			vtkSmartPointer<vtkCleanPolyData> clearPolyData =
				vtkSmartPointer<vtkCleanPolyData>::New();
			clearPolyData->SetInputConnection(_connectivity->GetOutputPort());
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

			if (clearPolyData->GetOutput()->GetNumberOfPoints() >= 3)
				(*polydataList[i])[slice]->push_back(clearPolyData->GetOutput());
		}

	}
}

void InteractorStyleSmartContour2::GenerateContour()
{
	if (!m_ContourIsOnFlag) {
		return;
	}
	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY) {
		return;
	}
	vtkImageData* images[2] = { m_vesselWallImage, m_lumenImage };
	vector<vtkSmartPointer<vtkContourWidget>>* lists[2] =
	{ &m_vesselWallContourWidgets, &m_lumenWallContourWidgets };
	vector<vector<vtkSmartPointer<vtkPolyData>>*>* polydataList[2] =
	{ &m_vesselWallPolyData, &m_lumenPolyData };

	// Save current slice contour widget to polydatalist
	if (this->m_lastSlice != -1) 
	{
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < lists[i]->size(); j++) {
				vtkSmartPointer<vtkContourRepresentation> rep = lists[i]->at(j)->GetContourRepresentation();
				vtkSmartPointer<vtkPolyData> repPD = vtkSmartPointer<vtkPolyData>::New();
				rep->GetNodePolyData(repPD);
				if (repPD->GetNumberOfPoints() == 0) 
				{
					(*polydataList[i])[this->m_lastSlice]->erase((*polydataList[i])[this->m_lastSlice]->begin() + j);
					lists[i]->at(j)->EnabledOff();
					lists[i]->erase(lists[i]->begin() + j);
				}
				else {
					(*polydataList[i])[this->m_lastSlice]->at(j)->DeepCopy(repPD);
				}
			}
		}
	}


	this->ClearAllContour();
	int realSlice = GetSlice() - GetExtent()[4];
	// build the contour with current polydata
	for (int i = 0; i < 2; ++i) {

		for (vector<vtkSmartPointer<vtkPolyData>>::const_iterator cit =
			(*polydataList[i])[GetSlice()]->cbegin();
			cit != (*polydataList[i])[GetSlice()]->cend(); ++cit) {

			vtkSmartPointer<vtkContourWidget> newWidget = MyContourWidgetFactory(i);
			newWidget->SetCurrentRenderer(m_imageViewer->GetAnnotationRenderer());
			newWidget->GetContourRepresentation()->
				SetRenderer(m_imageViewer->GetAnnotationRenderer());
			newWidget->Initialize(*cit);
			newWidget->CloseLoop();
			newWidget->SetInteractor(this->Interactor);
			newWidget->EnabledOn();
			newWidget->SetReferenceCount(10);
			lists[i]->push_back(newWidget);
		}


	}
	this->m_lastSlice = realSlice;
	m_imageViewer->Render();

}

void InteractorStyleSmartContour2::ClearAllContour()
{
	for (vector<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend();
		++cit) {

		(*cit)->EnabledOff();
	}
	for (vector<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_lumenWallContourWidgets.cbegin(); cit != m_lumenWallContourWidgets.cend();
		++cit) {

		(*cit)->EnabledOff();

	}

	m_vesselWallContourWidgets.clear();
	m_lumenWallContourWidgets.clear();
	m_imageViewer->Render();
}

void InteractorStyleSmartContour2::ResequenceLumenWallPolyData(vtkPolyData * lumenWallPolyData)
{

		//// Restructure lines only so that adjacent point has incremental vtkID
		double referenceDirection[3] = { 1,2,3 };

		vtkSmartPointer<vtkPolyData> tmpHolder = vtkSmartPointer<vtkPolyData>::New();
		vtkSmartPointer<vtkPoints> tmpPts = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> tmpCellArr = vtkSmartPointer<vtkCellArray>::New();

		// Clean the line before reconstruction to remove duplicated points and cells, which mess with the program
		vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
		cleaner->SetInputData(lumenWallPolyData);
		cleaner->PointMergingOn();
		cleaner->ConvertLinesToPointsOn();
		cleaner->ConvertPolysToLinesOn();
		cleaner->Update();
		lumenWallPolyData->DeepCopy(cleaner->GetOutput());
		lumenWallPolyData->BuildLinks(); // build link for pushing cells

		int l_nextCell;
		int l_nextPt;
		int l_looper = 0;
		int startID = 0;
		int l_prevID = -1;
		// Start from a point that has cell
		vtkIdList* startPointFinder = vtkIdList::New();
		lumenWallPolyData->GetCellPoints(0, startPointFinder);
		l_looper = startPointFinder->GetId(1);
		startID = l_looper;
		tmpPts->InsertNextPoint(lumenWallPolyData->GetPoint(l_looper));


		while (tmpPts->GetNumberOfPoints() < lumenWallPolyData->GetNumberOfPoints()) {
			vtkIdList* neighborPtID = vtkIdList::New();
			vtkIdList* neighborCellID = vtkIdList::New();
			// First point deside direction
			if (tmpPts->GetNumberOfPoints() == 1) {
				// Get CenterOfMass
				vtkSmartPointer<vtkCenterOfMass> comfilter = vtkSmartPointer<vtkCenterOfMass>::New();
				comfilter->SetInputData(lumenWallPolyData);
				comfilter->Update();
				double* com = comfilter->GetCenter();

				// Get neighbor cells
				lumenWallPolyData->GetPointCells(l_looper, neighborCellID);
				//cout « "Number of cell for pt " « l_looper « ": " « neighborCellID->GetNumberOfIds() « " "; //DEBUG
				double *vectorLooper = (double*)malloc(sizeof(double) * 3);
				double *vector1 = (double*)malloc(sizeof(double) * 3);
				double *vector2 = (double*)malloc(sizeof(double) * 3);
				vtkIdType neighborPtId1;
				vtkIdType neighborPtId2;

				lumenWallPolyData->GetCellPoints(neighborCellID->GetId(0), neighborPtID);
				if (neighborPtID->GetId(0) != l_looper) {
					neighborPtId1 = neighborPtID->GetId(0);
				}
				else {
					neighborPtId1 = neighborPtID->GetId(1);
				}
				lumenWallPolyData->GetCellPoints(neighborCellID->GetId(1), neighborPtID);
				if (neighborPtID->GetId(0) != l_looper) {
					neighborPtId2 = neighborPtID->GetId(0);
				}
				else {
					neighborPtId2 = neighborPtID->GetId(1);
				}

				memcpy(vectorLooper, lumenWallPolyData->GetPoint(l_looper), sizeof(double) * 3);
				memcpy(vector1, lumenWallPolyData->GetPoint(neighborPtId1), sizeof(double) * 3);
				memcpy(vector2, lumenWallPolyData->GetPoint(neighborPtId2), sizeof(double) * 3);

				vtkMath::Subtract(vectorLooper, com, vectorLooper);
				vtkMath::Subtract(vector1, com, vector1);
				vtkMath::Subtract(vector2, com, vector2);
				vtkMath::Normalize(vector1);
				vtkMath::Normalize(vector2);

				vtkMath::Cross(vector1, vectorLooper, vector1);
				vtkMath::Cross(vector2, vectorLooper, vector2);

				double d1 = vtkMath::Dot(vector1, referenceDirection);
				double d2 = vtkMath::Dot(vector2, referenceDirection);

				if (d1 >= d2) {
					// if d1 follows direction
					l_prevID = neighborCellID->GetId(1);
				}
				else if (d1 < d2) {
					// if d2 follows direction
					l_prevID = neighborCellID->GetId(0);
				}
				else if (d1*d2 > 0) {
					// both same sign
					std::cerr << "Both neighbor has same dot product with direction. Potential error of loop order direction" << endl;
					l_prevID = neighborCellID->GetId(0);
				}
				else {
					l_prevID = neighborCellID->GetId(0);
				}

				free(vectorLooper);
				free(vector1);
				free(vector2);
			}

			// Get neighbor cells
			lumenWallPolyData->GetPointCells(l_looper, neighborCellID);
			//cout « "Number of cell for pt " « l_looper « ": " « neighborCellID->GetNumberOfIds() « " " « endl; //DEBUG
			// if it only has one cell
			if (neighborCellID->GetNumberOfIds() == 1) {
				// attemps to connect back to the loop
				std::cerr << "Warning! Your input is not a loop, attempting to connect gap at ptID=" << l_looper << "." << endl;

				// Get other point of the cell
				vtkIdType l_prevPtID;
				lumenWallPolyData->GetCellPoints(l_looper, neighborPtID);
				if (neighborPtID->GetId(0) != l_looper) {
					l_prevPtID = neighborPtID->GetId(0);
				}
				else {
					l_prevPtID = neighborPtID->GetId(1);
				}

				vtkSmartPointer<vtkKdTree> l_kd = vtkSmartPointer<vtkKdTree>::New();
				l_kd->BuildLocatorFromPoints(lumenWallPolyData);
				l_kd->FindClosestNPoints(3, lumenWallPolyData->GetPoint(l_looper), neighborPtID);
				// Get points of neighbor cells
				vtkIdType l_tempLink[2];
				l_tempLink[0] = l_looper;
				for (int i = 0; i < 3; i++)
				{
					if (neighborPtID->GetId(i) != l_looper && neighborPtID->GetId(i) != l_prevPtID) {
						l_looper = neighborPtID->GetId(i);
						tmpPts->InsertNextPoint(lumenWallPolyData->GetPoint(l_looper));
					}
				}

				l_tempLink[1] = l_looper;
				l_prevID = lumenWallPolyData->InsertNextLinkedCell(VTK_LINE, 2, l_tempLink);
			}
			else {
				if (neighborCellID->GetId(0) != l_prevID) {
					l_nextCell = neighborCellID->GetId(0);
					l_prevID = l_nextCell;
				}
				else {
					l_nextCell = neighborCellID->GetId(1);
					l_prevID = l_nextCell;
				}
				// Get points of neighbor cells
				lumenWallPolyData->GetCellPoints(l_nextCell, neighborPtID);
				//cout « "Number of pt for cell " « l_nextCell « ": " « neighborPtID->GetNumberOfIds() « "\n"; //DEBUG
				if (neighborPtID->GetId(0) != l_looper) {
					l_looper = neighborPtID->GetId(0);
					tmpPts->InsertNextPoint(lumenWallPolyData->GetPoint(l_looper));
				}
				else
				{
					l_looper = neighborPtID->GetId(1);
					tmpPts->InsertNextPoint(lumenWallPolyData->GetPoint(l_looper));
				}
			}

			// Push the line into the new polydata
			vtkSmartPointer<vtkLine> l_line = vtkSmartPointer<vtkLine>::New();
			l_line->GetPointIds()->SetId(0, tmpPts->GetNumberOfPoints() - 2);
			l_line->GetPointIds()->SetId(1, tmpPts->GetNumberOfPoints() - 1);
			tmpCellArr->InsertNextCell(l_line);

			neighborPtID->Delete();
			neighborCellID->Delete();

			if (l_looper == startID) {
				// Break if reaching the end
				break;
			}
		}
		vtkSmartPointer<vtkLine> tmpline = vtkSmartPointer<vtkLine>::New();
		tmpline->GetPointIds()->SetId(0, tmpPts->GetNumberOfPoints() - 1);
		tmpline->GetPointIds()->SetId(1, 0);
		tmpCellArr->InsertNextCell(tmpline);

		tmpHolder->SetPoints(tmpPts);
		tmpHolder->SetLines(tmpCellArr);

		// Copy result to lumenWallPolyData
		lumenWallPolyData->DeepCopy(tmpHolder);

}

void InteractorStyleSmartContour2::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	Superclass::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	GenerateContour();
}


void InteractorStyleSmartContour2::FillPolygon()
{
	vector<vtkSmartPointer<vtkContourWidget>>* lists[2] =
	{ &m_vesselWallContourWidgets, &m_lumenWallContourWidgets };

	int label[2] = { vesselWallLabel , lumenLabel };
	for (int i = 0; i < 2; ++i) {
		for (vector<vtkSmartPointer<vtkContourWidget>>::const_iterator cit = lists[i]->cbegin();
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
			//bounds_int[GetSliceOrientation() * 2] = 0;
			//bounds_int[GetSliceOrientation() * 2 + 1] = 0;

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

void InteractorStyleSmartContour2::FillAllPolygons()
{
	// Only do for z-axis
	if (this->m_imageViewer->GetSliceOrientation() != 2)
		return;

	this->m_imageViewer->SetSlice(0);
	this->GenerateContour();
	int numOfSliceToProcess = { this->m_vesselWallPolyData.size() > this->m_lumenPolyData.size() ?
						int(this->m_vesselWallPolyData.size()) : int(this->m_lumenPolyData.size()) };

	for (int i = 0; i < numOfSliceToProcess;i++)
	{
		this->FillPolygon();
		this->OnMouseWheelForward();
	}
	this->FillPolygon();
}

vtkSmartPointer<vtkContourWidget> InteractorStyleSmartContour2::MyContourWidgetFactory(int type)
{
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
		vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	contourRepresentation->AlwaysOnTopOn();
	switch (type)
	{
	case VESSEL_WALL:
		contourRepresentation->GetLinesProperty()->SetColor(0, 0, 255);
		break;
	case LUMEN:
		contourRepresentation->GetLinesProperty()->SetColor(255, 0, 0);
		break;
	}

	vtkSmartPointer<vtkContourWidget> newWidget =
		vtkSmartPointer<vtkContourWidget>::New();
	newWidget->SetRepresentation(contourRepresentation);
	newWidget->ContinuousDrawOn();
	contourRepresentation->SetLineInterpolator(
		vtkSmartPointer<vtkBezierContourLineInterpolator>::New());
	return newWidget;
}

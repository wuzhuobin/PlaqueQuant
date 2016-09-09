#include "LumenSegmentation.h"

#include <vtkObjectFactory.h>
#include <vtkExtractVOI.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkGeometryFilter.h>
#include <vtkThreshold.h>
#include <vtkMath.h>
#include <vtkPolygon.h>

#include <vtkCleanPolyData.h>
#include <vtkCenterOfMass.h>
#include <vtkKdTree.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
//#include <vtkXMLPolyDataWriter.h>

vtkStandardNewMacro(LumenSegmentaiton)

void LumenSegmentaiton::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkObject::PrintSelf(os, indent);
}

void LumenSegmentaiton::SetInputData(vtkImageData * input)
{
	this->input = input;
	memcpy(this->VOI, input->GetExtent(), sizeof(VOI));
	//this->input->GetExtent(this->VOI);
}

vtkPolyData* LumenSegmentaiton::GetOutput()
{
	return this->m_contour;
}

void LumenSegmentaiton::SetVOI(int * VOI)
{
	memcpy(this->VOI, VOI, sizeof(this->VOI));
}

void LumenSegmentaiton::SetVOI(int extent0, int extent1, int extent2, int extent3, int extent4, int extent5)
{
	int VOI[6] = { extent0, extent1, extent2, extent3, extent4, extent5 };
	this->SetVOI(VOI);
}

void LumenSegmentaiton::SetSlice(int slice)
{
	this->SetVOI(VOI[0], VOI[1], VOI[2], VOI[3], slice, slice);
}

void LumenSegmentaiton::SetGenerateValues(double generateValues0, double generateValues1, double generateValues2)
{
	double generateValues[3] = { generateValues0, generateValues1, generateValues2 };
	this->SetGenerateValues(generateValues);
}

void LumenSegmentaiton::SetGenerateValues(double * generateValues)
{
	memcpy(this->generateValues, generateValues, sizeof(this->generateValues));
}

void LumenSegmentaiton::SetVesselWallContourRepresentation(vtkOrientedGlyphContourRepresentation * vesselWallContourRepresentation)
{
	this->vesselWallContourRepresentation = vesselWallContourRepresentation;
}

void LumenSegmentaiton::SetLumenWallContourRepresentation(vtkOrientedGlyphContourRepresentation * lumenWallContourRepresentation)
{
	this->lumenWallContourRepresentation = lumenWallContourRepresentation;
}

vtkOrientedGlyphContourRepresentation * LumenSegmentaiton::GetVesselWallContourRepresentation()
{
	return this->vesselWallContourRepresentation;
}

vtkOrientedGlyphContourRepresentation * LumenSegmentaiton::GetLumenWallContourRepresentation()
{
	return this->lumenWallContourRepresentation;
}
#include <vtkXMLPolyDataWriter.h>

void LumenSegmentaiton::Update()
{
	vtkSmartPointer<vtkPolyData> vesselWallPolyData = this->vesselWallContourRepresentation->GetContourRepresentationAsPolyData();
	vtkSmartPointer<vtkPolygon> vesselWallPolygon = vtkSmartPointer<vtkPolygon>::New();

	double lastPoint[3] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MAX, VTK_DOUBLE_MAX };

	// add all vesselWallPolyData into vesselWallPolygon
	for (vtkIdType i = 0; i < vesselWallPolyData->GetNumberOfPoints(); i++)
	{
		double worldCoordinate[3];

		vesselWallPolyData->GetPoint(i, worldCoordinate);

		// if the points is too close to the previous point, skip it to avoid error in PointInPolygon algorithm
		double d = vtkMath::Distance2BetweenPoints(lastPoint, worldCoordinate);
		if (d < 1E-5)
			continue;
		memcpy(lastPoint, worldCoordinate, sizeof(double) * 3);
		// becasue contourRepresentation can only get the top slice's worldcoordinates
		// it will be easy to ignore the third worldCoordinate

		vesselWallPolygon->GetPoints()->InsertNextPoint(worldCoordinate[0], worldCoordinate[1], 0);
	}
	double vesselWallPolygonNormal[3];
	vesselWallPolygon->ComputeNormal(vesselWallPolygon->GetPoints()->GetNumberOfPoints(),
		static_cast<double*>(vesselWallPolygon->GetPoints()->GetData()->GetVoidPointer(0)), vesselWallPolygonNormal);

	const double* spacing = input->GetSpacing();
	const double* origin = input->GetOrigin();
	const double* bounds = vesselWallPolygon->GetPoints()->GetBounds();
	int extent[6];
	input->GetExtent(extent);

	int newVOI[4] = {0};
	 //using the vesselWallPolygon to find a smaller VOI to extract
	newVOI[0] = (bounds[0] - origin[0]) / spacing[0] - 1;
	newVOI[1] = (bounds[1] - origin[0]) / spacing[0] + 1;
	newVOI[2] = (bounds[2] - origin[1]) / spacing[1] - 1;
	newVOI[3] = (bounds[3] - origin[1]) / spacing[1] + 1;

	this->VOI[0] = newVOI[0] >= extent[0] ? newVOI[0] : extent[0];
	this->VOI[1] = newVOI[1] <= extent[1] ? newVOI[1] : extent[1];
	this->VOI[2] = newVOI[2] >= extent[2] ? newVOI[2] : extent[2];
	this->VOI[3] = newVOI[3] <= extent[3] ? newVOI[3] : extent[3];

	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputData(input);
	extractVOI->SetVOI(this->VOI);
	extractVOI->Update();
	extract = extractVOI->GetOutput();

	// ouput contour as PolyData
	vtkSmartPointer<vtkContourFilter> contourFilter =
		vtkSmartPointer<vtkContourFilter>::New();
	contourFilter->SetInputConnection(extractVOI->GetOutputPort());
	contourFilter->GenerateValues(this->generateValues[0], this->generateValues[1], this->generateValues[2]); // (numContours, rangeStart, rangeEnd)
	contourFilter->Update();

	// output connected contour as PolyData
	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
		vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivityFilter->SetInputConnection(contourFilter->GetOutputPort());
	connectivityFilter->SetExtractionModeToAllRegions();
	connectivityFilter->ColorRegionsOn();
	connectivityFilter->Update();
	cerr << "number of extracted regions:" << connectivityFilter->GetNumberOfExtractedRegions() << endl;

	bool foundFlag = false;
	// Generate the threshold
	for (int i = 0; i < connectivityFilter->GetNumberOfExtractedRegions() && !foundFlag; i++)
	{
		//if (foundFlag == true)
			//break;
		cout << "finding in loop " << i << endl;
		vtkSmartPointer<vtkThreshold> thres = vtkSmartPointer<vtkThreshold>::New();
		thres->SetInputConnection(connectivityFilter->GetOutputPort());
		// using RegionID as the threshold of Threshold Filter
		thres->SetInputArrayToProcess(1, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "RegionID");
		thres->ThresholdBetween((double)i - 0.1, (double)i + 0.1);
		thres->Update();

		// convert UnstructedGrid data to PolyData
		vtkSmartPointer<vtkGeometryFilter> gfilter = vtkSmartPointer<vtkGeometryFilter>::New();
		gfilter->SetInputConnection(thres->GetOutputPort());
		gfilter->Update();
		vtkSmartPointer<vtkPolyData>lumenWallPolyData = gfilter->GetOutput();

		cout << "lumenWallPolyData has number of points" << gfilter->GetOutput()->GetNumberOfPoints() << endl;

		// clear all nodes from the last polyData
		if (this->lumenWallContourRepresentation != NULL) {
			this->lumenWallContourRepresentation->ClearAllNodes();
		}

		// check each loop if it is inside the region specified by contour widget
		for (vtkIdType j = 0; j < lumenWallPolyData->GetNumberOfPoints(); ++j) {


			if (this->lumenWallContourRepresentation != NULL) {
				this->lumenWallContourRepresentation->AddNodeAtWorldPosition(lumenWallPolyData->GetPoint(j));
			}
			cout << "finding in " << j << endl;
			// because this algorithm works in 2D
			// Try to ignore the third coordinate
			double point[3];
			memcpy(point, lumenWallPolyData->GetPoint(j), sizeof(point));
			point[2] = 0;
			// Test whether the points are inside the vesselWallPolygon or not
			// if one of the point is not inside break and goto next  
			if (!vesselWallPolygon->PointInPolygon(point,
				vesselWallPolygon->GetPoints()->GetNumberOfPoints(),
				static_cast<double*>(vesselWallPolygon->GetPoints()->GetData()->GetVoidPointer(0)),
				vesselWallPolygon->GetPoints()->GetBounds(), vesselWallPolygonNormal)) {
				break;
			}
			// if the last point is still in the vesselWallPolygon 
			// set foundFlag to true and save its polyData
			if (j == (lumenWallPolyData->GetNumberOfPoints() - 1)) {
				foundFlag = true;
				cout << "foundFlat" << endl;

				//vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
				//	vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
				//connectivityFilter->SetInputConnection(contourFilter->GetOutputPort());
				//connectivityFilter->SetExtractionModeToSpecifiedRegions();
				//connectivityFilter->AddSpecifiedRegion(i);
				//connectivityFilter->Update();


				// waiting for debuging 
				//vtkSmartPointer<vtkCleanPolyData> clearPolyData =
				//	vtkSmartPointer<vtkCleanPolyData>::New();
				//clearPolyData->SetInputData(lumenWallPolyData);
				//clearPolyData->SetAbsoluteTolerance(0.1);
				//clearPolyData->SetToleranceIsAbsolute(true);
				//clearPolyData->Update();

				//vtkSmartPointer<vtkXMLPolyDataWriter> w =
				//	vtkSmartPointer<vtkXMLPolyDataWriter>::New();
				//w->SetInputData(lumenWallPolyData);
				//w->SetFileName("C:\\Users\\user\\Desktop\\before.vtp");
				//w->Update();

				//lumenWallPolyData->DeepCopy(clearPolyData->GetOutput());
				//w->SetInputData(lumenWallPolyData);
				//w->SetFileName("C:\\Users\\user\\Desktop\\after.vtp");
				//w->Update();


				this->m_contour = lumenWallPolyData;
				LumenSegmentaiton::ResequenceLumenWallPolyData(lumenWallPolyData);

				// Down sample points
				vtkSmartPointer<vtkPolyData> temp = vtkSmartPointer<vtkPolyData>::New();
				vtkSmartPointer<vtkPoints> tempPts = vtkSmartPointer<vtkPoints>::New();
				vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
				for (int k = 0; k < lumenWallPolyData->GetNumberOfPoints()/5;k++)
				{
					tempPts->InsertNextPoint(lumenWallPolyData->GetPoint(k * 5));
					cells->InsertNextCell(2);
					cells->InsertCellPoint(k);
					if (k + 1 > lumenWallPolyData->GetNumberOfPoints() / 5) {
						cells->InsertCellPoint(0);
					}
					else {
						cells->InsertCellPoint(k + 1);
					}
				}
				temp->SetPoints(tempPts);
				temp->SetLines(cells);
				lumenWallPolyData->DeepCopy(temp);
			}
		}
		
	}


}

void LumenSegmentaiton::ResequenceLumenWallPolyData(vtkPolyData * lumenWallPolyData)
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

						//vtkXMLPolyDataWriter* writer = vtkXMLPolyDataWriter::New();
						//writer->SetFileName("../data/preRecon.vtp");
						//writer->SetInputData(lumenWallPolyData);
						//writer->Update();
						//writer->Write();

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

#include "LumenSegmentationFilter2.h"

#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>

#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkMath.h>
#include <vtkPolygon.h>
#include <vtkCleanPolyData.h>
#include <vtkCallbackCommand.h>


#include "ReorderPointIdOfContourFilter.h"


vtkStandardNewMacro(LumenSegmentationFilter2)

void LumenSegmentationFilter2::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkPolyDataAlgorithm::PrintSelf(os, indent);
}

void LumenSegmentationFilter2::SetSlice(int slice)
{
	VOI[4] = slice;
	VOI[5] = slice;
}

void LumenSegmentationFilter2::SetGenerateValues(double generateValues0, double generateValues1, double generateValues2)
{
	double generateValues[3] = { generateValues0, generateValues1, generateValues2 };
	this->SetGenerateValues(generateValues);
}

void LumenSegmentationFilter2::SetGenerateValues(double * generateValues)
{
	memcpy(this->generateValues, generateValues, sizeof(this->generateValues));
}

void LumenSegmentationFilter2::SetVesselWallContourRepresentation(vtkContourRepresentation * vesselWallContourRepresentation)
{
	this->vesselWallContourRepresentation = vesselWallContourRepresentation;
}

LumenSegmentationFilter2::LumenSegmentationFilter2()
{
	this->SetNumberOfInputPorts(1);
	this->SetNumberOfOutputPorts(1);
}

//#include <vtkXMLPolyDataWriter.h>

int LumenSegmentationFilter2::RequestData(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
{

	// get the info objects
	vtkInformation *inInfo0 = inputVector[0]->GetInformationObject(0);

	vtkInformation *outInfo = outputVector->GetInformationObject(0);


	// get the input and ouptut
	vtkImageData *input0 = 
		vtkImageData::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));

	vtkPolyData* output = 
		vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	if (!input0 || !vesselWallContourRepresentation) {
		return 0;
	}
	
	double lastPoint[3] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MAX, VTK_DOUBLE_MAX };
	vtkPolyData* vesselWallPolyData = vesselWallContourRepresentation->
		GetContourRepresentationAsPolyData();
	vtkSmartPointer<vtkPolygon> vesselWallPolygon = vtkSmartPointer<vtkPolygon>::New();

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

	const double* spacing = input0->GetSpacing();
	const double* origin = input0->GetOrigin();
	const double* bounds = vesselWallPolygon->GetPoints()->GetBounds();
	int extent[6];
	input0->GetExtent(extent);

	int newVOI[4] = { 0 };
	//using the vesselWallPolygon to find a smaller VOI to extract
	newVOI[0] = (bounds[0] - origin[0]) / spacing[0] - 1;
	newVOI[1] = (bounds[1] - origin[0]) / spacing[0] + 1;
	newVOI[2] = (bounds[2] - origin[1]) / spacing[1] - 1;
	newVOI[3] = (bounds[3] - origin[1]) / spacing[1] + 1;

	this->VOI[0] = newVOI[0] >= extent[0] ? newVOI[0] : extent[0];
	this->VOI[1] = newVOI[1] <= extent[1] ? newVOI[1] : extent[1];
	this->VOI[2] = newVOI[2] >= extent[2] ? newVOI[2] : extent[2];
	this->VOI[3] = newVOI[3] <= extent[3] ? newVOI[3] : extent[3];


	vtkSmartPointer<vtkMarchingSquares> marchingSquares =
		vtkSmartPointer<vtkMarchingSquares>::New();
	marchingSquares->SetInputData(input0);
	marchingSquares->SetImageRange(this->VOI);
	marchingSquares->GenerateValues(this->generateValues[0], this->generateValues[1], this->generateValues[2]);
	marchingSquares->Update();

	// output connected contour as PolyData
	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
		vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
	connectivityFilter->SetExtractionModeToLargestRegion();
	connectivityFilter->Update();

	// Write the file
	//vtkSmartPointer<vtkXMLPolyDataWriter> writer =
	//	vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	//writer->SetFileName("test.vtp");
	//writer->SetInputData(connectivityFilter->GetOutput());
	//writer->Write();


	// vtkError handling
	vtkSmartPointer<vtkCallbackCommand> errorCatch =
		vtkSmartPointer<vtkCallbackCommand>::New();
	// lambda function for handling error
	errorCatch->SetCallback([](vtkObject *caller, unsigned long eid,
		void *clientdata, void *calldata)->void {
		// error catch and error display
		char* ErrorMessage = static_cast<char *>(calldata);
		vtkOutputWindowDisplayErrorText(ErrorMessage);
		ReorderPointIdOfContourFilter* reorder =
			ReorderPointIdOfContourFilter::SafeDownCast(caller);
		// if error happened, skip this filer
		reorder->SetOutput(reorder->GetInput());
	});


	vtkSmartPointer<ReorderPointIdOfContourFilter> reorder =
		vtkSmartPointer<ReorderPointIdOfContourFilter>::New();
	reorder->SetInputConnection(connectivityFilter->GetOutputPort());
	reorder->AddObserver(vtkCommand::ErrorEvent, errorCatch);
	reorder->Update();


	double toleranceInitial = 1;
	int loopBreaker = 0;
	vtkSmartPointer<vtkCleanPolyData> clearPolyData =
		vtkSmartPointer<vtkCleanPolyData>::New();
	clearPolyData->SetInputConnection(reorder->GetOutputPort());
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

	output->ShallowCopy( clearPolyData->GetOutput());
	return 1;
}

int LumenSegmentationFilter2::FillInputPortInformation(int port, vtkInformation * info)
{
	if (port == 0)
	{
		info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData");
		return 1;
	}
	return 0;
}

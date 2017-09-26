#include "MaximumWallThickness2.h"

#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkMarchingSquares.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCenterOfMass.h>
#include <vtkMath.h>
#include <vtkCenterOfMass.h>
#include <vtkModifiedBSPTree.h>
#include <vtkImageThreshold.h>
#include <vtkCleanPolyData.h>


#include <vector>

vtkStandardNewMacro(MaximumWallThickness2);

void MaximumWallThickness2::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkThreadedImageAlgorithm::PrintSelf(os, indent);
}

MaximumWallThickness2::MaximumWallThickness2()
{
}

MaximumWallThickness2::~MaximumWallThickness2()
{
}

void MaximumWallThickness2::ThreadedRequestData(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector, vtkImageData *** inData, vtkImageData ** outData, int extent[6], int id)
{
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);

	const double* origin = inInfo->Get(vtkDataObject::ORIGIN());
	const double* spacing = inInfo->Get(vtkDataObject::SPACING());
	
	for (int range = extent[4]; range <= extent[5]; ++range) {

		int _extent[6];
		memcpy(_extent, extent, sizeof(_extent));
		_extent[4] = _extent[5] = range;

		vtkSmartPointer<vtkImageThreshold> vesselWallThreshold =
			vtkSmartPointer<vtkImageThreshold>::New();
		vesselWallThreshold->SetInputData(inData[0][0]);
		vesselWallThreshold->ThresholdByLower(VesselWallIntensity);
		vesselWallThreshold->ReplaceOutOn();
		vesselWallThreshold->SetOutValue(VesselWallIntensity);
		vesselWallThreshold->Update();
		
		vtkSmartPointer<vtkMarchingSquares> vesselWallMarchingSquare =
			vtkSmartPointer<vtkMarchingSquares>::New();
		vesselWallMarchingSquare->SetInputConnection(vesselWallThreshold->GetOutputPort());
		vesselWallMarchingSquare->SetImageRange(_extent);
		vesselWallMarchingSquare->GenerateValues(1, VesselWallIntensity, VesselWallIntensity);
		vesselWallMarchingSquare->Update();

		vtkSmartPointer<vtkPolyDataConnectivityFilter> vesselWallConnectivity =
			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		vesselWallConnectivity->SetInputConnection(vesselWallMarchingSquare->GetOutputPort());
		vesselWallConnectivity->SetExtractionModeToAllRegions();
		vesselWallConnectivity->Update();

		int numOfVesselWall = vesselWallConnectivity->GetNumberOfExtractedRegions();
		double maxDistance = 0;
		for (int i = 0; i < numOfVesselWall; ++i) {
			vesselWallConnectivity->InitializeSpecifiedRegionList();
			vesselWallConnectivity->SetExtractionModeToSpecifiedRegions();
			vesselWallConnectivity->AddSpecifiedRegion(i);
			vesselWallConnectivity->Update();

			vtkSmartPointer<vtkCleanPolyData> _vesselWallClean =
				vtkSmartPointer<vtkCleanPolyData>::New();
			_vesselWallClean->SetInputConnection(vesselWallConnectivity->GetOutputPort());
			_vesselWallClean->PointMergingOn();
			_vesselWallClean->Update();

			if (_vesselWallClean->GetOutput()->GetNumberOfPoints() < 3) {
				continue;
			}
			const double* _bounds = _vesselWallClean->GetOutput()->GetBounds();
			int lumenExtent[6];

			for (int j = 0; j < 3; j++)
			{
				lumenExtent[j * 2] = vtkMath::Round((_bounds[j * 2] - origin[j]) / spacing[j]);
				lumenExtent[j * 2 + 1] = vtkMath::Round((_bounds[j * 2 + 1] - origin[j]) / spacing[j]);
			}

			vtkSmartPointer<vtkImageThreshold> lumenImageThreshold =
				vtkSmartPointer<vtkImageThreshold>::New();
			lumenImageThreshold->SetInputData(inData[0][0]);
			lumenImageThreshold->ThresholdBetween(LumenIntensity, LumenIntensity);
			lumenImageThreshold->SetOutValue(0);
			lumenImageThreshold->Update();


			vtkSmartPointer<vtkMarchingSquares> lumenMarchingSquare =
				vtkSmartPointer<vtkMarchingSquares>::New();
			lumenMarchingSquare->SetInputConnection(lumenImageThreshold->GetOutputPort());
			lumenMarchingSquare->SetImageRange(lumenExtent);
			lumenMarchingSquare->GenerateValues(1, LumenIntensity, LumenIntensity);
			lumenMarchingSquare->Update();
			if (lumenMarchingSquare->GetOutput()->GetNumberOfPoints() < 3) {
				continue;
			}
			double _distance = MaximumWallThicknessOneLoopExcutation(_vesselWallClean->GetOutput(), lumenMarchingSquare->GetOutput());
			if (_distance > maxDistance) {
				maxDistance = _distance;
			}

		}

		MaxDistances[range] = maxDistance;
	}

}

double MaximumWallThickness2::MaximumWallThicknessOneLoopExcutation(vtkPolyData * vesselWall, vtkPolyData * lumen)
{
	vtkSmartPointer<vtkCenterOfMass> centerOfMass =
		vtkSmartPointer<vtkCenterOfMass>::New();
	centerOfMass->SetInputData(lumen);
	centerOfMass->SetUseScalarsAsWeights(false);
	centerOfMass->Update();

	double center[3];
	double maxDistance = 0;
	double maxPoint1[3], maxPoint2[3];
	centerOfMass->GetCenter(center);
	
	for (vtkIdType i = 0; i < vesselWall->GetNumberOfPoints(); i++)
	{
		double _p[3];
		vesselWall->GetPoints()->GetPoint(i, _p);
		//cout << _p[0] << ' ' << _p[1] << ' ' << _p[2] << endl;
		vtkSmartPointer<vtkModifiedBSPTree> bspTree =
			vtkSmartPointer<vtkModifiedBSPTree>::New();
		bspTree->SetDataSet(lumen);
		bspTree->BuildLocator();

		// Outputs
		double t; // Parametric coordinate of intersection (0 (corresponding to p1) to 1 (corresponding to p2))
		double _p2[3]; // The coordinate of the intersection
		double pcoords[3];
		int subId;

		// Note: for a typical use case (ray-triangle intersection), pcoords and subId will not be used
		vtkIdType iD = bspTree->IntersectWithLine(center, _p, 0.001, t, _p2, pcoords, subId);
		if (iD > 0) {
			double _distance = sqrt(vtkMath::Distance2BetweenPoints(_p, _p2));

			if (_distance > maxDistance) {
				maxDistance = _distance;
				memcpy(maxPoint1, _p, sizeof(maxPoint1));
				memcpy(maxPoint2, _p2, sizeof(maxPoint2));
			}
		}
	}
	return maxDistance;
}

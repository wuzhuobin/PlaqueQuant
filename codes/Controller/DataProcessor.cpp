#include "DataProcessor.h"

#include "StyleSwitch3D.h"
#include "PlaqueQuantImageManager.h"
#include "PlaqueQuantOverlay.h"
#include "CenterlineSurfaceViewer.h"

#include <vtkSplineFilter.h>

#include <vtkvmtkCurvedMPRImageFilter.h>
#include <vtkvmtkCenterlineGeometry.h>
#include <vtkvmtkCenterlineAttributesFilter.h>

#include <math.h>

class vtkvmtkCurvedMPRImageFilterNearestInterpolation : public vtkvmtkCurvedMPRImageFilter
{
public:
	static vtkvmtkCurvedMPRImageFilterNearestInterpolation* New() {
		return new vtkvmtkCurvedMPRImageFilterNearestInterpolation();
	}
	vtkTypeMacro(vtkvmtkCurvedMPRImageFilterNearestInterpolation, vtkvmtkCurvedMPRImageFilter);
protected:
	// Description:
	// This method is called by the superclass and performs the actual computation of the MPR image
	virtual int RequestData(
		vtkInformation * vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) {
		// get the info objects
		vtkInformation* outInfo = outputVector->GetInformationObject(0);
		vtkImageData *outputImage = vtkImageData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
		vtkInformation *inInfoImage = inputVector[0]->GetInformationObject(0);
		vtkImageData *inputImage = vtkImageData::SafeDownCast(inInfoImage->Get(vtkDataObject::DATA_OBJECT()));

		if (!this->Centerline)
		{
			vtkErrorMacro(<< "Centerline not set");
			return 1;
		}

		if (inputImage == NULL)
		{
			vtkErrorMacro(<< "RequestData: InputImageData is not set.");
			return 1;
		}

		//check if everything is allright
		if (inputImage == NULL || outputImage == NULL)
		{
			if (outputImage)
			{
				int outputExtent[6] = { 0,-1,0,-1,0,-1 };
				outputImage->SetExtent(outputExtent);
#if (VTK_MAJOR_VERSION <= 5)
				outputImage->SetWholeExtent(outputExtent);
				outputImage->SetUpdateExtent(outputExtent);
				outputImage->AllocateScalars();
#else
				this->UpdateInformation();
				vtkStreamingDemandDrivenPipeline::SetUpdateExtent(outInfo, outputExtent);
				this->Update();
				outputImage->AllocateScalars(outInfo);
#endif
			}

		}

		//check if FrenetTangent s are available
		if (this->FrenetTangentArrayName == NULL)
		{
			vtkErrorMacro(<< "RequestData: FrenetTangentArrayName is not set.");
			return 1;
		}

		vtkDataArray* frenetArray = this->Centerline->GetPointData()->GetArray(this->FrenetTangentArrayName);
		if (!frenetArray)
		{
			vtkErrorMacro(<< "RequestData: InputPolyData does not contain an array with the specified FrenetTangentArrayName.");
			return 1;
		}

		//check if ParallelTransportNormals are available
		if (this->ParallelTransportNormalsArrayName == NULL)
		{
			vtkErrorMacro(<< "RequestData: ParallelTransportNormalsArrayName is not set.");
			return 1;
		}

		//check if a Centerline is available
		vtkDataArray* parallelArray = this->Centerline->GetPointData()->GetArray(this->ParallelTransportNormalsArrayName);
		if (!parallelArray)
		{
			vtkErrorMacro(<< "RequestData: InputPolyData does not contain an array with the specified ParallelTransportNormalsArrayName.");
			return 1;
		}

		int firstLineInCells;
		vtkCell* line = NULL;
		//get the first line in the vktPolyData
		for (firstLineInCells = 0; firstLineInCells<this->Centerline->GetNumberOfCells(); firstLineInCells++)
		{
			line = this->Centerline->GetCell(firstLineInCells);
			if (line->GetCellType() == VTK_LINE || line->GetCellType() == VTK_POLY_LINE)
			{
				break;
			}
		}

		if (!line || (!(firstLineInCells < (int) this->Centerline->GetNumberOfCells())))
		{
			vtkErrorMacro(<< "RequestData: no line in input vtkPolyData.");
			return 1;
		}

		if (line->GetNumberOfPoints() < 2)
		{
			vtkErrorMacro(<< "RequestData: Too few points in Centerline.");
			return 1;
		}

		vtkPoints* linePoints = line->GetPoints();

		int outExtent[6];
#if (VTK_MAJOR_VERSION <= 5)
		outputImage->GetUpdateExtent(outExtent);
#else
		outInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), outExtent);
#endif

		outputImage->SetExtent(outExtent);
#if (VTK_MAJOR_VERSION <= 5)
		outputImage->SetWholeExtent(outExtent);
		outputImage->SetUpdateExtent(outExtent);
		outputImage->AllocateScalars();
#else
		this->UpdateInformation();
		vtkStreamingDemandDrivenPipeline::SetUpdateExtent(outInfo, outExtent);
		this->Update();
		outputImage->AllocateScalars(outInfo);
#endif

		vtkDataArray* frenetTangentArray = this->Centerline->GetPointData()->GetArray(this->FrenetTangentArrayName);
		vtkDataArray* parallelTransportNormalsArray = this->Centerline->GetPointData()->GetArray(this->ParallelTransportNormalsArrayName);

		//start computing
		int inExtent[6];
#if (VTK_MAJOR_VERSION <= 5)
		inputImage->GetWholeExtent(inExtent);
#else
		inInfoImage->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), inExtent);
#endif

		// if the input extent is empty then exit
		if (inExtent[1] < inExtent[0] ||
			inExtent[3] < inExtent[2] ||
			inExtent[5] < inExtent[4])
		{
			return 1;
		}

		vtkImageReslice* reslice = vtkImageReslice::New();
		reslice->SetOutputDimensionality(2);
#if (VTK_MAJOR_VERSION <= 5)
		reslice->SetInput(inputImage);
#else
		reslice->SetInputData(inputImage);
#endif
		reslice->SetInterpolationModeToNearestNeighbor();
		reslice->SetOutputScalarType(VTK_UNSIGNED_CHAR);
		//turn off transformation of the input spacin, origin and extent, so we can define what we want
		reslice->TransformInputSamplingOff();
		//set the value of the voxels that are out of the input data
		reslice->SetBackgroundLevel(this->ReslicingBackgroundLevel);
		//set the outputspacing
		//reslice->SetOutputSpacing(this->OutputSpacing[0],this->OutputSpacing[1],this->OutputSpacing[2]);
		reslice->SetOutputSpacing(this->OutputSpacing);
		// going to MPR
		for (int slice = outExtent[4]; slice<(outExtent[5] + 1); slice++)
		{// for each slice (or point on the line)
			double centerSlice[3];
			linePoints->GetPoint(slice, centerSlice);

			// To calculate the outputorigin & the necessarry transform 
			// the vectors are retreived from the array's
			//t is the vector in the direction of the Centerline, so along the z-axis in the MPR volume
			double t[3];
			frenetTangentArray->GetTuple(slice, t);

			//p is a normal of the Centerline, directed to the 'North' direction of the inputvolume,in the MPR volume this will be along the x-axis 
			double p[3];
			parallelTransportNormalsArray->GetTuple(slice, p);

			double tp[3];
			//tp is the crossproduct of  t and p, and will be directed to the 'West' direction of the inputvolume,in the MPR volume this will be along the y-axis 
			tp[0] = (t[1] * p[2] - t[2] * p[1]);
			tp[1] = (t[2] * p[0] - t[0] * p[2]);
			tp[2] = (t[0] * p[1] - t[1] * p[0]);

			//set the axis of the first slice according to the vectors of the first point in the line
			reslice->SetResliceAxesDirectionCosines(p[0], p[1], p[2], tp[0], tp[1], tp[2], t[0], t[1], t[2]);
			// the firstPoint on the Centerline coresponds to the origin of the output axes unit vectors
			reslice->SetResliceAxesOrigin(centerSlice[0], centerSlice[1], centerSlice[2]);
			//the outputextent will be one slice
			reslice->SetOutputExtent(this->OutputExtent[0], this->OutputExtent[1], this->OutputExtent[2], this->OutputExtent[3], 0, 0);
			//the outputorigin in the input coordinate system is set to zero
			double outputorigin[3];
			outputorigin[0] = 0.0;
			outputorigin[1] = 0.0;
			outputorigin[2] = 0.0;

			reslice->SetOutputOrigin(this->OutputOrigin);
			reslice->Update();

			int resliceUpdateExtent[6];
			resliceUpdateExtent[0] = outExtent[0];
			resliceUpdateExtent[1] = outExtent[1];
			resliceUpdateExtent[2] = outExtent[2];
			resliceUpdateExtent[3] = outExtent[3];
			resliceUpdateExtent[4] = 0;
			resliceUpdateExtent[5] = 0;

			vtkIdType outputInc[3];
			outputImage->GetIncrements(outputInc);

			int scalarType = reslice->GetOutput()->GetScalarType();
			switch (scalarType)
			{
				vtkTemplateMacro(
					FillSlice(
						static_cast<VTK_TT*>(reslice->GetOutput()->GetScalarPointerForExtent(resliceUpdateExtent)),
						static_cast<VTK_TT*>(outputImage->GetScalarPointerForExtent(outExtent)),
						resliceUpdateExtent, outExtent, outputInc, slice));
			}
		}// for each slice

		reslice->Delete();
		return 1;


	}


	//BTX
	//template<class T>
	//void FillSlice(T* outReslicePtr, T* outputImagePtr, int* resliceUpdateExtent, int* outExtent, vtkIdType* outputInc, int slice) {
	//	vtkvmtkCurvedMPRImageFilter::FillSlice<T>(outReslicePtr, outputImagePtr, resliceUpdateExtent, outExtent, outputInc, slice);
	//}
	//BTX
	template<class T>
	void FillSlice(T* outReslicePtr, T* outputImagePtr, int* resliceUpdateExtent, int* outExtent, vtkIdType* outputInc, int slice)
	{
		//write result to the new dataset
		int size = (outExtent[1] - outExtent[0] + 1)*(outExtent[3] - outExtent[2] + 1);
		for (int i = 0; i<size; i++)
		{
			outputImagePtr[i + (slice - outExtent[4]) * outputInc[2]] = outReslicePtr[i];
		}
	}

};

DataProcessor::DataProcessor(QObject * parent)
	:QObject(parent)
{
}

DataProcessor::~DataProcessor()
{
}

void DataProcessor::initializeCurved() 
{
	vtkPolyData* centerline;
	centerline = surfaceInteractorStyle->GetCurvedNavigation()->GetCenterlineSurfaceViewer()->GetSplineFilter()->GetOutput();
	vtkImageData* inputImage;
	inputImage = imageManager->getImage(0);
	if (!inputImage) {
		return;
	}
	double* spacing = inputImage->GetSpacing();
	int* size = inputImage->GetDimensions();
	int* extent = inputImage->GetExtent();
	double outputSpacing = std::min(std::min(spacing[0], spacing[1]), spacing[2]);
	int outputSize = std::max(std::max(extent[1] - extent[0], extent[3] - extent[2]),
		extent[5] - extent[4]);
	

	vtkSmartPointer<vtkvmtkCenterlineGeometry> centerlineGeometry =
		vtkSmartPointer<vtkvmtkCenterlineGeometry>::New();
	centerlineGeometry->SetInputData(centerline);
	centerlineGeometry->SetLengthArrayName("Length");
	centerlineGeometry->SetCurvatureArrayName("Curvature");
	centerlineGeometry->SetTorsionArrayName("Torsion");
	centerlineGeometry->SetTortuosityArrayName("Tortuosity");
	centerlineGeometry->SetFrenetTangentArrayName("FrenetTangent");
	centerlineGeometry->SetFrenetNormalArrayName("FrenetNormal");
	centerlineGeometry->SetFrenetBinormalArrayName("FrenetBinormal");
	centerlineGeometry->LineSmoothingOff();
	centerlineGeometry->Update();

	vtkSmartPointer<vtkvmtkCenterlineAttributesFilter> centerlineAttributes =
		vtkSmartPointer<vtkvmtkCenterlineAttributesFilter>::New();
	centerlineAttributes->SetInputConnection(centerlineGeometry->GetOutputPort());
	centerlineAttributes->SetAbscissasArrayName("Abscissas");
	centerlineAttributes->SetParallelTransportNormalsArrayName("Normals");
	centerlineAttributes->Update();

	for (int i = 0; i < imageManager->getNumOfImages(); ++i) {


		inputImage = imageManager->getImage(i);
		if (!inputImage) {
			continue;
		}


		vtkSmartPointer<vtkvmtkCurvedMPRImageFilter> curvedMPRImageFilter =
			vtkSmartPointer<vtkvmtkCurvedMPRImageFilter>::New();
		curvedMPRImageFilter->SetInputData(inputImage);
		curvedMPRImageFilter->SetCenterline(centerlineAttributes->GetOutput());
		curvedMPRImageFilter->SetParallelTransportNormalsArrayName("Normals");
		curvedMPRImageFilter->SetFrenetTangentArrayName("FrenetTangent");
		curvedMPRImageFilter->SetInplaneOutputSpacing(outputSpacing, outputSpacing);
		curvedMPRImageFilter->SetInplaneOutputSize(outputSize, outputSize);
		curvedMPRImageFilter->SetReslicingBackgroundLevel(0.0);
		curvedMPRImageFilter->Update();
		// since marching cubes begins from the world coordinate(0, 0, 0) point, 
		// the original orgin of the curved image is set to the first point of the 
		// centerline, which is middle of the first slice
		// so if the image need to do marching cube, the world point (0, 0, 0) should not 
		// close to any label image which needs to do marching cube
		// otherwise, the generated surface will be messed up or splited by the point
		// (0, 0, 0)
		curvedMPRImageFilter->GetOutput()->SetOrigin(0, 0, 0);

		imageManager->setCurvedImage(i, curvedMPRImageFilter->GetOutput());

	}

	inputImage = imageManager->getOverlay()->getData();

	vtkSmartPointer<vtkvmtkCurvedMPRImageFilterNearestInterpolation> curvedMPRImageFilter =
		vtkSmartPointer<vtkvmtkCurvedMPRImageFilterNearestInterpolation>::New();
	curvedMPRImageFilter->SetInputData(inputImage);
	curvedMPRImageFilter->SetCenterline(centerlineAttributes->GetOutput());
	curvedMPRImageFilter->SetParallelTransportNormalsArrayName("Normals");
	curvedMPRImageFilter->SetFrenetTangentArrayName("FrenetTangent");
	curvedMPRImageFilter->SetInplaneOutputSpacing(outputSpacing, outputSpacing);
	curvedMPRImageFilter->SetInplaneOutputSize(outputSize, outputSize);
	curvedMPRImageFilter->SetReslicingBackgroundLevel(0.0);
	curvedMPRImageFilter->Update();
	// since marching cubes begins from the world coordinate(0, 0, 0) point, 
	// the original orgin of the curved image is set to the first point of the 
	// centerline, which is middle of the first slice
	// so if the image need to do marching cube, the world point (0, 0, 0) should not 
	// close to any label image which needs to do marching cube
	// otherwise, the generated surface will be messed up or splited by the point
	// (0, 0, 0) 
	curvedMPRImageFilter->GetOutput()->SetOrigin(0, 0, 0);

	imageManager->setCurvedOverlay(curvedMPRImageFilter->GetOutput());


}

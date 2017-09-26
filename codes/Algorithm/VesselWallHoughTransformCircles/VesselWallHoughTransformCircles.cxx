#include "VesselWallHoughTransformCircles.h"
#include <vtkImageData.h>
#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkSmartPointer.h>
#include <vtkImageCast.h>
#include <vtkRegularPolygonSource.h>
#include <vtkAppendPolyData.h>
#include <vtkObjectFactory.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

vtkStandardNewMacro(VesselWallHoughTransformCircles);
VesselWallHoughTransformCircles::VesselWallHoughTransformCircles()
{
	//this->ImageRange[0] = 0; this->ImageRange[1] = VTK_INT_MAX;
	//this->ImageRange[2] = 0; this->ImageRange[3] = VTK_INT_MAX;
	//this->ImageRange[4] = 0; this->ImageRange[5] = 0;

	this->SetNumberOfInputPorts(2);
}

VesselWallHoughTransformCircles::~VesselWallHoughTransformCircles()
{
}

int VesselWallHoughTransformCircles::RequestData(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
{
	// get the info objects
	vtkInformation *inInfo0 = inputVector[0]->GetInformationObject(0);
	vtkInformation *inInfo1 = inputVector[1]->GetInformationObject(0);

	vtkInformation *outInfo = outputVector->GetInformationObject(0);


	// get the input and ouptut
	vtkImageData *input0 = vtkImageData::SafeDownCast(
		inInfo0->Get(vtkDataObject::DATA_OBJECT()));


	vtkImageData *input1 = vtkImageData::SafeDownCast(
		inInfo1->Get(vtkDataObject::DATA_OBJECT()));
	
	vtkPolyData *output = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	int roi[6] = { 0 };
	int plane = 0;
	int dim = 0;
	int i = 0;
	int* ext = input0->GetExtent();

	if (input0->GetDataDimension() != 2)
	{
		for (int i = 0; i < 6; i++)
		{
			roi[i] = this->ImageRange[i];
		}
	}
	else
	{
		input0->GetExtent(roi);
	}

	// check the final region of interest to make sure its acceptable
	for (dim = 0, i = 0; i < 3; i++)
	{
		if (roi[2 * i + 1] > ext[2 * i + 1])
		{
			roi[2 * i + 1] = ext[2 * i + 1];
		}
		else if (roi[2 * i + 1] < ext[2 * i])
		{
			roi[2 * i + 1] = ext[2 * i];
		}

		if (roi[2 * i] > roi[2 * i + 1])
		{
			roi[2 * i] = roi[2 * i + 1];
		}
		else if (roi[2 * i] < ext[2 * i])
		{
			roi[2 * i] = ext[2 * i];
		}

		if ((roi[2 * i + 1] - roi[2 * i]) > 0)
		{
			dim++;
		}
		else
		{
			plane = i;
		}
	}

	if (dim != 2)
	{
		vtkErrorMacro(<< "VesselWallHoughTransformCircles requires 2D data");
		vtkErrorMacro(<< "This is a " << dim  <<"D data");
		return 1;
	}


	 switch(input0->GetScalarType())
	 {
		 vtkTemplateMacro(itkProcess<VTK_TT>(input0, input1, output, roi, plane));
	 }


	return 1;
}

int VesselWallHoughTransformCircles::FillInputPortInformation(int port, vtkInformation * info)
{
	if (port == 0)
	{
		info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData");
		return 1;
	}
	else if (port == 1)
	{
		info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData");
		return 1;
	}

	return 0;
}

#include <itkVTKImageToImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkSubtractImageFilter.h>
#include <IADEHoughTransform2DCirclesImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkStatisticsImageFilter.h>
#include <itkBinaryImageToLabelMapFilter.h>
#include <itkLabelMapToLabelImageFilter.h>
#include <itkLabelStatisticsImageFilter.h>

template<typename TPixelType>
void VesselWallHoughTransformCircles::itkProcess(
	vtkImageData * input0, 
	vtkImageData * overlay, 
	vtkPolyData* output,
	int* roi, int plane)
{
	using namespace itk;
	typedef typename TPixelType VoxelInput;
	typedef unsigned char VoxelOverlay;
	typedef Image<VoxelInput, 3> ImageInput3D;
	typedef Image<VoxelInput, 2> ImageInput2D;
	typedef Image<VoxelOverlay, 3> ImageOverlay3D;
	typedef Image<VoxelOverlay, 2> ImageOverlay2D;

	// number of circles to extract
	IdentifierType numberOfCircles;
	// min radius to extract 
	double minRadius = 999;
	// max radius to extract
	double maxRadius = 0;

	ImageOverlay3D::RegionType::IndexType indexOverlay;
	indexOverlay[0] = roi[0];
	indexOverlay[1] = roi[2];
	indexOverlay[2] = roi[4];

	ImageOverlay3D::RegionType::SizeType sizeOverlay;
	sizeOverlay[0] = roi[1] - roi[0];
	sizeOverlay[1] = roi[3] - roi[2];
	sizeOverlay[2] = roi[5] - roi[4];
	sizeOverlay[plane] = 0;

	ImageOverlay3D::RegionType extractionRegionOverlay(indexOverlay, sizeOverlay);

	vtkSmartPointer<vtkImageCast> cast =
		vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData(overlay);
	cast->SetOutputScalarTypeToUnsignedChar();
	cast->Update();
	overlay = cast->GetOutput();

	VTKImageToImageFilter<ImageOverlay3D>::Pointer connectorOverlay 
		= VTKImageToImageFilter<ImageOverlay3D>::New();

	connectorOverlay->SetInput(overlay);
	connectorOverlay->Update();
	

	ExtractImageFilter<ImageOverlay3D, ImageOverlay2D>::Pointer extractImageFilterOverlay =
		ExtractImageFilter<ImageOverlay3D, ImageOverlay2D>::New();
	extractImageFilterOverlay->SetInput(connectorOverlay->GetOutput());
	extractImageFilterOverlay->SetExtractionRegion(extractionRegionOverlay);
	//extractImageFilterOverlay->SetNumberOfThreads(16);
	extractImageFilterOverlay->SetDirectionCollapseToIdentity();
	extractImageFilterOverlay->Update();

	ConnectedComponentImageFilter<ImageOverlay2D, ImageOverlay2D>::Pointer connectedComponent =
		ConnectedComponentImageFilter<ImageOverlay2D, ImageOverlay2D>::New();
	connectedComponent->SetInput(extractImageFilterOverlay->GetOutput());
	connectedComponent->Update();
	// get number of Circles
	numberOfCircles = connectedComponent->GetObjectCount();
	// no circles
	if (numberOfCircles < 1) {
		return;
	}
	LabelStatisticsImageFilter< ImageOverlay2D, ImageOverlay2D >::Pointer labelStatisticsImageFilter = 
		LabelStatisticsImageFilter< ImageOverlay2D, ImageOverlay2D >::New();
	labelStatisticsImageFilter->SetLabelInput(connectedComponent->GetOutput());
	labelStatisticsImageFilter->SetInput(extractImageFilterOverlay->GetOutput());
	labelStatisticsImageFilter->Update();

	// calculate the minRadius/maxRadius of all circles.
	for (LabelStatisticsImageFilter< ImageOverlay2D, ImageOverlay2D >::ValidLabelValuesContainerType::const_iterator vIt = labelStatisticsImageFilter->GetValidLabelValues().begin();
		vIt != labelStatisticsImageFilter->GetValidLabelValues().end();
		++vIt)
	{
		if ((*vIt) && labelStatisticsImageFilter->HasLabel(*vIt))
		{
			minRadius = std::min(minRadius, std::sqrt(labelStatisticsImageFilter->GetCount(*vIt) * itk::Math::one_over_pi));
			maxRadius = std::max(maxRadius, std::sqrt(labelStatisticsImageFilter->GetCount(*vIt) * itk::Math::one_over_pi));
		}
	}
	std::cout << "minRadius" << endl;
	std::cout << minRadius << endl;
	std::cout << "maxRadius" << endl;
	std::cout << maxRadius << endl;
	BinaryBallStructuringElement<VoxelOverlay, 2>   innerElement;
	innerElement.SetRadius(InnerRadius);
	innerElement.CreateStructuringElement();

	// inner dilate to remove the lumen edge
	BinaryDilateImageFilter <ImageOverlay2D, ImageOverlay2D, BinaryBallStructuringElement<VoxelOverlay, 2>>::Pointer innerDilate
		= BinaryDilateImageFilter <ImageOverlay2D, ImageOverlay2D, BinaryBallStructuringElement<VoxelOverlay, 2>>::New();
	innerDilate->SetForegroundValue(LumenLabel);
	innerDilate->SetInput(extractImageFilterOverlay->GetOutput());
	innerDilate->SetKernel(innerElement);
	innerDilate->Update();

	BinaryBallStructuringElement<VoxelOverlay, 2>   outerElement;
	outerElement.SetRadius(OuterRadius);
	outerElement.CreateStructuringElement();

	// outer dilate to restrict the hough circle searching scope
	BinaryDilateImageFilter <ImageOverlay2D, ImageOverlay2D, BinaryBallStructuringElement<VoxelOverlay, 2>>::Pointer outerDilate
		= BinaryDilateImageFilter <ImageOverlay2D, ImageOverlay2D, BinaryBallStructuringElement<VoxelOverlay, 2>>::New();
	outerDilate->SetForegroundValue(LumenLabel);
	outerDilate->SetInput(extractImageFilterOverlay->GetOutput());
	outerDilate->SetKernel(outerElement);
	outerDilate->Update();

	// outerDilate substract innerDilate to get a ring
	SubtractImageFilter<ImageOverlay2D>::Pointer subtractImage =
		SubtractImageFilter<ImageOverlay2D>::New();
	subtractImage->SetInput1(outerDilate->GetOutput());
	subtractImage->SetInput2(innerDilate->GetOutput());
	subtractImage->Update();

	VTKImageToImageFilter<ImageInput3D>::Pointer connectorInput =
		VTKImageToImageFilter<ImageInput3D>::New();

	connectorInput->SetInput(input0);
	connectorInput->Update();

	ImageInput3D::RegionType::IndexType indexInput;
	indexInput[0] = roi[0];
	indexInput[1] = roi[2];
	indexInput[2] = roi[4];

	ImageInput3D::RegionType::SizeType sizeInput;
	sizeInput[0] = roi[1] - roi[0];
	sizeInput[1] = roi[3] - roi[2];
	sizeInput[2] = roi[5] - roi[4];
	sizeInput[plane] = 0;

	ImageInput3D::RegionType extractionRegionInput(indexInput, sizeInput);

	ExtractImageFilter<ImageInput3D, ImageInput2D>::Pointer extractImageFilterInput =
		ExtractImageFilter<ImageInput3D, ImageInput2D>::New();
	extractImageFilterInput->SetInput(connectorInput->GetOutput());
	extractImageFilterInput->SetExtractionRegion(extractionRegionInput);
	//extractImageFilterInput->SetNumberOfThreads(16);
	extractImageFilterInput->SetDirectionCollapseToIdentity();
	extractImageFilterInput->Update();

	IADEHoughTransform2DCirclesImageFilter<VoxelInput, VoxelInput, VoxelOverlay>::Pointer houghCircle =
		IADEHoughTransform2DCirclesImageFilter<VoxelInput, VoxelInput, VoxelOverlay>::New();
	houghCircle->SetInput(extractImageFilterInput->GetOutput());
	houghCircle->SetMaskImage(subtractImage->GetOutput());
	houghCircle->SetNumberOfCircles(numberOfCircles);
	houghCircle->SetThreshold(0);
	houghCircle->SetSweepAngle(0.2);
	houghCircle->SetSigmaGradient(1);
	houghCircle->SetMinimumRadius(minRadius + InnerRadius);
	houghCircle->SetMaximumRadius(maxRadius + OuterRadius);
	houghCircle->SetVariance(1);
	houghCircle->Update();

	IADEHoughTransform2DCirclesImageFilter<VoxelInput, VoxelInput, VoxelOverlay>::CirclesListType circles =
		houghCircle->GetCircles(numberOfCircles);

	vtkSmartPointer<vtkAppendPolyData> appendPolyData =
		vtkSmartPointer<vtkAppendPolyData>::New();

	for (IADEHoughTransform2DCirclesImageFilter<VoxelInput, VoxelInput, VoxelOverlay>::CirclesListType::const_iterator cit = circles.cbegin();
		cit != circles.cend(); ++cit) {
		
		// the center of the circle is a 3-d coordinate
		double center[3] = { 0 };
		int i1 = 0, i2 = 0;
		while (i1 < 3)
		{
			if (i1 == plane) {
				center[i1] = roi[i1 * 2];
			}
			else {
				center[i1] = (*cit)->GetObjectToParentTransform()->GetOffset()[i2];
				++i2;
			}
			++i1;
		}

		vtkSmartPointer<vtkRegularPolygonSource> polygonSource =
			vtkSmartPointer<vtkRegularPolygonSource>::New();

		polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
		polygonSource->SetNumberOfSides((*cit)->GetRadius()[0] * 2);
		polygonSource->SetRadius((*cit)->GetRadius()[0]);
		polygonSource->SetCenter(center);
		polygonSource->Update();

		vtkSmartPointer<vtkTransform> transform =
			vtkSmartPointer<vtkTransform>::New();
		transform->Translate(input0->GetOrigin());
		transform->Scale(input0->GetSpacing());

		vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
			vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformFilter->SetInputConnection(polygonSource->GetOutputPort());
		transformFilter->SetTransform(transform);
		transformFilter->Update();

		appendPolyData->AddInputConnection(transformFilter->GetOutputPort());

		Circles.push_back(transformFilter->GetOutput());
		std::cout << "Center: ";
		std::cout << 
			polygonSource->GetCenter()[0] << ' ' << 
			polygonSource->GetCenter()[1] << ' ' <<
			polygonSource->GetCenter()[2] << ' ' <<
			std::endl;
		std::cout << "Radius: " << polygonSource->GetRadius() << std::endl;
	}

	appendPolyData->Update();

	output->ShallowCopy(appendPolyData->GetOutput());
}

void VesselWallHoughTransformCircles::SetImageRange(int imin, int imax, int jmin, int jmax, int kmin, int kmax)
{
	int dim[6];

	dim[0] = imin;
	dim[1] = imax;
	dim[2] = jmin;
	dim[3] = jmax;
	dim[4] = kmin;
	dim[5] = kmax;

	this->SetImageRange(dim);
}

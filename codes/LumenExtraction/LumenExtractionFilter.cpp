#include "LumenExtractionFilter.h"

#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkImageCast.h>

#include <itkImageToVTKImageFilter.h>
#include <itkVTKImageToImageFilter.h>


vtkStandardNewMacro(LumenExtractionFilter);

LumenExtractionFilter::LumenExtractionFilter() 
{
	CoreFilter = LEFilter::New();
}
LumenExtractionFilter::~LumenExtractionFilter() {}

void LumenExtractionFilter::PrintSelf(ostream & os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
}

int LumenExtractionFilter::RequestUpdateExtent(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
{
	return vtkImageAlgorithm::RequestUpdateExtent(request, inputVector, outputVector);
}

int LumenExtractionFilter::RequestInformation(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
{
	double in_origin[3];
	double in_spacing[3];
	double out_spacing[3];
	double out_origin[3];

	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	int wholeExtent[6], outWholeExt[6];

	inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
	inInfo->Get(vtkDataObject::SPACING(), in_spacing);
	inInfo->Get(vtkDataObject::ORIGIN(), in_origin);

		memcpy(out_spacing, in_spacing, sizeof(double) * 3);
		memcpy(out_origin, in_origin, sizeof(double) * 3);
		memcpy(outWholeExt, wholeExtent, sizeof(int) * 6);

	outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
		outWholeExt, 6);
	outInfo->Set(vtkDataObject::SPACING(), out_spacing, 3);
	outInfo->Set(vtkDataObject::ORIGIN(), out_origin, 3);

	return vtkImageAlgorithm::RequestInformation(request, inputVector, outputVector);
}
//#include <qlist.h>
int LumenExtractionFilter::RequestData(vtkInformation* request,
	vtkInformationVector** inputVector,
	vtkInformationVector* outputVector) {

	typedef itk::VTKImageToImageFilter<InputImageType> VTKImageToImageFilter;
	typedef itk::ImageToVTKImageFilter<InputImageType> ImageToVtkImageFilter;

	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and output
	vtkImageData *input = vtkImageData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkImageData *output = vtkImageData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	VTKImageToImageFilter::Pointer vtkToItk =
		VTKImageToImageFilter::New();
	vtkToItk->SetInput(input);
	vtkToItk->Update();

	CoreFilter->SetInput(vtkToItk->GetOutput());
	CoreFilter->Update();

	ImageToVtkImageFilter::Pointer itkToVtk =
		ImageToVtkImageFilter::New();
	itkToVtk->SetInput(CoreFilter->GetOutput());
	itkToVtk->Update();


	vtkSmartPointer<vtkImageCast> cast =
		vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData(itkToVtk->GetOutput());
	cast->SetOutputScalarTypeToUnsignedChar();
	cast->Update();

	output->ShallowCopy(cast->GetOutput());
	//QList<int> l = { 1, 2, 3 };
	//for (int i = 0; i < 3; ++i) {
	//	// get the info objects
	//	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	//	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	//	// get the input and output
	//	vtkImageData *input = vtkImageData::SafeDownCast(
	//		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	//	vtkImageData *output = vtkImageData::SafeDownCast(
	//		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	//	VTKImageToImageFilter::Pointer vtkToItk =
	//		VTKImageToImageFilter::New();
	//	vtkToItk->SetInput(input);
	//	vtkToItk->Update();

	//	CoreFilter->SetInput(vtkToItk->GetOutput());
	//	CoreFilter->SetNumberOfIterations(l[i]);
	//	CoreFilter->Update();

	//	ImageToVtkImageFilter::Pointer itkToVtk =
	//		ImageToVtkImageFilter::New();
	//	itkToVtk->SetInput(CoreFilter->GetOutput());
	//	itkToVtk->Update();

	//	output->ShallowCopy(itkToVtk->GetOutput());
	//}
	return 1;
}
#include "LumenExtractionFilter.h"

#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include "vtkStreamingDemandDrivenPipeline.h"


vtkStandardNewMacro(LumenExtractionFilter);


void LumenExtractionFilter::PrintSelf(ostream & os, vtkIndent indent)
{
	return Superclass::PrintSelf(os, indent);
}

int LumenExtractionFilter::RequestUpdateExtent(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
{
	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	int wholeExtent[6], outWholeExt[6];

	inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
	memcpy(outWholeExt, wholeExtent, sizeof(int) * 6);
	return 1;
}

int LumenExtractionFilter::RequestInformation(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector)
{
	return Superclass::RequestInformation(request, inputVector, outputVector);
}
#include <vtkNIFTIImageWriter.h>
#include <vtkImageCast.h>
#include <qstring.h>
#include <qlist.h>

int LumenExtractionFilter::RequestData(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
{
	QList<double> multipler = {2.0, 2.3, 2.5};
	QList<int> iteractions = { 3, 9, 15, 20, 23};
	QList<int> initialNeighborhoodRadius = {1, 2, 4};

	// Get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	// Get the input and ouptut
	vtkImageData *input = vtkImageData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkImageData *output = vtkImageData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkSmartPointer<vtkNIFTIImageWriter> writer =
		vtkSmartPointer<vtkNIFTIImageWriter>::New();

	writer->SetInputData(input);
	QString name = "C:\\Users\\jieji\\Desktop\\testResult\\input.nii";
	writer->SetFileName(name.toStdString().c_str());
	writer->Write();


	for (int i = 0; i < multipler.size(); ++i) {
		for (int j = 0; j < iteractions.size(); ++j) {
			for (int k = 0; k < initialNeighborhoodRadius.size(); ++k) {


				VTKImageToImageFilter::Pointer vtkImageToImageFilter =
					VTKImageToImageFilter::New();
				vtkImageToImageFilter->SetInput(input);
				vtkImageToImageFilter->Update();

				CoreFilter->SetInput(vtkImageToImageFilter->GetOutput());
				CoreFilter->SetMultiplier(multipler[i]);
				CoreFilter->SetNumberOfIterations(iteractions[j]);
				CoreFilter->SetInitialNeighborhoodRadius(initialNeighborhoodRadius[k]);
				CoreFilter->Update();

				ImageToVtkImageFilter::Pointer imageToImageFilter =
					ImageToVtkImageFilter::New();
				imageToImageFilter->SetInput(CoreFilter->GetOutput());
				imageToImageFilter->Update();
				output->ShallowCopy(imageToImageFilter->GetOutput());

				vtkSmartPointer<vtkImageCast> castFilter =
					vtkSmartPointer<vtkImageCast>::New();
				castFilter->SetInputData(output);
				castFilter->SetOutputScalarTypeToUnsignedChar();
				castFilter->Update();


				QString test = "multipler=" + QString::number(multipler[i]) + 
					" iteractions=" + QString::number(iteractions[j]) + 
					" Radius=" + QString::number(initialNeighborhoodRadius[k]);
				QString fileName = "C:\\Users\\jieji\\Desktop\\testResult\\" + test + ".nii";
				writer->SetFileName(fileName.toStdString().c_str());
				writer->SetInputConnection(castFilter->GetOutputPort());
				//writer->SetInputData(output);
				writer->Write();
			}
		}
	}
	//// Get the info objects
	//vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	//vtkInformation *outInfo = outputVector->GetInformationObject(0);

	//// Get the input and ouptut
	//vtkImageData *input = vtkImageData::SafeDownCast(
	//	inInfo->Get(vtkDataObject::DATA_OBJECT()));

	//vtkImageData *output = vtkImageData::SafeDownCast(
	//	outInfo->Get(vtkDataObject::DATA_OBJECT()));

	//VTKImageToImageFilter::Pointer vtkImageToImageFilter =
	//	VTKImageToImageFilter::New();
	//vtkImageToImageFilter->SetInput(input);
	//vtkImageToImageFilter->Update();
	//
	//CoreFilter->SetInput(vtkImageToImageFilter->GetOutput());
	//CoreFilter->Update();

	//ImageToVtkImageFilter::Pointer imageToImageFilter =
	//	ImageToVtkImageFilter::New();
	//imageToImageFilter->SetInput(CoreFilter->GetOutput());
	//imageToImageFilter->Update();
	//output->ShallowCopy(imageToImageFilter->GetOutput());

	//return 1;
}

LumenExtractionFilter::LumenExtractionFilter()
{
	CoreFilter = LEFilter::New();
}

LumenExtractionFilter::~LumenExtractionFilter()
{
}
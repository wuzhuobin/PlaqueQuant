#include "vtkPolylineToTubularVolume.h"

#include "vtkImageThreshold.h"
#include "vtkTubeFilter.h"
#include "vtkImageStencil.h"
#include "vtkImageStencilToImage.h"
#include "vtkImageEuclideanDistance.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkPolyData.h"
#include "vtkTriangleFilter.h"
#include "vtkSmartPointer.h"

vtkStandardNewMacro(vtkPolylineToTubularVolume);

int vtkPolylineToTubularVolume::RequestData(vtkInformation *vtkNotUsed(request),
											vtkInformationVector **inputVector,
											vtkInformationVector *outputVector)
{
	vtkImageData*	input	= vtkImageData::GetData(inputVector[0], 0);
	vtkImageData*	output	= vtkImageData::GetData(outputVector, 0);

	if (!this->Polyline)
	{
		vtkErrorMacro("Missing polyline input!");
		return 0;
	}
	else if (this->Polyline->GetNumberOfCells() > 1)
	{
		vtkWarningMacro("This algorithm may not work for surface input.");
	}

	try {
		/// Prepare output
		output->SetDimensions(input->GetDimensions());
		output->SetOrigin(input->GetOrigin());
		output->SetExtent(input->GetExtent());
		output->SetSpacing(input->GetSpacing());
		output->AllocateScalars(VTK_FLOAT, 1);

		/// Creates a tube from the input polyline
		// Calculate average spacing
		double tubeRadius = (input->GetSpacing()[0] + input->GetSpacing()[1] +
			input->GetSpacing()[2]) / 3.;

		// Create tube polydata
		vtkSmartPointer<vtkTubeFilter> tubeFilter
			= vtkSmartPointer<vtkTubeFilter>::New();
		tubeFilter->SetInputData(this->Polyline);
		tubeFilter->SetRadius(tubeRadius);
		tubeFilter->SetNumberOfSides(6);
		tubeFilter->CappingOn();
		tubeFilter->Update();

		// Triangulate output
		vtkSmartPointer<vtkTriangleFilter> triFilter
			= vtkSmartPointer<vtkTriangleFilter>::New();
		triFilter->SetInputConnection(tubeFilter->GetOutputPort());
		triFilter->Update();

		/// Creates an image stencil from the tube for DT (Distance transform)
		float inVal = 0;
		float outVal = VTK_FLOAT_MAX;
		vtkSmartPointer<vtkPolyDataToImageStencil> pol2stenc
			= vtkSmartPointer<vtkPolyDataToImageStencil>::New();
		pol2stenc->SetInputData(triFilter->GetOutput());
		pol2stenc->SetOutputSpacing(output->GetSpacing());
		pol2stenc->SetOutputWholeExtent(output->GetExtent());
		pol2stenc->SetOutputOrigin(output->GetOrigin());
		pol2stenc->Update();

		// cut the corresponding white image and set the background:
		vtkSmartPointer<vtkImageStencil> imgstenc
			= vtkSmartPointer<vtkImageStencil>::New();
		imgstenc->SetInputData(output);
		imgstenc->SetStencilConnection(pol2stenc->GetOutputPort());
		imgstenc->ReverseStencilOff();
		imgstenc->SetBackgroundValue(outVal);
		imgstenc->Update();

		/// Carry out distance transform
		vtkSmartPointer<vtkImageEuclideanDistance> distanceTransform
			= vtkSmartPointer<vtkImageEuclideanDistance>::New();
		distanceTransform->SetInputData(imgstenc->GetOutput());
		distanceTransform->SetConsiderAnisotropy(1); // Note that something was done to VTK source code before this works properly
		distanceTransform->SetMaximumDistance(500);
		distanceTransform->InitializeOn();
		distanceTransform->Update();


		/// Carry out thresholding according to desired radius
		vtkSmartPointer<vtkImageThreshold> threshold
			= vtkSmartPointer<vtkImageThreshold>::New();
		threshold->SetInputConnection(distanceTransform->GetOutputPort());
		threshold->SetInValue(VTK_UNSIGNED_CHAR_MAX);
		threshold->SetOutValue(0);
		threshold->SetOutputScalarTypeToUnsignedChar();
		threshold->ThresholdByLower(pow(this->TubeRadius, 2));
		threshold->Update();

		/// Copy result to output
		output->DeepCopy(threshold->GetOutput());
	}
	catch (...) {
		return 0;
	}

	/// Free memory
	return 1;
}

vtkPolylineToTubularVolume::vtkPolylineToTubularVolume()
{
	this->SetNumberOfInputPorts(1);
	this->SetNumberOfOutputPorts(1);

	this->Polyline = NULL;
}

vtkPolylineToTubularVolume::~vtkPolylineToTubularVolume()
{
	this->Polyline = NULL;
}

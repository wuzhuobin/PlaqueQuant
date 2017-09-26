#include "vtkPolylineToTubularVolume.h"

#include "vtkImageThreshold.h"
#include "vtkTubeFilter.h"
#include "vtkImageStencil.h"
//#include "vtkImageStencilToImage.h"
#include "vtkImageEuclideanDistance.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkPointData.h"

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
		/// Creates a tube from the input polyline
		// Calculate average spacing
		double tubeRadius = fmin(fmin(input->GetSpacing()[0], input->GetSpacing()[1]), input->GetSpacing()[2]) * 0.5;
		// Create tube polydata
		vtkSmartPointer<vtkTubeFilter> tubeFilter
			= vtkSmartPointer<vtkTubeFilter>::New();
		tubeFilter->SetInputData(this->Polyline);
		tubeFilter->SetRadius(tubeRadius);
		tubeFilter->SetNumberOfSides(6);
		tubeFilter->CappingOn();
		tubeFilter->Update();

		/// Creates an image stencil from the tube for DT (Distance transform)
		vtkSmartPointer<vtkPolyDataToImageStencil> pol2stenc
			= vtkSmartPointer<vtkPolyDataToImageStencil>::New();
		pol2stenc->SetInputData(tubeFilter->GetOutput());
		pol2stenc->SetOutputSpacing(input->GetSpacing());
		pol2stenc->SetOutputWholeExtent(input->GetExtent());
		pol2stenc->SetOutputOrigin(input->GetOrigin());
		pol2stenc->Update();

		// cut the corresponding white image and set the background:
		vtkSmartPointer<vtkImageData> blackImage =
			vtkSmartPointer<vtkImageData>::New();
		blackImage->DeepCopy(input);
		vtkIdType count = blackImage->GetNumberOfPoints();
		for (vtkIdType i = 0; i < count; ++i)
		{
			blackImage->GetPointData()->GetScalars()->SetTuple1(i, VTK_UNSIGNED_CHAR_MIN);
		}
		vtkSmartPointer<vtkImageStencil> imgstenc
			= vtkSmartPointer<vtkImageStencil>::New();
		imgstenc->SetInputData(blackImage);
		imgstenc->SetStencilConnection(pol2stenc->GetOutputPort());
		imgstenc->ReverseStencilOff();
		imgstenc->SetBackgroundValue(VTK_UNSIGNED_CHAR_MAX);
		imgstenc->Update();

		/// Carry out distance transform
		vtkSmartPointer<vtkImageEuclideanDistance> distanceTransform
			= vtkSmartPointer<vtkImageEuclideanDistance>::New();
		distanceTransform->SetInputData(imgstenc->GetOutput());
		distanceTransform->ConsiderAnisotropyOn(); // Note that something was done to VTK source code before this works properly
		distanceTransform->SetMaximumDistance(VTK_UNSIGNED_CHAR_MAX);
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
		output->ShallowCopy(threshold->GetOutput());
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

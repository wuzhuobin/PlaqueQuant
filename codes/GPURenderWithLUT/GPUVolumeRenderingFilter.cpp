#include <vtkSmartPointer.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include "GPUVolumeRenderingFilter.h"

vtkStandardNewMacro(GPUVolumeRenderingFilter);

GPUVolumeRenderingFilter::GPUVolumeRenderingFilter()
{
	this->GPUVolumeMapper		= vtkGPUVolumeRayCastMapper::New();
	this->Volume				= vtkVolume::New();
	this->VolumeProperty		= vtkVolumeProperty::New();
	this->LookUpTable = NULL;
}

GPUVolumeRenderingFilter::~GPUVolumeRenderingFilter()
{
	
	if (this->Volume) {
		this->Volume->Delete();
		this->Volume = NULL;
	}
	if (this->GPUVolumeMapper) {
		this->GPUVolumeMapper->Delete();
		this->GPUVolumeMapper = NULL;
	}

	if (this->VolumeProperty) {
		this->VolumeProperty->Delete();
		this->VolumeProperty = NULL;
	}


}

void GPUVolumeRenderingFilter::SetInputData(vtkImageData *dat)
{
	this->InputData = dat;
	this->GPUVolumeMapper->SetInputData(dat);
}

void GPUVolumeRenderingFilter::Update()
{
	if (!this->LookUpTable) {
		vtkErrorMacro("No Input Lookup Table!");
		return;
	}

	if (!this->InputData) {
		vtkErrorMacro("No Input Data!");
		return;
	}

	double range[2];
	range[0] = 0;
	range[1] = 6.;
	int numberOfValues = this->LookUpTable->GetNumberOfTableValues();

	cout << "Range: " << range[0] << " " << range[1] << endl;

	vtkColorTransferFunction*  color = vtkColorTransferFunction::New();
	vtkPiecewiseFunction* compositeOpacity = vtkPiecewiseFunction::New();
	for (int i = 0; i < numberOfValues; i++)
	{
		cout << "Val: " << i / double(numberOfValues - 1) * (range[1] - range[0]) + range[0] << endl;
		double *rgba = this->LookUpTable->GetTableValue(i);
		compositeOpacity->AddPoint(double(i/double(numberOfValues - 1) * (range[1] - range[0]) + range[0]), rgba[3]);
		color->AddRGBPoint(double(i / double(numberOfValues - 1) * (range[1] - range[0]) + range[0]), rgba[0], rgba[1], rgba[2]);
	}

	compositeOpacity->AddPoint(double(range[1]) + 0.1, 0);
	
	this->VolumeProperty->SetScalarOpacity(compositeOpacity);
	this->VolumeProperty->SetColor(color);

	this->Volume->SetMapper(this->GPUVolumeMapper);
	this->Volume->SetProperty(this->VolumeProperty);
}

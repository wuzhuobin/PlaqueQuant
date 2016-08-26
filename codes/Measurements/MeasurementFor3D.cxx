#include "MeasurementFor3D.h"



MeasurementFor3D::MeasurementFor3D()
{
	this->overlay = NULL;
	this->lookupTable = NULL;
	this->volumes = NULL;
}

MeasurementFor3D::~MeasurementFor3D()
{
	if (this->volumes != NULL) {
		delete volumes;
	}
}

void MeasurementFor3D::SetInputData(vtkImageData * overlay)
{
	this->overlay = overlay;
}

void MeasurementFor3D::SetLookupTable(vtkLookupTable * lookupTable)
{
	this->lookupTable = lookupTable;
}

void MeasurementFor3D::Update()
{
	if (this->overlay == NULL)
		return;
	double spacing[3];
	int extent[6];
	double pixelSize = 1;
	int num = lookupTable->GetNumberOfColors();
	int* numberOfPixels = new int[num];
	volumes = new double[num];

	overlay->GetSpacing(spacing);
	overlay->GetExtent(extent);
	// calculate the volume of a single pixel
	for (int i = 0; i < 3; ++i) {
		pixelSize *= spacing[i];
	}
	// initialize the numberOfPixels
	for (int i = 0; i < num; ++i) {
		numberOfPixels[i] = 0;
	}
	// iterate an overlay to calculate the number of pixels
	for (int i = extent[0]; i < extent[1]; ++i) {
		for (int j = extent[2]; j < extent[3]; ++j) {
			for (int k = extent[4]; k < extent[5]; ++k) {
				double* val = static_cast<double*>(overlay->GetScalarPointer(i, j, k));
				numberOfPixels[int(*val)]++;
			}
		}
		
	}
	volumes[0] = 0;
	for (int index = 1; index < num; ++index) {
		volumes[index] = numberOfPixels[index] * pixelSize;
		cout << "3D Measurement " << index <<": \n";
		cout << volumes[index] << endl;
		if (index > 2) {
			volumes[0] += volumes[index];
		}
	}
	delete[] numberOfPixels;
}

double* MeasurementFor3D::GetVolumes()
{
	return this->volumes;
}


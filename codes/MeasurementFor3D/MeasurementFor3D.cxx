#include "MeasurementFor3D.h"



MeasurementFor3D::MeasurementFor3D()
{
	this->overlay = NULL;
	this->lookupTable = NULL;
	this->volumes = NULL;
}

MeasurementFor3D::~MeasurementFor3D()
{
	if (volumes != NULL) {
		delete[] volumes;
		volumes = NULL;
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
	int* numberOfPixels = new int[lookupTable->GetNumberOfTableValues()];

	overlay->GetSpacing(spacing);
	overlay->GetExtent(extent);
	// calculate the volume of a single pixel
	for (int i = 0; i < 3; ++i) {
		pixelSize *= spacing[i];
	}
	// iterate an overlay to calculate the number of pixels
	for (int index = 0; index < lookupTable->GetNumberOfColors(); ++index) {
		numberOfPixels[index] = 0;
		for (int i = extent[0]; i < extent[1]; ++i) {
			for (int j = extent[2]; j < extent[3]; ++j) {
				for (int k = extent[4]; k < extent[5]; ++k) {
					double* val = static_cast<double*>(overlay->GetScalarPointer(i, j, k));

					if (*val == index) {
						++numberOfPixels[index];
					}
				}


			}
		}
		volumes[index] = numberOfPixels[index] * pixelSize;
		cout << "3D Measurement: \n";
		cout << volumes[index] << endl;
	}


	delete[] numberOfPixels;
}

void MeasurementFor3D::GetVolumes(double* volumes)
{
	memcpy(volumes, this->volumes, sizeof(this->volumes));
}


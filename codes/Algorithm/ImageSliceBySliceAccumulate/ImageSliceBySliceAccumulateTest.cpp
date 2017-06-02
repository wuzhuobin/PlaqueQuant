/*
* In this example, the resulting histogram has 6 pixels (all of them) in the 3rd bin (value = 2) and zero in the rest of the bins
*/
#include <vtkVersion.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

#include "ImageSliceBySliceAccumulate.h"
static void CreateImage(vtkSmartPointer<vtkImageData> image);

int main(int, char *[])
{
	vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
	CreateImage(image);

	vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
		vtkSmartPointer<vtkImageAccumulate>::New();
	imageAccumulate->SetInputData(image);
	imageAccumulate->SetComponentExtent(0, 3, 0, 0, 0, 0);
	imageAccumulate->SetComponentOrigin(0, 0, 0);
	imageAccumulate->SetComponentSpacing(1, 0, 0); // This will count exactly the number of pixels of each color. Use (10,0,0) to make bins of width 10 instead.
	imageAccumulate->Update();

	int dims[3];
	imageAccumulate->GetOutput()->GetDimensions(dims);
	std::cout << "vtkImageAccumulate" << std::endl;
	for (vtkIdType bin = 0; bin < dims[0]; ++bin)
	{
		std::cout << *(static_cast<int*>(imageAccumulate->GetOutput()->GetScalarPointer(bin, 0, 0))) << " ";
	}
	std::cout << std::endl;

	vtkSmartPointer<ImageSliceBySliceAccumulate> imageSliceBySliceAccumulate =
		vtkSmartPointer<ImageSliceBySliceAccumulate>::New();
	imageSliceBySliceAccumulate->SetReferenceCount(1);
	imageSliceBySliceAccumulate->SetInputData(image);
	imageSliceBySliceAccumulate->SetSliceOrientationToYZ();
	imageSliceBySliceAccumulate->SetExtent(0, 3, 0, 0, 0, 0);
	imageSliceBySliceAccumulate->SetOrigin(0, 0, 0);
	imageSliceBySliceAccumulate->SetSpacing(1, 0, 0); // This will count exactly the number of pixels of each color. Use (10,0,0) to make bins of width 10 instead.
	imageSliceBySliceAccumulate->Update();

	std::cout << "imageSliceBySliceAccumulate" << std::endl;

	const int* extent = imageSliceBySliceAccumulate->GetOutput()->GetExtent();
	for (int k = extent[4]; k <= extent[5]; ++k) {
		for (int j = extent[2]; j <= extent[3]; ++j) {
			for (int i = extent[0]; i <= extent[1]; ++i) {
				std::cout << *(static_cast<int*>(imageSliceBySliceAccumulate->GetOutput()->GetScalarPointer(i, j, k))) << " ";
			}
		}
	}

	return  EXIT_SUCCESS;
}

void CreateImage(vtkSmartPointer<vtkImageData> image)
{
	// Specify the size of the image data
	image->SetDimensions(2, 3, 5);
	//image->SetExtent(0,2,0,3,0,1);
#if VTK_MAJOR_VERSION <= 5
	image->SetNumberOfScalarComponents(1);
	image->SetScalarTypeToDouble();
	image->AllocateScalars();
#else
	image->AllocateScalars(VTK_DOUBLE, 1);
#endif
	int* dims = image->GetDimensions();
	// int dims[3]; // can't do this

	std::cout << "Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;

	// Fill every entry of the image data with "2.0"
	for (int z = 0; z < dims[2]; z++)
	{
		for (int y = 0; y < dims[1]; y++)
		{
			for (int x = 0; x < dims[0]; x++)
			{
				double* pixel = static_cast<double*>(image->GetScalarPointer(x, y, z));
				pixel[0] = 2.0;
			}
		}
	}
}
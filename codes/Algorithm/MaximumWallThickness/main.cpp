/*
* In this example, the resulting histogram has 6 pixels (all of them) in the 3rd bin (value = 2) and zero in the rest of the bins
*/
#include <vtkVersion.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>


#include <vtkNIFTIImageReader.h>

#include "MaximumWallThickness2.h"
static void CreateImage(vtkSmartPointer<vtkImageData> image);

int main(int, char *[])
{
	vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
	CreateImage(image);

	vtkSmartPointer<vtkNIFTIImageReader> reader =
		vtkSmartPointer<vtkNIFTIImageReader>::New();
	reader->SetFileName("C:/Users/jieji/Desktop/work/PlaqueQuant/b2.nii");
	reader->Update();

	vtkSmartPointer<MaximumWallThickness2> f =
		vtkSmartPointer<MaximumWallThickness2>::New();
	f->SetInputConnection(reader->GetOutputPort());
	f->Update();

	

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
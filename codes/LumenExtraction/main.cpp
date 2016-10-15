#include <vtkXMLImageDataReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include <lumenExtraction.h>

#include <vtkXMLImageDataWriter.h>
#include <vtkSTLWriter.h>

#include <itkImage.h>
#include <itkImageFileWriter.h>

typedef itk::Image<float, 3> ImageType;
typedef itk::ImageFileWriter<ImageType> ImageFileWriterType;

int main()
{
	// read image
	vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
	reader->SetFileName("../test_data/T1_crop.vti");
	reader->Update();

	vtkSmartPointer<vtkImageData> image = reader->GetOutput();

	double seed1[3];
	seed1[0] = 33;
	seed1[1] = 29;
	seed1[2] = 5;

	double seed2[3];
	seed2[0] = 41;
	seed2[1] = 19;
	seed2[2] = 47;

	double seed3[3];
	seed3[0] = 27;
	seed3[1] = 59;
	seed3[2] = 30;

	LumenExtraction* lumenExtractor = new LumenExtraction;
	lumenExtractor->SetInputData(image);
	lumenExtractor->SetMultiplier(2.3);
	lumenExtractor->SetInitialNeighborhoodRadius(4);
	// constant
	lumenExtractor->SetNumberOfIterations(50);
	lumenExtractor->SetDilationValue(3);
	lumenExtractor->AddSeed(seed1);
	lumenExtractor->AddSeed(seed2);
	//lumenExtractor->AddSeed(seed3);
	lumenExtractor->Update();

	ImageType::Pointer inputItkImage = ImageType::New();
	lumenExtractor->GetInputItkImage(inputItkImage);

	ImageFileWriterType::Pointer writer = ImageFileWriterType::New();
	writer->SetFileName("../test_data/T1_crop.nii");
	writer->SetInput(inputItkImage);
	writer->Write();

	ImageType::Pointer lumenItkImage = ImageType::New();
	lumenExtractor->GetLumenItkImage(lumenItkImage);

	writer->SetFileName("../test_data/lumen.nii");
	writer->SetInput(lumenItkImage);
	writer->Write();

	ImageType::Pointer dilatedItkImage = ImageType::New();
	lumenExtractor->GetDilatedItkImage(dilatedItkImage);

	writer->SetFileName("../test_data/lumen_dilated.nii");
	writer->SetInput(dilatedItkImage);
	writer->Write();

	vtkSmartPointer<vtkImageData> lumenImage = vtkSmartPointer<vtkImageData>::New();
	lumenExtractor->GetLumenVtkImage(lumenImage);

	vtkSmartPointer<vtkImageData> dilatedImage = vtkSmartPointer<vtkImageData>::New();
	lumenExtractor->GetDilatedVtkImage(dilatedImage);

	vtkSmartPointer<vtkXMLImageDataWriter> imageWriter = vtkSmartPointer<vtkXMLImageDataWriter>::New();
	imageWriter->SetFileName("../test_data/lumen.vti");
	imageWriter->SetInputData(lumenImage);
	imageWriter->Write();

	imageWriter->SetFileName("../test_data/lumen_dilated.vti");
	imageWriter->SetInputData(dilatedImage);
	imageWriter->Write();

	//vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
	//writer->SetFileName("D:\\cxxProjects\\lumenExtraction\\test_data\\lumen.stl");
	//writer->SetInputData(lumen);
	//writer->Write();



	system("pause");

}
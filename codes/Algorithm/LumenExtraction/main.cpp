#include <vtkXMLImageDataReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include <lumenExtraction.h>

#include <vtkXMLImageDataWriter.h>
#include <vtkSTLWriter.h>
#include <vtkImageCast.h>
#include <vtkNIFTIImageReader.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkExtractVOI.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <itkImage.h>
#include <itkImageFileWriter.h>

typedef itk::Image<float, 3> ImageType;
typedef itk::ImageFileWriter<ImageType> ImageFileWriterType;

int main()
{
	// read image
	//vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
	vtkSmartPointer<vtkNIFTIImageReader> reader =
		vtkSmartPointer<vtkNIFTIImageReader>::New();
	//reader->SetFileName("C:\\Users\\jieji\\Desktop\\MACOSX_BUNDLE\\JackyData\\"
	//	"nifti_corrected\\CUBE T1 corrected.nii");
	reader->SetFileName("C:\\Users\\jieji\\Desktop\\test\\voi.nii");
	reader->Update();

	vtkSmartPointer<vtkExtractVOI> voi =
		vtkSmartPointer<vtkExtractVOI>::New();
	voi->SetInputConnection(reader->GetOutputPort());
	voi->SetVOI(128, 172, 221, 312, 0, 55);
	voi->Update();


	vtkSmartPointer<vtkNIFTIImageWriter> imageWriter = vtkSmartPointer<vtkNIFTIImageWriter>::New();
	imageWriter->SetFileName("C:\\Users\\jieji\\Desktop\\voi.nii");
	imageWriter->SetInputConnection(reader->GetOutputPort());
	imageWriter->Write();


	vtkSmartPointer<vtkImageData> image = reader->GetOutput();

	int seed1[3];
	//seed1[0] = 33;
	//seed1[1] = 29;
	//seed1[2] = 5;

	seed1[0] = 146;
	seed1[1] = 283;
	seed1[2] = 53;


	//int seed2[3];
	//seed2[0] = 41;
	//seed2[1] = 19;
	//seed2[2] = 47;

	//int seed3[3];
	//seed3[0] = 27;
	//seed3[1] = 59;
	//seed3[2] = 30;

	LumenExtraction* lumenExtractor = new LumenExtraction;
	lumenExtractor->SetInputData(image);
	lumenExtractor->SetMultiplier(2.3);
	lumenExtractor->SetInitialNeighborhoodRadius(1);
	// constant
	lumenExtractor->SetNumberOfIterations(30);
	lumenExtractor->SetDilationValue(4);
	lumenExtractor->AddSeed(seed1);
	//lumenExtractor->AddSeed(seed2);
	//lumenExtractor->AddSeed(seed3);
	lumenExtractor->Update();

	//ImageType::Pointer inputItkImage = ImageType::New();
	//lumenExtractor->GetInputItkImage(inputItkImage);

	//ImageFileWriterType::Pointer writer = ImageFileWriterType::New();
	//writer->SetFileName("C:\\Users\\jieji\\Desktop\\T1_crop.nii");
	//writer->SetInput(inputItkImage);
	//writer->Write();

	//ImageType::Pointer lumenItkImage = ImageType::New();
	//lumenExtractor->GetLumenItkImage(lumenItkImage);

	//writer->SetFileName("C:\\Users\\jieji\\Desktop\\lumen.nii");
	//writer->SetInput(lumenItkImage);
	//writer->Write();

	//ImageType::Pointer dilatedItkImage = ImageType::New();
	//lumenExtractor->GetDilatedItkImage(dilatedItkImage);

	//writer->SetFileName("C:\\Users\\jieji\\Desktop\\lumen_dilated.nii");
	//writer->SetInput(dilatedItkImage);
	//writer->Write();

	vtkSmartPointer<vtkImageData> lumenImage = vtkSmartPointer<vtkImageData>::New();
	lumenExtractor->GetLumenVtkImage(lumenImage);

	vtkSmartPointer<vtkImageData> dilatedImage = vtkSmartPointer<vtkImageData>::New();
	lumenExtractor->GetDilatedVtkImage(dilatedImage);

	imageWriter->SetFileName("C:\\Users\\jieji\\Desktop\\lumen.nii");
	imageWriter->SetInputData(lumenImage);
	imageWriter->Write();

	//imageWriter->SetFileName("C:\\Users\\jieji\\Desktop\\lumen_dilated.nii");
	//imageWriter->SetInputData(dilatedImage);
	//imageWriter->Write();

	//vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
	//writer->SetFileName("D:\\cxxProjects\\lumenExtraction\\test_data\\lumen.stl");
	//writer->SetInputData(lumen);
	//writer->Write();



	//system("pause");

}
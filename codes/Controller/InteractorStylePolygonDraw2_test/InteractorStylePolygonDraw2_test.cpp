//
// This example demonstrates how to read a series of dicom images
// and how to scroll with the mousewheel or the up/down keys
// through all slices
//
// some standard vtk headers
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
// headers needed for this example
#include <ImageViewer.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include "ImageViewer.h"
#include "InteractorStyleVOI2.h"
// needed to easily convert int to std::string
#include <sstream>




int main(int argc, char* argv[])
{
	// Verify input arguments
	//if (argc != 2)
	//{
	//	std::cout << "Usage: " << argv[0]
	//		<< " FolderName" << std::endl;
	//	return EXIT_FAILURE;
	//}


	std::string folder = "C:/Users/jieji/Desktop/work/IADE/T2propeller&MRA/MRA";

	// Read all the DICOM files in the specified directory.
	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();

	// Visualize
	vtkSmartPointer<ImageViewer> imageViewer =
		vtkSmartPointer<ImageViewer>::New();
	imageViewer->SetInputData(reader->GetOutput());


	// create an interactor with our own style (inherit from vtkInteractorStyleImage)
	// in order to catch mousewheel and key events
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<InteractorStyleVOI2> myInteractorStyle =
		vtkSmartPointer<InteractorStyleVOI2>::New();
	myInteractorStyle->SetViewer(imageViewer);



	imageViewer->SetupInteractor(renderWindowInteractor);
	// make the interactor use our own interactorstyle
	// cause SetupInteractor() is defining it's own default interatorstyle 
	// this must be called after SetupInteractor()
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	myInteractorStyle->CustomEnabledOn();


	// initialize rendering and interaction
	//imageViewer->GetRenderWindow()->SetSize(400, 300);
	//imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	renderWindowInteractor->Start();
	return EXIT_SUCCESS;
}
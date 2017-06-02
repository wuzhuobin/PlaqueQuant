#include "CenterlineSurfaceViewer.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <InteractorStyleSurfaceCenterLineSimpleClipping.h>
#include <vtkNIFTIImageReader.h>
#include <vtkLookupTable.h>
#include <vtkRenderer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageData.h>
#include <vtkImageProperty.h>

#include <vtkSmartPointer.h>



int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkNIFTIImageReader> niftiReader =
		vtkSmartPointer<vtkNIFTIImageReader>::New();
	niftiReader->SetFileName("C:/Users/jieji/Desktop/222.nii");
	niftiReader->Update();

	//vtkSmartPointer<vtkDICOMImageReader> dicomReader =
	//	vtkSmartPointer<vtkDICOMImageReader>::New();
	//dicomReader->SetDirectoryName("C:/Users/jieji/Desktop/work/IADE/T2propeller&MRA/MRA");
	//dicomReader->Update();
	//dicomReader->GetOutput()->Print(cout);

	//vtkSmartPointer<vtkXMLPolyDataReader> polyDataReader =
	//	vtkSmartPointer<vtkXMLPolyDataReader>::New();
	//polyDataReader->SetFileName("C:/Users/jieji/Desktop/centerline.vtp");
	//polyDataReader->Update();

	//vtkSmartPointer<vtkActor> actor =
	//	vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(vtkSmartPointer<vtkPolyDataMapper>::New());
	//actor->GetMapper()->SetInputConnection(polyDataReader->GetOutputPort());


	vtkSmartPointer<vtkLookupTable> lookupTable =
		vtkSmartPointer<vtkLookupTable>::New();
	lookupTable->SetNumberOfTableValues(4);
	lookupTable->SetTableValue(0, 1, 1, 1, 0);
	lookupTable->SetTableValue(1, 0, 0, 0, 0);
	lookupTable->SetTableValue(2, 1, 0, 0, 0.8);
	lookupTable->SetTableValue(3, 0, 0, 1, 1);


	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();



	//vtkSmartPointer<vtkImageActor> imageActor =
	//	vtkSmartPointer<vtkImageActor>::New();
	//imageActor->GetMapper()->SetInputConnection(niftiReader->GetOutputPort());
	//imageActor->GetProperty()->SetLookupTable(lookupTable);
	//imageActor->GetProperty()->UseLookupTableScalarRangeOn();
	//imageActor->SetDisplayExtent(niftiReader->GetOutput()->GetExtent());

	vtkSmartPointer<vtkPolyData> centerline =
		vtkSmartPointer<vtkPolyData>::New();

	vtkSmartPointer<CenterlineSurfaceViewer> viewer =
		vtkSmartPointer<CenterlineSurfaceViewer>::New();
	viewer->EnableDepthPeelingOn();
	viewer->SetLookupTable(lookupTable);
	viewer->SetInputData(niftiReader->GetOutput());
	
	viewer->SetCenterline(centerline);
	viewer->SetupInteractor(interactor);
	//viewer->GetRenderer()->AddActor(actor);
	//viewer->GetRenderer()->AddActor(imageActor);
	viewer->GetRenderer()->ResetCamera();
	viewer->Render();
	viewer->GetRenderer()->ResetCameraClippingRange();
	viewer->Render();


	vtkSmartPointer<InteractorStyleSurfaceCenterLineSimpleClipping> style =
		vtkSmartPointer<InteractorStyleSurfaceCenterLineSimpleClipping>::New();
	style->SetViewer(viewer);
	interactor->SetInteractorStyle(style);
	style->SetCustomEnabled(true);
	//style->SetSurfaceViewer(viewer);
	//style->SetCustomEnabled(true);

	interactor->Start();


}

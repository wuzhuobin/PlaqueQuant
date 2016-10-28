#include "MaximumWallThickness.h"
#include <vtkImageData.h>
#include <vtkNIFTIImageReader.h>
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkSphereSource.h"
#include "vtkSmartPointer.h"
#include "vtkCenterOfMass.h"
#include "vtkCellData.h"
#include "vtkPointData.h"

namespace Testing 
{
vtkActor* GetActor(vtkPolyData* inPD, double r, double g, double b) {
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(inPD);
	mapper->Update();

	vtkActor* actor = vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(r, g, b);
	return actor;
}

vtkActor*	GetSphereActor(double* coord, double *color, double radius) {
	vtkActor* outAct;

	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetCenter(coord);
	sphere->SetPhiResolution(20);
	sphere->SetThetaResolution(20);
	sphere->SetRadius(radius);
	sphere->Update();

	outAct = GetActor(sphere->GetOutput(), 1, 1, 1);
	outAct->GetProperty()->SetColor(color);
	return outAct;
}

vtkActor *GetSphereActor(double x1, double x2, double x3, double R, double G, double B, double radius) {
	double coord[3], color[3];
	coord[0] = x1;
	coord[1] = x2;
	coord[2] = x3;
	color[0] = R;
	color[1] = G;
	color[2] = B;

	return GetSphereActor(coord, color, radius);
}

}

int main() {

	vtkSmartPointer<vtkNIFTIImageReader> reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	reader->SetFileName("C:/Users/lwong/Source/datacenter/nifti_corrected/generatedSegmentation.nii");
	reader->Update();

	MaximumWallThickness mwt;
	mwt.SetImage(reader->GetOutput());
	mwt.SetSliceNumber(48);

	std::vector<MaximumWallThickness::DistanceLoopPair> lp;
	try {
		mwt.Update();
		lp =  mwt.GetDistanceLoopPairVect();
	}
	catch (MaximumWallThickness::ERROR_CODE e) {
		cerr << "Error: " << e;
	}


	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renwin = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	double red[3] = { 1, 0, 0 };
	double blue[3] = { 0,0,1 };
	double green[3] = { 0, 1, 0 };

	for (int i = 0; i < lp.size(); i++)
	{
		vtkSmartPointer<vtkPolyData> vess = vtkSmartPointer<vtkPolyData>::New();
		vtkSmartPointer<vtkPolyData> lum = vtkSmartPointer<vtkPolyData>::New();
		vess->DeepCopy(lp.at(i).LoopPair.first);
		lum->DeepCopy(lp.at(i).LoopPair.second);
		vess->GetCellData()->RemoveArray("RegionId");
		lum->GetCellData()->RemoveArray("RegionId");
		vess->GetPointData()->RemoveArray("RegionId");
		lum->GetPointData()->RemoveArray("RegionId");

		vtkSmartPointer<vtkCenterOfMass> comFilter = vtkSmartPointer<vtkCenterOfMass>::New();
		comFilter->SetInputData(lum);
		comFilter->Update();

		vtkActor* loop1 = Testing::GetActor(vess, 0, 0, 1);
		vtkActor* loop2 = Testing::GetActor(lum, 1, 1, 0);

		vtkActor* pt1 = Testing::GetSphereActor(vess->GetPoint(lp.at(i).PIDs.first), green, 0.1);
		vtkActor* pt2 = Testing::GetSphereActor(lum->GetPoint(lp.at(i).PIDs.second), green, 0.1);
		vtkActor* com = Testing::GetSphereActor(comFilter->GetCenter(), red, 0.5);

		ren->AddActor(loop1);
		ren->AddActor(loop2);
		ren->AddActor(pt1);
		ren->AddActor(pt2);
		ren->AddActor(com);
	}
	renwin->AddRenderer(ren);
	iren->SetRenderWindow(renwin);
	iren->Initialize();
	renwin->Render();
	iren->Start();
	return 0;
}
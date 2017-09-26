#include "MagneticContourLineInterpolator.h"

#include <vtkObjectFactory.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkContourRepresentation.h>
#include <vtkImageActor.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkImageShiftScale.h>
#include <vtkDijkstraImageGeodesicPath.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageData.h>

vtkStandardNewMacro(MagneticContourLineInterpolator);

void MagneticContourLineInterpolator::PrintSelf(ostream & os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
	os << indent << "DijkstraImageGeodesicPath: " << this->DijkstraImageGeodesicPath << endl;
	os << indent << "ImageGradientMagnitude: " << this->ImageGradientMagnitude << endl;
	os << indent << "ImageShiftScale: " << this->ImageShiftScale << endl;
	os << indent << "ImageGaussianSmooth: " << this->ImageGaussianSmooth << endl;
	os << indent << "CostImage: " << this->GetCostImage() << endl;
}

int MagneticContourLineInterpolator::InterpolateLine(
	vtkRenderer* ren, vtkContourRepresentation *rep,
	int idx1, int idx2 )
{
	vtkImageActorPointPlacer *placer =
		vtkImageActorPointPlacer::SafeDownCast(rep->GetPointPlacer());

	if (!placer)
	{
		return 1;
	}

	vtkImageActor* actor = placer->GetImageActor();
	if (!actor || !(actor->GetInput()))
	{
		return 1;
	}
	this->ImageGaussianSmooth->SetInputData(actor->GetInput());
	this->ImageShiftScale->Update();
	
	if (this->CostImage != this->ImageShiftScale->GetOutput()) {
		this->SetCostImage(this->ImageShiftScale->GetOutput());
	}
	return Superclass::InterpolateLine(ren, rep, idx1, idx2);
}

MagneticContourLineInterpolator::MagneticContourLineInterpolator()
{
	this->ImageGaussianSmooth = vtkImageGaussianSmooth::New();
	this->ImageGaussianSmooth->SetStandardDeviation(1, 1);

	this->ImageGradientMagnitude = vtkImageGradientMagnitude::New();
	this->ImageGradientMagnitude->SetInputConnection(this->ImageGaussianSmooth->GetOutputPort());

	this->ImageShiftScale = vtkImageShiftScale::New();
	this->ImageShiftScale->SetInputConnection(this->ImageGradientMagnitude->GetOutputPort());
	this->ImageShiftScale->SetScale(-1);
}

MagneticContourLineInterpolator::~MagneticContourLineInterpolator()
{
	this->ImageShiftScale->SetInputConnection(nullptr);
	this->ImageShiftScale->Delete(); 
	this->ImageShiftScale = nullptr;

	this->ImageGradientMagnitude->Delete();
	this->ImageGradientMagnitude = nullptr;
}

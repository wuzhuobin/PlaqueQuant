#include "CenterlineSurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkCleanPolyData.h>
#include <vtkKdTreePointLocator.h>
#include <vtkRenderer.h>
#include <vtkTubeFilter.h>
#include <vtkSplineFilter.h>
#include <vtkImageData.h>

vtkStandardNewMacro(CenterlineSurfaceViewer);

void CenterlineSurfaceViewer::PrintSelf(ostream & os, vtkIndent indent)
{
	SurfaceViewer::PrintSelf(os, indent);
}

void CenterlineSurfaceViewer::SetCenterline(vtkPolyData * centerline)
{
	if (GetCenterline() != centerline) {
		this->CenterlineActor->VisibilityOn();
		this->CleanPolyData->SetInputData(centerline);
		if (GetInput()) {
			const double* spacing = GetInput()->GetSpacing();
			this->SplineFilter->SetLength(fmin(spacing[0], fmin(spacing[1], spacing[2])));
		}
		//this->KdTreePointLocator->SetDataSet(centerline);
		InvokeEvent(vtkCommand::UpdateDataEvent);
	}
}

vtkPolyData * CenterlineSurfaceViewer::GetCenterline()
{
	return vtkPolyData::SafeDownCast(this->CleanPolyData->GetInput());
}

CenterlineSurfaceViewer::CenterlineSurfaceViewer()
{
	this->CleanPolyData = vtkCleanPolyData::New();
	this->CleanPolyData->PointMergingOn();
	this->SplineFilter = vtkSplineFilter::New();
	this->SplineFilter->SetInputConnection(this->CleanPolyData->GetOutputPort());
	this->SplineFilter->SetSubdivideToLength();
	this->TubeFilter = vtkTubeFilter::New();
	this->TubeFilter->SetInputConnection(this->SplineFilter->GetOutputPort());
	this->TubeFilter->CappingOn();
	this->TubeFilter->SetRadius(0.05);
	this->TubeFilter->SetRadiusFactor(0.05);
	this->TubeFilter->SetNumberOfSides(10);
	//this->KdTreePointLocator = vtkKdTreePointLocator::New();
	this->CenterlineActor = vtkActor::New();
	this->CenterlineActor->VisibilityOff();
	this->CenterlineMapper = vtkPolyDataMapper::New();

	if (this->CenterlineActor && this->CenterlineMapper) {
		// for make sure the Actor is not opaque
		this->CenterlineActor->SetMapper(this->CenterlineMapper);
	}
	this->InstallPipeline();
}

CenterlineSurfaceViewer::~CenterlineSurfaceViewer()
{
	if (this->CleanPolyData) {
		this->CleanPolyData->Delete();
		this->CleanPolyData = nullptr;
	}
	if (this->SplineFilter) {
		this->SplineFilter->Delete();
		this->SplineFilter = nullptr;
	}
	if (this->TubeFilter) {
		this->TubeFilter->Delete();
		this->TubeFilter = nullptr;
	}
	//if (this->KdTreePointLocator) {
	//	this->KdTreePointLocator->Delete();
	//	this->KdTreePointLocator = nullptr;
	//}
	if (this->CenterlineMapper)
	{
		this->CenterlineMapper->Delete();
		this->CenterlineMapper = nullptr;
	}
	if (this->CenterlineActor)
	{
		this->CenterlineActor->Delete();
		this->CenterlineActor = nullptr;
	}
}

void CenterlineSurfaceViewer::InstallPipeline()
{
	SurfaceViewer::InstallPipeline();
	if (this->CenterlineActor && this->CleanPolyData)
	{
		// for now not use tube filter
		this->CenterlineActor->GetMapper()->SetInputConnection(this->SplineFilter->GetOutputPort());
	}
	if (this->Renderer && this->CenterlineActor) {
		this->Renderer->AddActor(this->CenterlineActor);
	}
}

void CenterlineSurfaceViewer::UnInstallPipeline()
{
	SurfaceViewer::UnInstallPipeline();
	if (this->CenterlineActor)
	{
		this->CenterlineActor->GetMapper()->SetInputConnection(nullptr);
	}

	if (this->Renderer && this->CenterlineActor) {
		this->Renderer->RemoveActor(this->CenterlineActor);
	}
}

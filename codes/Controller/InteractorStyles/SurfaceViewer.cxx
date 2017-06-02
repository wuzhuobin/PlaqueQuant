#include "SurfaceViewer.h"
#include <vtkRenderWindow.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkMapper.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkCamera.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformation.h>
#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkImageResample.h>
#include <vtkDepthSortPolyData.h>
#include <vtkAxesActor.h>
#include <vtkCommand.h>
#include <vtkOrientationMarkerWidget.h>

#include <algorithm>

vtkStandardNewMacro(SurfaceViewer);

void SurfaceViewer::PrintSelf(ostream & os, vtkIndent indent)
{
}

const char * SurfaceViewer::GetWindowName()
{
	return this->RenderWindow->GetWindowName();
}

void SurfaceViewer::Render(void)
{
	if (this->FirstRender)
	{
		// Initialize the size if not set yet

		vtkAlgorithm *input = this->GetInputAlgorithm();
		if (input)
		{
			input->UpdateInformation();
			int *w_ext = this->GetInputInformation()->Get(
				vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
			int xs = 0, ys = 0;
			xs = std::max(w_ext[1] - w_ext[0] + 1, w_ext[3] - w_ext[2] + 1);
			xs = std::max(xs, w_ext[5] - w_ext[4] + 1);
			ys = xs;

			// if it would be smaller than 150 by 100 then limit to 150 by 100
			if (this->RenderWindow->GetSize()[0] == 0)
			{
				this->RenderWindow->SetSize(
					xs < 150 ? 150 : xs, ys < 100 ? 100 : ys);
			}

			if (this->Renderer)
			{
				this->Renderer->ResetCamera();
				this->Renderer->GetActiveCamera()->SetParallelScale(
					xs < 150 ? 75 : (xs - 1) / 2.0);
			}
			OrientationMarkerWidget->EnabledOn();
			OrientationMarkerWidget->InteractiveOff();
			//this->AxesActor->SetOrigin(GetInput()->GetOrigin());
			//this->AxesActor->VisibilityOn();
			//this->AxesActor->VisibilityOff();
			this->FirstRender = 0;
		}
	}
	if (this->GetInput())
	{
		this->RenderWindow->Render();
	}
}

void SurfaceViewer::SetOrientation(int orientation)
{
	if (orientation < ORIENTATION_YZ ||
		orientation > ORIENTATION_XY)
	{
		vtkErrorMacro("Error - invalid slice orientation " << orientation);
		return;
	}

	if (this->Orientation == orientation)
	{
		return;
	}

	this->Orientation = orientation;


	this->UpdateOrientation();
	this->UpdateDisplayExtent();

	this->Render();
}

void SurfaceViewer::SetInputData(vtkImageData * in)
{
	if (GetInput() != in) {
		// when input is a different extent, First Render
		if (GetInput() == nullptr ||
			!std::equal(in->GetExtent(), in->GetExtent() + 6, GetInput()->GetExtent())) {
			this->FirstRender = true;
		}
		this->SurfaceActor->VisibilityOn();
		this->ImageResample->SetInputData(in);
		double spacing = std::fmin(std::fmin(in->GetSpacing()[0], in->GetSpacing()[1]), in->GetSpacing()[2]);
		this->ImageResample->SetOutputSpacing(spacing, spacing, spacing);
		UpdateDisplayExtent();
		InvokeEvent(vtkCommand::UpdateDataEvent);
	}
}

vtkImageData * SurfaceViewer::GetInput()
{
	return vtkImageData::SafeDownCast(this->ImageResample->GetInput());
}

void SurfaceViewer::UpdateDisplayExtent()
{
	vtkAlgorithm *input = this->GetInputAlgorithm();
	if (!input || !this->SurfaceActor)
	{
		return;
	}

	input->UpdateInformation();
	vtkInformation* outInfo = input->GetOutputInformation(0);
	double* w_bounds = outInfo->Get(
		vtkStreamingDemandDrivenPipeline::BOUNDS());

	if (this->Renderer)
	{
		if (this->InteractorStyle &&
			this->InteractorStyle->GetAutoAdjustCameraClippingRange())
		{
			this->Renderer->ResetCameraClippingRange();
			this->Renderer->ResetCamera();
		}
		else
		{
			this->Renderer->ResetCameraClippingRange(w_bounds);
			this->Renderer->ResetCamera();
		}
	}

}

//----------------------------------------------------------------------------
void SurfaceViewer::SetPosition(int a, int b)
{
	this->RenderWindow->SetPosition(a, b);
}

//----------------------------------------------------------------------------
int* SurfaceViewer::GetPosition()
{
	return this->RenderWindow->GetPosition();
}

//----------------------------------------------------------------------------
void SurfaceViewer::SetDisplayId(void *a)
{
	this->RenderWindow->SetDisplayId(a);
}

//----------------------------------------------------------------------------
void SurfaceViewer::SetWindowId(void *a)
{
	this->RenderWindow->SetWindowId(a);
}

//----------------------------------------------------------------------------
void SurfaceViewer::SetParentId(void *a)
{
	this->RenderWindow->SetParentId(a);
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void SurfaceViewer::SetSize(int a, int b)
{
	this->RenderWindow->SetSize(a, b);
}

//----------------------------------------------------------------------------
int* SurfaceViewer::GetSize()
{
	return this->RenderWindow->GetSize();
}

//----------------------------------------------------------------------------

void SurfaceViewer::SetLookupTable(vtkLookupTable* lookupTable)
{
	this->LookupTable = lookupTable;
	MarchingCubes->GenerateValues(this->LookupTable->GetNumberOfTableValues(),
		0, this->LookupTable->GetNumberOfTableValues() - 1);
	SurfaceMapper->SetScalarRange(0, this->LookupTable->GetNumberOfTableValues());
	SurfaceMapper->SetLookupTable(lookupTable);
}

void SurfaceViewer::SetRenderWindow(vtkRenderWindow * arg)
{
	if (this->RenderWindow == arg)
	{
		return;
	}

	this->UnInstallPipeline();

	if (this->RenderWindow)
	{
		this->RenderWindow->UnRegister(this);
	}

	this->RenderWindow = arg;

	if (this->RenderWindow)
	{
		this->RenderWindow->Register(this);
	}

	this->InstallPipeline();
}

void SurfaceViewer::SetRenderer(vtkRenderer * arg)
{
	if (this->Renderer == arg)
	{
		return;
	}

	this->UnInstallPipeline();

	if (this->Renderer)
	{
		this->Renderer->UnRegister(this);
	}

	this->Renderer = arg;

	if (this->Renderer)
	{
		this->Renderer->Register(this);
	}

	this->InstallPipeline();
	this->UpdateOrientation();
}

void SurfaceViewer::SetupInteractor(vtkRenderWindowInteractor * arg)
{
	if (this->Interactor == arg)
	{
		return;
	}

	this->UnInstallPipeline();

	if (this->Interactor)
	{
		this->Interactor->UnRegister(this);
	}

	this->Interactor = arg;

	if (this->Interactor)
	{
		this->Interactor->Register(this);
	}

	this->InstallPipeline();

	if (this->Renderer)
	{
		this->Renderer->GetActiveCamera()->ParallelProjectionOff();
	}
}

void SurfaceViewer::SetEnableDepthSorting(bool flag)
{
	if (flag && this->WindowedSincPolyDataFilter && this->DepthSortPolyData) {
		this->DepthSortPolyData->SetInputConnection(
			this->WindowedSincPolyDataFilter->GetOutputPort());
		this->SurfaceActor->GetMapper()->SetInputConnection(
			this->DepthSortPolyData->GetOutputPort());

	}
	else if(this->WindowedSincPolyDataFilter){
		this->SurfaceActor->GetMapper()->SetInputConnection(
			this->WindowedSincPolyDataFilter->GetOutputPort());
	}
}

void SurfaceViewer::SetEnableDepthPeeling(bool flag)
{
	if (this->RenderWindow || !this->Renderer) {
		// 1. Use a render window with alpha bits (as initial value is 0 (false)):
		this->RenderWindow->SetAlphaBitPlanes(flag);

		if (flag) {
			// 2. Force to not pick a framebuffer with a multisample buffer
			// (as initial value is 8):
			this->RenderWindow->SetMultiSamples(0);
		}
		else {
			this->RenderWindow->SetMultiSamples(8);
		}

		// 3. Choose to use depth peeling (if supported) (initial value is 0 (false)):
		this->Renderer->SetUseDepthPeeling(flag);
		// 4. Set depth peeling parameters
		// - Set the maximum number of this->Rendering passes (initial value is 4):
		this->Renderer->SetMaximumNumberOfPeels(this->MaxNoOfPeels);
		// - Set the occlusion ratio (initial value is 0.0, exact image):
		this->Renderer->SetOcclusionRatio(this->OcclusionRatio);
	}
}

void SurfaceViewer::SetOffScreenRendering(int flag)
{
	this->RenderWindow->SetOffScreenRendering(flag);
}

int SurfaceViewer::GetOffScreenRendering()
{
	return this->RenderWindow->GetOffScreenRendering();
}

SurfaceViewer::SurfaceViewer()
{
	this->RenderWindow = nullptr;
	this->Renderer = nullptr;
	this->SurfaceActor = vtkActor::New();
	this->SurfaceActor->VisibilityOff();
	this->SurfaceMapper = vtkPolyDataMapper::New();
	this->ImageResample = vtkImageResample::New();
	this->ImageResample->SetNumberOfThreads(16);
	this->ImageResample->SetInterpolationModeToLinear();
	this->ImageResample->SetDimensionality(3);
	this->MarchingCubes = vtkDiscreteMarchingCubes::New();
	this->MarchingCubes->SetInputConnection(ImageResample->GetOutputPort());
	this->MarchingCubes->ComputeGradientsOff();
	this->MarchingCubes->ComputeNormalsOff();
	this->MarchingCubes->ComputeScalarsOn();
	this->WindowedSincPolyDataFilter = vtkWindowedSincPolyDataFilter::New();
	this->WindowedSincPolyDataFilter->SetInputConnection(MarchingCubes->GetOutputPort());
	this->WindowedSincPolyDataFilter->BoundarySmoothingOff();
	this->WindowedSincPolyDataFilter->FeatureEdgeSmoothingOff();
	this->WindowedSincPolyDataFilter->NonManifoldSmoothingOn();
	this->WindowedSincPolyDataFilter->NormalizeCoordinatesOn();
	this->WindowedSincPolyDataFilter->SetNumberOfIterations(15);
	this->WindowedSincPolyDataFilter->SetFeatureAngle(120.0);
	this->WindowedSincPolyDataFilter->SetPassBand(0.001);

	this->DepthSortPolyData = vtkDepthSortPolyData::New();
	this->DepthSortPolyData->SetDirectionToBackToFront();
	//this->DepthSortPolyData->SetVector(1, 1, 1);
	this->DepthSortPolyData->SortScalarsOff();

	this->Interactor = nullptr;
	this->InteractorStyle = nullptr;
	this->LookupTable = nullptr;

	if (this->SurfaceActor && this->SurfaceMapper) {
		// for make sure the Actor is not opaque
		this->SurfaceActor->SetMapper(this->SurfaceMapper);
	}

	this->FirstRender = true;

	// Setup the pipeline

	vtkRenderWindow *renwin = vtkRenderWindow::New();
	this->SetRenderWindow(renwin);
	renwin->Delete();

	vtkRenderer *ren = vtkRenderer::New();
	this->SetRenderer(ren);
	ren->Delete();

	this->AxesActor = vtkAxesActor::New();
	this->AxesActor->SetXAxisLabelText("L");
	this->AxesActor->SetYAxisLabelText("P");
	this->AxesActor->SetZAxisLabelText("S");
	//this->AxesActor->DragableOff();
	//this->AxesActor->VisibilityOff();
	this->OrientationMarkerWidget = vtkOrientationMarkerWidget::New();
	OrientationMarkerWidget->SetOrientationMarker(this->AxesActor);
	OrientationMarkerWidget->SetViewport(0, 0.8, 0.2, 1);

	this->InstallPipeline();
}

SurfaceViewer::~SurfaceViewer()
{
	if (this->ImageResample) {
		this->ImageResample->Delete();
		this->ImageResample = nullptr;
	}
	if (this->MarchingCubes)
	{
		this->MarchingCubes->Delete();
		this->MarchingCubes = nullptr;
	}
	if (this->WindowedSincPolyDataFilter) {
		this->WindowedSincPolyDataFilter->Delete();
		this->WindowedSincPolyDataFilter = nullptr;
	}
	if(this->DepthSortPolyData) {
		this->DepthSortPolyData->Delete();
		this->DepthSortPolyData = nullptr;
	}
	if (this->SurfaceMapper) 
	{
		this->SurfaceMapper->Delete();
		this->SurfaceMapper = nullptr;
	}
	if (this->AxesActor) {
		this->AxesActor->Delete();
		this->AxesActor = nullptr;
	}
	if (this->SurfaceActor)
	{
		this->SurfaceActor->Delete();
		this->SurfaceActor = nullptr;
	}

	if (this->Renderer)
	{
		this->Renderer->Delete();
		this->Renderer = nullptr;
	}

	if (this->RenderWindow)
	{
		this->RenderWindow->Delete();
		this->RenderWindow = nullptr;
	}

	if (this->Interactor)
	{
		this->Interactor->Delete();
		this->Interactor = nullptr;
	}

	if (this->InteractorStyle)
	{
		this->InteractorStyle->Delete();
		this->InteractorStyle = nullptr;
	}
}

void SurfaceViewer::InstallPipeline()
{
	if (this->RenderWindow && this->Renderer)
	{
		this->RenderWindow->AddRenderer(this->Renderer);
	}

	if (this->Interactor)
	{
		if (!this->InteractorStyle)
		{
			this->InteractorStyle = vtkInteractorStyle::New();
		}

		this->Interactor->SetInteractorStyle(this->InteractorStyle);
		this->Interactor->SetRenderWindow(this->RenderWindow);
	}
	if (this->Interactor && this->OrientationMarkerWidget) {
		this->OrientationMarkerWidget->SetInteractor(this->Interactor);
	}
	//if (this->Renderer && this->AxesActor)
	//{
	//	this->Renderer->AddViewProp(this->AxesActor);
	//}
	if (this->Renderer && this->SurfaceActor)
	{
		this->Renderer->AddViewProp(this->SurfaceActor);
		this->DepthSortPolyData->SetCamera(this->Renderer->GetActiveCamera());
	}

	if (this->SurfaceActor && this->WindowedSincPolyDataFilter)
	{
		this->SurfaceActor->GetMapper()->SetInputConnection(
			this->WindowedSincPolyDataFilter->GetOutputPort());
	}

}

void SurfaceViewer::UnInstallPipeline()
{
	if (this->SurfaceActor)
	{
		this->SurfaceActor->GetMapper()->SetInputConnection(nullptr);
	}
	//if (this->Renderer && this->AxesActor)
	//{
	//	this->Renderer->RemoveActor(this->AxesActor);
	//}
	if (this->Renderer && this->SurfaceActor)
	{
		this->Renderer->RemoveViewProp(this->SurfaceActor);
	}

	if (this->RenderWindow && this->Renderer)
	{
		this->RenderWindow->RemoveRenderer(this->Renderer);
	}

	if (this->Interactor)
	{
		this->Interactor->SetInteractorStyle(nullptr);
		this->Interactor->SetRenderWindow(nullptr);
	}
}

void SurfaceViewer::UpdateOrientation()
{
	// Set the camera position

	vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;
	if (cam)
	{
		switch (this->Orientation)
		{
		case ORIENTATION_XY:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(0, 0, -1); // -1 if medical ?
			cam->SetViewUp(0, -1, 0);
			break;

		case ORIENTATION_XZ:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(0, -1, 0); // 1 if medical ?
			cam->SetViewUp(0, 0, 1);
			break;

		case ORIENTATION_YZ:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(1, 0, 0); // -1 if medical ?
			cam->SetViewUp(0, 0, 1);
			break;
		}
	}
}

vtkAlgorithm * SurfaceViewer::GetInputAlgorithm()
{
	return this->ImageResample->GetInputAlgorithm();
}

vtkInformation * SurfaceViewer::GetInputInformation()
{
	return this->ImageResample->GetInputInformation();
}

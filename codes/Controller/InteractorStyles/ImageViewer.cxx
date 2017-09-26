/*=========================================================================

Program:   Visualization Toolkit
Module:    ImageViewer.cxx

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "ImageViewer.h"

#include <vtkInformation.h>
#include <vtkCamera.h>
//#include <vtkImageProperty.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkCachedStreamingDemandDrivenPipeline.h>
#include <vtkImageMapper3D.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCommand.h>

#include <vtkTextActor.h>
#include <vtkImageMapToColors.h>
#include <vtkLogLookupTable.h>
#include <vtkImageActor.h>
#include <vtkCursor3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkExtractVOI.h>
#include <vtkObjectFactory.h>

#include <algorithm>


using std::string;
using std::min;
using std::max;
vtkStandardNewMacro(ImageViewer);

// update orientation and header text
class vtkResizeHeaderAndOrientationTextCallback : public vtkCommand
{
public:
	static vtkResizeHeaderAndOrientationTextCallback* New() { return new vtkResizeHeaderAndOrientationTextCallback; }
	ImageViewer* IV;
	void Execute(vtkObject *caller,
		unsigned long event,
		void *vtkNotUsed(callData)) override
	{
		if (this->IV->GetInput() == NULL)
		{
			return;
		}
		if (event == RenderEvent) {
			IV->ResizeHeaderAndOrientationText();
		}
	}
};

//----------------------------------------------------------------------------
ImageViewer::ImageViewer()
{
	this->ImageActor->VisibilityOff();
	this->OverlayActor = vtkImageActor::New();
	this->OverlayActor->InterpolateOff();
	this->OverlayActor->VisibilityOff();
	this->OverlayImageMapToColors = vtkImageMapToColors::New();

	// OrientationTextActor 
	this->IntTextActor = vtkTextActor::New();
	this->IntTextActor->GetTextProperty()->SetFontSize(15);
	this->IntTextActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
	this->HeaderActor = vtkTextActor::New();
	this->HeaderActor->GetTextProperty()->SetFontSize(15);
	this->HeaderActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
	for (int i = 0; i < 4; i++)
	{
		this->OrientationTextActor[i] = vtkTextActor::New();
		this->OrientationTextActor[i]->GetTextProperty()->SetFontSize(15);
		this->OrientationTextActor[i]->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
	}

	//Cursor
	Cursor3D = vtkCursor3D::New();
	Cursor3D->SetTranslationMode(false);
	Cursor3D->AllOff();
	Cursor3D->AxesOn();
	Cursor3D->SetModelBounds(0, 0, 0, 0, 0, 0);
	Cursor3D->Update();

	CursorMapper = vtkPolyDataMapper::New();

	CursorActor = vtkActor::New();
	CursorActor->GetProperty()->SetColor(0, 0, 1);
	CursorActor->GetProperty()->SetLineStipplePattern(0xf0f0);
	if (this->CursorActor && this->CursorMapper) {
		this->CursorActor->SetMapper(this->CursorMapper);
	}

	this->InstallPipeline();
	UpdateOrientation();
}

//----------------------------------------------------------------------------
ImageViewer::~ImageViewer()
{
	if (this->OverlayImageMapToColors) {
		this->OverlayImageMapToColors->Delete();
		this->OverlayImageMapToColors = NULL;
	}
	if (this->OverlayActor) {
		this->OverlayActor->Delete();
		this->OverlayActor = NULL;
	}
	// OrientationTextActor
	if (IntTextActor != NULL) {
		this->IntTextActor->Delete();
		this->IntTextActor = NULL;
	}
	if (HeaderActor != NULL) {
		this->HeaderActor->Delete();
		this->HeaderActor = NULL;
	}
	for (int i = 0; i < 4; i++)
	{
		if (OrientationTextActor[i] != NULL) {
			OrientationTextActor[i]->Delete();
			this->OrientationTextActor[i] = NULL;
		}
	}
	// Cursor
	if (this->Cursor3D != NULL) {
		this->Cursor3D->Delete();
		this->Cursor3D = NULL;
	}
	if (this->CursorMapper != NULL) {
		this->CursorMapper->Delete();
		this->CursorMapper = NULL;
	}
	if (this->CursorActor != NULL) {
		this->CursorActor->Delete();
		this->CursorActor = NULL;
	}

}

void ImageViewer::SetDisplayExtent(int displayExtent1, int displayExtent2, int displayExtent3,
	int displayExtent4, int displayExtent5, int displayExtent6)
{
	this->DisplayExtent[0] = max(displayExtent1, GetInput()->GetExtent()[0]);
	this->DisplayExtent[1] = min(displayExtent2, GetInput()->GetExtent()[1]);
	this->DisplayExtent[2] = max(displayExtent3, GetInput()->GetExtent()[2]);
	this->DisplayExtent[3] = min(displayExtent4, GetInput()->GetExtent()[3]);
	this->DisplayExtent[4] = max(displayExtent5, GetInput()->GetExtent()[4]);
	this->DisplayExtent[5] = min(displayExtent6, GetInput()->GetExtent()[5]);
	UpdateDisplayExtent();
}

void ImageViewer::SetDisplayExtent(int * displayExtent)
{
	//memcpy(DisplayExtent, displayExtent, sizeof(DisplayExtent));
	SetDisplayExtent(
		displayExtent[0],
		displayExtent[1],
		displayExtent[2],
		displayExtent[3],
		displayExtent[4],
		displayExtent[5] );
}

//----------------------------------------------------------------------------
void ImageViewer::UpdateDisplayExtent()
{
	vtkAlgorithm *input = this->GetInputAlgorithm();
	if (!input || !this->ImageActor || !this->OverlayActor)
	{
		return;
	}

	input->UpdateInformation();
	vtkInformation* outInfo = input->GetOutputInformation(0);

	// Is the slice in range ? If not, fix it

	int slice_min = DisplayExtent[this->SliceOrientation * 2];
	int slice_max = DisplayExtent[this->SliceOrientation * 2 + 1];
	if (this->Slice < slice_min || this->Slice > slice_max)
	{
		this->Slice = static_cast<int>((slice_min + slice_max) * 0.5);
	}

	// Set the image actor

	switch (this->SliceOrientation)
	{
	case vtkImageViewer2::SLICE_ORIENTATION_XY:
		this->ImageActor->SetDisplayExtent(
			DisplayExtent[0], DisplayExtent[1], DisplayExtent[2], DisplayExtent[3], this->Slice, this->Slice);
		this->OverlayActor->SetDisplayExtent(
			DisplayExtent[0], DisplayExtent[1], DisplayExtent[2], DisplayExtent[3], this->Slice, this->Slice);
		break;

	case vtkImageViewer2::SLICE_ORIENTATION_XZ:
		this->ImageActor->SetDisplayExtent(
			DisplayExtent[0], DisplayExtent[1], this->Slice, this->Slice, DisplayExtent[4], DisplayExtent[5]);
		this->OverlayActor->SetDisplayExtent(
			DisplayExtent[0], DisplayExtent[1], this->Slice, this->Slice, DisplayExtent[4], DisplayExtent[5]);
		break;

	case vtkImageViewer2::SLICE_ORIENTATION_YZ:
		this->ImageActor->SetDisplayExtent(
			this->Slice, this->Slice, DisplayExtent[2], DisplayExtent[3], DisplayExtent[4], DisplayExtent[5]);
		this->OverlayActor->SetDisplayExtent(
			this->Slice, this->Slice, DisplayExtent[2], DisplayExtent[3], DisplayExtent[4], DisplayExtent[5]);
		break;
	}
	InitializeCursorBoundary();
	// Figure out the correct clipping range

	if (this->Renderer)
	{
		if (this->InteractorStyle &&
			this->InteractorStyle->GetAutoAdjustCameraClippingRange())
		{
			this->Renderer->ResetCameraClippingRange();
		}
		else
		{
			vtkCamera *cam = this->Renderer->GetActiveCamera();
			if (cam)
			{
				double bounds[6];
				this->ImageActor->GetBounds(bounds);
				double spos = bounds[this->SliceOrientation * 2];
				double cpos = cam->GetPosition()[this->SliceOrientation];
				double range = fabs(spos - cpos);
				double *spacing = outInfo->Get(vtkDataObject::SPACING());
				double avg_spacing =
					(spacing[0] + spacing[1] + spacing[2]) / 3.0;
				cam->SetClippingRange(
					range - avg_spacing * 3.0, range + avg_spacing * 3.0);
			}
		}
	}
}
void ImageViewer::ResetDisplayExtent()
{
	vtkAlgorithm *input = this->GetInputAlgorithm();
	if (!input || !this->ImageActor || !this->OverlayActor)
	{
		return;
	}

	input->UpdateInformation();
	vtkInformation* outInfo = input->GetOutputInformation(0);
	int *ResetExtent = outInfo->Get(
		vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
	SetDisplayExtent(ResetExtent);
}

void ImageViewer::GetSliceRange(int & min, int & max)
{
	min = this->DisplayExtent[this->SliceOrientation * 2];
	max = this->DisplayExtent[this->SliceOrientation * 2 + 1];
}

int * ImageViewer::GetSliceRange()
{
	return this->DisplayExtent + this->SliceOrientation*2;
}

void ImageViewer::SetColorLevel(double level)
{
	double* range = GetInput()->GetScalarRange();
	level = fmax(level, range[0]);
	level = fmin(level, range[1]);
	vtkImageViewer2::SetColorLevel(level);
}

void ImageViewer::SetColorWindow(double window)
{
	double* range = GetInput()->GetScalarRange();
	window = fmax(window, range[0]);
	window = fmin(window, range[1]);
	vtkImageViewer2::SetColorWindow(window);
}

//----------------------------------------------------------------------------
void ImageViewer::InstallPipeline()
{
	vtkImageViewer2::InstallPipeline();
	if (this->RenderWindow) {
		vtkResizeHeaderAndOrientationTextCallback* cbk =
			vtkResizeHeaderAndOrientationTextCallback::New();
		cbk->IV = this;
		this->RenderWindow->AddObserver(vtkCommand::RenderEvent, cbk);
		cbk->Delete();
	}
	// add label view prop to renderer
	if (this->Renderer && this->OverlayActor) 
	{
		this->Renderer->AddViewProp(this->OverlayActor);
	}
	// Setup connection with window level mapper
	if (this->OverlayActor && this->OverlayImageMapToColors)
	{
		this->OverlayActor->GetMapper()->SetInputConnection(
			this->OverlayImageMapToColors->GetOutputPort());
	}
	// add cursor 
	if (this->CursorMapper && this->Cursor3D) {
		this->CursorMapper->SetInputConnection(this->Cursor3D->GetOutputPort());
	}

	if (this->Renderer && this->CursorActor)
	{
		this->Renderer->AddActor(this->CursorActor);
	}
	// Text 
	if (this->HeaderActor && this->Renderer) {
		this->Renderer->AddActor(this->HeaderActor);
	}
	if (this->IntTextActor && this->Renderer) {
		this->Renderer->AddActor(this->IntTextActor);
	}
	for (int i = 0; i < 4; ++i) {
		if (this->OrientationTextActor[i] && this->Renderer) {
			this->Renderer->AddActor(this->OrientationTextActor[i]);
		}
	}
}

//----------------------------------------------------------------------------
void ImageViewer::UnInstallPipeline()
{
	if (this->OverlayActor)
	{
		this->OverlayActor->GetMapper()->SetInputConnection(NULL);
	}
	if (this->Renderer && this->OverlayActor) {
		this->Renderer->RemoveActor(OverlayActor);
	}
	if (this->CursorMapper) {
		this->CursorMapper->SetInputConnection(nullptr);
	}
	if (this->Renderer && this->CursorActor)
	{
		this->Renderer->RemoveActor(this->CursorActor);
	}

	// Text 
	if (this->HeaderActor && this->Renderer) {
		this->Renderer->RemoveActor(this->HeaderActor);
	}
	if (this->IntTextActor && this->Renderer) {
		this->Renderer->RemoveActor(this->IntTextActor);
	}
	for (int i = 0; i < 4; ++i) {
		if (this->OrientationTextActor[i] && this->Renderer) {
			this->Renderer->RemoveActor(this->OrientationTextActor[i]);
		}
	}
	vtkImageViewer2::UnInstallPipeline();
}

//----------------------------------------------------------------------------
void ImageViewer::UpdateOrientation()
{
	// Set the camera position

	vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;
	if (cam)
	{
		switch (this->SliceOrientation)
		{
		case vtkImageViewer2::SLICE_ORIENTATION_XY:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(0, 0, -1); // -1 if medical ?
			cam->SetViewUp(0, -1, 0);
			break;

		case vtkImageViewer2::SLICE_ORIENTATION_XZ:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(0, -1, 0); // 1 if medical ?
			cam->SetViewUp(0, 0, 1);
			break;

		case vtkImageViewer2::SLICE_ORIENTATION_YZ:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(1, 0, 0); // -1 if medical ?
			cam->SetViewUp(0, 0, 1);
			break;
		}
	}
}
//----------------------------------------------------------------------------
void ImageViewer::Render()
{
	//if (this->FirstRender)
	//{
	//	this->ResetDisplayExtent();
	//}

	vtkImageViewer2::Render();

}
//----------------------------------------------------------------------------
void ImageViewer::SetInputData(vtkImageData *in)
{
	// when there is a new input, Update the member DisplayExtent 
	if (in != GetInput()) {
		// when input is a different extent, First Render
		if (GetInput() == nullptr ||
			!std::equal(in->GetExtent(), in->GetExtent() + 6, GetInput()->GetExtent())) {
			this->FirstRender = true;
		}
		this->ImageActor->VisibilityOn();
		vtkImageViewer2::SetInputData(in);
		//Color Map
		double* range = in->GetScalarRange();
		this->SetColorWindow(range[1] - range[0]);
		this->SetColorLevel((range[1] + range[0])*0.5);
		// when input is a different extent, call#ResetDisplayExtent()
		if (FirstRender){
			ResetDisplayExtent();
		}
		this->InvokeEvent(vtkCommand::UpdateDataEvent);
	}
}

void ImageViewer::SetOverlay(vtkImageData *in)
{
	if (in != GetOverlay()) {
		this->OverlayActor->VisibilityOn();
		OverlayImageMapToColors->SetInputData(in);
		// in case when LookupTable has not been set
		//this->UpdateDisplayExtent();
		this->InvokeEvent(vtkCommand::UpdateDataEvent);
	}
}
//----------------------------------------------------------------------------
vtkImageData* ImageViewer::GetOverlay()
{
	return vtkImageData::SafeDownCast(this->OverlayImageMapToColors->GetInput());
}

void ImageViewer::SetSliceOrientation(int orientation)
{
	if (GetSliceOrientation() != orientation) {
		vtkImageViewer2::SetSliceOrientation(orientation);
		this->InvokeEvent(vtkCommand::UpdateDataEvent);
	}

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ImageViewer::PrintSelf(ostream& os, vtkIndent indent)
{
	this->vtkImageViewer2::PrintSelf(os, indent);

	os << indent << "RenderWindow:\n";
	this->RenderWindow->PrintSelf(os, indent.GetNextIndent());
	os << indent << "Renderer:\n";
	this->Renderer->PrintSelf(os, indent.GetNextIndent());
	os << indent << "ImageActor:\n";
	this->ImageActor->PrintSelf(os, indent.GetNextIndent());
	os << indent << "ImageActorContour:\n";
	this->OverlayActor->PrintSelf(os, indent.GetNextIndent());
	os << indent << "WindowLevel:\n" << endl;
	this->OverlayImageMapToColors->PrintSelf(os, indent.GetNextIndent());
	os << indent << "Slice: " << this->Slice << endl;
	os << indent << "SliceOrientation: " << this->SliceOrientation << endl;
	os << indent << "InteractorStyle: " << endl;
	if (this->InteractorStyle)
	{
		os << "\n";
		this->InteractorStyle->PrintSelf(os, indent.GetNextIndent());
	}
	else
	{
		os << "None";
	}
}

void ImageViewer::InitializeCursorBoundary()
{
	// uncomment the following code will lead the cursor boundary updated normally
	// but it will lead to the canvas source behaviour very strange.

	//this->GetInputAlgorithm()->Update();
	//const double* spacing = GetInput()->GetSpacing();
	//const double* origin = GetInput()->GetOrigin();
	//const int* extent = GetInput()->GetExtent();
	//double bound[6];
	//bound[0] = origin[0] + extent[0] * spacing[0];
	//bound[1] = origin[0] + extent[1] * spacing[0];
	//bound[2] = origin[1] + extent[2] * spacing[1];
	//bound[3] = origin[1] + extent[3] * spacing[1];
	//bound[4] = origin[2] + extent[4] * spacing[2];
	//bound[5] = origin[2] + extent[5] * spacing[2];

	//Cursor3D->SetModelBounds(bound);
	Cursor3D->SetModelBounds(GetInput()->GetBounds());
	Cursor3D->Update();
}

void ImageViewer::SetLookupTable(vtkLookupTable * LookupTable)
{
	this->LookupTable = LookupTable;
	//this->OverlayActor->GetProperty()->SetInterpolationTypeToNearest();
	//this->OverlayActor->GetProperty()->SetLookupTable(LookupTable);

	int num = LookupTable->GetNumberOfTableValues();
	OverlayImageMapToColors->SetLookupTable(this->LookupTable);
	//OverlayImageMapToColors->SetWindow(num - 1);
	//OverlayImageMapToColors->SetLevel((num - 1) * 0.5);
}

void ImageViewer::SetFocalPointWithWorldCoordinate(double x, double y, double z)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	int i = static_cast<int>((x - origin[0])/spacing[0] + 0.5);
	int j = static_cast<int>((y - origin[1])/spacing[1] + 0.5);
	int k = static_cast<int>((z - origin[2])/spacing[2] + 0.5);
	SetFocalPointWithImageCoordinate(i, j, k);

}

void ImageViewer::SetFocalPointWithImageCoordinate(int i, int j, int k)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	const int* extent = GetInput()->GetExtent();
	if (i < extent[0] || i > extent[1] || 
		j < extent[2] || j > extent[3] || 
		k < extent[4] || k > extent[5]
		) {
		return;
	}
	double point[3] = { 
		i*spacing[0] + origin[0],
		j*spacing[1] + origin[1], 
		k*spacing[2] + origin[2] };

	Cursor3D->SetFocalPoint(point);
	Cursor3D->Update();
	int ijk[3] = { i, j, k };
	SetSlice(ijk[this->SliceOrientation]);
	IntTextActor->SetInput(std::to_string(
		(GetInput()->GetScalarComponentAsFloat(i, j, k, 0))).c_str());
	this->Render();

}

void ImageViewer::GetFocalPointWithImageCoordinate(int * coordinate)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	const double* point = Cursor3D->GetFocalPoint();
	for (int i = 0; i < 3; ++i) {
		coordinate[i] = static_cast<int>((point[i] - origin[i]) / spacing[i] + 0.5);
	}
}

void ImageViewer::GetFocalPointWithWorldCoordinate(double * coordinate)
{
	memcpy(coordinate, Cursor3D->GetFocalPoint(), sizeof(double) * 3);
}

double * ImageViewer::GetFocalPointWithWorldCoordinate()
{
	return Cursor3D->GetFocalPoint();
}

double * ImageViewer::GetCursorBoundWithWorldCoordinate()
{
	return this->Cursor3D->GetModelBounds();
}

void ImageViewer::SetAllBlack(bool flag)
{
	if (this->AllBlackFlag == flag) {
		return;
	}
	this->AllBlackFlag = flag;
	//QList<vtkProp*>  props;
	//props += this->ImageActor;
	//props += this->OrientationTextActor[0];
	//props += this->OrientationTextActor[1];
	//props += this->OrientationTextActor[2];
	//props += this->OrientationTextActor[3];
	//props += this->HeaderActor;
	//props += this->IntTextActor;
	//props += this->CursorActor;
	//props += this->OverlayActor;


	//for (QList<vtkProp*>::const_iterator cit = props.cbegin(); cit != props.cend(); ++cit) {
	//	if ((*cit) != NULL) {
	//		(*cit)->SetVisibility(!flag);
	//	}
	//}
	//Render();
	//emit AllBlackAlready(flag);
}

bool ImageViewer::GetAllBlack()
{
	return AllBlackFlag;
}

void ImageViewer::InitializeHeader(string file)
{
	HeaderActor->SetInput(file.c_str());
	RenderWindow->SetWindowName(file.c_str());
}

void ImageViewer::ResizeHeaderAndOrientationText()
{
	int* size = Renderer->GetSize();
	int margin = 15;
	int coord[2] = { 5,size[1] - margin };
	int down[2] = { size[0] / 2	,5 };
	int up[2] = { size[0] / 2	,size[1] - margin };
	int left[2] = { 5			,size[1] / 2 };
	int right[2] = { size[0] - margin	,size[1] / 2 };
	int* position[4] = { up, down, left, right };
	switch (SliceOrientation)
	{
	case 0:
		OrientationTextActor[0]->SetInput("S");
		OrientationTextActor[1]->SetInput("I");
		OrientationTextActor[2]->SetInput("A");
		OrientationTextActor[3]->SetInput("P");
		break;
	case 1:
		OrientationTextActor[0]->SetInput("S");
		OrientationTextActor[1]->SetInput("I");
		OrientationTextActor[2]->SetInput("R");
		OrientationTextActor[3]->SetInput("L");
		break;
	case 2:
		OrientationTextActor[0]->SetInput("A");
		OrientationTextActor[1]->SetInput("P");
		OrientationTextActor[2]->SetInput("R");
		OrientationTextActor[3]->SetInput("L");
		break;
	}
	for (int i = 0; i<4; i++)
	{
		OrientationTextActor[i]->SetDisplayPosition(position[i][0], position[i][1]);
	}
	HeaderActor->SetDisplayPosition(coord[0], coord[1]);
	IntTextActor->SetDisplayPosition(5, 5);

}

void ImageViewer::SetEnableDepthPeeling(bool flag)
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



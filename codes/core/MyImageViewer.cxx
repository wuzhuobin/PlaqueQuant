/*=========================================================================

Program:   Visualization Toolkit
Module:    MyImageViewer.cxx

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "MyImageViewer.h"

#include <vtkInformation.h>
#include <vtkCamera.h>
#include <vtkImageProperty.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkCachedStreamingDemandDrivenPipeline.h>
#include <vtkImageMapper3D.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>

#include <QList>
using namespace std;
vtkStandardNewMacro(MyImageViewer);

//----------------------------------------------------------------------------
MyImageViewer::MyImageViewer(QObject* parent)
	:vtkImageViewer2(), QObject(parent)
{
	this->CursorActor = NULL;
	this->AnnotationRenderer = NULL;
	this->OverlayActor = vtkImageActor::New();
	this->OverlayWindowLevel = vtkImageMapToWindowLevelColors::New();
	this->OverlayExtractVOI = vtkExtractVOI::New();
	this->ImageExtractVOI = vtkExtractVOI::New();


	//Setup the pipeline again because of Annotation
	this->UnInstallPipeline();

	vtkRenderer *ren = vtkRenderer::New();
	this->SetAnnotationRenderer(ren);
	ren->Delete();

	// OrientationTextActor 
	this->IntTextActor = vtkTextActor::New();
	this->HeaderActor = vtkTextActor::New();
	for (int i = 0; i < 4; i++)
	{
		this->OrientationTextActor[i] = vtkTextActor::New();
	}

	//Cursor
	Cursor3D = vtkCursor3D::New();
	Cursor3D->SetTranslationMode(false);
	Cursor3D->AllOff();
	Cursor3D->AxesOn();
	Cursor3D->SetModelBounds(0, 0, 0, 0, 0, 0);
	Cursor3D->Update();

	CursorMapper = vtkPolyDataMapper::New();
	CursorMapper->SetInputData(Cursor3D->GetOutput());

	CursorActor = vtkActor::New();
	CursorActor->SetMapper(CursorMapper);
	CursorActor->GetProperty()->SetColor(0, 0, 1);
	CursorActor->GetProperty()->SetLineStipplePattern(0xf0f0);

	//AnnotationRenderer
	AnnotationRenderer->SetGlobalWarningDisplay(false);
	AnnotationRenderer->SetLayer(1);

	RenderWindow->SetNumberOfLayers(2);

	this->InstallPipeline();
}

//----------------------------------------------------------------------------
MyImageViewer::~MyImageViewer()
{
	if (this->ImageExtractVOI) {
		this->ImageExtractVOI->Delete();
		this->ImageExtractVOI = NULL;
	}
	if (this->OverlayExtractVOI) {
		this->OverlayExtractVOI->Delete();
		this->OverlayExtractVOI = NULL;
	}
	if (this->OverlayWindowLevel) {
		this->OverlayWindowLevel->Delete();
		this->OverlayWindowLevel = NULL;
	}
	if (this->OverlayActor) {
		this->OverlayActor->Delete();
		this->OverlayActor = NULL;
	}
	if (this->AnnotationRenderer != NULL) {
		this->AnnotationRenderer->Delete();
		this->AnnotationRenderer = NULL;
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

//----------------------------------------------------------------------------
void MyImageViewer::UpdateDisplayExtent()
{
	vtkImageViewer2::UpdateDisplayExtent();
	vtkAlgorithm *input = this->GetInputAlgorithm();
	vtkInformation* outInfo = input->GetOutputInformation(0);
	int *w_ext = outInfo->Get(
		vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
	if (this->GetInputLayer())
	{
		switch (this->SliceOrientation)
		{
		case MyImageViewer::SLICE_ORIENTATION_XY:
			this->OverlayActor->SetDisplayExtent(
				w_ext[0], w_ext[1], w_ext[2], w_ext[3], this->Slice, this->Slice);
			break;

		case MyImageViewer::SLICE_ORIENTATION_XZ:
			this->OverlayActor->SetDisplayExtent(
				w_ext[0], w_ext[1], this->Slice, this->Slice, w_ext[4], w_ext[5]);
			break;

		case MyImageViewer::SLICE_ORIENTATION_YZ:
			this->OverlayActor->SetDisplayExtent(
				this->Slice, this->Slice, w_ext[2], w_ext[3], w_ext[4], w_ext[5]);
			break;
		}
	}

}

void MyImageViewer::SetColorLevel(double level)
{
	if (GetColorLevel() == level)
		return;
	vtkImageViewer2::SetColorLevel(level);
	emit ColorLevelChanged(level);

}

void MyImageViewer::SetColorWindow(double window)
{
	if (GetColorWindow() == window)
		return;
	vtkImageViewer2::SetColorWindow(window);
	emit ColorWindowChanged(window);
}

//----------------------------------------------------------------------------
void MyImageViewer::InstallPipeline()
{
	vtkImageViewer2::InstallPipeline();
	// add the renderer to render window if it hasn't been added
	if (this->RenderWindow && this->AnnotationRenderer ) {
		this->RenderWindow->AddRenderer(this->AnnotationRenderer); 
	}
	// add label view prop to renderer
	if (this->AnnotationRenderer && this->OverlayActor) 
	{
		this->AnnotationRenderer->AddViewProp(this->OverlayActor);
	}

	// Setup connection with window level mapper
	if (this->OverlayActor && this->OverlayWindowLevel)
	{
		this->OverlayActor->GetMapper()->SetInputConnection(
			this->OverlayWindowLevel->GetOutputPort());
	}
	// add cursor 
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
void MyImageViewer::UnInstallPipeline()
{
	Superclass::UnInstallPipeline();
	if (this->OverlayActor)
	{
		this->OverlayActor->GetMapper()->SetInputConnection(NULL);
	}
	if (this->AnnotationRenderer && this->OverlayActor) {
		this->AnnotationRenderer->RemoveActor(OverlayActor);
	}
	if (this->RenderWindow && this->AnnotationRenderer) {
		this->RenderWindow->RemoveRenderer(this->AnnotationRenderer);
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
}

//----------------------------------------------------------------------------
void MyImageViewer::UpdateOrientation()
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
void MyImageViewer::Render()
{
	if (this->FirstRender)
	{
		this->InitializeOrientationText();
		this->InitializeIntensityText("");
		this->InitializeHeader(string());
	}
	vtkImageViewer2::Render();

	// update orientation and header text
	ResizeHeaderAndOrientationText();
}
//----------------------------------------------------------------------------
void MyImageViewer::SetInputData(vtkImageData *in)
{
	ImageExtractVOI->SetInputData(in);
	ImageExtractVOI->SetVOI(in->GetExtent());
	ImageExtractVOI->Update();

	Superclass::SetInputConnection(ImageExtractVOI->GetOutputPort());
	//Superclass::SetInputData(in);

	//Color Map
	double* range = in->GetScalarRange();
	this->SetColorWindow(range[1] - range[0]);
	this->SetColorLevel((range[1] + range[0])*0.5);

	DefaultWindowLevel[0] = this->GetColorWindow();
	DefaultWindowLevel[1] = this->GetColorLevel();

	//Cursor
	this->InitializeCursorBoundary();

}

vtkImageData* MyImageViewer::GetOriginalInput()
{
	return vtkImageData::SafeDownCast( ImageExtractVOI->GetInput());
}

void MyImageViewer::SetInputDataLayer(vtkImageData *in)
{
	OverlayExtractVOI->SetInputData(in);
	OverlayExtractVOI->SetVOI(in->GetExtent());
	OverlayExtractVOI->Update();

	OverlayWindowLevel->SetInputConnection(OverlayExtractVOI->GetOutputPort());
	// in case when LookupTable has not been set
	if (this->LookupTable != NULL) {
		int num = this->LookupTable->GetNumberOfTableValues();
		OverlayWindowLevel->SetWindow(num - 1 );
		OverlayWindowLevel->SetLevel((num - 1)*0.5);
	}
	this->UpdateDisplayExtent();

}
//----------------------------------------------------------------------------
vtkImageData* MyImageViewer::GetInputLayer()
{
	return vtkImageData::SafeDownCast(this->OverlayWindowLevel->GetInput());
}
//----------------------------------------------------------------------------
void MyImageViewer::SetOverlay(Overlay * overlay)
{
	this->SegmentationOverlay = overlay;
	SetLookupTable(SegmentationOverlay->GetLookupTable());
	SetInputDataLayer(SegmentationOverlay->GetOutput());
}

Overlay * MyImageViewer::GetOverlay()
{
	return this->SegmentationOverlay;
}
void MyImageViewer::SetImageVOI(int * extent)
{
	const int* originalExtent = 
		vtkImageData::SafeDownCast(ImageExtractVOI->GetInput())->GetExtent();
	extent[0] = extent[0] > originalExtent[0] ? extent[0] : originalExtent[0];
	extent[1] = extent[1] < originalExtent[1] ? extent[1] : originalExtent[1];
	extent[2] = extent[2] > originalExtent[2] ? extent[2] : originalExtent[2];
	extent[3] = extent[3] < originalExtent[3] ? extent[3] : originalExtent[3];
	extent[4] = extent[4] > originalExtent[4] ? extent[4] : originalExtent[4];
	extent[5] = extent[5] < originalExtent[5] ? extent[5] : originalExtent[5];
	ImageExtractVOI->SetVOI(extent);
	Render();
}
void MyImageViewer::ResetImageVOI()
{
	int* originalExtent =
		vtkImageData::SafeDownCast(ImageExtractVOI->GetInput())->GetExtent();
	SetImageVOI(originalExtent);
	Render();
}
void MyImageViewer::SetOverlayVOI(int * extent)
{
	const int* originalExtent =
		vtkImageData::SafeDownCast(OverlayExtractVOI->GetInput())->GetExtent();
	extent[0] = extent[0] > originalExtent[0] ? extent[0] : originalExtent[0];
	extent[1] = extent[1] < originalExtent[1] ? extent[1] : originalExtent[1];
	extent[2] = extent[2] > originalExtent[2] ? extent[2] : originalExtent[2];
	extent[3] = extent[3] < originalExtent[3] ? extent[3] : originalExtent[3];
	extent[4] = extent[4] > originalExtent[4] ? extent[4] : originalExtent[4];
	extent[5] = extent[5] < originalExtent[5] ? extent[5] : originalExtent[5];
	OverlayExtractVOI->SetVOI(extent);
	Render();
}
void MyImageViewer::ResetOverlayVOI()
{
	int* originalExtent =
		vtkImageData::SafeDownCast(OverlayExtractVOI->GetInput())->GetExtent();
	SetOverlayVOI(originalExtent);
	Render();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void MyImageViewer::PrintSelf(ostream& os, vtkIndent indent)
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
	this->WindowLevel->PrintSelf(os, indent.GetNextIndent());
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

void MyImageViewer::InitializeCursorBoundary()
{
	// uncomment the following code will lead the cursor boundary updated normally
	// but it will lead to the canvas source behaviour very strange.

	//this->GetInputAlgorithm()->Update();
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	const int* extent = GetInput()->GetExtent();
	double bound[6];
	bound[0] = origin[0] + extent[0] * spacing[0];
	bound[1] = origin[0] + extent[1] * spacing[0];
	bound[2] = origin[1] + extent[2] * spacing[1];
	bound[3] = origin[1] + extent[3] * spacing[1];
	bound[4] = origin[2] + extent[4] * spacing[2];
	bound[5] = origin[2] + extent[5] * spacing[2];

	Cursor3D->SetModelBounds(bound);
	Cursor3D->Update();
}

void MyImageViewer::SetAnnotationRenderer(vtkRenderer * arg)
{
	if (this->AnnotationRenderer == arg) {
		return;
	}
	this->UnInstallPipeline();

	if (this->AnnotationRenderer)
	{
		this->AnnotationRenderer->UnRegister(this);
	}

	this->AnnotationRenderer = arg;

	if (this->AnnotationRenderer)
	{
		this->AnnotationRenderer->Register(this);
	}

	this->InstallPipeline();
	this->UpdateOrientation();
}

vtkLookupTable* MyImageViewer::GetLookupTable()
{
	return this->LookupTable;
}

void MyImageViewer::SetLookupTable(vtkLookupTable * LookupTable)
{
	this->LookupTable = LookupTable;
	this->OverlayActor->GetProperty()->SetInterpolationTypeToNearest();
	this->OverlayActor->GetProperty()->SetLookupTable(LookupTable);

	int num = LookupTable->GetNumberOfTableValues();
	OverlayWindowLevel->SetWindow(num - 1);
	OverlayWindowLevel->SetLevel((num - 1) * 0.5);
}

void MyImageViewer::SetFocalPointWithWorldCoordinate(double x, double y, double z)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	int i = (x - origin[0])/spacing[0];
	int j = (y - origin[1])/spacing[1];
	int k = (z - origin[2])/spacing[2];
	SetFocalPointWithImageCoordinate(i, j, k);

}

void MyImageViewer::SetFocalPointWithImageCoordinate(int i, int j, int k)
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

	const double* pointOld = Cursor3D->GetFocalPoint();

	if (pointOld[0] == point[0] && pointOld[1] == point[1] && pointOld[2] == point[2]) {
		return;
	}
	

	Cursor3D->SetFocalPoint(point);
	Cursor3D->Update();
	int ijk[3] = { i, j, k };
	SetSlice(ijk[this->SliceOrientation]);
	InitializeIntensityText(QString::number(
		(GetInput()->GetScalarComponentAsDouble(i, j, k, 0))));
	this->Render();

	emit FocalPointWithImageCoordinateChanged(i, j, k);
}

void MyImageViewer::GetFocalPointWithImageCoordinate(int * coordinate)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	const double* point = Cursor3D->GetFocalPoint();
	for (int i = 0; i < 3; ++i) {
		coordinate[i] = (point[i] - origin[i]) / spacing[i];
	}
}

void MyImageViewer::GetFocalPointWithWorldCoordinate(double * coordinate)
{
	memcpy(coordinate, Cursor3D->GetFocalPoint(), sizeof(double) * 3);
}

double * MyImageViewer::GetFocalPointWithWorldCoordinate()
{
	return Cursor3D->GetFocalPoint();
}

double * MyImageViewer::GetCursorBoundWithWorldCoordinate()
{
	return this->Cursor3D->GetModelBounds();
}

void MyImageViewer::SetAllBlack(bool flag)
{
	if (this->AllBlackFlag == flag) {
		return;
	}
	this->AllBlackFlag = flag;
	QList<vtkProp*>  props;
	props += this->ImageActor;
	props += this->OrientationTextActor[0];
	props += this->OrientationTextActor[1];
	props += this->OrientationTextActor[2];
	props += this->OrientationTextActor[3];
	props += this->HeaderActor;
	props += this->IntTextActor;
	props += this->CursorActor;
	props += this->OverlayActor;


	for (QList<vtkProp*>::const_iterator cit = props.cbegin(); cit != props.cend(); ++cit) {
		if ((*cit) != NULL) {
			(*cit)->SetVisibility(!flag);
		}
	}
	emit AllBlackAlready(flag);
}

bool MyImageViewer::GetAllBlack()
{
	return AllBlackFlag;
}

double* MyImageViewer::GetDefaultWindowLevel()
{
	return DefaultWindowLevel;
}

void MyImageViewer::InitializeHeader(string file)
{

	const int* size = Renderer->GetSize();
	const int margin = 15;
	int coord[2] = { 5,size[1] - margin };
	if (this->FirstRender && HeaderActor != NULL) {
		HeaderActor->SetDisplayPosition(coord[0], coord[1]);
		HeaderActor->GetTextProperty()->SetFontSize(15);
		HeaderActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
		Renderer->AddActor2D(HeaderActor);
	}

	if (GetInput() != NULL)
		HeaderActor->SetInput(file.c_str());
	else {
		HeaderActor->SetInput("");
		cout << "Error in setting text, file not found" << endl;
	}
}

void MyImageViewer::InitializeHeader(QString file)
{
	InitializeHeader(file.toStdString());
}

void MyImageViewer::InitializeIntensityText(QString IntText)
{
	const int* size = Renderer->GetSize();
	const int coord[2] = { 5,5 };
	if (this->FirstRender && IntTextActor != NULL) {
		IntTextActor->SetDisplayPosition(coord[0], coord[1]);
		IntTextActor->GetTextProperty()->SetFontSize(15);
		IntTextActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
		Renderer->AddActor2D(IntTextActor);
		return;
	}

	if (GetInput() != NULL)
		IntTextActor->SetInput(IntText.toStdString().c_str());
	else {
		IntTextActor->SetInput("");
		cout << "Error in setting text, file not found" << endl;
	}

}

void MyImageViewer::InitializeOrientationText()
{
	int* size = Renderer->GetSize();
	int margin = 15;

	int down[2] = { size[0] / 2	,5 };
	int up[2] = { size[0] / 2	,size[1] - margin };
	int left[2] = { 5			,size[1] / 2 };
	int right[2] = { size[0] - margin	,size[1] / 2 };
	int* position[4] = { up, down, left, right };

	for (int i = 0; i<4; i++)
	{
		if (this->FirstRender && OrientationTextActor[i] != NULL) {
			OrientationTextActor[i]->SetDisplayPosition(position[i][0], position[i][1]);
			OrientationTextActor[i]->GetTextProperty()->SetFontSize(15);
			OrientationTextActor[i]->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
			Renderer->AddActor2D(OrientationTextActor[i]);
		}
	}
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

}

void MyImageViewer::ResizeHeaderAndOrientationText()
{
	int* size = Renderer->GetSize();
	int margin = 15;
	int coord[2] = { 5,size[1] - margin };
	int down[2] = { size[0] / 2	,5 };
	int up[2] = { size[0] / 2	,size[1] - margin };
	int left[2] = { 5			,size[1] / 2 };
	int right[2] = { size[0] - margin	,size[1] / 2 };
	int* position[4] = { up, down, left, right };

	for (int i = 0; i<4; i++)
	{
		OrientationTextActor[i]->SetDisplayPosition(position[i][0], position[i][1]);
	}
	HeaderActor->SetDisplayPosition(coord[0], coord[1]);
}

void MyImageViewer::SetSlice(int s)
{
	if (Slice == s)
		return;
	vtkImageViewer2::SetSlice(s);
	emit SliceChanged(s);
}

void MyImageViewer::SetupInteractor(vtkRenderWindowInteractor * arg)
{
	vtkImageViewer2::SetupInteractor(arg);
	if (this->AnnotationRenderer) {
		this->AnnotationRenderer->SetActiveCamera(this->Renderer->GetActiveCamera());
	}
}

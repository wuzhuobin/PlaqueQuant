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

#include <vtkVersion.h>
#include <vtkInformation.h>
#include <vtkCamera.h>
#include <vtkImageProperty.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkCachedStreamingDemandDrivenPipeline.h>
#include <vtkImageMapper3D.h>
#include <vtkTextProperty.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkAxisActor2D.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLeaderActor2D.h>


vtkStandardNewMacro(MyImageViewer);

//----------------------------------------------------------------------------
MyImageViewer::MyImageViewer()
	:vtkImageViewer2()
{
	this->CursorActor = NULL;
	this->AnnotationRenderer = NULL;
	this->drawActor = vtkImageActor::New();
	this->AnnotationWindowLevel = vtkImageMapToWindowLevelColors::New();


	//Setup the pipeline again because of Annotation
	this->UnInstallPipeline();

	vtkRenderer *ren = vtkRenderer::New();
	this->SetAnnotationRenderer(ren);
	ren->Delete();
	// Widget 
	this->DistanceWidget = vtkDistanceWidget::New();
	this->AngleWidget = vtkAngleWidget::New();

	// OrientationTextActor 
	this->IntTextActor = vtkTextActor::New();
	this->HeaderActor = vtkTextActor::New();
	for (int i = 0; i < 4; i++)
	{
		this->OrientationTextActor[i] = vtkTextActor::New();
	}


	//this->SliceImplicitPlane = vtkPlane::New();
	//this->SliceImplicitPlane->SetOrigin(0, 0, 0);
	//this->SliceImplicitPlane->SetNormal(0, 0, 1);

	//Disable the warning
	//this->WindowLevel->SetGlobalWarningDisplay(false);
	//this->imageMapToWindowLevelColors->SetGlobalWarningDisplay(false);


	// right
	//this->LookupTable = vtkLookupTable::New();
	//this->LookupTable->SetNumberOfTableValues(7);
	//this->LookupTable->SetTableRange(0.0, 6);
	//this->LookupTable->SetTableValue(0, 0, 0, 0, 0);
	//this->LookupTable->SetTableValue(1, 1, 0, 0, 0.8);
	//this->LookupTable->SetTableValue(2, 0, 0, 1, 0.3);
	//this->LookupTable->SetTableValue(3, 0, 1, 0, 0.5);
	//this->LookupTable->SetTableValue(4, 1, 1, 0, 0.8);
	//this->LookupTable->SetTableValue(5, 0, 1, 1, 0.9);
	//this->LookupTable->SetTableValue(6, 1, 0, 1, 1);
	//this->LookupTable->Build();




	//Cursor
	Cursor3D = vtkCursor3D::New();
	Cursor3D->AllOff();
	Cursor3D->AxesOn();
	Cursor3D->SetModelBounds(0, 0, 0, 0, 0, 0);
	Cursor3D->Update();

	Cursormapper = vtkPolyDataMapper::New();
	Cursormapper->SetInputData(Cursor3D->GetOutput());

	CursorActor = vtkActor::New();
	CursorActor->SetMapper(Cursormapper);
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
	if (this->AnnotationWindowLevel) {
		this->AnnotationWindowLevel->Delete();
		this->AnnotationWindowLevel = NULL;
	}
	if (this->drawActor) {
		this->drawActor->Delete();
		this->drawActor = NULL;
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
	if (this->Cursormapper != NULL) {
		this->Cursormapper->Delete();
		this->Cursormapper = NULL;
	}
	if (this->CursorActor != NULL) {
		this->CursorActor->Delete();
		this->CursorActor = NULL;
	}

	//Widget
	if (this->DistanceWidget != NULL)
		this->DistanceWidget->Delete();
	if (this->AngleWidget != NULL)
		this->AngleWidget->Delete();


	//if (this->SliceImplicitPlane) {
	//	this->SliceImplicitPlane->Delete();
	//}
	////lookupTable
	//if (this->LookupTable != NULL) {
	//	this->LookupTable->Delete();
	//}


}


//----------------------------------------------------------------------------
void MyImageViewer::SetSliceOrientation(int m_orientation)
{
	vtkImageViewer2::SetSliceOrientation(m_orientation);

	//switch (m_orientation)
	//{
	//case 0:
	//	SliceImplicitPlane->SetNormal(1, 0, 0);
	//	SliceImplicitPlane->SetOrigin(0, 0, 0);
	//	break;

	//case 1:
	//	SliceImplicitPlane->SetNormal(0, 1, 0);
	//	SliceImplicitPlane->SetOrigin(0, 0, 0);
	//	break;

	//case 2:
	//	SliceImplicitPlane->SetNormal(0, 0, 1);
	//	SliceImplicitPlane->SetOrigin(0, 0, 0);
	//	break;
	//}
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
			this->drawActor->SetDisplayExtent(
				w_ext[0], w_ext[1], w_ext[2], w_ext[3], this->Slice, this->Slice);
			break;

		case MyImageViewer::SLICE_ORIENTATION_XZ:
			this->drawActor->SetDisplayExtent(
				w_ext[0], w_ext[1], this->Slice, this->Slice, w_ext[4], w_ext[5]);
			break;

		case MyImageViewer::SLICE_ORIENTATION_YZ:
			this->drawActor->SetDisplayExtent(
				this->Slice, this->Slice, w_ext[2], w_ext[3], w_ext[4], w_ext[5]);
			break;
		}
	}

}

//----------------------------------------------------------------------------
void MyImageViewer::InstallPipeline()
{
	vtkImageViewer2::InstallPipeline();
	// add the renderer to render window if it hasn't been added
	if (this->RenderWindow && this->AnnotationRenderer && !this->RenderWindow->HasRenderer(this->AnnotationRenderer)) {
		this->RenderWindow->AddRenderer(this->AnnotationRenderer); 
	}
	// add label view prop to renderer
	if (this->AnnotationRenderer && this->drawActor)
	{
		this->AnnotationRenderer->AddViewProp(this->drawActor);
	}

	// Setup connection with window level mapper
	if (this->drawActor && this->AnnotationWindowLevel)
	{
		this->drawActor->GetMapper()->SetInputConnection(
			this->AnnotationWindowLevel->GetOutputPort());
	}
	// add cursor 
	if (this->Renderer && this->CursorActor)
	{
		this->Renderer->AddActor(this->CursorActor);
	}


}

//----------------------------------------------------------------------------
void MyImageViewer::UnInstallPipeline()
{
	Superclass::UnInstallPipeline();
	if (this->drawActor)
	{
		this->drawActor->GetMapper()->SetInputConnection(NULL);
	}
	if (this->AnnotationRenderer && this->drawActor) {
		this->AnnotationRenderer->RemoveActor(drawActor);
	}
	if (this->RenderWindow && this->AnnotationRenderer) {
		this->RenderWindow->RemoveRenderer(this->AnnotationRenderer);
	}
	if (this->Renderer && this->CursorActor)
	{
		this->Renderer->RemoveActor(this->CursorActor);
	}
}

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
		this->InitializeHeader("");
	}
	vtkImageViewer2::Render();
	if (this->AnnotationRenderer) {
		this->AnnotationRenderer->SetActiveCamera(this->Renderer->GetActiveCamera());
	}

	if (this->GetInput())
	{
		this->RenderWindow->Render();
	}
}
//----------------------------------------------------------------------------
void MyImageViewer::SetInputData(vtkImageData *in)
{
	Superclass::SetInputData(in);

	//Color Map
	double* range = in->GetScalarRange();
	this->SetColorWindow(range[1] - range[0]);
	this->SetColorLevel((range[1] + range[0])*0.5);

	DefaultWindowLevel[0] = this->GetColorWindow();
	DefaultWindowLevel[1] = this->GetColorLevel();

	//Cursor
	this->SetCursorBoundary();

}

void MyImageViewer::SetInputDataLayer(vtkImageData *in)
{
	this->AnnotationWindowLevel->SetInputData(in);
	// in case when LookupTable has not been set
	if (this->LookupTable != NULL) {
		int num = this->LookupTable->GetNumberOfTableValues();
		AnnotationWindowLevel->SetWindow(num - 1 );
		AnnotationWindowLevel->SetLevel((num - 1)*0.5);
	}
	this->UpdateDisplayExtent();

}
//----------------------------------------------------------------------------
vtkImageData* MyImageViewer::GetInputLayer()
{
	return vtkImageData::SafeDownCast(this->AnnotationWindowLevel->GetInput());
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
	this->drawActor->PrintSelf(os, indent.GetNextIndent());
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

void MyImageViewer::SetCursorBoundary()
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	int* extent = GetInput()->GetExtent();

	Bound[0] = origin[0] + extent[0] * spacing[0];
	Bound[1] = origin[0] + extent[1] * spacing[0];
	Bound[2] = origin[1] + extent[2] * spacing[1];
	Bound[3] = origin[1] + extent[3] * spacing[1];
	Bound[4] = origin[2] + extent[4] * spacing[2];
	Bound[5] = origin[2] + extent[5] * spacing[2];

	Cursor3D->SetTranslationMode(false);
	Cursor3D->SetModelBounds(Bound);
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

vtkLookupTable* MyImageViewer::getLookupTable()
{
	return this->LookupTable;
}

void MyImageViewer::SetLookupTable(vtkLookupTable * LookupTable)
{
	this->LookupTable = LookupTable;
	this->drawActor->GetProperty()->SetInterpolationTypeToNearest();
	this->drawActor->GetProperty()->SetLookupTable(LookupTable);

	int num = LookupTable->GetNumberOfTableValues();
	AnnotationWindowLevel->SetWindow(num - 1);
	AnnotationWindowLevel->SetLevel((num - 1) * 0.5);
	//this->drawActor->SetVisibility(false);
}

void MyImageViewer::SetFocalPointWithWorldCoordinate(double x, double y, double z)
{
	Cursor3D->SetFocalPoint(x, y, z);
	//cout << __func__ << endl;
	//cout << x << ' ' << y << ' ' << z << endl;
	Cursor3D->Update();
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	x = (x - origin[0])/spacing[0];
	y = (y - origin[1])/spacing[1];
	z = (z - origin[2])/spacing[2];
	InitializeIntensityText(QString::number(
		(GetInput()->GetScalarComponentAsDouble(x, y, z, 0))));
	//SliceImplicitPlane->SetOrigin(x, y, z);
}

void MyImageViewer::SetFocalPointWithImageCoordinate(int i, int j, int k)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	double point[3] = { i*spacing[0] + origin[0],
		j*spacing[1] + origin[1], k*spacing[2] + origin[2] };
	//cout << __func__ << endl;
	//for (int in = 0; in < 3; in++) {
	//	cout << point[in]<<' ';
	//}
	Cursor3D->SetFocalPoint(i*spacing[0] + origin[0],
		j*spacing[1] + origin[1], k*spacing[2] + origin[2]);
	InitializeIntensityText(QString::number(
		(GetInput()->GetScalarComponentAsDouble(i, j, k, 0))));
	this->Render();
}

//void MyImageViewer::AddPolyData(vtkPolyData* polydata, vtkProperty* property)
//{
//	ClipActor = vtkActor::New();
//	//vtkCutter* cutter = vtkCutter::New();
//	//cutter->SetInputData (dataset);
//	//cutter->SetCutFunction (this->SliceImplicitPlane);
//	vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
//	//mapper->SetInputData(cutter->GetOutput());
//	mapper->SetInputData(polydata);
//	mapper->Update();
//	ClipActor = vtkActor::New();
//	ClipActor->SetMapper(mapper);
//	if (property)
//		ClipActor->SetProperty(property);
//	ClipActor->SetDragable(false);
//	this->Renderer->AddActor(ClipActor);
//	//cutter->Delete();
//	mapper->Delete();
//	//actor->Delete();
//}

void MyImageViewer::SetBound(double* b)
{
	memcpy(this->Bound, b, sizeof(this->Bound));
	//Cursor
	this->SetCursorBoundary();
}

double* MyImageViewer::GetBound()
{
	return this->Bound;
}

double* MyImageViewer::GetDefaultWindowLevel()
{
	return DefaultWindowLevel;
}

double* MyImageViewer::GetFocalPoint()
{
	return Cursor3D->GetFocalPoint();
}

void MyImageViewer::InitializeHeader(QString File)
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
	//if (HeaderActor != NULL) {
	//	Renderer->RemoveActor2D(HeaderActor);
	//}

	if (GetInput() != NULL)
		HeaderActor->SetInput(File.toStdString().c_str());
	else {
		HeaderActor->SetInput("");
		cout << "Error in setting text, file not found" << endl;
	}

	//int* size = Renderer->GetSize();
	//double margin = 15;
	//int coord[2] = { 5,size[1] - (int)margin };

	//if (HeaderActor != NULL) {
	//	Renderer->RemoveActor2D(HeaderActor);
	//	//HeaderActor->Delete();
	//}
	//HeaderActor->GetTextProperty()->SetFontSize(15);
	//HeaderActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
	//Renderer->AddActor2D(HeaderActor);

	//HeaderActor->SetDisplayPosition(coord[0], coord[1]);


	//QByteArray Fileba = File.toLatin1();
	//const char *Filestr = Fileba.data();


	//if (GetInput() != NULL)
	//	HeaderActor->SetInput(Filestr);
	//else
	//	cout << "Error in setting text, file not found" << endl;
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

	//int* size = Renderer->GetSize();
	////double margin = 15;
	//int coord[2] = { 5,5 };

	//if (this->FirstRender) {
	//	IntTextActor->GetTextProperty()->SetFontSize(15);
	//	IntTextActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
	//	Renderer->AddActor2D(IntTextActor);
	//	IntTextActor->SetDisplayPosition(coord[0], coord[1]);
	//	return;
	//}

	//QByteArray IntTextba = IntText.toLatin1();
	//const char *IntTextstr = IntTextba.data();


	//if (GetInput() != NULL)
	//	IntTextActor->SetInput(IntTextstr);
	//else
	//	cout << "Error in setting text, file not found" << endl;
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

	//int* size = Renderer->GetSize();
	//double margin = 15;

	//int down[2] = { size[0] / 2	,5 };
	//int up[2] = { size[0] / 2	,size[1] - (int)margin };
	//int left[2] = { 5			,size[1] / 2 };
	//int right[2] = { size[0] - (int)margin	,size[1] / 2 };

	//for (int i = 0; i<4; i++)
	//{
	//	if (OrientationTextActor[i] != NULL) {
	//		OrientationTextActor[i]->GetTextProperty()->SetFontSize(15);
	//		OrientationTextActor[i]->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
	//		Renderer->AddActor2D(OrientationTextActor[i]);
	//	}
	//	
	//}

	//OrientationTextActor[0]->SetDisplayPosition(up[0], up[1]);
	//OrientationTextActor[1]->SetDisplayPosition(down[0], down[1]);
	//OrientationTextActor[2]->SetDisplayPosition(left[0], left[1]);
	//OrientationTextActor[3]->SetDisplayPosition(right[0], right[1]);

	//switch (SliceOrientation)
	//{
	//case 0:
	//	OrientationTextActor[0]->SetInput("S");
	//	OrientationTextActor[1]->SetInput("I");
	//	OrientationTextActor[2]->SetInput("A");
	//	OrientationTextActor[3]->SetInput("P");
	//	break;
	//case 1:
	//	OrientationTextActor[0]->SetInput("S");
	//	OrientationTextActor[1]->SetInput("I");
	//	OrientationTextActor[2]->SetInput("R");
	//	OrientationTextActor[3]->SetInput("L");
	//	break;
	//case 2:
	//	OrientationTextActor[0]->SetInput("A");
	//	OrientationTextActor[1]->SetInput("P");
	//	OrientationTextActor[2]->SetInput("R");
	//	OrientationTextActor[3]->SetInput("L");
	//	break;
	//}
}

void MyImageViewer::ResizeOrientationText()
{
	//int* size = Renderer->GetSize();

	//double margin = 15;
	//int coord[2] = { 5,size[1] - (int)margin };
	//int down[2] = { size[0] / 2	,5 };
	//int up[2] = { size[0] / 2	,size[1] - (int)margin };
	//int left[2] = { 5			,size[1] / 2 };
	//int right[2] = { size[0] - (int)margin	,size[1] / 2 };

	//OrientationTextActor[0]->SetDisplayPosition(up[0], up[1]);
	//OrientationTextActor[1]->SetDisplayPosition(down[0], down[1]);
	//OrientationTextActor[2]->SetDisplayPosition(left[0], left[1]);
	//OrientationTextActor[3]->SetDisplayPosition(right[0], right[1]);
	//HeaderActor->SetDisplayPosition(coord[0], coord[1]);

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

void MyImageViewer::SetRulerEnabled(bool b)
{
	if (b)
	{
		if (DistanceWidget)
			DistanceWidget->Delete();

		DistanceWidget = vtkDistanceWidget::New();
		DistanceWidget->SetInteractor(RenderWindow->GetInteractor());
		DistanceWidget->SetPriority(RenderWindow->GetInteractor()->GetInteractorStyle()->GetPriority() + 0.1);

		vtkSmartPointer< vtkPointHandleRepresentation2D > rulerHandleRep = vtkSmartPointer< vtkPointHandleRepresentation2D >::New();
		vtkSmartPointer< vtkDistanceRepresentation2D > distanceRep = vtkSmartPointer< vtkDistanceRepresentation2D >::New();
		distanceRep->SetHandleRepresentation(rulerHandleRep);
		DistanceWidget->SetRepresentation(distanceRep);
		distanceRep->InstantiateHandleRepresentation();
		distanceRep->SetLabelFormat("%-#11.2f mm");
		distanceRep->GetAxis()->GetProperty()->SetColor(0, 1, 0);

		DistanceWidget->CreateDefaultRepresentation();
		DistanceWidget->On();
	}
	else
	{
		DistanceWidget->Off();

	}

	RenderWindow->Render();
}



void MyImageViewer::SetProtractorEnabled(bool b)
{
	if (b)
	{
		if (AngleWidget)
			AngleWidget->Delete();
		AngleWidget = vtkAngleWidget::New();
		AngleWidget->SetInteractor(RenderWindow->GetInteractor());
		AngleWidget->SetPriority(RenderWindow->GetInteractor()->GetInteractorStyle()->GetPriority() + 0.1);
		vtkSmartPointer< vtkPointHandleRepresentation2D > angleHandleRep = vtkSmartPointer< vtkPointHandleRepresentation2D >::New();
		vtkSmartPointer< vtkAngleRepresentation2D > angleRep = vtkSmartPointer< vtkAngleRepresentation2D >::New();
		angleRep->SetHandleRepresentation(angleHandleRep);
		AngleWidget->SetRepresentation(angleRep);
		angleRep->InstantiateHandleRepresentation();
		angleRep->SetLabelFormat("%-#11.2f mm");
		angleRep->GetRay1()->GetProperty()->SetColor(0, 1, 0);
		angleRep->GetRay2()->GetProperty()->SetColor(0, 1, 0);

		AngleWidget->CreateDefaultRepresentation();
		AngleWidget->On();
	}
	else
	{
		AngleWidget->Off();

	}

	RenderWindow->Render();
}

void MyImageViewer::SetupInteractor(vtkRenderWindowInteractor * arg)
{
	vtkImageViewer2::SetupInteractor(arg);
	if (this->AnnotationRenderer) {
		this->AnnotationRenderer->SetActiveCamera(this->Renderer->GetActiveCamera());
	}
}

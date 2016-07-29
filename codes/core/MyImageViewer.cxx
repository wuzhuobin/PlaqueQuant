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


vtkStandardNewMacro(MyImageViewer);

//----------------------------------------------------------------------------
MyImageViewer::MyImageViewer()
	:Superclass()
{

	// Widget 
	this->DistanceWidget = vtkDistanceWidget::New();
	this->AngleWidget = vtkAngleWidget::New();
	this->ContourWidget = vtkContourWidget::New();

	// TextActor 
	this->IntTextActor = vtkTextActor::New();
	this->HeaderActor = vtkTextActor::New();
	for (int i = 0; i < 4; i++)
	{
		TextActor[i] = vtkTextActor::New();
	}


	this->drawActor = vtkImageActor::New();
	this->imageMapToWindowLevelColors = vtkImageMapToWindowLevelColors::New();
	this->LookUpTable = vtkLookupTable::New();
	this->SliceImplicitPlane = vtkPlane::New();

	//Disable the warning
	//this->WindowLevel->SetGlobalWarningDisplay(false);
	//this->imageMapToWindowLevelColors->SetGlobalWarningDisplay(false);
	//set label Look Up Table
	this->LookUpTable->SetNumberOfTableValues(7);
	this->LookUpTable->SetTableRange(0.0, 6);
	//this->LookUpTable->SetValueRange(-1, 1);
	this->LookUpTable->SetTableValue(0, 0, 0, 0, 0);
	this->LookUpTable->SetTableValue(1, 1, 0, 0, 1);
	this->LookUpTable->SetTableValue(2, 0, 0, 1, 1);
	this->LookUpTable->SetTableValue(3, 0, 1, 0, 1);
	this->LookUpTable->SetTableValue(4, 1, 1, 0, 1);
	this->LookUpTable->SetTableValue(5, 0, 1, 1, 1);
	this->LookUpTable->SetTableValue(6, 1, 0, 1, 1);
	this->LookUpTable->Build();
	this->drawActor->GetProperty()->SetLookupTable(LookUpTable);

	this->SliceImplicitPlane->SetOrigin(0, 0, 0);
	this->SliceImplicitPlane->SetNormal(0, 0, 1);

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

	//annotationRenderer
	annotationRenderer = vtkRenderer::New();
	annotationRenderer->AddActor(CursorActor);
	annotationRenderer->SetGlobalWarningDisplay(false);
	annotationRenderer->SetActiveCamera(Renderer->GetActiveCamera());
	annotationRenderer->SetLayer(1);

	//for (int i = 0; i < 4; ++i) {
	//	Renderer->AddActor2D(TextActor[i]);
	//}
	//Renderer->AddActor(IntTextActor);
	//Renderer->AddActor(HeaderActor);
	this->drawActor->SetVisibility(false);

	RenderWindow->SetNumberOfLayers(2);
	RenderWindow->AddRenderer(annotationRenderer);

}

//----------------------------------------------------------------------------
MyImageViewer::~MyImageViewer()
{
	// TextActor
	if (IntTextActor != NULL)
		this->IntTextActor = vtkTextActor::New();
	if (HeaderActor != NULL)
		this->HeaderActor = vtkTextActor::New();
	for (int i = 0; i < 4; i++)
	{
		if (TextActor[i] != NULL)
			TextActor[i] = vtkTextActor::New();
	}
	if (Cursor3D != NULL) {
		Cursor3D->Delete();
	}
	if (Cursormapper != NULL) {
		Cursormapper->Delete();
	}
	if (CursorActor != NULL) {
		CursorActor->Delete();
	}


	if (this->imageMapToWindowLevelColors) {
		this->imageMapToWindowLevelColors->Delete();
	}
	if (this->drawActor) {
		this->drawActor->Delete();
	}
	if (this->SliceImplicitPlane) {
		this->SliceImplicitPlane->Delete();
	}
	if (this->annotationRenderer != NULL) {
		this->annotationRenderer->Delete();
	}
}


//----------------------------------------------------------------------------
void MyImageViewer::SetSliceOrientation(int orientation)
{
	Superclass::SetSliceOrientation(orientation);
	switch (orientation)
	{
	case 0:
		SliceImplicitPlane->SetNormal(1, 0, 0);
		SliceImplicitPlane->SetOrigin(0, 0, 0);
		break;

	case 1:
		SliceImplicitPlane->SetNormal(0, 1, 0);
		SliceImplicitPlane->SetOrigin(0, 0, 0);
		break;

	case 2:
		SliceImplicitPlane->SetNormal(0, 0, 1);
		SliceImplicitPlane->SetOrigin(0, 0, 0);
		break;
	}
}

//----------------------------------------------------------------------------
void MyImageViewer::UpdateDisplayExtent()
{
	Superclass::UpdateDisplayExtent();
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
double MyImageViewer::GetColorWindowLayer()
{
	return this->imageMapToWindowLevelColors->GetWindow();
}

//----------------------------------------------------------------------------
double MyImageViewer::GetColorLevelLayer()
{
	return this->imageMapToWindowLevelColors->GetLevel();
}

//----------------------------------------------------------------------------
void MyImageViewer::SetColorWindowLayer(double s)
{
	this->imageMapToWindowLevelColors->SetWindow(s);
}

//----------------------------------------------------------------------------
void MyImageViewer::SetColorLevelLayer(double s)
{
	this->imageMapToWindowLevelColors->SetLevel(s);
}

//----------------------------------------------------------------------------
void MyImageViewer::InstallPipeline()
{
	Superclass::InstallPipeline();
	if (this->Renderer && this->drawActor)
	{
		this->Renderer->AddViewProp(this->drawActor);
	}
	if (this->Renderer && this->CursorActor)
	{
		this->Renderer->AddViewProp(this->CursorActor);
	}

	if (this->drawActor && this->imageMapToWindowLevelColors)
	{
		this->drawActor->GetMapper()->SetInputConnection(this->imageMapToWindowLevelColors->GetOutputPort());
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
	if (this->Renderer && this->drawActor)
	{
		this->Renderer->RemoveViewProp(this->drawActor);
	}

	if (this->Renderer && this->CursorActor)
	{
		this->Renderer->RemoveViewProp(this->CursorActor);
	}
	if (this->Interactor)
	{
		this->Interactor->SetInteractorStyle(NULL);
		this->Interactor->SetRenderWindow(NULL);
	}
}

//----------------------------------------------------------------------------
void MyImageViewer::Render()
{
	if (this->FirstRender)
	{
		this->InitializeOrientationText();
		this->InitializeIntensityText("");
		//this->InitializeHeader();

		// Initialize the size if not set yet
#if VTK_MAJOR_VERSION <= 5
		vtkImageData *input = this->GetInput();
#else
		vtkAlgorithm *input = this->GetInputAlgorithm();
#endif
		if (input)
		{
			input->UpdateInformation();
#if VTK_MAJOR_VERSION <= 5
			int *w_ext = input->GetWholeExtent();
#else
			int *w_ext = this->GetInputInformation()->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
#endif
			int xs = 0, ys = 0;
			double d = 0;

			switch (this->SliceOrientation)
			{

				//
			case MyImageViewer::SLICE_ORIENTATION_XY:
			default:
				xs = w_ext[1] - w_ext[0] + 1;
				ys = w_ext[3] - w_ext[2] + 1;
				this->Renderer->GetActiveCamera()->Roll(180);
				this->Renderer->GetActiveCamera()->Azimuth(180);
				break;

			case MyImageViewer::SLICE_ORIENTATION_XZ:
				xs = w_ext[1] - w_ext[0] + 1;
				ys = w_ext[5] - w_ext[4] + 1;
				this->Renderer->GetActiveCamera()->Azimuth(180);
				this->Renderer->GetActiveCamera()->Elevation(180);
				break;

			case MyImageViewer::SLICE_ORIENTATION_YZ:
				xs = w_ext[3] - w_ext[2] + 1;
				ys = w_ext[5] - w_ext[4] + 1;
				//this->Renderer->GetActiveCamera()->Azimuth(180);
				break;
			}

			// if it would be smaller than 150 by 100 then limit to 150 by 100
			if (this->RenderWindow->GetSize()[0] == 0)
			{
				this->RenderWindow->SetSize(
					xs < 150 ? 150 : xs, ys < 100 ? 100 : ys);
			}

			if (this->Renderer)
			{
				this->Renderer->ResetCamera();

				this->Renderer->GetActiveCamera()->SetParallelScale(xs < 150 ? 75 : (xs - 1) / 3);
				d = this->Renderer->GetActiveCamera()->GetDistance();

				this->Renderer->GetActiveCamera()->SetParallelProjection(true);
			}
			this->FirstRender = 0;
		}
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
	this->SetColorLevel((range[1] + range[0]) / 2.0);
	DefaultWindowLevel[0] = range[1] - range[0];
	DefaultWindowLevel[1] = (range[1] + range[0]) / 2.0;

	//Cursor
	this->SetCursorBoundary();

}

void MyImageViewer::SetInputDataLayer(vtkImageData *in)
{
	this->imageMapToWindowLevelColors->SetInputData(in);
	this->UpdateDisplayExtent();
	this->drawActor->SetInputData(this->imageMapToWindowLevelColors->GetOutput());

	double* range = in->GetScalarRange();
	this->SetColorWindowLayer(range[1] - range[0]);
	this->SetColorLevelLayer((range[1] + range[0]) / 2.0);
}
//----------------------------------------------------------------------------
vtkImageData* MyImageViewer::GetInputLayer()
{
	return vtkImageData::SafeDownCast(this->imageMapToWindowLevelColors->GetInput());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void MyImageViewer::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

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
	double spacing[3];
	double origin[3];
	int extent[6];
	vtkImageData* image = vtkImageData::SafeDownCast(this->WindowLevel->GetInput());
	image->GetSpacing(spacing);
	image->GetOrigin(origin);
	image->GetExtent(extent);


	Bound[0] = origin[0];
	Bound[1] = origin[0] + extent[1] * spacing[0];
	Bound[2] = origin[1];
	Bound[3] = origin[1] + extent[3] * spacing[1];
	Bound[4] = origin[2];
	Bound[5] = origin[2] + extent[5] * spacing[2];

	Cursor3D->SetTranslationMode(false);
	Cursor3D->SetModelBounds(Bound);
	Cursor3D->Update();
}

vtkLookupTable* MyImageViewer::getLookupTable()
{
	return LookUpTable;
}

void MyImageViewer::SetFocalPoint(double x, double y, double z)
{
	Cursor3D->SetFocalPoint(x, y, z);
	Cursor3D->Update();
	SliceImplicitPlane->SetOrigin(x, y, z);
}

void MyImageViewer::AddPolyData(vtkPolyData* polydata, vtkProperty* property)
{
	ClipActor = vtkActor::New();
	//vtkCutter* cutter = vtkCutter::New();
	//cutter->SetInputData (dataset);
	//cutter->SetCutFunction (this->SliceImplicitPlane);
	vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
	//mapper->SetInputData(cutter->GetOutput());
	mapper->SetInputData(polydata);
	mapper->Update();
	ClipActor = vtkActor::New();
	ClipActor->SetMapper(mapper);
	if (property)
		ClipActor->SetProperty(property);
	ClipActor->SetDragable(false);
	this->Renderer->AddActor(ClipActor);
	//cutter->Delete();
	mapper->Delete();
	//actor->Delete();
}

void MyImageViewer::SetBound(double* b)
{
	for (int i = 0; i < 6; i++)
	{
		Bound[i] = b[i];
	}
	//Cursor
	this->SetCursorBoundary();
	//Image Actor

	//this->ImageActor->Set
}

double* MyImageViewer::GetBound()
{
	return Bound;
}

double* MyImageViewer::GetDefaultWindowLevel()
{
	return DefaultWindowLevel;
}

double* MyImageViewer::GetFocalPoint()
{
	return Cursor3D->GetFocalPoint();
}

void MyImageViewer::AddActor(vtkActor* actor, bool renderAfterward)
{
	Renderer->AddActor(actor);

	if (renderAfterward)
		RenderWindow->Render();
}

void MyImageViewer::RemoveActor(vtkActor* actor, bool renderAfterward)
{
	Renderer->RemoveActor(actor);

	if (renderAfterward)
		RenderWindow->Render();
}

void MyImageViewer::AddAnnotationActor(vtkProp* prop, bool renderAfterward)
{
	//3D view use dataRenderer to add actor to present problem of Z-buffer
	if (SliceOrientation< 3)
		annotationRenderer->AddActor(prop);
	else
		Renderer->AddActor(prop);

	if (renderAfterward)
		RenderWindow->Render();
}

void MyImageViewer::RemoveAnnotationActor(vtkProp* prop, bool renderAfterward)
{
	//3D view use dataRenderer to add actor to present problem of Z-buffer
	if (SliceOrientation < 3)
		annotationRenderer->RemoveActor(prop);
	else
		Renderer->RemoveActor(prop);

	if (renderAfterward)
		RenderWindow->Render();
}

void MyImageViewer::RemoveInput()
{

	if (this->WindowLevel)
	{
		this->WindowLevel->Delete();
		this->WindowLevel = NULL;
	}
	if (this->imageMapToWindowLevelColors)
	{
		this->imageMapToWindowLevelColors->Delete();
		this->imageMapToWindowLevelColors = NULL;
	}
	if (this->ImageActor)
	{
		this->ImageActor->Delete();
		this->ImageActor = NULL;
	}
	if (this->Renderer)
	{
		this->Renderer->Delete();
		this->Renderer = NULL;
	}
	/*
	if (this->RenderWindow)
	{
	this->RenderWindow->Delete();
	this->RenderWindow = NULL;
	}
	*/
	if (this->Interactor)
	{
		this->Interactor->Delete();
		this->Interactor = NULL;
	}

	if (this->InteractorStyle)
	{
		this->InteractorStyle->Delete();
		this->InteractorStyle = NULL;
	}

	if (this->SliceImplicitPlane)
	{
		this->SliceImplicitPlane->Delete();
		this->SliceImplicitPlane = NULL;
	}

}

void MyImageViewer::RemoveInputLayer()
{
	if (this->drawActor)
	{
		this->drawActor->Delete();
		this->drawActor = NULL;
	}

	if (this->imageMapToWindowLevelColors)
		this->imageMapToWindowLevelColors->Delete();
	this->imageMapToWindowLevelColors = vtkImageMapToWindowLevelColors::New();

}
void MyImageViewer::InitializeHeader(QString File)
{
	int* size = Renderer->GetSize();
	double margin = 15;
	int coord[2] = { 5,size[1] - (int)margin };

	if (HeaderActor != NULL) {
		Renderer->RemoveActor2D(HeaderActor);
		//HeaderActor->Delete();
	}
	HeaderActor = vtkTextActor::New();
	HeaderActor->GetTextProperty()->SetFontSize(15);
	HeaderActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
	Renderer->AddActor2D(HeaderActor);

	HeaderActor->SetDisplayPosition(coord[0], coord[1]);


	QByteArray Fileba = File.toLatin1();
	const char *Filestr = Fileba.data();


	if (GetInput() != NULL)
		HeaderActor->SetInput(Filestr);
	else
		cout << "Error in setting text, file not found" << endl;
}

void MyImageViewer::InitializeIntensityText(QString IntText)
{
	int* size = Renderer->GetSize();
	//double margin = 15;
	int coord[2] = { 5,5 };

	if (this->FirstRender) {
		IntTextActor->GetTextProperty()->SetFontSize(15);
		IntTextActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
		Renderer->AddActor2D(IntTextActor);
		IntTextActor->SetDisplayPosition(coord[0], coord[1]);
		return;
	}

	QByteArray IntTextba = IntText.toLatin1();
	const char *IntTextstr = IntTextba.data();


	if (GetInput() != NULL)
		IntTextActor->SetInput(IntTextstr);
	else
		cout << "Error in setting text, file not found" << endl;
}

void MyImageViewer::InitializeOrientationText()
{
	int* size = Renderer->GetSize();
	double margin = 15;

	int down[2] = { size[0] / 2	,5 };
	int up[2] = { size[0] / 2	,size[1] - (int)margin };
	int left[2] = { 5			,size[1] / 2 };
	int right[2] = { size[0] - (int)margin	,size[1] / 2 };

	for (int i = 0; i<4; i++)
	{
		TextActor[i] = vtkTextActor::New();
		TextActor[i]->GetTextProperty()->SetFontSize(15);
		TextActor[i]->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
		Renderer->AddActor2D(TextActor[i]);
	}

	TextActor[0]->SetDisplayPosition(up[0], up[1]);
	TextActor[1]->SetDisplayPosition(down[0], down[1]);
	TextActor[2]->SetDisplayPosition(left[0], left[1]);
	TextActor[3]->SetDisplayPosition(right[0], right[1]);

	switch (SliceOrientation)
	{
	case 0:
		TextActor[0]->SetInput("S");
		TextActor[1]->SetInput("I");
		TextActor[2]->SetInput("A");
		TextActor[3]->SetInput("P");
		break;
	case 1:
		TextActor[0]->SetInput("S");
		TextActor[1]->SetInput("I");
		TextActor[2]->SetInput("R");
		TextActor[3]->SetInput("L");
		break;
	case 2:
		TextActor[0]->SetInput("A");
		TextActor[1]->SetInput("P");
		TextActor[2]->SetInput("R");
		TextActor[3]->SetInput("L");
		break;
	}
}

void MyImageViewer::ResizeOrientationText()
{
	int* size = Renderer->GetSize();

	double margin = 15;
	int coord[2] = { 5,size[1] - (int)margin };
	int down[2] = { size[0] / 2	,5 };
	int up[2] = { size[0] / 2	,size[1] - (int)margin };
	int left[2] = { 5			,size[1] / 2 };
	int right[2] = { size[0] - (int)margin	,size[1] / 2 };

	TextActor[0]->SetDisplayPosition(up[0], up[1]);
	TextActor[1]->SetDisplayPosition(down[0], down[1]);
	TextActor[2]->SetDisplayPosition(left[0], left[1]);
	TextActor[3]->SetDisplayPosition(right[0], right[1]);
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
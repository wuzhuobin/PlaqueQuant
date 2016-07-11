#include "MyImageViewer.h"

vtkStandardNewMacro(MyImageViewer);

MyImageViewer::MyImageViewer()
	:vtkImageViewer2()
{
	//this->DistanceWidget = vtkDistanceWidget::New();
	//this->AngleWidget = vtkAngleWidget::New();
	//this->ContourWidget = vtkContourWidget::New();
	this->ImageActorLayer = vtkImageActor::New();
	this->WindowLevelLayer = vtkImageMapToWindowLevelColors::New();
	this->LookUpTableLayer = vtkLookupTable::New();
	this->SliceImplicitPlane = vtkPlane::New();
	this->IntTextActor = vtkTextActor::New();
	this->HeaderActor = vtkTextActor::New();
	for (int i = 0; i < 4; i++)
	{
		TextActor[i] = vtkTextActor::New();
	}


	//Disable the warning
	this->WindowLevel->SetGlobalWarningDisplay(false);
	this->WindowLevelLayer->SetGlobalWarningDisplay(false);
	//set label Look Up Table
	this->LookUpTableLayer->SetNumberOfTableValues(4);
	this->LookUpTableLayer->SetTableRange(0.0, 3);
	//this->LookUpTableLayer->SetValueRange(-1, 1);
	this->LookUpTableLayer->SetTableValue(0, 0, 0, 0, 0);
	this->LookUpTableLayer->SetTableValue(1, 1, 0, 0, 1);
	this->LookUpTableLayer->SetTableValue(2, 0, 0, 1, 1);
	this->LookUpTableLayer->SetTableValue(3, 0, 1, 0, 1);
	this->LookUpTableLayer->Build();
	this->ImageActorLayer->GetProperty()->SetLookupTable(LookUpTableLayer);


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
	//Image Actor
	this->ImageActor->GetProperty()->SetInterpolationTypeToNearest();
	ImageActorLayer->GetProperty()->SetInterpolationTypeToNearest();

	this->ImageActor->SetOpacity(1.0);
	this->ImageActorLayer->SetOpacity(1);
	this->ImageActorLayer->SetVisibility(false);
}


MyImageViewer::~MyImageViewer()
{
	//if (this->DistanceWidget) {
	//	this->DistanceWidget->Delete();
	//	this->DistanceWidget = NULL;
	//}
	//if (this->AngleWidget) {
	//	this->AngleWidget->Delete();
	//	this->AngleWidget = NULL;
	//}
	//if (this->ContourWidget) {
	//	this->ContourWidget->Delete();
	//	this->ContourWidget = NULL;
	//}
	if (this->ImageActorLayer) {
		this->ImageActorLayer->Delete();
		this->ImageActorLayer = NULL;
	}
	if (this->WindowLevelLayer) {
		this->WindowLevelLayer->Delete();
		this->WindowLevelLayer = NULL;
	}
	if (this->LookUpTableLayer) {
		this->LookUpTableLayer->Delete();
		this->LookUpTableLayer = NULL;
	}
	if (this->SliceImplicitPlane) {
		this->SliceImplicitPlane->Delete();
		this->SliceImplicitPlane = NULL;
	}
	if (this->Cursor3D) {
		this->Cursor3D->Delete();
		this->Cursor3D = NULL;
	}
	if (Cursormapper) {
		Cursormapper->Delete();
		Cursormapper = NULL;
	}
	if (CursorActor) {
		CursorActor->Delete();
		CursorActor = NULL;
	}
	
	if (HeaderActor) {
		HeaderActor->Delete();
		HeaderActor = NULL;
	}
	if(IntTextActor){
		this->IntTextActor->Delete();
		IntTextActor = NULL;
	}
	for (int i = 0; i < 4; i++)
	{
		if(TextActor[i])
		TextActor[i] = NULL;
	}
}

double MyImageViewer::GetColorWindowLayer()
{
	return this->WindowLevelLayer->GetWindow();
}

double MyImageViewer::GetColorLevelLayer()
{
	return this->WindowLevelLayer->GetWindow();
}

void MyImageViewer::SetColorWindowLayer(double s)
{
	this->WindowLevelLayer->SetWindow(s);
}

void MyImageViewer::SetColorLevelLayer(double s)
{
	this->WindowLevelLayer->SetLevel(s);
}

void MyImageViewer::SetDefaultWindowLevel(double * defaultWindowLevel)
{
	this->DefaultWindowLevel[0] = defaultWindowLevel[0];
	this->DefaultWindowLevel[1] = defaultWindowLevel[1];
}


void MyImageViewer::SetInputDataLayer(vtkImageData *in)
{
	this->WindowLevelLayer->SetInputData(in);
	this->UpdateDisplayExtent();
	this->ImageActorLayer->SetInputData(this->WindowLevelLayer->GetOutput());

	double* range = in->GetScalarRange();
	this->SetColorWindowLayer(range[1] - range[0]);
	this->SetColorLevelLayer((range[1] + range[0]) / 2.0);
}

vtkImageData * MyImageViewer::GetInputLayer()
{
	return vtkImageData::SafeDownCast(this->WindowLevelLayer->GetInput());
}

/*------------------------------------------------------------------------------------------------------------------*/

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
	GetRenderer()->AddActor(ClipActor);
	//cutter->Delete();
	mapper->Delete();
	//actor->Delete();
}

void MyImageViewer::SetSliceOrientation(int orientation)
{
	vtkImageViewer2::SetSliceOrientation(orientation);
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

	this->Render();

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

//void MyImageViewer::Render(void)
//{
//	//switch (this->SliceOrientation)
//	//{
//
//	//	//
//	//case MyImageViewer::SLICE_ORIENTATION_XY:
//	//default:
//	//	GetRenderer()->GetActiveCamera()->Roll(180);
//	//	GetRenderer()->GetActiveCamera()->Azimuth(180);
//	//	break;
//
//	//case MyImageViewer::SLICE_ORIENTATION_XZ:
//	//	GetRenderer()->GetActiveCamera()->Azimuth(180);
//	//	GetRenderer()->GetActiveCamera()->Elevation(180);
//	//	break;
//	//}
//	if (this->FirstRender)
//	{
//		this->InitializeOrientationText();
//		this->InitializeIntensityText("");
//		//switch (this->SliceOrientation)
//		//{
//
//		//	//
//		//case MyImageViewer::SLICE_ORIENTATION_XY:
//		//default:
//		//	GetRenderer()->GetActiveCamera()->Roll(180);
//		//	GetRenderer()->GetActiveCamera()->Azimuth(180);
//		//	break;
//
//		//case MyImageViewer::SLICE_ORIENTATION_XZ:
//		//	GetRenderer()->GetActiveCamera()->Azimuth(180);
//		//	GetRenderer()->GetActiveCamera()->Elevation(180);
//		//	break;
//		//}
//	}
//	vtkImageViewer2::Render();
//
//
//
//}

void MyImageViewer::RemoveInput()
{

	if (this->WindowLevel)
	{
		this->WindowLevel->Delete();
		this->WindowLevel = NULL;
	}
	if (this->WindowLevelLayer)
	{
		this->WindowLevelLayer->Delete();
		this->WindowLevelLayer = NULL;
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
	//if (this->Interactor)
	//{
	//	this->Interactor->Delete();
	//	this->Interactor = NULL;
	//}

	//if (this->InteractorStyle)
	//{
	//	this->InteractorStyle->Delete();
	//	this->InteractorStyle = NULL;
	//}

	//if (this->SliceImplicitPlane)
	//{
	//	this->SliceImplicitPlane->Delete();
	//	this->SliceImplicitPlane = NULL;
	//}

	/*
	//Remove Header
	if (HeaderActor != NULL)
	Renderer->RemoveActor2D(HeaderActor);

	//REmove Orientation Text
	for (int i = 0 ; i < 4 ; i++)
	{
	Renderer->RemoveActor2D(TextActor[i]);
	}

	//Remove cursor bound
	if (CursorActor != NULL)
	Renderer->RemoveActor(CursorActor);

	if (this->WindowLevel)
	this->WindowLevel->Delete();
	this->WindowLevel = vtkImageMapToWindowLevelColors::New();


	*/
}

void MyImageViewer::RemoveInputLayer()
{
	if (this->ImageActorLayer)
	{
		this->ImageActorLayer->Delete();
		this->ImageActorLayer = NULL;
	}

	if (this->WindowLevelLayer)
		this->WindowLevelLayer->Delete();
	this->WindowLevelLayer = vtkImageMapToWindowLevelColors::New();

}
void MyImageViewer::initializeHeader(std::string File)
{
	int* size = GetRenderer()->GetSize();
	double margin = 15;
	int coord[2] = { 5,size[1] - (int)margin };

	if (HeaderActor != NULL) {
		GetRenderer()->RemoveActor2D(HeaderActor);
	}
	HeaderActor->GetTextProperty()->SetFontSize(15);
	HeaderActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
	GetRenderer()->AddActor2D(HeaderActor);

	HeaderActor->SetDisplayPosition(coord[0], coord[1]);


	if (GetInput() != NULL)
		HeaderActor->SetInput(File.c_str());
	else
		cout << "Error in setting text, file not found" << endl;
}

void MyImageViewer::InitializeIntensityText(std::string IntText)
{
	int* size = GetRenderer()->GetSize();
	//double margin = 15;
	int coord[2] = { 5,5 };

	if (this->FirstRender) {
		IntTextActor->GetTextProperty()->SetFontSize(15);
		IntTextActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
		GetRenderer()->AddActor2D(IntTextActor);
		IntTextActor->SetDisplayPosition(coord[0], coord[1]);
		return;
	}


	if (GetInput() != NULL)
		IntTextActor->SetInput(IntText.c_str());
	else
		cout << "Error in setting text, file not found" << endl;
}

void MyImageViewer::InitializeOrientationText()
{
	int* size = GetRenderer()->GetSize();
	double margin = 15;

	int down[2] = { size[0] / 2	,5 };
	int up[2] = { size[0] / 2	,size[1] - (int)margin };
	int left[2] = { 5			,size[1] / 2 };
	int right[2] = { size[0] - (int)margin	,size[1] / 2 };

	TextActor[0]->SetDisplayPosition(up[0], up[1]);
	TextActor[1]->SetDisplayPosition(down[0], down[1]);
	TextActor[2]->SetDisplayPosition(left[0], left[1]);
	TextActor[3]->SetDisplayPosition(right[0], right[1]);
	for (int i = 0; i<4; i++)
	{
		TextActor[i]->GetTextProperty()->SetFontSize(15);
		TextActor[i]->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
		GetRenderer()->AddActor2D(TextActor[i]);
	}
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
		TextActor[0]->SetInput("P");
		TextActor[1]->SetInput("A");
		TextActor[2]->SetInput("R");
		TextActor[3]->SetInput("L");
		break;
	}
}

void MyImageViewer::ResizeOrientationText()
{
	int* size = GetRenderer()->GetSize();

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

//void MyImageViewer::SetRulerEnabled(bool b)
//{
//	if (b)
//	{
//		if (DistanceWidget)
//			DistanceWidget->Delete();
//
//		DistanceWidget = vtkDistanceWidget::New();
//		DistanceWidget->SetInteractor(GetRenderWindow()->GetInteractor());
//		DistanceWidget->SetPriority(GetRenderWindow()->GetInteractor()->GetInteractorStyle()->GetPriority() + 0.1);
//
//		vtkSmartPointer< vtkPointHandleRepresentation2D > rulerHandleRep = vtkSmartPointer< vtkPointHandleRepresentation2D >::New();
//		vtkSmartPointer< vtkDistanceRepresentation2D > distanceRep = vtkSmartPointer< vtkDistanceRepresentation2D >::New();
//		distanceRep->SetHandleRepresentation(rulerHandleRep);
//		DistanceWidget->SetRepresentation(distanceRep);
//		distanceRep->InstantiateHandleRepresentation();
//		distanceRep->SetLabelFormat("%-#11.2f mm");
//		distanceRep->GetAxis()->GetProperty()->SetColor(0, 1, 0);
//
//		DistanceWidget->CreateDefaultRepresentation();
//		DistanceWidget->On();
//	}
//	else
//	{
//		DistanceWidget->Off();
//
//	}
//
//	GetRenderWindow()->Render();
//}
//
//
//
//void MyImageViewer::SetProtractorEnabled(bool b)
//{
//	if (b)
//	{
//		if (AngleWidget)
//			AngleWidget->Delete();
//		AngleWidget = vtkAngleWidget::New();
//		AngleWidget->SetInteractor(GetRenderWindow()->GetInteractor());
//		AngleWidget->SetPriority(GetRenderWindow()->GetInteractor()->GetInteractorStyle()->GetPriority() + 0.1);
//		vtkSmartPointer< vtkPointHandleRepresentation2D > angleHandleRep = vtkSmartPointer< vtkPointHandleRepresentation2D >::New();
//		vtkSmartPointer< vtkAngleRepresentation2D > angleRep = vtkSmartPointer< vtkAngleRepresentation2D >::New();
//		angleRep->SetHandleRepresentation(angleHandleRep);
//		AngleWidget->SetRepresentation(angleRep);
//		angleRep->InstantiateHandleRepresentation();
//		angleRep->SetLabelFormat("%-#11.2f mm");
//		angleRep->GetRay1()->GetProperty()->SetColor(0, 1, 0);
//		angleRep->GetRay2()->GetProperty()->SetColor(0, 1, 0);
//
//		AngleWidget->CreateDefaultRepresentation();
//		AngleWidget->On();
//	}
//	else
//	{
//		AngleWidget->Off();
//
//	}
//
//	GetRenderWindow()->Render();
//}


#include "MyImageViewer.h"

vtkStandardNewMacro(MyImageViewer);

MyImageViewer::MyImageViewer()
	:vtkImageViewer2()
{
	this->IntTextActor = vtkTextActor::New();
	this->HeaderActor = vtkTextActor::New();
	for (int i = 0; i < 4; i++)
	{
		TextActor[i] = vtkTextActor::New();
	}


	//Disable the warning
	this->WindowLevel->SetGlobalWarningDisplay(false);
}

MyImageViewer::~MyImageViewer()
{
	
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

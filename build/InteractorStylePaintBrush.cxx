#include "InteractorStylePaintBrush.hxx"
vtkStandardNewMacro(InteractorStylePaintBrush);

InteractorStylePaintBrush::InteractorStylePaintBrush()
:brush(vtkSmartPointer<vtkImageCanvasSource2D>::New()), 
brushActor(vtkSmartPointer<vtkImageActor>::New()),
brushRender(vtkSmartPointer<vtkRenderer>::New())
{
	//m_drawValueComboBox = NULL;
	//m_drawBrushShapeComboBox = NULL;
	//m_drawBrushSizeSpinBox = NULL;
	//m_drawVolumetricCheckBox = NULL;
	//m_drawIsotropicCheckBox = NULL;
	m_retangleRep = NULL;
	//Default color: Red
	red = 0;
	green = 0;
	blue = 0;
	opacity = 0;
	brush->SetScalarTypeToUnsignedChar();
	brush->SetNumberOfScalarComponents(4);



}

InteractorStylePaintBrush::~InteractorStylePaintBrush()
{
}

void InteractorStylePaintBrush::SetPaintBrushModeEnabled(bool b)
{
	if (!b) return;
	for (int i = 0; i < 3; ++i) {
		cout << spacing[i] << endl;
	}
	brush->SetDrawColor(0, 0, 0, 0);
	switch (orientation)
	{
	case vtkImageViewer2::SLICE_ORIENTATION_XY:
		brush->SetExtent(extent);
		break;
	case vtkImageViewer2::SLICE_ORIENTATION_XZ:
		brush->SetExtent(extent[0], extent[1], 0, 0, extent[4], extent[5]);
		break;
	case vtkImageViewer2::SLICE_ORIENTATION_YZ:
		brush->SetExtent(extent[2], extent[3], extent[4], extent[5], 0, 0);
		break;

	default:
		break;
	}
	//brush->SetExtent(extent);
	brush->FillBox(0, 100, 0, 100);
	brush->SetDrawColor(255, 255, 0, 255);
	brush->FillBox(0, 50, 0, 50);
	brush->Update();

	brushActor->SetInputData(brush->GetOutput());
	brushRender->AddActor(brushActor);
	brushRender->SetLayer(1);
	imageViewer->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetLayer(0);
	imageViewer->GetRenderWindow()->SetNumberOfLayers(2);
	imageViewer->GetRenderWindow()->AddRenderer(brushRender);
	Render();
}

//void InteractorStylePaintBrush::SetDrawBrushSizeSpinBox(QSpinBox* s)
//{
//	m_drawBrushSizeSpinBox = s;
//}
//
//void InteractorStylePaintBrush::SetDrawBrushShapeComBox(QComboBox* comboBox)
//{
//	m_drawBrushShapeComboBox = comboBox;
//}
//
//void InteractorStylePaintBrush::SetDrawVolumetricCheckBox(QCheckBox* checkBox)
//{
//	m_drawVolumetricCheckBox = checkBox;
//}
//
//void InteractorStylePaintBrush::SetDrawIsotropicCheckBox(QCheckBox* checkBox)
//{
//	m_drawIsotropicCheckBox = checkBox;
//}
void InteractorStylePaintBrush::SetDrawColor(int r, int g, int b)
{
	red = r;
	green = g;
	blue = b;
}
void InteractorStylePaintBrush::SetDrawColor(int* rgb)
{
	red = rgb[0];
	green = rgb[1];
	blue = rgb[2];
}
void InteractorStylePaintBrush::SetDrawOpacity(int opacity)
{
	opacity = opacity;
}



void InteractorStylePaintBrush::Render()
{
	brush->Update();
	//m_brush->GetOutput()->SetOrigin(origin);
	//m_brush->GetOutput()->SetSpacing(spacing);
	imageViewer->Render();
}



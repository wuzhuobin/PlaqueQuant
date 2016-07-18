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
	mode = InteractorStylePaintBrush::OtherMode;
	for (int i = 0; i < 3; ++i) {
		cout << spacing[i] << endl;
	}
	brush->SetDrawColor(0, 0, 0, 0);
	switch (orientation)
	{
	case vtkImageViewer2::SLICE_ORIENTATION_XY:
		brush->SetExtent(extent);
		brush->FillBox(extent[0], extent[1], extent[2], extent[3]);
		break;
	case vtkImageViewer2::SLICE_ORIENTATION_XZ:
		brush->SetExtent(extent[0], extent[1], extent[4] * spacing[2] / spacing[1], extent[5] * spacing[2] / spacing[1],
			0, 0);
		brush->FillBox(extent[0], extent[1], extent[4] * spacing[2] / spacing[1], extent[5] * spacing[2] / spacing[1]);
		break;

	case vtkImageViewer2::SLICE_ORIENTATION_YZ:
		brush->SetExtent(extent[2], extent[3], extent[4] * spacing[2] / spacing[1], extent[5] * spacing[2] / spacing[1],
			0, 0);
		brush->FillBox(extent[2], extent[3], extent[4] * spacing[2] / spacing[1], extent[5] * spacing[2] / spacing[1]);
		break;

	default:
		break;
	}
	brush->SetDrawColor(255, 255, 0, 255);
	brush->FillBox(0, 50, 0, 50);
	brush->Update();

	brushActor->GetProperty()->SetInterpolationTypeToNearest();
	brushActor->SetInputData(brush->GetOutput());

	brushRender->AddActor(brushActor);
	brushRender->SetLayer(1);
	//brushRender->SetActiveCamera(imageViewer->GetRenderer()->GetActiveCamera());

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
	brush->SetDrawColor(r, g, b, opacity);
}
void InteractorStylePaintBrush::SetDrawColor(int* rgb)
{
	red = rgb[0];
	green = rgb[1];
	blue = rgb[2];
	brush->SetDrawColor(red, green, blue, opacity);

}
void InteractorStylePaintBrush::SetDrawOpacity(int opacity)
{
	this->opacity = opacity;
	brush->SetDrawColor(red, green, blue, opacity);

}

void InteractorStylePaintBrush::OnLeftButtonDown()
{
	lclickDrag = true;
	CalculateIndex(index);
}

void InteractorStylePaintBrush::OnMouseMove()
{
	if(lclickDrag){
		draw();
	}
}

void InteractorStylePaintBrush::draw()
{
	SetDrawColor(255, 0, 0);
	SetDrawOpacity(255);
	double newIndex[3] = { 0, 0, 0 };
	CalculateIndex(newIndex);

	int x2Dindex, y2Dindex;
	switch (orientation)
	{
	case vtkImageViewer2::SLICE_ORIENTATION_YZ:
		x2Dindex = 1;
		y2Dindex = 2;
		break;
	case vtkImageViewer2::SLICE_ORIENTATION_XZ:
		x2Dindex = 0;
		y2Dindex = 2;
		break;
	case vtkImageViewer2::SLICE_ORIENTATION_XY:
		x2Dindex = 0;
		y2Dindex = 1;
		brush->DrawSegment3D(index[0], index[1], 0, newIndex[0], newIndex[1], 0);
		break;
	}

	if ("Circle" == "Circle") {
		if (brushSize % 2 == 1)
			drawCircle(index[x2Dindex], index[y2Dindex], newIndex[x2Dindex], newIndex[y2Dindex], brushSize);
		else
		{
			drawCircle(index[x2Dindex] - 1, index[y2Dindex], newIndex[x2Dindex] - 1, newIndex[y2Dindex], brushSize/ 2 - 1);
			drawCircle(index[x2Dindex] - 1, index[y2Dindex] - 1, newIndex[x2Dindex] - 1, newIndex[y2Dindex] - 1, brushSize / 2 - 1);
			drawCircle(index[x2Dindex], index[y2Dindex] - 1, newIndex[x2Dindex], newIndex[y2Dindex] - 1, brushSize / 2 - 1);
			drawCircle(index[x2Dindex] , index[y2Dindex], newIndex[x2Dindex], newIndex[y2Dindex], brushSize / 2 - 1);
		}
	}
	//Default:Square
	//else
	//{
	//	if (m_drawBrushSizeSpinBox->value() % 2 == 1)
	//	{// Odd Number Brush Size
	//		for (int x = -(m_drawBrushSizeSpinBox->value() - 1) / 2; x < (m_drawBrushSizeSpinBox->value() - 1) / 2 + 1; x++)
	//		{
	//			for (int y = -(m_drawBrushSizeSpinBox->value() - 1) / 2; y < (m_drawBrushSizeSpinBox->value() - 1) / 2 + 1; y++)
	//			{
	//				this->DrawLine3D(draw_index_old[x2Dindex] + x, draw_index_old[y2Dindex] + y, draw_index[x2Dindex] + x, draw_index[y2Dindex] + y);
	//			}
	//		}
	//	}
	//	else
	//	{ // Even Number Brush Size
	//		for (int x = -(m_drawBrushSizeSpinBox->value()) / 2; x < (m_drawBrushSizeSpinBox->value()) / 2; x++)
	//		{
	//			for (int y = -(m_drawBrushSizeSpinBox->value()) / 2; y < (m_drawBrushSizeSpinBox->value()) / 2; y++)
	//			{
	//				this->DrawLine3D(draw_index_old[x2Dindex] + x, draw_index_old[y2Dindex] + y, draw_index[x2Dindex] + x, draw_index[y2Dindex] + y);
	//			}
	//		}

	//	}
	//}

	// Reset the old draw index
	for (int i = 0; i < 3; i++)
	{
		index[i] = newIndex[i];
	}
	Render();
}

void InteractorStylePaintBrush::drawCircle(int x0, int y0, int x1, int y1, int brushSize)
{
	int x = 0, y = brushSize;
	int d = 1 - brushSize;
	while (x <= y) {
		for (int i = -x; i <= x; i++)
		{
			this->drawLine3D(x0 + i, y0 + y, x1 + i, y1 + y);
			this->drawLine3D(x0 + i, y0 - y, x1 + i, y1 - y);
		}

		for (int j = -y; j <= y; j++)
		{
			this->drawLine3D(x0 + j, y0 + x, x1 + j, y1 + x);
			this->drawLine3D(x0 + j, y0 - x, x1 + j, y1 - x);
		}

		if (d<0)
			d += (2 * x) + 3;
		else {
			d += (2 * (x - y)) + 5;
			y -= 1;
		}
		x++;
	}
}

void InteractorStylePaintBrush::drawLine3D(int x0, int y0, int x1, int y1)
{
	int index[6];
	//switch (orientation)
	//{
	//case vtkImageViewer2::SLICE_ORIENTATION_YZ:
	//	if (x0 < extent[2] || x0 > extent[3] || y0 < extent[4] || y0 > extent[5] || x1 < extent[2] || x1 > extent[3] || y1 < extent[4] || y1 > extent[5])
	//		return;
	//	index[0] = 0;
	//	index[1] = x0;
	//	index[2] = y0;
	//	index[3] = 0;
	//	index[4] = x1;
	//	index[5] = y1;
	//	break;
	//case vtkImageViewer2::SLICE_ORIENTATION_XZ:
	//	if (x0 < extent[0] || x0 > extent[1] || y0 < extent[4] || y0 > extent[5] || x1 < extent[0] || x1 > extent[1] || y1 < extent[4] || y1 > extent[5])
	//		return;
	//	index[0] = x0;
	//	index[1] = 0;
	//	index[2] = y0;
	//	index[3] = x1;
	//	index[4] = 0;
	//	index[5] = y1;
	//	break;
	//case vtkImageViewer2::SLICE_ORIENTATION_XY:
	//	if (x0 < extent[0] || x0 > extent[1] || y0 < extent[2] || y0 > extent[3] || x1 < extent[0] || x1 > extent[1] || y1 < extent[2] || y1 > extent[3])
	//		return;
		index[0] = x0;
		index[1] = y0;
		index[2] = 0;
		index[3] = x1;
		index[4] = y1;
		index[5] = 0;
	//	break;
	//}
	brush->DrawSegment3D(index[0], index[1], index[2], index[3], index[4], index[5]);
}



void InteractorStylePaintBrush::Render()
{
	brush->Update();
	//m_brush->GetOutput()->SetOrigin(origin);
	//m_brush->GetOutput()->SetSpacing(spacing);
	imageViewer->Render();
}



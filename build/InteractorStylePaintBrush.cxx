#include "InteractorStylePaintBrush.hxx"
vtkStandardNewMacro(InteractorStylePaintBrush);

InteractorStylePaintBrush::InteractorStylePaintBrush()
{
	//m_drawValueComboBox = NULL;
	//m_drawBrushShapeComboBox = NULL;
	//m_drawBrushSizeSpinBox = NULL;
	//m_drawVolumetricCheckBox = NULL;
	//m_drawIsotropicCheckBox = NULL;
	m_borderWidget = NULL;
	m_retangleRep = NULL;
	m_circleRep = NULL;
	m_brush = NULL;
	BrushActor = NULL;
	//Default color: Red
	color_r = 255;
	color_g = 0;
	color_b = 0;
	m_opacity = 255;


}

InteractorStylePaintBrush::~InteractorStylePaintBrush()
{
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
	color_r = r;
	color_g = g;
	color_b = b;
}
void InteractorStylePaintBrush::SetDrawColor(int* rgb)
{
	color_r = rgb[0];
	color_g = rgb[1];
	color_b = rgb[2];
}
void InteractorStylePaintBrush::SetDrawOpacity(int opacity)
{
	m_opacity = opacity;
}

void InteractorStylePaintBrush::SetImageViewer(vtkImageViewer2* imageViewer)
{
	Superclass::SetImageViewer(imageViewer);
	imageViewer->GetInput()->GetExtent(extent);
}

void InteractorStylePaintBrush::OnLeftButtonUp()
{
	if (!isDraw)
	{
		//this->Write2ImageData();
		//Clear Layer
		m_brush->SetDrawColor(0, 0, 0, 0);
		this->FillBox3D();
		this->Render();
		isDraw = true;
	}
	lclickDrag = false;

}

void InteractorStylePaintBrush::OnLeftButtonDown()
{
	lclickDrag = true;
	isDraw = false;
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	double index[3];
	if (imageViewer->GetInput() != NULL) {
		picked[orientation] = origin[orientation] + imageViewer->GetSlice() * spacing[orientation];
		for (int i = 0; i<3; i++)
		{
			index[i] = (picked[i] - origin[i]) / spacing[i];
		}
	}
	draw_index_old[0] = (int)(index[0] + 0.5);
	draw_index_old[1] = (int)(index[1] + 0.5);
	draw_index_old[2] = (int)(index[2] + 0.5);
	if (rclickDrag == true)
	{
		rclickDrag = false;
	}
	else
	{
		//this->ReadfromImageData();
		this->Render();
	}
	Draw(true);
	this->Render();
}

void InteractorStylePaintBrush::OnRightButtonUp()
{
	rclickDrag = false;
	if (!isDraw)
	{
		//this->Write2ImageData();
		//Clear Layer
		m_brush->SetDrawColor(0, 0, 0, 0);
		this->FillBox3D();
		this->Render();
		isDraw = true;
	}
	Superclass::OnRightButtonUp();
}

void InteractorStylePaintBrush::OnRightButtonDown()
{
	isDraw = false;
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	double index[3];
	if (imageViewer->GetInput() != NULL) {
		picked[orientation] = origin[orientation] + imageViewer->GetSlice() * spacing[orientation];
		for (int i = 0; i<3; i++)
		{
			index[i] = (picked[i] - origin[i]) / spacing[i];
		}
	}
	draw_index_old[0] = (int)(index[0] + 0.5);
	draw_index_old[1] = (int)(index[1] + 0.5);
	draw_index_old[2] = (int)(index[2] + 0.5);
	if (lclickDrag == true)
	{
		lclickDrag = false;
	}
	else
	{
		//this->ReadfromImageData();
		this->Render();
	}
	this->Draw(false);
	this->Render();
	rclickDrag = true;
	Superclass::OnRightButtonDown();
}

void InteractorStylePaintBrush::OnMouseMove()
{
	if (lclickDrag == true)
		Draw(true);
	if (rclickDrag == true)
		Draw(false);
	//this->UpdateBorderWidgetPosition();
	this->Render();
	Superclass::OnMouseMove();
}



void InteractorStylePaintBrush::SetPaintBrushModeEnabled(bool b)
{

	//Overlay Image
	//if (!mainWnd->GetVtkOverlay())
	//{

//	}
	// Boarder Widget
	if (m_borderWidget != NULL)
	{
		m_circleRep->Delete();
		m_retangleRep->Delete();
		m_borderWidget->Delete();
		m_borderWidget = NULL;
	}

	if (b)
	{
		m_retangleRep = vtkBorderRepresentation::New();
		m_circleRep = vtkCircleBorderRepresentation::New();
		m_borderWidget = vtkBorderWidget::New();
		m_borderWidget->SetInteractor(this->GetInteractor());
		m_borderWidget->SetCurrentRenderer(imageViewer->GetRenderer());
		m_borderWidget->SetRepresentation(m_retangleRep);
		m_borderWidget->SetManagesCursor(true);
		m_borderWidget->GetBorderRepresentation()->SetMoving(false);
		m_borderWidget->GetBorderRepresentation()->SetProportionalResize(true);
		m_borderWidget->SelectableOff();
		m_borderWidget->ProcessEventsOff();
		m_borderWidget->Off();
	}

	//mainWnd->GetViewers(m_orientation)->GetAnnotationRenderer()->ResetCameraClippingRange();
	this->GetInteractor()->Render();

	if (m_brush)
	{
		Renderer2->RemoveActor(BrushActor);
		Renderer2->Delete();
		Renderer2 = NULL;

		m_brush->Delete();
		m_brush = NULL;
		BrushActor->Delete();
		BrushActor = NULL;

	}

	m_brush = vtkImageCanvasSource2D::New();
	m_brush->SetScalarTypeToUnsignedChar();
	m_brush->SetNumberOfScalarComponents(4);
	switch (orientation)
	{
		case 0:
		{
			m_brush->SetExtent(0, 0, extent[2], extent[3], extent[4], extent[5]);
			break;
		}
		case 1:
		{
			m_brush->SetExtent(extent[0], extent[1], 0, 0, extent[4], extent[5]);
			break;
		}
		case 2:
		{
			m_brush->SetExtent(extent[0], extent[1], extent[2], extent[3], 0, 0);
			break;
		}
	}
	m_brush->SetDrawColor(0, 0, 0, 0);
	this->FillBox3D();
	m_brush->Update();
	m_brush->GetOutput()->SetOrigin(origin);
	m_brush->GetOutput()->SetSpacing(spacing);

	BrushActor = vtkImageActor::New();
	BrushActor->SetInputData(m_brush->GetOutput());
	BrushActor->GetProperty()->SetInterpolationTypeToNearest();
	// Set the image actor
	
	switch (this->orientation)
	{
	case MyImageViewer::SLICE_ORIENTATION_XY:
		BrushActor->SetDisplayExtent(
			extent[0], extent[1], extent[2], extent[3], 0, 0);
		break;

	case MyImageViewer::SLICE_ORIENTATION_XZ:
		BrushActor->SetDisplayExtent(
			extent[0], extent[1], 0, 0, extent[4], extent[5]);
		break;

	case MyImageViewer::SLICE_ORIENTATION_YZ:
		BrushActor->SetDisplayExtent(
			0, 0, extent[2], extent[3], extent[4], extent[5]);
		break;
	}
	Renderer2 = vtkRenderer::New();
	Renderer2->SetLayer(1);
	Renderer2->SetActiveCamera(imageViewer->GetRenderer()->GetActiveCamera());
	imageViewer->GetRenderer()->SetLayer(0);
	imageViewer->GetRenderWindow()->SetNumberOfLayers(2);
	imageViewer->GetRenderWindow()->AddRenderer(Renderer2);

	Renderer2->AddActor(BrushActor);
	imageViewer->GetRenderWindow()->Render();

	cout << "setup" << endl;
}



void InteractorStylePaintBrush::Draw(bool b)
{

	if (b)
		m_brush->SetDrawColor(color_r, color_g, color_b, m_opacity);
	else
		m_brush->SetDrawColor(0, 0, 0, 0);

	//Pick
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	double index[3];
	if (imageViewer->GetInput() != NULL) {
		picked[orientation] = origin[orientation] + imageViewer->GetSlice() * spacing[orientation];
		for (int i = 0; i<3; i++)
		{
			index[i] = (picked[i] - origin[i]) / spacing[i];
		}
	}
	int draw_index[3];

	draw_index[0] = (int)(index[0] + 0.5);
	draw_index[1] = (int)(index[1] + 0.5);
	draw_index[2] = (int)(index[2] + 0.5);

	int x2Dindex, y2Dindex;
	switch (orientation)
	{
	case 0:
		x2Dindex = 1;
		y2Dindex = 2;
		break;
	case 1:
		x2Dindex = 0;
		y2Dindex = 2;
		break;
	case 2:
		x2Dindex = 0;
		y2Dindex = 1;
		break;
	}
	//Determine Brush Size and Position
	cout << draw_index_old[0] << " " << draw_index_old[1] << " " << draw_index_old[2] << endl;
	m_brush->DrawSegment3D(draw_index_old[0], draw_index_old[0] + 10,
		draw_index_old[1], draw_index_old[1] + 10, draw_index_old[2], draw_index_old[2] + 10);

	//Circle
	if ("Circle" == "Circle") {
		if ((int)brushSize % 2 == 1)
			DrawCircle(draw_index_old[x2Dindex], draw_index_old[y2Dindex], draw_index[x2Dindex], draw_index[y2Dindex], (int)brushSize / 2);
		else
		{
			DrawCircle(draw_index_old[x2Dindex] - 1, draw_index_old[y2Dindex], draw_index[x2Dindex] - 1, draw_index[y2Dindex], (int)brushSize / 2 - 1);
			DrawCircle(draw_index_old[x2Dindex] - 1, draw_index_old[y2Dindex] - 1, draw_index[x2Dindex] - 1, draw_index[y2Dindex] - 1, (int)brushSize / 2 - 1);
			DrawCircle(draw_index_old[x2Dindex], draw_index_old[y2Dindex] - 1, draw_index[x2Dindex], draw_index[y2Dindex] - 1, (int)brushSize / 2 - 1);
			DrawCircle(draw_index_old[x2Dindex], draw_index_old[y2Dindex], draw_index[x2Dindex], draw_index[y2Dindex], (int)brushSize / 2 - 1);
		}
	}
	//Default:Square
	else
	{
		if ((int)brushSize % 2 == 1)
		{// Odd Number Brush Size
			for (int x = -((int)brushSize - 1) / 2; x < ((int)brushSize - 1) / 2 + 1; x++)
			{
				for (int y = -((int)brushSize - 1) / 2; y < ((int)brushSize - 1) / 2 + 1; y++)
				{
					this->DrawLine3D(draw_index_old[x2Dindex] + x, draw_index_old[y2Dindex] + y, draw_index[x2Dindex]+x, draw_index[y2Dindex]+y);
				}
			}
		}
		else
		{ // Even Number Brush Size
			for (int x = -((int)brushSize) / 2; x < ((int)brushSize) / 2; x++)
			{
				for (int y = -((int)brushSize) / 2; y < ((int)brushSize) / 2; y++)
				{
					this->DrawLine3D(draw_index_old[x2Dindex] + x, draw_index_old[y2Dindex] + y, draw_index[x2Dindex] + x, draw_index[y2Dindex] + y);
				}
			}

		}
	}
	
	// Reset the old draw index
	for (int i = 0; i < 3; i++)
	{
		draw_index_old[i] = draw_index[i];
	}

}

void InteractorStylePaintBrush::FillBox3D()
{
	switch (this->orientation)
	{
	case MyImageViewer::SLICE_ORIENTATION_XY:
		m_brush->FillBox(extent[0], extent[1], extent[2], extent[3]);
		break;
	case MyImageViewer::SLICE_ORIENTATION_XZ:
		for (int x = 0; x < extent[1]; x++)
		{
			m_brush->DrawSegment3D(x, 0, extent[4], x, 0, extent[5]);
		}
		break;
	case MyImageViewer::SLICE_ORIENTATION_YZ:
		for (int y = 0; y < extent[3]; y++)
		{
			m_brush->DrawSegment3D(0,y, extent[4], 0, y, extent[5]);
		}
		break;	
	}
}

void InteractorStylePaintBrush::DrawLine3D(int x0, int y0, int x1, int y1)
{
	int index[6];
	switch (orientation)
	{
	case MyImageViewer::SLICE_ORIENTATION_YZ:
		if (x0 < extent[2] || x0 > extent[3] || y0 < extent[4] || y0 > extent[5] || x1 < extent[2] || x1 > extent[3] || y1 < extent[4] || y1 > extent[5])
			return;
		index[0] = 0;
		index[1] = x0;
		index[2] = y0;
		index[3] = 0;
		index[4] = x1;
		index[5] = y1;
		break;
	case MyImageViewer::SLICE_ORIENTATION_XZ:
		if (x0 < extent[0] || x0 > extent[1] || y0 < extent[4] || y0 > extent[5] || x1 < extent[0] || x1 > extent[1] || y1 < extent[4] || y1 > extent[5])
			return;
		index[0] = x0;
		index[1] = 0;
		index[2] = y0;
		index[3] = x1;
		index[4] = 0;
		index[5] = y1;
		break;
	case MyImageViewer::SLICE_ORIENTATION_XY:
		if (x0 < extent[0] || x0 > extent[1] || y0 < extent[2] || y0 > extent[3] || x1 < extent[0] || x1 > extent[1] || y1 < extent[2] || y1 > extent[3])
			return;
		index[0] = x0;
		index[1] = y0;
		index[2] = 0;
		index[3] = x1;
		index[4] = y1;
		index[5] = 0;
		break;
	}
	m_brush->DrawSegment3D(index[0], index[1], index[2], index[3], index[4], index[5]);
}

void InteractorStylePaintBrush::DrawCircle(int x0, int y0, int x1, int y1, int radius)
{
	int x = 0, y = radius;
	int d = 1 - radius;   
	while (x <= y) {
		for (int i = -x; i <= x; i++)
		{
			this->DrawLine3D(x0 + i, y0 + y, x1 + i, y1 + y);
			this->DrawLine3D(x0 + i, y0 - y, x1 + i, y1 - y);
		}

		for (int j = -y; j <= y; j++)
		{
			this->DrawLine3D(x0 + j, y0 + x, x1 + j, y1 + x);
			this->DrawLine3D(x0 + j, y0 - x, x1 + j, y1 - x);
		}

		if (d<0)
			d += (2 * x) + 3;
		else {
			d += (2 * (x - y)) + 5;
			y -= 1;
		}
		x++;
	}

	/*			//MidPoint Circle algorithm //Th shape of the circle is a little bit wierd
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		for (int i = -x; i <= x; i++)
		{
			this->DrawLine3D(x0 + i, y0 + y, x1 + i, y1 + y);
			this->DrawLine3D(x0 + i, y0 - y, x1 + i, y1 - y);
		}

		for (int j = -y; j <= y; j++)
		{
			this->DrawLine3D(x0 + j, y0 + x, x1 + j, y1 + x);
			this->DrawLine3D(x0 + j, y0 - x, x1 + j, y1 - x);
		}

		y += 1;
		err += 1 + 2 * y;
		if (2 * (err - x) + 1 > 0)
		{
			x -= 1;
			err += 1 - 2 * x;
		}
	}
	*/ 
}

//void InteractorStylePaintBrush::UpdateBorderWidgetPosition()
//{
//	if (m_borderWidget == NULL)
//		return;
//
//	double size = brushSize ;
//	double color[3] = { (double)color_r / 255.0,(double)color_g / 255.0,(double)color_b / 255.0 };
//	
//	//Restrict the box applying on whole pixel
//	this->GetInteractor()->GetPicker()->Pick(
//		this->GetInteractor()->GetEventPosition()[0],
//		this->GetInteractor()->GetEventPosition()[1],
//		0,  // always zero.
//		imageViewer->GetRenderer());
//
//	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();
//
//	//Check if valid pick
//	if (picked[0] == 0.0&&picked[1] == 0.0)
//		return;
//	double index[3];
//	if (imageViewer->GetInput() != NULL) {
//		picked[orientation] = origin[orientation] + imageViewer->GetSlice() * spacing[orientation];
//		for (int i = 0; i<3; i++)
//		{
//			index[i] = (picked[i] - origin[i]) / spacing[i];
//		}
//	}
//
//	int x = this->GetInteractor()->GetEventPosition()[0];
//	int y = this->GetInteractor()->GetEventPosition()[1];
//	//int x = (int)(index[0] + 0.5)*spacing[0] + origin[0];
//	//int y = (int)(index[1] + 0.5)*spacing[1] + origin[1];
//	
//
//	double X1 = x;
//	double Y1 = y;
//	double display_origin[3];
//	double display_size[3];
//
//	imageViewer->GetRenderer()->SetWorldPoint(0, 0, 0, 1);
//	imageViewer->GetRenderer()->WorldToDisplay();
//	imageViewer->GetRenderer()->GetDisplayPoint(display_origin);
//	//cout << "World->Display vtkRenderer 00: " << "(" << x << "," << y << ")" << " --> " << "(" << result2[0] << "," << result2[1] << ")\n";
//
//	imageViewer->GetRenderer()->SetWorldPoint(size*spacing[0], size*spacing[1], size*spacing[2], 1);
//	imageViewer->GetRenderer()->WorldToDisplay();
//	imageViewer->GetRenderer()->GetDisplayPoint(display_size);
//	//cout << "World->Display vtkRenderer: "	<< "(" << x << "," << y << ")" << " --> " << "(" << result[0] << "," << result[1] << ")\n";
//
//	double X3 = display_size[0] - display_origin[0];
//	double Y3 = display_size[1] - display_origin[1];
//
//	imageViewer->GetRenderer()->DisplayToNormalizedDisplay(X1, Y1);
//	imageViewer->GetRenderer()->NormalizedDisplayToViewport(X1, Y1);
//	imageViewer->GetRenderer()->ViewportToNormalizedViewport(X1, Y1);
//
//	imageViewer->GetRenderer()->DisplayToNormalizedDisplay(X3, Y3);
//	imageViewer->GetRenderer()->NormalizedDisplayToViewport(X3, Y3);
//	imageViewer->GetRenderer()->ViewportToNormalizedViewport(X3, Y3);
//
//	//if (m_drawBrushShapeComboBox->currentText() == "Square") //Square
//	//{
//		m_borderWidget->SetRepresentation(m_retangleRep);
//		X1 = X1 - X3 / 2.0;
//		Y1 = Y1 - Y3 / 2.0;
////	}
//
//	if (m_drawBrushShapeComboBox->currentText() == "Circle") //Circle
//	{
//		m_borderWidget->SetRepresentation(m_circleRep);
//		X1 = X1;
//		Y1 = Y1;
//	}
//
//	vtkCoordinate* pos1 = m_borderWidget->GetBorderRepresentation()->GetPositionCoordinate();
//	pos1->SetValue(X1, Y1);
//
//	vtkCoordinate* pos2 = m_borderWidget->GetBorderRepresentation()->GetPosition2Coordinate();
//	pos2->SetValue(X3, Y3);
//	m_borderWidget->GetBorderRepresentation()->GetBorderProperty()->SetColor(color);
//	m_borderWidget->GetBorderRepresentation()->GetBorderProperty()->SetLineStipplePattern(0xf0f0);
//	m_borderWidget->GetBorderRepresentation()->Modified();
//	m_borderWidget->Modified();
//	m_borderWidget->On();
//
//	//double rsize[2];
//	//m_borderWidget->GetBorderRepresentation()->GetSize(rsize);
//	//qDebug() << "Size: " << size;
//	//qDebug() << "rSize: " << rsize[0] <<", "<<rsize[1];
//	//qDebug() << "Event: " << x << y;
//	//qDebug() << "Pos3:" << X3 << Y3;
//	//qDebug() << "rPos: " << QString::number(m_borderWidget->GetBorderRepresentation()->GetPosition()[0]) << QString::number(m_borderWidget->GetBorderRepresentation()->GetPosition()[1]);
//	//qDebug() << "rPos2: " << QString::number(m_borderWidget->GetBorderRepresentation()->GetPosition2()[0]) << QString::number(m_borderWidget->GetBorderRepresentation()->GetPosition2()[1]);
//
//}
//
//void InteractorStylePaintBrush::ReadfromImageData()
//{
//
//	//Clear Layer
//	m_brush->SetDrawColor(0, 0, 0, 0);
//	this->FillBox3D();
//	//m_brush->Update();
//	m_brush->SetDrawColor(color_r, color_g, color_b, m_opacity);
//
//	int pos[3];
//	switch (this->orientation)
//	{
//	case MyImageViewer::SLICE_ORIENTATION_YZ:
//		for (int y = extent[2]; y < extent[3]; y++)
//		{
//			for (int z = extent[4]; z < extent[5]; z++)
//			{
//				pos[0] = imageViewer->GetSlice();
//				pos[1] = y;
//				pos[2] = z;
//					m_brush->DrawSegment3D(0, y, z, 0, y, z);
//			}
//		}
//		break;
//	case MyImageViewer::SLICE_ORIENTATION_XZ:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			for (int z = extent[4]; z < extent[5]; z++)
//			{
//				pos[0] = x;
//				pos[1] = imageViewer[m_orientation]->GetSlice();
//				pos[2] = z;
//				double* val = static_cast<double *>(mainWnd->GetOverlay()->GetOutput()->GetScalarPointer(pos));
//				if (val[0] > 0)
//					m_brush->DrawSegment3D(x, 0, z, x, 0, z);
//			}
//		}
//		break;
//	case MyViewer::SLICE_ORIENTATION_XY:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			for (int y = extent[2]; y < extent[3]; y++)
//			{
//				pos[0] = x;
//				pos[1] = y;
//				pos[2] = imageViewer[m_orientation]->GetSlice();
//				double * val = static_cast<double *>(mainWnd->GetOverlay()->GetOutput()->GetScalarPointer(pos));
//				if (val[0] > 0)
//					m_brush->DrawPoint(x, y);
//			}
//		}
//		break;
//	}
//
//}
//
//void InteractorStylePaintBrush::Write2ImageData()
//{
//	//Update Layer
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	int pos[3];
//	double pixelval;
//	switch (this->m_orientation)
//	{
//	case MyViewer::SLICE_ORIENTATION_YZ:
//		for (int y = extent[2]; y < extent[3]; y++)
//		{
//			for (int z = extent[4]; z < extent[5]; z++)
//			{
//				pos[0] = imageViewer[m_orientation]->GetSlice();
//				pos[1] = y;
//				pos[2] = z;
//				uchar* val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(0, y, z));
//				if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0)
//					pixelval = 1;
//				else
//					pixelval = 0;
//				mainWnd->GetOverlay()->SetPixel(pos, pixelval);
//			}
//		}
//		break;
//	case MyViewer::SLICE_ORIENTATION_XZ:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			for (int z = extent[4]; z < extent[5]; z++)
//			{
//				pos[0] = x;
//				pos[1] = imageViewer[m_orientation]->GetSlice();
//				pos[2] = z;
//				uchar* val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(x, 0, z));
//				if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0)
//					pixelval = 1;
//				else
//					pixelval = 0;
//				mainWnd->GetOverlay()->SetPixel(pos, pixelval);
//			}
//		}
//		break;
//	case MyViewer::SLICE_ORIENTATION_XY:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			for (int y = extent[2]; y < extent[3]; y++)
//			{
//				pos[0] = x;
//				pos[1] = y;
//				pos[2] = imageViewer[m_orientation]->GetSlice();
//				uchar * val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(x, y, 0));
//				if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0)
//					pixelval = 1;
//				else
//					pixelval = 0;
//				mainWnd->GetOverlay()->SetPixel(pos, pixelval);
//
//			}
//		}
//		break;
//	}
//	mainWnd->GetOverlay()->GetOutput()->Modified();
//
//}
//

void InteractorStylePaintBrush::Render()
{
	m_brush->Update();
	m_brush->GetOutput()->SetOrigin(origin);
	m_brush->GetOutput()->SetSpacing(spacing);
	imageViewer->Render();
}



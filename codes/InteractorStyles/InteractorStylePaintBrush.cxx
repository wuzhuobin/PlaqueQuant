/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			Junior Research Assistant

Author:		Lok, Ka Hei Jason
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

This class allows interactive segmentation on images.

Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#include "InteractorStylePaintBrush.h"

#include <vtkTransformPolyDataFilter.h>
#include <vtkImageProperty.h>
#include <vtkProperty2D.h>
#include <vtkAbstractPicker.h>


vtkStandardNewMacro(InteractorStylePaintBrush);

#define SEGMENTATION_CIRCLE 1
#define SEGMENTATION_SQUARE 0

InteractorStylePaintBrush::InteractorStylePaintBrush()
	:AbstractInteractorStyleImage()
{


	m_borderWidget = NULL;
	m_retangleRep = NULL;
	m_brush = NULL;
	m_brushActor = NULL;
	//Default color: Red
	m_colorRed = 255;
	m_colorGreen = 0;
	m_colorBule = 0;
	m_opacity = 255;
	m_brushShape = SQUARE;
	m_brushSize = 10;
	
	this->m_paintBrushEnabled = false;
}

InteractorStylePaintBrush::~InteractorStylePaintBrush()
{
	//this->SetPaintBrushModeEnabled(false);

	// Clean again if the upper dosen't clean
	if (m_brushActor != NULL && m_imageViewer != NULL) {
		//this->m_imageViewer->GetAnnotationRenderer()->RemoveActor(m_brushActor);
		this->m_brushActor->Delete();
		this->m_brushActor = NULL;
		this->m_brush->Delete();
		this->m_brush = NULL;
	}

	if (m_borderWidget != NULL) {
		this->m_borderWidget->Delete();
		this->m_retangleRep->Delete();
	}
}


void InteractorStylePaintBrush::SetDrawColor(int r, int g, int b)
{
	m_colorRed = r;
	m_colorGreen = g;
	m_colorBule = b;
}
void InteractorStylePaintBrush::SetDrawColor(const int* rgb)
{
	SetDrawColor(rgb[0], rgb[1], rgb[2]);
}
void InteractorStylePaintBrush::SetDrawOpacity(int opacity)
{
	this->m_opacity = opacity;
}

void InteractorStylePaintBrush::SetBrushShape(BRUSH_SHAPE brushShape)
{
	m_brushShape = brushShape;
}

void InteractorStylePaintBrush::SetBrushSize(int size)
{
	m_brushSize = size;
}

//void InteractorStylePaintBrush::SetDrawBrushSizeSpinBox(QSpinBox * s)
//{
//	m_drawBrushSizeSpinBox = s;
//}
//
//void InteractorStylePaintBrush::SetDrawBrushShapeComBox(QComboBox * comboBox)
//{
//	m_drawBrushShapeComboBox = comboBox;
//}
//
//void InteractorStylePaintBrush::SetDrawVolumetricCheckBox(QCheckBox * checkBox)
//{
//	m_drawVolumetricCheckBox = checkBox;
//}
//
//void InteractorStylePaintBrush::SetDrawIsotropicCheckBox(QCheckBox * checkBox)
//{
//	m_drawIsotropicCheckBox = checkBox;
//}

void InteractorStylePaintBrush::OnLeftButtonUp()
{
	if (!m_isDraw)
	{
		this->Write2ImageData();
		m_imageViewer->GetdrawActor()->SetVisibility(true);
		//Clear Layer
		m_brush->SetDrawColor(0, 0, 0, 0);
		this->FillBox3D();
		this->Render();
		m_isDraw = true;
	}
	m_leftFunctioning = false;

}

void InteractorStylePaintBrush::OnLeftButtonDown()
{
	m_leftFunctioning = true;
	m_isDraw = false;
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		m_imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	// Check if valid pick
	//if (!this->CheckValidPick(picked))
	//	return;

	// Project picked world point to plane and obtain ijk index
	double index[3];
	if (m_imageViewer->GetInput() != NULL) {
		picked[m_orientation] = m_origin[m_orientation] + GetSlice() * m_spacing[m_orientation];
		for (int i = 0; i < 3; i++)
		{
			index[i] = (picked[i] - m_origin[i]) / m_spacing[i];
		}
	}
	draw_index_old[0] = (int)(index[0] + 0.5);
	draw_index_old[1] = (int)(index[1] + 0.5);
	draw_index_old[2] = (int)(index[2] + 0.5);
	// 
	if (m_rightFunctioning == true)
	{
		m_rightFunctioning = false;
	}
	else
	{
		this->ReadfromImageData();
		this->Render();
	}
	m_imageViewer->GetdrawActor()->SetVisibility(false);
	Draw(true);
	this->Render();
}

void InteractorStylePaintBrush::OnRightButtonUp()
{
	m_rightFunctioning = false;
	if (!m_isDraw)
	{
		this->Write2ImageData();
		m_imageViewer->GetdrawActor()->SetVisibility(true);
		//Clear Layer
		m_brush->SetDrawColor(0, 0, 0, 0);
		this->FillBox3D();
		this->Render();
		m_isDraw = true;
	}
	AbstractInteractorStyleImage::OnRightButtonUp();
}

void InteractorStylePaintBrush::OnRightButtonDown()
{

	m_isDraw = false;

	// return if label image is not found

	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		m_imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();


	//// Check if valid pick
	//if (!this->CheckValidPick(picked))
	//	return;
	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;

	// Project picked world point to plane and obtain ijk index
	double index[3];
	if (m_imageViewer->GetInput() != NULL) {
		picked[m_orientation] = m_origin[m_orientation] + GetSlice() * m_spacing[m_orientation];
		for (int i = 0; i < 3; i++)
		{
			index[i] = (picked[i] - m_origin[i]) / m_spacing[i];
		}
	}
	draw_index_old[0] = (int)(index[0] + 0.5);
	draw_index_old[1] = (int)(index[1] + 0.5);
	draw_index_old[2] = (int)(index[2] + 0.5);

	if (m_leftFunctioning == true)
	{
		m_leftFunctioning = false;
	}
	else
	{
		this->ReadfromImageData();
		this->Render();
	}

	m_imageViewer->GetdrawActor()->SetVisibility(false);
	this->Draw(false);
	this->Render();
	m_rightFunctioning = true;
	AbstractInteractorStyleImage::OnRightButtonDown();
}

void InteractorStylePaintBrush::OnLeave()
{
	if (this->m_paintBrushEnabled) {
		this->m_borderWidget->Off();
	}

	AbstractInteractorStyleImage::OnLeave();
}

void InteractorStylePaintBrush::OnMouseMove()
{
	if (!this->m_paintBrushEnabled)
		this->SetPaintBrushModeEnabled(true);

	if (m_leftFunctioning == true)
		Draw(true);
	if (m_rightFunctioning == true)
		Draw(false);

	this->UpdateBorderWidgetPosition();
	this->Render();

	if (!m_rightFunctioning && !m_leftFunctioning)
		AbstractInteractorStyleImage::OnMouseMove();
}



void InteractorStylePaintBrush::SetPaintBrushModeEnabled(bool b)
{
	/// Delete stuff if the mode is disabled to save memory
	if (m_borderWidget != NULL)
	{
		m_retangleRep->Delete();
		m_borderWidget->Delete();
		m_borderWidget = NULL;
	}


	if (m_brush != NULL)
	{
		if (m_brushActor != NULL && m_imageViewer != NULL) {
			this->m_imageViewer->GetAnnotationRenderer()->RemoveActor(m_brushActor);
		}

		m_brush->Delete();
		m_brush = NULL;

		m_brushActor->Delete();
		m_brushActor = NULL;

	}

	if (b)
	{
		m_retangleRep = vtkBorderRepresentation::New();
		m_borderWidget = vtkBorderWidget::New();
		m_borderWidget->SetInteractor(this->GetInteractor());
		m_borderWidget->SetCurrentRenderer(m_imageViewer->GetAnnotationRenderer());
		m_borderWidget->SetRepresentation(m_retangleRep);
		m_borderWidget->SetManagesCursor(false);
		m_borderWidget->GetBorderRepresentation()->SetMoving(false);
		m_borderWidget->GetBorderRepresentation()->SetProportionalResize(true);
		m_borderWidget->SelectableOff();
		m_borderWidget->ProcessEventsOff();
		m_borderWidget->Off();

		// Delete m_overlay image


		if (this->Interactor)
			this->Interactor->Render();

		/// Set up brush boarder widget
		m_brush = vtkImageCanvasSource2D::New();
		m_brush->SetScalarTypeToUnsignedChar();
		m_brush->SetNumberOfScalarComponents(4);
		switch (m_orientation)
		{
		case 0:
		{
			m_brush->SetExtent(0, 0, m_extent[2], m_extent[3], m_extent[4], m_extent[5]);
			break;
		}
		case 1:
		{
			m_brush->SetExtent(m_extent[0], m_extent[1], 0, 0, m_extent[4], m_extent[5]);
			break;
		}
		case 2:
		{
			m_brush->SetExtent(m_extent[0], m_extent[1], m_extent[2], m_extent[3], 0, 0);
			break;
		}
		}
		
		m_brush->SetDrawColor(0, 0, 0, 0);
		this->FillBox3D();
		m_brush->Update();
		m_brush->GetOutput()->SetOrigin(m_origin);
		m_brush->GetOutput()->SetSpacing(m_spacing);

		m_brushActor = vtkImageActor::New();
		m_brushActor->SetInputData(m_brush->GetOutput());
		m_brushActor->GetProperty()->SetInterpolationTypeToNearest();
		m_brushActor->SetPickable(false);
		// Set the image actor

		switch (this->m_orientation)
		{
		case 0:
			m_brushActor->SetDisplayExtent(
				0, 0, m_extent[2], m_extent[3], m_extent[4], m_extent[5]);
			break;

		case 1:
			m_brushActor->SetDisplayExtent(
				m_extent[0], m_extent[1], 0, 0, m_extent[4], m_extent[5]);
			break;

		case 2:
			m_brushActor->SetDisplayExtent(
				m_extent[0], m_extent[1], m_extent[2], m_extent[3], 0, 0);
			break;
		}
		m_imageViewer->GetAnnotationRenderer()->AddActor(m_brushActor);
		m_imageViewer->GetAnnotationRenderer()->Render();
	}

	this->m_paintBrushEnabled = b;
}



void InteractorStylePaintBrush::Draw(bool b)
{
	if (b) {
		m_brush->SetDrawColor(m_colorRed, m_colorGreen, m_colorBule, m_opacity);
	}
	else
		m_brush->SetDrawColor(0, 0, 0, 0);

	//Pick
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		m_imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	////Check if valid pick
	//if (!this->CheckValidPick(picked))
	//	return;
	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	double index[3];
	if (m_imageViewer->GetInput() != NULL) {
		picked[m_orientation] = m_origin[m_orientation] + GetSlice() * m_spacing[m_orientation];
		for (int i = 0; i < 3; i++)
		{
			index[i] = (picked[i] - m_origin[i]) / m_spacing[i];
		}
	}
	int draw_index[3];

	draw_index[0] = (int)(index[0] + 0.5);
	draw_index[1] = (int)(index[1] + 0.5);
	draw_index[2] = (int)(index[2] + 0.5);

	int x2Dindex, y2Dindex;
	switch (m_orientation)
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

	//Circle
	if (m_brushShape == CIRCLE) {
		// if size is odd
		DrawCircle(draw_index_old[x2Dindex], draw_index_old[y2Dindex], draw_index[x2Dindex],
			draw_index[y2Dindex], m_brushSize / 2.);

	}
	//Default:Square
	else
	{
		int loopLowerBoundX = ceil(-m_brushSize / m_spacing[x2Dindex] / 2.);
		int loopUpperBoundX = ceil(m_brushSize / m_spacing[x2Dindex] / 2.);
		int loopLowerBoundY = ceil(-m_brushSize / m_spacing[y2Dindex] / 2.);
		int loopUpperBoundY = ceil(m_brushSize / m_spacing[y2Dindex] / 2.);

		for (int x = loopLowerBoundX; x < loopUpperBoundX; x++)
		{
			for (int y = loopLowerBoundY; y < loopUpperBoundY; y++)
			{
				this->DrawLine3D(draw_index_old[x2Dindex] + x, draw_index_old[y2Dindex] + y, draw_index[x2Dindex]+x, draw_index[y2Dindex]+y);
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
	switch (this->m_orientation)
	{
	case 2:
		m_brush->FillBox(m_extent[0], m_extent[1], m_extent[2], m_extent[3]);
		break;
	case 1:
		for (int x = m_extent[0]; x < m_extent[1]; x++)
		{
			m_brush->DrawSegment3D(x, 0, m_extent[4], x, 0, m_extent[5]);
		}
		break;
	case 0:
		for (int y = m_extent[2]; y < m_extent[3]; y++)
		{
			m_brush->DrawSegment3D(0,y, m_extent[4], 0, y, m_extent[5]);
		}
		break;	
	}
}

void InteractorStylePaintBrush::DrawLine3D(int x0, int y0, int x1, int y1)
{
	// Check for Extent
	int brushExtent[6];
	this->m_brush->GetUpdateExtent(brushExtent);

	int index[6];
	switch (m_orientation)
	{
	case 0:
		if (x0 < m_extent[2] || x0 > m_extent[3] || y0 < m_extent[4] || y0 > m_extent[5] || x1 < m_extent[2] || x1 > m_extent[3] || y1 < m_extent[4] || y1 > m_extent[5])
			return;
		index[0] = 0;
		index[1] = x0;
		index[2] = y0;
		index[3] = 0;
		index[4] = x1;
		index[5] = y1;
		break;
	case 1:
		if (x0 < m_extent[0] || x0 > m_extent[1] || y0 < m_extent[4] || y0 > m_extent[5] || x1 < m_extent[0] || x1 > m_extent[1] || y1 < m_extent[4] || y1 > m_extent[5])
			return;
		index[0] = x0;
		index[1] = 0;
		index[2] = y0;
		index[3] = x1;
		index[4] = 0;
		index[5] = y1;
		break;
	case 2:
		if (x0 < m_extent[0] || x0 > m_extent[1] || y0 < m_extent[2] || y0 > m_extent[3] || x1 < m_extent[0] || x1 > m_extent[1] || y1 < m_extent[2] || y1 > m_extent[3])
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

// Draw lines from (x0, y0) -> (y0, y1) with base as circle
void InteractorStylePaintBrush::DrawCircle(int x0, int y0, int x1, int y1, double radius)
{
	int x2Dindex, y2Dindex;
	switch (m_orientation)
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

	int loopLowerBoundX = floor(-radius / (this->m_spacing[x2Dindex]));
	int loopUpperBoundX = ceil(radius / (this->m_spacing[x2Dindex]));
	int loopLowerBoundY = floor(-radius / (this->m_spacing[y2Dindex]));
	int loopUpperBoundY = ceil(radius / (this->m_spacing[y2Dindex]));


	// check bound
	if (loopLowerBoundX >= loopUpperBoundX || loopLowerBoundY >= loopUpperBoundY) {
		std::cerr << "Bound calculation failed.";
		return;
	}

	for (int i = loopLowerBoundX; i < loopUpperBoundX; i++)
	{
		for (int j = loopLowerBoundY; j < loopUpperBoundY; j++)
		{
			double d = pow(i * m_spacing[x2Dindex], 2) + pow(j * m_spacing[y2Dindex], 2.);
			if (sqrt(d) < radius)
				this->DrawLine3D(x0 + i, y0 + j, x1 + i, y1 + j);
		}
	}
}

void InteractorStylePaintBrush::UpdateBorderWidgetPosition()
{
	if (m_borderWidget == NULL)
		return;

	// Update Brush propertyies
	double size = m_brushSize;
	double color[3] = { (double)m_colorRed / 255.0,(double)m_colorGreen / 255.0,(double)m_colorBule / 255.0 };
	
	//Restrict the box applying on whole pixel 
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		m_imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	////Check if valid pick
	//if (!this->CheckValidPick(picked)) {
	//	this->m_borderWidget->Off();
	//	return;
	//}

	int x = this->GetInteractor()->GetEventPosition()[0];
	int y = this->GetInteractor()->GetEventPosition()[1];
	//int x = (int)(index[0] + 0.5)*m_spacing[0] + m_origin[0];
	//int y = (int)(index[1] + 0.5)*m_spacing[1] + m_origin[1];


	double X1 = x;
	double Y1 = y;
	double display_origin[3];
	double display_size[3];

	m_imageViewer->GetRenderer()->SetWorldPoint(0, 0, 0, 1);
	m_imageViewer->GetRenderer()->WorldToDisplay();
	m_imageViewer->GetRenderer()->GetDisplayPoint(display_origin);
	//cout << "World->Display vtkRenderer 00: " << "(" << x << "," << y << ")" << " --> " << "(" << result2[0] << "," << result2[1] << ")\n";

	m_imageViewer->GetRenderer()->SetWorldPoint(size, size, size, 1);
	m_imageViewer->GetRenderer()->WorldToDisplay();
	m_imageViewer->GetRenderer()->GetDisplayPoint(display_size);
	//cout << "World->Display vtkRenderer: "	<< "(" << x << "," << y << ")" << " --> " << "(" << result[0] << "," << result[1] << ")\n";

	double X3 = display_size[0] - display_origin[0];
	double Y3 = display_size[1] - display_origin[1];

	m_imageViewer->GetRenderer()->DisplayToNormalizedDisplay(X1, Y1);
	m_imageViewer->GetRenderer()->NormalizedDisplayToViewport(X1, Y1);
	m_imageViewer->GetRenderer()->ViewportToNormalizedViewport(X1, Y1);

	m_imageViewer->GetRenderer()->DisplayToNormalizedDisplay(X3, Y3);
	m_imageViewer->GetRenderer()->NormalizedDisplayToViewport(X3, Y3);
	m_imageViewer->GetRenderer()->ViewportToNormalizedViewport(X3, Y3);

	//if (m_drawBrushShapeComboBox->currentText() == "Square") //Square
	//{
	m_borderWidget->SetRepresentation(m_retangleRep);
	X1 = X1 - X3 / 2.0;
	Y1 = Y1 - Y3 / 2.0;
	m_retangleRep->SetPosition(X1, Y1);
	m_retangleRep->SetPosition2(X3, Y3);
	//	}

	vtkCoordinate* pos1 = m_borderWidget->GetBorderRepresentation()->GetPositionCoordinate();
	pos1->SetValue(X1, Y1);

	vtkCoordinate* pos2 = m_borderWidget->GetBorderRepresentation()->GetPosition2Coordinate();
	pos2->SetValue(X3, Y3);
	m_borderWidget->GetBorderRepresentation()->GetBorderProperty()->SetColor(color);
	m_borderWidget->GetBorderRepresentation()->GetBorderProperty()->SetLineStipplePattern(0xAAAA);
	m_borderWidget->GetBorderRepresentation()->Modified();
	m_borderWidget->Modified();
	m_borderWidget->On();
}

void InteractorStylePaintBrush::ReadfromImageData()
{
	switch (this->m_orientation)
	{
	case 0:
		m_brush->SetExtent(0, 0, m_extent[2], m_extent[3], m_extent[4], m_extent[5]);
		m_brush->Update();
		m_brushActor->SetDisplayExtent(0, 0, m_extent[2], m_extent[3], m_extent[4], m_extent[5]);
		m_brushActor->Update();
		break;
	case 1:
		m_brush->SetExtent(m_extent[0], m_extent[1], 0, 0, m_extent[4], m_extent[5]);
		m_brush->Update();
		m_brushActor->SetDisplayExtent(m_extent[0], m_extent[1], 0, 0, m_extent[4], m_extent[5]);
		m_brushActor->Update();
		break;

	case 2:
		m_brush->SetExtent(m_extent[0], m_extent[1], m_extent[2], m_extent[3], 0, 0);
		m_brush->Update();
		m_brushActor->SetDisplayExtent(m_extent[0], m_extent[1], m_extent[2], m_extent[3], 0, 0);
		m_brushActor->Update();
		break;
	}

	//Clear Layer
	m_brush->SetDrawColor(0, 0, 0, 0);
	this->FillBox3D();

	int pos[3], extent[6];
	memcpy(extent, m_extent, sizeof(extent));
	extent[m_orientation * 2] = GetSlice();
	extent[m_orientation * 2 + 1] = GetSlice();
	for (int x = extent[0]; x <= extent[1]; ++x) {
		for (int y = extent[2]; y <= extent[3]; ++y) {
			for (int z = extent[4]; z <= extent[5]; ++z) {
				pos[0] = x;
				pos[1] = y;
				pos[2] = z;
				double* val = static_cast<double *>(m_imageViewer->GetInputLayer()->GetScalarPointer(pos));
				if (val == nullptr)
					continue;
				if (*val > 0) {
					for (int i = 0; i < m_imageViewer->getLookupTable()->GetNumberOfColors(); ++i) {
						if (*val == i) {
							double rgba[4];
							uchar rgbaUCHAR[4];
							m_imageViewer->getLookupTable()->GetIndexedColor(i, rgba);
							m_imageViewer->getLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar
							try {
								m_brush->SetDrawColor(rgbaUCHAR[0], rgbaUCHAR[1], rgbaUCHAR[2], rgbaUCHAR[3]);
								pos[m_orientation] = 0;
								m_brush->DrawSegment3D(pos[0], pos[1], pos[2], 
									pos[0], pos[1], pos[2]);
							}
							catch (...) {
								break;
							}
							break;
						}
					}
				}
			}
		}
	}
	
	//switch (m_orientation)
	//{
	//case 0:
	//	for (int y = m_extent[2]; y < m_extent[3]; y++)
	//	{
	//		for (int z = m_extent[4]; z < m_extent[5]; z++)
	//		{
	//			pos[0] = m_sliceSplinBox[m_orientation]->value();
	//			pos[1] = y;
	//			pos[2] = z;
	//			double* val = static_cast<double *>(m_imageViewer->GetInputLayer()->GetScalarPointer(pos));
	//			if (val == nullptr)
	//				continue;
	//			if (*val > 0) {
	//				for (int i = 0; i < m_imageViewer->getLookupTable()->GetNumberOfColors(); ++i) {
	//					if (*val == i) {
	//						double rgba[4];
	//						uchar rgbaUCHAR[4];
	//						m_imageViewer->getLookupTable()->GetIndexedColor(i, rgba);
	//						m_imageViewer->getLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar
	//						try {
	//							m_brush->SetDrawColor(rgbaUCHAR[0], rgbaUCHAR[1], rgbaUCHAR[2], rgbaUCHAR[3]);
	//							m_brush->DrawSegment3D(0, y, z, 0, y, z);
	//						}
	//						catch (...) {
	//							break;
	//						}
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//	break;
	//case 1:
	//	for (int x = m_extent[0]; x < m_extent[1]; x++)
	//	{
	//		for (int z = m_extent[4]; z < m_extent[5]; z++)
	//		{
	//			pos[0] = x;
	//			pos[1] = m_sliceSplinBox[m_orientation]->value();
	//			pos[2] = z;
	//			double* val = static_cast<double *>(m_imageViewer->GetInputLayer()->GetScalarPointer(pos));
	//			if (val == nullptr)
	//				continue;
	//			if (*val > 0) {
	//				for (int i = 0; i < m_imageViewer->getLookupTable()->GetNumberOfColors(); ++i) {
	//					if (*val == i) {
	//						double rgba[4];
	//						uchar rgbaUCHAR[4];
	//						m_imageViewer->getLookupTable()->GetIndexedColor(i, rgba);
	//						m_imageViewer->getLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar
	//						try {
	//							m_brush->SetDrawColor(rgbaUCHAR[0], rgbaUCHAR[1], rgbaUCHAR[2], rgbaUCHAR[3]);
	//							//m_brush->SetDrawColor(255, 0, 0, 1);
	//							m_brush->DrawSegment3D(x, 0, z, x, 0, z);
	//						}
	//						catch (...) {
	//							break;
	//						}
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//	break;
	//case 2:
	//	for (int x = m_extent[0]; x < m_extent[1]; x++)
	//	{
	//		for (int y = m_extent[2]; y < m_extent[3]; y++)
	//		{
	//			pos[0] = x;
	//			pos[1] = y;
	//			pos[2] = m_sliceSplinBox[m_orientation]->value();
	//			double* val = static_cast<double *>(m_imageViewer->GetInputLayer()->GetScalarPointer(pos));
	//			if (val == nullptr)
	//				continue;
	//			if (*val > 0) {
	//				for (int i = 0; i < m_imageViewer->getLookupTable()->GetNumberOfColors(); ++i) {
	//					if (*val == i) {
	//						double rgba[4];
	//						uchar rgbaUCHAR[4];
	//						m_imageViewer->getLookupTable()->GetIndexedColor(i, rgba);
	//						m_imageViewer->getLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar
	//						m_brush->SetDrawColor(rgbaUCHAR[0], rgbaUCHAR[1], rgbaUCHAR[2], rgbaUCHAR[3]);
	//						m_brush->DrawPoint(x, y);
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//	break;
	//}

}

void InteractorStylePaintBrush::Write2ImageData()
{
	//Update Layer
	int pos[3], extent[6];
	memcpy(extent, m_extent, sizeof(extent));
	extent[m_orientation * 2] = GetSlice();
	extent[m_orientation * 2 + 1] = GetSlice();
	double pixelval;

	for (int x = extent[0]; x <= extent[1]; ++x) {
		for (int y = extent[2]; y <= extent[3]; ++y) {
			for (int z = extent[4]; z <= extent[5]; ++z) {
				pos[0] = x;
				pos[1] = y;
				pos[2] = z;
				pos[m_orientation] = 0;
				uchar* val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(pos));
				pixelval = 0;
				if (val == nullptr)
					continue;
				if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0) {
					for (int i = 0; i < m_imageViewer->getLookupTable()->GetNumberOfColors(); i++)
					{
						double rgba[4];
						uchar rgbaUCHAR[4];
						m_imageViewer->getLookupTable()->GetIndexedColor(i, rgba);
						m_imageViewer->getLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar

						if (val[0] == rgbaUCHAR[0] && val[1] == rgbaUCHAR[1] && val[2] == rgbaUCHAR[2]) {
							pixelval = i;
							break;
						}

					}
				}
				pos[0] = x;
				pos[1] = y;
				pos[2] = z;
				m_imageViewer->GetOverlay()->SetPixel(pos, pixelval);
			}
		}
	}

	//switch (m_orientation)
	//{
	//case 0:
	//	;
	//	for (int y = m_extent[2]; y < m_extent[3]; y++)
	//	{
	//		for (int z = m_extent[4]; z < m_extent[5]; z++)
	//		{
	//			pos[0] = m_sliceSplinBox[m_orientation]->value();
	//			pos[1] = y;
	//			pos[2] = z;
	//			uchar* val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(0, y, z));
	//			if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0) {
	//				// Check if color of the lookup table matches the color drawn on canvas
	//				for (int i = 0; i < m_imageViewer->getLookupTable()->GetNumberOfColors(); i++)
	//				{
	//					double rgba[4];
	//					uchar rgbaUCHAR[4];
	//					m_imageViewer->getLookupTable()->GetIndexedColor(i, rgba);
	//					m_imageViewer->getLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar

	//					if (val[0] == rgbaUCHAR[0] && val[1] == rgbaUCHAR[1] && val[2] == rgbaUCHAR[2]) {
	//						pixelval = i;
	//						break;
	//					}
	//				}
	//			}
	//			else {
	//				pixelval = 0;
	//			}
	//			double * pixel = static_cast<double *>(
	//				m_imageViewer->GetInputLayer()->GetScalarPointer(pos));
	//			if (pixel == nullptr) {
	//				return;
	//			}
	//			*pixel = pixelval;
	//		}
	//	}
	//	break;
	//case 1:
	//	for (int x = m_extent[0]; x < m_extent[1]; x++)
	//	{
	//		for (int z = m_extent[4]; z < m_extent[5]; z++)
	//		{
	//			pos[0] = x;
	//			pos[1] = m_sliceSplinBox[m_orientation]->value();
	//			pos[2] = z;
	//			uchar* val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(x, 0, z));
	//			if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0)
	//				for (int i = 0; i < m_imageViewer->getLookupTable()->GetNumberOfColors(); i++)
	//				{
	//					double rgba[4];
	//					uchar rgbaUCHAR[4];
	//					m_imageViewer->getLookupTable()->GetIndexedColor(i, rgba);
	//					m_imageViewer->getLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar

	//					if (val[0] == rgbaUCHAR[0] && val[1] == rgbaUCHAR[1] && val[2] == rgbaUCHAR[2]) {
	//						pixelval = i;
	//						break;
	//					}
	//				}
	//			else {
	//				pixelval = 0;
	//			}
	//			double * pixel = static_cast<double *>(
	//				m_imageViewer->GetInputLayer()->GetScalarPointer(pos));
	//			if (pixel == nullptr) {
	//				return;
	//			}
	//			*pixel = pixelval;
	//		}
	//	}
	//	break;
	//case 2:
	//	for (int x = m_extent[0]; x < m_extent[1]; x++)
	//	{
	//		for (int y = m_extent[2]; y < m_extent[3]; y++)
	//		{
	//			pos[0] = x;
	//			pos[1] = y;
	//			pos[2] = m_sliceSplinBox[m_orientation]->value();
	//			uchar * val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(x, y, 0));
	//			if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0)
	//				for (int i = 0; i < m_imageViewer->getLookupTable()->GetNumberOfColors(); i++)
	//				{
	//					double rgba[4];
	//					uchar rgbaUCHAR[4];
	//					m_imageViewer->getLookupTable()->GetIndexedColor(i, rgba);
	//					m_imageViewer->getLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar

	//					if (val[0] == rgbaUCHAR[0] && val[1] == rgbaUCHAR[1] && val[2] == rgbaUCHAR[2]) {
	//						pixelval = i;
	//						break;
	//					}
	//				}
	//			else {
	//				pixelval = 0;
	//			}
	//			double * pixel = static_cast<double *>(
	//				m_imageViewer->GetInputLayer()->GetScalarPointer(pos));
	//			if (pixel == nullptr) {
	//				return;
	//			}
	//			*pixel = pixelval;

	//		}
	//	}
	//	break;
	//}
	m_imageViewer->GetInputLayer()->Modified();

}


void InteractorStylePaintBrush::Render()
{
	//MainWindow* mainWnd = MainWindow::GetMainWindow();
	//if (m_brush == NULL)
	//	return;

	//mainWnd->RenderAll2DViewers();
	m_imageViewer->Render();
	m_brush->Update();
	m_brush->GetOutput()->SetOrigin(m_origin);
	m_brush->GetOutput()->SetSpacing(m_spacing);

}


/* Return true if pick is valid, false ortherwise */
bool InteractorStylePaintBrush::CheckValidPick(double *picked)
{
	//MainWindow* mainWnd = MainWindow::GetMainWindow();
	//Image* labelMap = mainWnd->GetPlanner()->GetImageByUniqueName("Label");

	//if (!labelMap)
	//	return false;


	///// Check if valid pick on display
	//if (picked[0] == 0.0&&picked[1] == 0.0)
	//	return false;

	///// Check if pick is inside label map
	//int* m_extent = labelMap->GetImageData()->GetExtent();
	//
	//// obtain ijk index of the clicked point
	//int ijk[3];
	//labelMap->GetReslicer()->GetITKIndexFromVTKImageActorPoint(picked, ijk, this->m_orientation);

	//// index has to be within m_extent
	//for (int i = 0; i < 3; i++)
	//{
	//	int l_index = ijk[i];
	//	int l_lowerRange = m_extent[i * 2], l_upperRange = m_extent[i * 2 + 1];
	//
	//	if (l_index < l_lowerRange || l_index > l_upperRange) {
	//		return false;
	//	}
	//}

	return true;
}



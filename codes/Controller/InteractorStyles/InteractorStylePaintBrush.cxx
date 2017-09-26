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
#include <vtkRenderWindowInteractor.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkBorderWidget.h>
#include <vtkBorderRepresentation.h>
#include <vtkObjectFactory.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkCommand.h>

#include "ImageViewer.h"
#include "vtkCircleBorderRepresentation.h"

#define SEGMENTATION_CIRCLE 1
#define SEGMENTATION_SQUARE 0

vtkStandardNewMacro(InteractorStylePaintBrush);

InteractorStylePaintBrush::InteractorStylePaintBrush()
	:AbstractNavigation()
{


	m_borderWidget = nullptr;
	m_retangleRep = nullptr;
	m_brush = nullptr;
	m_brushActor = nullptr;
	m_brushRenderer = nullptr;
	//Default color: Red
	m_colorRed = 0;
	m_colorGreen = 0;
	m_colorBule = 0;
	m_opacity = 255;
	m_brushShape = SQUARE;
	m_brushSize = 1;

	this->m_customFlag = false;
}

InteractorStylePaintBrush::~InteractorStylePaintBrush()
{
	this->SetCustomEnabled(false);
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

void InteractorStylePaintBrush::SetBrushShape(int brushShape)
{
	m_brushShape = brushShape;
	if (m_retangleRep) {
		m_retangleRep->Delete();
	}
	if (m_brushShape == CIRCLE) {
		m_retangleRep = vtkCircleBorderRepresentation::New();
	}
	else {
		m_retangleRep = vtkBorderRepresentation::New();
	}
	m_borderWidget->SetRepresentation(m_retangleRep);
}

void InteractorStylePaintBrush::SetBrushSize(int size)
{
	m_brushSize = size;
}

void InteractorStylePaintBrush::EnableEraserMode(bool flag)
{
	this->m_eraserModeFlag = flag;
}

void InteractorStylePaintBrush::OnLeftButtonUp()
{
	if (!m_isDraw)
	{
		//this->WriteToOverlay();
		this->WriteToImageData();
		GetImageViewer()->GetOverlayActor()->SetVisibility(true);
		//Clear Layer
		m_brush->SetDrawColor(0, 0, 0, 0);
		this->FillBox3D();
		SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStylePaintBrush, Render());
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
		GetImageViewer()->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	// Check if valid pick
	//if (!this->CheckValidPick(picked))
	//	return;

	// Project picked world point to plane and obtain ijk index
	if (GetImageViewer()->GetInput() != NULL) {
		picked[GetSliceOrientation()] = GetOrigin()[GetSliceOrientation()] + GetSlice() * GetSpacing()[GetSliceOrientation()];
		for (int i = 0; i < 3; i++)
		{
			draw_index_old[i] = (picked[i] - GetOrigin()[i]) / GetSpacing()[i] + 0.5;
		}
	}
	if (m_rightFunctioning == true)
	{
		m_rightFunctioning = false;
	}
	else
	{
		this->ReadfromImageData();
		this->Render();
	}
	GetImageViewer()->GetOverlayActor()->SetVisibility(false);
	// if paint is in eraser mode, the left click will also eraser the painting 
	Draw(!m_eraserModeFlag);
	this->Render();
}

void InteractorStylePaintBrush::OnRightButtonUp()
{
	m_rightFunctioning = false;
	if (!m_isDraw)
	{
		//this->WriteToOverlay();
		this->WriteToImageData();
		GetImageViewer()->GetOverlayActor()->SetVisibility(true);
		//Clear Layer
		m_brush->SetDrawColor(0, 0, 0, 0);
		this->FillBox3D();
		this->Render();
		m_isDraw = true;
	}
	AbstractNavigation::OnRightButtonUp();
}

void InteractorStylePaintBrush::OnRightButtonDown()
{

	m_isDraw = false;

	// return if label image is not found

	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		GetImageViewer()->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();


	//// Check if valid pick
	//if (!this->CheckValidPick(picked))
	//	return;
	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;

	// Project picked world point to plane and obtain ijk index
	double index[3];
	if (GetImageViewer()->GetInput() != NULL) {
		picked[GetSliceOrientation()] = GetOrigin()[GetSliceOrientation()] + GetSlice() * GetSpacing()[GetSliceOrientation()];
		for (int i = 0; i < 3; i++)
		{
			index[i] = (picked[i] - GetOrigin()[i]) / GetSpacing()[i];
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

	GetImageViewer()->GetOverlayActor()->SetVisibility(false);
	this->Draw(false);
	this->Render();
	m_rightFunctioning = true;
	AbstractNavigation::OnRightButtonDown();
}

void InteractorStylePaintBrush::OnLeave()
{
	if (this->m_customFlag) {
		this->m_borderWidget->Off();
	}

	AbstractNavigation::OnLeave();
}

void InteractorStylePaintBrush::OnMouseMove()
{
	//if (!this->m_customFlag)
	//	this->SetCustomEnabled(true);

	if (m_leftFunctioning == true) {
		// if paint is in eraser mode, the left click will also eraser the painting 
		Draw(!m_eraserModeFlag);
	}
	if (m_rightFunctioning == true)
		Draw(false);

	this->UpdateBorderWidgetPosition();
	this->Render();

	if (!m_rightFunctioning && !m_leftFunctioning)
		AbstractNavigation::OnMouseMove();
}



void InteractorStylePaintBrush::SetCustomEnabled(bool b)
{
	AbstractNavigation::SetCustomEnabled(b);
	if (b)
	{
		m_borderWidget = vtkBorderWidget::New();
		m_borderWidget->SetInteractor(this->GetInteractor());
		m_borderWidget->SetCurrentRenderer(GetImageViewer()->GetRenderer());
		SetBrushShape(m_brushShape);

		m_borderWidget->SetManagesCursor(false);
		m_borderWidget->GetBorderRepresentation()->SetMoving(false);
		m_borderWidget->GetBorderRepresentation()->SetProportionalResize(true);
		m_borderWidget->SelectableOff();
		m_borderWidget->ProcessEventsOff();
		m_borderWidget->Off();

		// Delete m_overlay image


		//if (this->Interactor)
		//	this->Interactor->Render();

		/// Set up brush boarder widget
		int extent[6];
		memcpy(extent, GetExtent(), sizeof(extent));
		extent[GetSliceOrientation() * 2] = 0;
		extent[GetSliceOrientation() * 2 + 1] = 0;

		m_brush = vtkImageCanvasSource2D::New();
		m_brush->SetScalarTypeToUnsignedChar();
		m_brush->SetNumberOfScalarComponents(4);
		m_brush->SetExtent(extent);
		m_brush->SetDrawColor(0, 0, 0, 0);
		this->FillBox3D();
		m_brush->Update();
		m_brush->GetOutput()->SetOrigin(GetOrigin());
		m_brush->GetOutput()->SetSpacing(GetSpacing());

		m_brushActor = vtkImageActor::New();
		m_brushActor->SetInputData(m_brush->GetOutput());
		m_brushActor->GetProperty()->SetInterpolationTypeToNearest();
		m_brushActor->SetPickable(false);

		m_brushRenderer = vtkRenderer::New();
		m_brushRenderer->AddActor(m_brushActor);
		m_brushRenderer->SetLayer(1);

		if (GetImageViewer() && GetImageViewer()->GetRenderWindow()) {

			// Set the image actor
			int displayExtent[6];
			memcpy(displayExtent, GetImageViewer()->GetImageActor()->GetDisplayExtent(),
				sizeof(displayExtent));
			displayExtent[GetSliceOrientation() * 2] = 0;
			displayExtent[GetSliceOrientation() * 2 + 1] = 0;
			m_brushActor->SetDisplayExtent(displayExtent);

			m_brushRenderer->SetActiveCamera(GetImageViewer()->GetRenderer()->GetActiveCamera());
			GetImageViewer()->GetRenderWindow()->SetNumberOfLayers(2);
			GetImageViewer()->GetRenderer()->SetLayer(0);
			GetImageViewer()->GetRenderWindow()->AddRenderer(m_brushRenderer);
		}
		GetImageViewer()->Render();
	}
	else {
		/// Delete stuff if the mode is disabled to save memory
		if (m_retangleRep) {
			m_retangleRep->Delete();
			m_retangleRep = nullptr;
		}
		if (m_borderWidget)
		{
			m_borderWidget->Delete();
			m_borderWidget = nullptr;
		}
		if (m_brush)
		{
			m_brush->Delete();
			m_brush = NULL;
			
			if (m_brushActor && m_brushRenderer) {
				m_brushRenderer->RemoveActor(m_brushActor);
				m_brushActor->Delete();
				m_brushActor = nullptr;
				
				if (GetImageViewer()) {
					GetImageViewer()->GetRenderWindow()->SetNumberOfLayers(1);
					GetImageViewer()->GetRenderWindow()->RemoveRenderer(m_brushRenderer);
					m_brushRenderer->Delete();
					m_brushRenderer = nullptr;
				}
			}



		}
	}

	this->m_customFlag = b;
}

//void InteractorStylePaintBrush::SetOverlay(vtkImageData * overlay)
//{
//	m_overlay = overlay;
//}

void InteractorStylePaintBrush::SetPaintBrushLabel(int paintBrushLabel)
{
	this->paintBrushLabel = paintBrushLabel;
	vtkLookupTable* lookupTable = GetImageViewer()->GetLookupTable();
	double rgba[4];
	const double* _colour = lookupTable->GetTableValue(paintBrushLabel);
	for (int i = 0; i < 3; ++i) {
		rgba[i] = _colour[i] * 255;
	}
	EnableEraserMode(paintBrushLabel == 0);
	SetDrawColor(rgba[0], rgba[1], rgba[2]);
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
		GetImageViewer()->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	////Check if valid pick
	//if (!this->CheckValidPick(picked))
	//	return;
	//Check if valid pick
	if (picked[0] == 0.0&&picked[1] == 0.0)
		return;
	double index[3];
	if (GetImageViewer()->GetInput() != NULL) {
		picked[GetSliceOrientation()] = GetOrigin()[GetSliceOrientation()] + GetSlice() * GetSpacing()[GetSliceOrientation()];
		for (int i = 0; i < 3; i++)
		{
			index[i] = (picked[i] - GetOrigin()[i]) / GetSpacing()[i];
		}
	}
	int draw_index[3];

	draw_index[0] = (int)(index[0] + 0.5);
	draw_index[1] = (int)(index[1] + 0.5);
	draw_index[2] = (int)(index[2] + 0.5);

	int x2Dindex, y2Dindex;
	switch (GetSliceOrientation())
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
		int loopLowerBoundX = ceil(-m_brushSize / GetSpacing()[x2Dindex] / 2.);
		int loopUpperBoundX = ceil(m_brushSize / GetSpacing()[x2Dindex] / 2.);
		int loopLowerBoundY = ceil(-m_brushSize / GetSpacing()[y2Dindex] / 2.);
		int loopUpperBoundY = ceil(m_brushSize / GetSpacing()[y2Dindex] / 2.);

		for (int x = loopLowerBoundX; x < loopUpperBoundX; x++)
		{
			for (int y = loopLowerBoundY; y < loopUpperBoundY; y++)
			{
				this->DrawLine3D(draw_index_old[x2Dindex] + x, draw_index_old[y2Dindex] + y, draw_index[x2Dindex] + x, draw_index[y2Dindex] + y);
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
	switch (this->GetSliceOrientation())
	{
	case 2:
		m_brush->FillBox(GetExtent()[0], GetExtent()[1], GetExtent()[2], GetExtent()[3]);
		break;
	case 1:
		for (int x = GetExtent()[0]; x <= GetExtent()[1]; x++)
		{
			m_brush->DrawSegment3D(x, 0, GetExtent()[4], x, 0, GetExtent()[5]);
		}
		break;
	case 0:
		for (int y = GetExtent()[2]; y <= GetExtent()[3]; y++)
		{
			m_brush->DrawSegment3D(0, y, GetExtent()[4], 0, y, GetExtent()[5]);
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
	switch (GetSliceOrientation())
	{
	case 0:
		if (x0 < GetExtent()[2] || x0 > GetExtent()[3] || y0 < GetExtent()[4] || y0 > GetExtent()[5] || x1 < GetExtent()[2] || x1 > GetExtent()[3] || y1 < GetExtent()[4] || y1 > GetExtent()[5])
			return;
		index[0] = 0;
		index[1] = x0;
		index[2] = y0;
		index[3] = 0;
		index[4] = x1;
		index[5] = y1;
		break;
	case 1:
		if (x0 < GetExtent()[0] || x0 > GetExtent()[1] || y0 < GetExtent()[4] || y0 > GetExtent()[5] || x1 < GetExtent()[0] || x1 > GetExtent()[1] || y1 < GetExtent()[4] || y1 > GetExtent()[5])
			return;
		index[0] = x0;
		index[1] = 0;
		index[2] = y0;
		index[3] = x1;
		index[4] = 0;
		index[5] = y1;
		break;
	case 2:
		if (x0 < GetExtent()[0] || x0 > GetExtent()[1] || y0 < GetExtent()[2] || y0 > GetExtent()[3] || x1 < GetExtent()[0] || x1 > GetExtent()[1] || y1 < GetExtent()[2] || y1 > GetExtent()[3])
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
	switch (GetSliceOrientation())
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

	int loopLowerBoundX = floor(-radius / (this->GetSpacing()[x2Dindex]));
	int loopUpperBoundX = ceil(radius / (this->GetSpacing()[x2Dindex]));
	int loopLowerBoundY = floor(-radius / (this->GetSpacing()[y2Dindex]));
	int loopUpperBoundY = ceil(radius / (this->GetSpacing()[y2Dindex]));


	// check bound
	if (loopLowerBoundX >= loopUpperBoundX || loopLowerBoundY >= loopUpperBoundY) {
		std::cerr << "Bound calculation failed.";
		return;
	}

	for (int i = loopLowerBoundX; i < loopUpperBoundX; i++)
	{
		for (int j = loopLowerBoundY; j < loopUpperBoundY; j++)
		{
			double d = pow(i * GetSpacing()[x2Dindex], 2) + pow(j * GetSpacing()[y2Dindex], 2.);
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
		GetImageViewer()->GetRenderer());

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

	GetImageViewer()->GetRenderer()->SetWorldPoint(0, 0, 0, 1);
	GetImageViewer()->GetRenderer()->WorldToDisplay();
	GetImageViewer()->GetRenderer()->GetDisplayPoint(display_origin);
	//cout << "World->Display vtkRenderer 00: " << "(" << x << "," << y << ")" << " --> " << "(" << result2[0] << "," << result2[1] << ")\n";

	GetImageViewer()->GetRenderer()->SetWorldPoint(size, size, size, 1);
	GetImageViewer()->GetRenderer()->WorldToDisplay();
	GetImageViewer()->GetRenderer()->GetDisplayPoint(display_size);
	//cout << "World->Display vtkRenderer: "	<< "(" << x << "," << y << ")" << " --> " << "(" << result[0] << "," << result[1] << ")\n";

	double X3 = display_size[0] - display_origin[0];
	double Y3 = display_size[1] - display_origin[1];

	GetImageViewer()->GetRenderer()->DisplayToNormalizedDisplay(X1, Y1);
	GetImageViewer()->GetRenderer()->NormalizedDisplayToViewport(X1, Y1);
	GetImageViewer()->GetRenderer()->ViewportToNormalizedViewport(X1, Y1);

	GetImageViewer()->GetRenderer()->DisplayToNormalizedDisplay(X3, Y3);
	GetImageViewer()->GetRenderer()->NormalizedDisplayToViewport(X3, Y3);
	GetImageViewer()->GetRenderer()->ViewportToNormalizedViewport(X3, Y3);

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
	int extent[6];
	memcpy(extent, GetExtent(), sizeof(extent));
	extent[GetSliceOrientation() * 2] = 0;
	extent[GetSliceOrientation() * 2 + 1] = 0;
	m_brush->SetExtent(extent);
	m_brush->Update();



	int displayExtent[6];
	memcpy(displayExtent, GetImageViewer()->GetImageActor()->GetDisplayExtent(),
		sizeof(displayExtent));
	displayExtent[GetSliceOrientation() * 2] = 0;
	displayExtent[GetSliceOrientation() * 2 + 1] = 0;
	m_brushActor->SetDisplayExtent(displayExtent);
	m_brushActor->Update();

	//Clear Layer
	m_brush->SetDrawColor(0, 0, 0, 0);
	this->FillBox3D();

	int pos[3];
	memcpy(extent, GetExtent(), sizeof(extent));
	extent[GetSliceOrientation() * 2] = GetSlice();
	extent[GetSliceOrientation() * 2 + 1] = GetSlice();
	for (int x = extent[0]; x <= extent[1]; ++x) {
		for (int y = extent[2]; y <= extent[3]; ++y) {
			for (int z = extent[4]; z <= extent[5]; ++z) {
				pos[0] = x;
				pos[1] = y;
				pos[2] = z;
				unsigned char* val = static_cast<unsigned char*>(GetImageViewer()->GetOverlay()->GetScalarPointer(pos));
				if (val == nullptr)
					continue;
				if (*val > 0) {
					for (int i = 0; i < GetImageViewer()->GetLookupTable()->GetNumberOfColors(); ++i) {
						if (*val == i) {
							double rgba[4];
							unsigned char rgbaUCHAR[4];
							GetImageViewer()->GetLookupTable()->GetIndexedColor(i, rgba);
							GetImageViewer()->GetLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar
																									   //try {
							m_brush->SetDrawColor(rgbaUCHAR[0], rgbaUCHAR[1], rgbaUCHAR[2], rgbaUCHAR[3]);
							pos[GetSliceOrientation()] = 0;
							m_brush->DrawSegment3D(pos[0], pos[1], pos[2],
								pos[0], pos[1], pos[2]);

							break;
						}
					}
				}
			}
		}
	}


}

void InteractorStylePaintBrush::WriteToImageData()
{
	//Update Layer
	int pos[3], extent[6];
	memcpy(extent, GetExtent(), sizeof(extent));
	extent[GetSliceOrientation() * 2] = GetSlice();
	extent[GetSliceOrientation() * 2 + 1] = GetSlice();


	unsigned char pixelval;

	for (int x = extent[0]; x <= extent[1]; ++x) {
		for (int y = extent[2]; y <= extent[3]; ++y) {
			for (int z = extent[4]; z <= extent[5]; ++z) {
				pos[0] = x;
				pos[1] = y;
				pos[2] = z;
				pos[GetSliceOrientation()] = 0;
				unsigned char* val = static_cast<unsigned char*>(m_brush->GetOutput()->GetScalarPointer(pos));
				pixelval = 0;
				if (val == nullptr)
					continue;
				if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0) {
					for (int i = 0; i < GetImageViewer()->GetLookupTable()->GetNumberOfColors(); i++)
					{
						double rgba[4];
						unsigned char rgbaUCHAR[4];
						GetImageViewer()->GetLookupTable()->GetIndexedColor(i, rgba);
						GetImageViewer()->GetLookupTable()->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar

						if (val[0] == rgbaUCHAR[0] && val[1] == rgbaUCHAR[1] && val[2] == rgbaUCHAR[2]) {
							pixelval = i;
							break;
						}

					}
				}
				pos[0] = x;
				pos[1] = y;
				pos[2] = z;
				unsigned char* pixel = static_cast<unsigned char*>(GetImageViewer()->GetOverlay()->GetScalarPointer(pos));
				*pixel = pixelval;
				//GetImageViewer()->GetOverlay()->SetPixel(pos, pixelval);
			}
		}
	}
	GetImageViewer()->GetOverlay()->Modified();
	GetImageViewer()->GetOverlay()->InvokeEvent(vtkCommand::UpdateDataEvent);

}

void InteractorStylePaintBrush::WriteToOverlay()
{
	int extent[6];
	memcpy(extent, m_brush->GetUpdateExtent(), sizeof(extent));
	extent[GetSliceOrientation() * 2] = GetSlice();
	extent[GetSliceOrientation() * 2 + 1] = GetSlice();

	//GetImageViewer()->GetOverlay()->SetPixels(extent, m_brush->GetOutput());
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStylePaintBrush, GetImageViewer()->Render());
}


void InteractorStylePaintBrush::Render()
{
	m_brush->Update();
	m_brush->GetOutput()->SetOrigin(GetOrigin());
	m_brush->GetOutput()->SetSpacing(GetSpacing());
	GetImageViewer()->Render();

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

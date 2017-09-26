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

#ifndef INTERACTOR_STYLE_PAINTBRUSH_H
#define INTERACTOR_STYLE_PAINTBRUSH_H

#include "AbstractNavigation.h"

//Extra


// Our codes
class vtkImageCanvasSource2D;
class vtkBorderWidget;
class vtkBorderRepresentation;
class vtkCircleBorderRepresentation;
class vtkImageActor;
class vtkRenderer;

class InteractorStylePaintBrush : public AbstractNavigation
{
public:
	enum BRUSH_SHAPE
	{
		SQUARE = 0,
		CIRCLE = 1
	};
	vtkTypeMacro(InteractorStylePaintBrush, AbstractNavigation);
	static InteractorStylePaintBrush* New();
	virtual void SetCustomEnabled(bool b);
	//virtual void SetOverlay(vtkImageData* overlay);
	virtual void SetPaintBrushLabel(int paintBrushLabel);
	virtual void SetDrawColor(int r, int g, int b);
	virtual void SetDrawColor(const int* rgb);
	virtual void SetDrawOpacity(int opacity);

	virtual void SetBrushShape(int brushShape);
	virtual void SetBrushSize(int size);
	virtual void EnableEraserMode(bool flag);

	void DrawCircle(int x0, int y0, int x1, int y1, double radius);
	void DrawLine3D(int x0, int y0, int x1, int y1);
	void FillBox3D();
	void Draw(bool b);
	void UpdateBorderWidgetPosition();
protected:
	InteractorStylePaintBrush();
	~InteractorStylePaintBrush();

	virtual void OnLeave();
	virtual void OnMouseMove();
	virtual void OnLeftButtonUp();
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();

	virtual void Render();


	/**
	*@deprecated
	*/
	void WriteToImageData();
	void WriteToOverlay();
	void ReadfromImageData();

	bool CheckValidPick(double*);



	//vtkImageData* m_overlay;
	vtkBorderWidget* m_borderWidget;
	vtkBorderRepresentation* m_retangleRep;
	vtkImageCanvasSource2D* m_brush;
	vtkImageActor* m_brushActor;
	vtkRenderer* m_brushRenderer;

	int paintBrushLabel;
	int draw_index_old[3];
	int m_colorRed, m_colorGreen, m_colorBule;
	int m_opacity;
	int m_brushSize;
	bool m_isDraw;
	int m_brushShape = SQUARE;
	bool m_eraserModeFlag = true;

};

#endif // INTERACTOR_STYLE_PAINTBRUSH_H
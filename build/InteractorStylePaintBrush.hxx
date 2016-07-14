#ifndef INTERACTOR_STYLE_PAINTBRUSH_H
#define INTERACTOR_STYLE_PAINTBRUSH_H

#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>

//Extra
#include "vtkImageCanvasSource2D.h"
#include "vtkBorderWidget.h"
#include "vtkProperty2D.h"
#include "vtkBorderRepresentation.h"

#include "MyVtkInteractorStyleImage.h"
#include "vtkCircleBorderRepresentation.h"


class InteractorStylePaintBrush : public MyVtkInteractorStyleImage
{
public:
	vtkTypeMacro(InteractorStylePaintBrush, MyVtkInteractorStyleImage);
	static InteractorStylePaintBrush* New();
	//void SetDrawBrushSizeSpinBox(QSpinBox* s);
	//void SetDrawBrushShapeComBox(QComboBox* comboBox);
	//void SetDrawVolumetricCheckBox(QCheckBox* checkBox);
	//void SetDrawIsotropicCheckBox(QCheckBox* checkBox);
	void SetPaintBrushModeEnabled(bool b);
	void SetDrawColor(int r, int g, int b);
	void SetDrawColor(int* rgb);
	void SetDrawOpacity(int opacity);
	void SetImageViewer(vtkImageViewer2* imageViewer);

protected:
	InteractorStylePaintBrush();
	~InteractorStylePaintBrush();

	virtual void OnMouseMove();
	virtual void OnLeftButtonUp();
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();	

private:
	void DrawCircle(int x0, int y0, int x1, int y1, int radius);
	void DrawLine3D(int x0, int y0, int x1, int y1);
	void FillBox3D();
	void Draw(bool b);
	//void UpdateBorderWidgetPosition();
	//void Write2ImageData();
	//void ReadfromImageData();
	void Render();

	vtkBorderWidget* m_borderWidget;
	vtkBorderRepresentation*		m_retangleRep;
	vtkCircleBorderRepresentation*	m_circleRep;
	vtkImageCanvasSource2D* m_brush;
	vtkImageActor* BrushActor;
	vtkRenderer* Renderer2;

	int extent[6];
	int draw_index_old[3];
	int color_r, color_g, color_b;
	int m_opacity;
	bool isDraw;

	double brushSize = 10;

};


#endif // INTERACTOR_STYLE_PAINTBRUSH_H
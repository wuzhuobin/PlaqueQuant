#ifndef INTERACTOR_STYLE_PAINTBRUSH_H
#define INTERACTOR_STYLE_PAINTBRUSH_H

#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>

//Extra
#include "vtkImageCanvasSource2D.h"
#include "vtkBorderWidget.h"
#include "vtkProperty2D.h"
#include "vtkBorderRepresentation.h"
#include <vtkRendererCollection.h>

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

protected:
	InteractorStylePaintBrush();
	~InteractorStylePaintBrush();


private:
	void Render();

	vtkBorderRepresentation*		m_retangleRep;
	vtkSmartPointer<vtkImageCanvasSource2D> brush;
	vtkSmartPointer<vtkImageActor> brushActor;
	vtkSmartPointer<vtkRenderer> brushRender;

	int draw_index_old[3];
	int red, green, blue, opacity;
	bool isDraw;

	double brushSize = 10;

};


#endif // INTERACTOR_STYLE_PAINTBRUSH_H
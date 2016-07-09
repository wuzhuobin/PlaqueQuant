#ifndef MYVTKINTERACTORYSTYLEIMAGE_H
#define MYVTKINTERACTORYSTYLEIMAGE_H

#include "Define.h"
#include "MyImageViewer.h"
#include "MyVtkCommand.h"

//#include <QSpinBox>
//#include <QDoubleSpinBox>
//#include <QComboBox>
//#include <QCheckBox>
#include <iostream>
//
//#include <QTime>
//#include <QCoreApplication>
//
#include <vtkObjectFactory.h>
//#include "vtkRenderer.h"
//#include <vector>
//#include <vtkSmartPointer.h>
//#include "vtkBorderWidget.h"
//#include "vtkBorderRepresentation.h"
//#include <vtkProperty.h>
//#include "vtkProperty2D.h"
//#include <vtkContourWidget.h>
//#include <vtkContourRepresentation.h>
//#include <vtkSelectEnclosedPoints.h>
//#include <vtkTransformPolyDataFilter.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>


//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkOrientedGlyphContourRepresentation.h>
//#include <vtkCommand.h>
//#include <vtkDebugLeaks.h>
//#include <vtkCamera.h>
//#include <vtkPlane.h>
//#include <vtkPolyData.h>
//#include <vtkPolygon.h>
//#include <vtkCellArray.h>
//#include <vtkPoints.h>
//#include <vtkPointData.h>
//#include <vtkDataArray.h>
//#include <vtkMath.h>
//#include <vtkTransform.h>
//#include <vtkWidgetEvent.h>
//#include <vtkWidgetEventTranslator.h>
//#include <vtkImageCanvasSource2D.h>


class ModuleWidget;
using namespace std;
// Define own interaction style
class MyVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
	static MyVtkInteractorStyleImage* New();
	vtkTypeMacro(MyVtkInteractorStyleImage, vtkInteractorStyleImage);


//private:
//	bool isDraw;
//
public:
	// Model
	enum Mode { OtherMode = 0, NavaigationMode = 1, WindowLevelMode = 2, PaintBrushMode = 3, PolygonMode = 4 };

	void SetImageViewer(vtkImageViewer2* imageViewer);
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseMove();
	void MoveSliceForward();
	void MoveSliceBackward();
	void CalculateIndex(double* index);
	virtual void OnKeyPress();
	int getOrientation();


	//void SetOrientation(MyImageViewer* imageViewer);
//	void SetSliceSpinBox(QSpinBox* x, QSpinBox* y, QSpinBox* z);
//	void SetWindowLevelSpinBox(QDoubleSpinBox* w,QDoubleSpinBox* l);
//	// Paint Brush
//	void SetDrawBrushSizeSpinBox(QSpinBox* s);
//	void SetDrawBrushShapeComBox(QComboBox* comboBox);
//	void SetDrawVolumetricCheckBox(QCheckBox* checkBox);
//	void SetDrawIsotropicCheckBox(QCheckBox* checkBox);
//	void SetPaintBrushModeEnabled(bool b);
//	void SetDrawColor(int r, int g, int b);
//	void SetDrawColor(int*);
//	void SetDrawOpacity(int opacity);
//	// Contour Widget
//	void SetPolygonModeEnabled(bool b);
	void SetMode(int m);
	void SetMode(Mode m);
	void setViewerSlice();
	double* getIndex();

//	void SetSliceRange(int min, int max);
//	void SetCurrentSlice(int slice);
//    void RemoveContourWidget();
//    vtkPoints* GetPointsInsideContour();
//    vtkOrientedGlyphContourRepresentation* GetContourRep();
//    bool FillPolygon();
//    void DrawSynchronize(vtkPolyData*);
//	void SetContourVisibility(bool b);
//	void RemoveContourWidgetAll();
//

// These methods for the different interactions in different modes
// are overridden in subclasses to perform the correct motion. Since
// they might be called from OnTimer, they do not have mouse coord parameters
// (use interactor's GetEventPosition and GetLastEventPosition)
	virtual void WindowLevel();

protected:
	MyVtkInteractorStyleImage();
	~MyVtkInteractorStyleImage();

	vtkImageViewer2* imageViewer;
	//    int no_of_points;
	//    int max_point;
	int slice;
	int minSlice;
	int maxSlice;
	int orientation;
	Mode mode;
	//	QSpinBox* m_sliceSplinBox[3];
	//	QDoubleSpinBox* m_wlDoubleSpinBox[2];
	//	double* m_wlDouble[2];
	//	bool m_functioning;
	bool rclickDrag;
	bool lclickDrag;
	double index[3];

    //bool mclickDrag = false;
	//
	//	//PaintBrush
	//	QComboBox*	m_drawValueComboBox;
	//	QComboBox*  m_drawBrushShapeComboBox;
	//	QSpinBox*	m_drawBrushSizeSpinBox;
	//	QCheckBox*	m_drawVolumetricCheckBox;
	//	QCheckBox*	m_drawIsotropicCheckBox;
	//	vtkBorderWidget* m_borderWidget;
	//	vtkBorderRepresentation*		m_retangleRep;
	//	vtkImageCanvasSource2D* m_brush;
	//	vtkImageActor* BrushActor;
	double origin[3];
	double spacing[3];
	//	int extent[6];
	//	int draw_index_old[3];
	//	int color_r, color_g, color_b;
	//	int m_opacity;
//protected:

//	virtual void OnChar();
//    virtual void OnRightButtonUp();
//    virtual void OnRightButtonDown();
//    virtual void OnMiddleButtonUp();
//    virtual void OnMiddleButtonDown();
//    

//    void Synchronize();
	//void CalculateIndex();

//	void WindowLevel();
//
//	void DrawCircle(int x0, int y0, int x1, int y1, int radius);
//	void DrawLine3D(int x0, int y0, int x1, int y1);
//	void FillBox3D();
//	void Draw(bool b);
//	void UpdateBorderWidgetPosition();
//	void Write2ImageData();
//	void ReadfromImageData();
//	void Render();
//
//private:
//    // Drawing points and lines tool
//    vtkRenderer* Renderer;
//    vtkRenderer* Renderer2;
//    vtkRenderWindow* RenderWindow;
//    vtkOrientedGlyphContourRepresentation* contourRep;
//    vtkOrientedGlyphContourRepresentation* contourRepA;
//    vtkContourWidget* contourWidget;
//    vtkContourWidget* contourWidgetA;
//    vtkPoints* pointsInsideContour;
//    
};

#endif // MYVTKINTERACTORYSTYLEIMAGE_H

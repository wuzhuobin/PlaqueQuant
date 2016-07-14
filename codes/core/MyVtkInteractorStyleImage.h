#ifndef MYVTKINTERACTORYSTYLEIMAGE_H
#define MYVTKINTERACTORYSTYLEIMAGE_H

#include "MyImageViewer.h"
#include "MyVtkCommand.h"

#include <iostream>
#include <sstream>
//
#include <vtkObjectFactory.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>

class ModuleWidget;
using namespace std;
// Define own interaction style
class MyVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
	static MyVtkInteractorStyleImage* New();
	vtkTypeMacro(MyVtkInteractorStyleImage, vtkInteractorStyleImage);

public:
	// Model
	enum Mode { OtherMode = 0, NavaigationMode = 1, WindowLevelMode = 2 };

	// Description:
	// Event bindings controlling the effects of pressing mouse buttons
	// or moving the mouse.
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseMove();
	virtual void MoveSliceForward();
	virtual void MoveSliceBackward();

	/**
	 *
	 */
	virtual void CalculateIndex(double* index);
	virtual void OnKeyPress();
	virtual int getOrientation();
	virtual void SetImageViewer(vtkImageViewer2* imageViewer);



	virtual void SetMode(int m);
	virtual void SetMode(Mode m);
	virtual void setViewerSlice();
	virtual void setSlice(int slice);
	virtual double* getIndex();


// These methods for the different interactions in different modes
// are overridden in subclasses to perform the correct motion. Since
// they might be called from OnTimer, they do not have mouse coord parameters
// (use interactor's GetEventPosition and GetLastEventPosition)
	// new windowLevel()
	virtual void WindowLevel();

protected:
	MyVtkInteractorStyleImage();
	~MyVtkInteractorStyleImage();

	vtkImageViewer2* imageViewer;

	int slice;
	int minSlice;
	int maxSlice;
	int orientation;
	Mode mode;
	bool rclickDrag;
	bool lclickDrag;
	double index[3];

	double origin[3];
	double spacing[3];
};

#endif // MYVTKINTERACTORYSTYLEIMAGE_H

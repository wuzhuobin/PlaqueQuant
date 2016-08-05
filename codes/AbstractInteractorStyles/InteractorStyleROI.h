#pragma once
#include "AbstractInteractorStyleImage.h"

#include "MyPlaneWidget.h"

class InteractorStyleROI :
	public AbstractInteractorStyleImage
{
public:
	vtkTypeMacro(InteractorStyleROI, AbstractInteractorStyleImage);
	static InteractorStyleROI* New();
	void SetImageViewer(MyImageViewer * imageViewer);
	void SetPlaneWidgetEnabled(bool b);

	MyPlaneWidget* GetPlaneWidget();

protected:
	InteractorStyleROI();
	~InteractorStyleROI();


private:
	MyPlaneWidget* planeWidget;
	MyPlaneWidgetCallback* planeWidgetCallback;

};


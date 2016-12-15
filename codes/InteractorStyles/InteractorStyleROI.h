#pragma once
#include "InteractorStyleNavigation.h"

#include "MyPlaneWidget.h"

class InteractorStyleROI :
	public InteractorStyleNavigation
{
public:
	//vtkTypeMacro(InteractorStyleROI, InteractorStyleNavigation);
	static InteractorStyleROI* New();
	void SetPlaneWidgetEnabled(bool flag);
	void OnMouseMove();
	void OnLeftButtonDown();
	void OnLeftButtonUp();
	void SelectROI(int* newExtent);
	void UpdateAllWidget();
	void UpdateAllWidget(double* bound);

	void AddSynchronalROI(InteractorStyleROI* roi);
	MyPlaneWidget* GetPlaneWidget();

protected:
	InteractorStyleROI();
	~InteractorStyleROI();


private:

	std::vector<InteractorStyleROI*> rois;

	MyPlaneWidget* planeWidget;
	MyPlaneWidgetCallback* planeWidgetCallback;

	double displayBound[6];
};


#pragma once

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>


class MultiWidgetsController
{
public:
	static const int NUMOFRENDERERWINDOWS = 3;
	MultiWidgetsController();
	~MultiWidgetsController();
	void setRenderWindows(vtkRenderWindow** renderWindow);

private:
	vtkSmartPointer<vtkRenderer> renderers[NUMOFRENDERERWINDOWS];
	vtkRenderWindow* renderWindow[NUMOFRENDERERWINDOWS];
};


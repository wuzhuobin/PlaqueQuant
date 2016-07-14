#pragma once

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkProperty.h>
#include <vtkBorderWidget.h>
#include <vtkBorderRepresentation.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPicker.h>

#include "MyVtkCommand.h"

class MultiWidgetsController
{
public:
	static const int NUMOFRENDERERWINDOWS = 3;
	MultiWidgetsController();
	~MultiWidgetsController();
	void setupRenderWindows(vtkRenderWindow** renderWindow);
	void contourMotion();
	void paintBrush();

private:
	vtkSmartPointer<vtkRenderer> renderers[NUMOFRENDERERWINDOWS];
	vtkRenderWindow* renderWindow[NUMOFRENDERERWINDOWS];

	vtkSmartPointer<vtkContourWidget> contour;
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRep;
	vtkSmartPointer<vtkBorderWidget> borderWidget;
	vtkSmartPointer<vtkBorderRepresentation> retangleRep;
	vtkSmartPointer<vtkImageCanvasSource2D> brush;

	friend class callback;
};

class callback :public vtkCommand {
public:
	static callback *New();
	virtual void Execute(vtkObject* caller, unsigned long eventId, void* callData);

	MultiWidgetsController* controller;
};


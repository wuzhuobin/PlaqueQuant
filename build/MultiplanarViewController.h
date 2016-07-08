#pragma once

#include "MyImageViewer.h"
#include "MyVtkInteractorStyleImage.h":

#include <vtkSmartPointer.h>
#include <vtkSetGet.h>
#include <vtkImageData.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>

class MultiplanarViewController
{
public:
	const static int NUMOFVIEWERS = 3;
	
	
	MultiplanarViewController();
	~MultiplanarViewController();
	void synchronizeSlices(vtkObject*, unsigned long eid, void* clientdata, void* claadata);


	vtkSmartPointer<MyImageViewer> getViewers(int i);
	void initialize(vtkImageData* in);

protected:
	vtkSmartPointer<MyImageViewer> viewers[NUMOFVIEWERS];
	vtkSmartPointer<vtkRenderWindowInteractor> interactor[NUMOFVIEWERS];
	vtkSmartPointer<MyVtkInteractorStyleImage> style[NUMOFVIEWERS];

};


#pragma once

#include "MyImageViewer.h"
#include "MyVtkInteractorStyleImage.h"

#include <vtkSmartPointer.h>
#include <vtkSetGet.h>
#include <vtkImageData.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindowInteractor.h>

#include <vector>

using namespace std;

void CallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData);


class MultiplanarViewControllerCallback : public vtkCallbackCommand
{
public:

	static enum _enum_name { 
		SliceChangeEvent = 1001, 
		ColorWindowEvent = 1002,
		ColorLevelEvent = 1003
	};
};

class MultiplanarViewController
{
public:
	const static int NUMOFVIEWERS = 3;
	
	
	MultiplanarViewController();
	~MultiplanarViewController();

	void setSlices(int* slices);
	void setColorWindow(double colorWindow);
	void setColorLevel(double colorWindow);

	MyImageViewer* getViewers(int i);
	void initialize(vtkImageData* in);

protected:
	int slices[3];
	double colorWindow;
	double colorLevel;
	vector<vtkSmartPointer<MyImageViewer>> viewers;
	vector<vtkSmartPointer<vtkRenderWindowInteractor>> interactor;
	vector<vtkSmartPointer<MyVtkInteractorStyleImage>> style;

};




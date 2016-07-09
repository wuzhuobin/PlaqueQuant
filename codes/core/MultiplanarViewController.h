#pragma once

#include "MyImageViewer.h"
#include "MyVtkInteractorStyleImage.h"
#include "MyVtkCommand.h"

#include <vtkSmartPointer.h>
#include <vtkSetGet.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>

#include <vector>

using namespace std;

//void CallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData);

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

	friend class MultiplanarViewControllerCallback;
};

class MultiplanarViewControllerCallback : public MyVtkCommand
{
public:
	static MultiplanarViewControllerCallback* New();
	virtual void Execute(vtkObject* caller, unsigned long event, void* callData);
private:
	MultiplanarViewController* controller;
};



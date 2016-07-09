#pragma once

#include "MyImageViewer.h"
#include "MyVtkInteractorStyleImage.h"
#include "MyVtkCommand.h"
#include "Define.h"

#include <vtkSmartPointer.h>
#include <vtkSetGet.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>

#include <vector>
#include <list>
#include <string>

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
	void setColorLevel(double colorLevel);
	void render();

	MyImageViewer* getViewers(int i);
	int loadImage(list<string> in);
	void initialize(vtkImageData* in);
	vtkImageData* getImage(int i);


protected:
	int slices[NUMOFVIEWERS];
	double colorWindow;
	double colorLevel;
	vector<vtkSmartPointer<MyImageViewer>> viewers;
	vector<vtkSmartPointer<vtkRenderWindowInteractor>> interactor;
	vector<vtkSmartPointer<MyVtkInteractorStyleImage>> style;
	vector<vtkSmartPointer<vtkImageData>> imgs;

	friend class MultiplanarViewControllerCallback;

};

class MultiplanarViewControllerCallback : public MyVtkCommand
{
public:
	static MultiplanarViewControllerCallback* New();
	virtual void Execute(vtkObject* caller, unsigned long event, void* callData);
	MultiplanarViewController* controller;
};



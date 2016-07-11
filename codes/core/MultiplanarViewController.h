#pragma once

#include "MyImageViewer.h"
#include "MyVtkInteractorStyleImage.h"
#include "MyVtkCommand.h"
#include "Define.h"

#include <vtkSmartPointer.h>
#include <vtkSetGet.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>

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
	void setMode(MyVtkInteractorStyleImage::Mode mode);
	void setMode(int mode);
	void setIntensityText(int* index);

	MyImageViewer* getViewers(int i);
	int loadImage(list<string> in);
	void setImageName(string name);
	void initialize();
	/**
	 * @deprecated
	 */
	void initialize(vtkImageData* in);
	vtkImageData* getImage();


protected:
	int slices[NUMOFVIEWERS];
	double colorWindow;
	double colorLevel;
	vtkSmartPointer<MyImageViewer> viewers[NUMOFVIEWERS];
	vtkSmartPointer<vtkRenderWindowInteractor> interactor[NUMOFVIEWERS];
	vtkSmartPointer<MyVtkInteractorStyleImage> style[NUMOFVIEWERS];
	vtkSmartPointer<vtkImageData> img;
	string imgName;

	//Cursor
	//vector<vtkSmartPointer<vtkCursor3D>> Cursor3D;
	//vector<vtkSmartPointer<vtkPolyDataMapper>> Cursormapper;
	//vector<vtkSmartPointer<vtkActor>> CursorActor;


	friend class MultiplanarViewControllerCallback;

};

class MultiplanarViewControllerCallback : public MyVtkCommand
{
public:
	static MultiplanarViewControllerCallback* New();
	virtual void Execute(vtkObject* caller, unsigned long event, void* callData);
	MultiplanarViewController* controller;
};



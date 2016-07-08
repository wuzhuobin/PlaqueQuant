#include "MultiplanarViewController.h"



MultiplanarViewController::MultiplanarViewController()
{
	for (int i = 0; i < NUMOFVIEWERS; ++i) {
		viewers[i] = vtkSmartPointer<MyImageViewer>::New();
		interactor[i] = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		style[i] = vtkSmartPointer<MyVtkInteractorStyleImage>::New();

	}
}


MultiplanarViewController::~MultiplanarViewController()
{
}

void MultiplanarViewController::synchronizeSlices(vtkObject *, unsigned long eid, void * clientdata, void * claadata)
{
}

vtkSmartPointer<MyImageViewer> MultiplanarViewController::getViewers(int i)
{
	if (i > -1 && i < NUMOFVIEWERS) {
		return viewers[i];
	}
	else
		return NULL;
}

void MultiplanarViewController::initialize(vtkImageData * in)
{
	double* range = in->GetScalarRange();
	for (int i = 0; i < NUMOFVIEWERS; ++i) {

		viewers[i]->SetInputData(in);
		viewers[i]->SetColorWindow(range[1] - range[0]);
		viewers[i]->SetColorLevel((range[1] + range[0]) * 0.5);
		double defaultWindowLevel[2] = { range[1] - range[0], (range[1] + range[0]) * 0.5 };
		viewers[i]->SetDefaultWindowLevel(defaultWindowLevel);

		viewers[i]->SetSliceOrientation(i);
		viewers[i]->SetSlice(viewers[i]->GetSliceMax() / 2);
		viewers[i]->SetupInteractor(interactor[i]);

		style[i]->SetImageViewer(viewers[i]);
		style[i]->SetMode(MyVtkInteractorStyleImage::NavaigationMode);
		//style[i]->setController(this);

		interactor[i]->SetInteractorStyle(style[i]);

		vtkSmartPointer<vtkCallbackCommand> sliceChange =
			vtkSmartPointer<vtkCallbackCommand>::New();
		sliceChange->SetCallback(dynamic_cast<void*>(synchronizeSlices));
		interactor[i]->AddObserver();


	}
}

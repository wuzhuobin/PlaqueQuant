#include "MultiplanarViewController.h"



MultiplanarViewController::MultiplanarViewController()
{
	for (int i = 0; i < NUMOFVIEWERS; ++i) {
		viewers.push_back(vtkSmartPointer<MyImageViewer>::New());
		interactor.push_back(vtkSmartPointer<vtkRenderWindowInteractor>::New());
		style.push_back(vtkSmartPointer<MyVtkInteractorStyleImage>::New());

	}
}


MultiplanarViewController::~MultiplanarViewController()
{
	//for (int i = 0; i < NUMOFVIEWERS; ++i) {
	//	viewers[i]->Delete();
	//	interactor[i]->Delete();
	//	//style[i]->Delete();

	//}
}

void MultiplanarViewController::setSlices(int * slices)
{
	for (int i = 0; i < 3; ++i) {
		this->slices[i] = slices[i];
	}
}

void MultiplanarViewController::setColorWindow(double colorWindow)
{
	for (int i = 0; i < 3; ++i) {

	}
}

void MultiplanarViewController::setColorLevel(double colorWindow)
{
}

MyImageViewer* MultiplanarViewController::getViewers(int i)
{
	if (i > -1 && i < NUMOFVIEWERS) {
		return viewers[i];
	}
	else
		return NULL;
}

void MultiplanarViewController::initialize(vtkImageData * in)
{
	vtkSmartPointer<MultiplanarViewControllerCallback> callback =
		vtkSmartPointer<MultiplanarViewControllerCallback>::New();
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
		style[i]->SetMode(MyVtkInteractorStyleImage::WindowLevelMode);

		interactor[i]->SetInteractorStyle(style[i]);

		slices[i] = viewers[i]->GetSlice();

		style[i]->AddObserver(MyVtkCommand::ResetWindowLevelEvent, callback);
		style[i]->AddObserver(MyVtkCommand::SliceChangeEvent, callback);


	}
	colorLevel = viewers[0]->GetColorLevel();
	colorWindow = viewers[0]->GetColorWindow();

}

//void CallbackFunction(vtkObject * caller, long unsigned int eventId, void * clientData, void * callData)
//{
//	MyImageViewer* viewer = static_cast<MyImageViewer*>(caller);
//	MultiplanarViewController* controller = static_cast<MultiplanarViewController*>(clientData);
//	int orientation = viewer->GetSliceOrientation();
//	double colorLevel = viewer->GetColorLevel();
//	double colorWindow = viewer->GetColorWindow();
//	switch ((MultiplanarViewControllerCallback::_enum_name)eventId)
//	{
//	case(MultiplanarViewControllerCallback::SliceChangeEvent):
//		cout << "slice change" << endl;
//		break;
//	case(MultiplanarViewControllerCallback::WindowLevelEvent):
//		
//	default:
//
//		break;
//	}
//}


MultiplanarViewControllerCallback* MultiplanarViewControllerCallback::New() {
	return new MultiplanarViewControllerCallback;
}

void MultiplanarViewControllerCallback::Execute(vtkObject* caller,
	unsigned long event, void* callData) {
	if (controller->viewers.size() == 0) {
		return;
	}

	switch (event) {
	case MyVtkCommand::SliceChangeEvent:
		cout << "slice change" << endl;
		break;

	case MyVtkCommand::StartWindowLevelEvent:
		cout << " window level " << endl;
		break;
	default:
		return;
	}

}
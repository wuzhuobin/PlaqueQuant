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
	this->colorWindow = colorWindow;
	for (int i = 0; i < NUMOFVIEWERS; ++i) {
		cout << viewers[i]->GetColorLevel() << " " << viewers[i]->GetColorWindow() << endl;

		viewers[i]->SetColorWindow(colorWindow);
	}
}

void MultiplanarViewController::setColorLevel(double colorLevel)
{
	this->colorLevel = colorLevel;
	for (int i = 0; i < NUMOFVIEWERS; ++i) {
		cout << viewers[i]->GetColorLevel() << " " << viewers[i]->GetColorWindow() << endl;

		viewers[i]->SetColorLevel (colorLevel);
	}
}

void MultiplanarViewController::render()
{
	for (int i = 0; i < NUMOFVIEWERS; ++i) {
		cout << viewers[i]->GetColorLevel() << " " << viewers[i]->GetColorWindow() << endl;
		viewers[i]->Render();
	}
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
	callback->controller = this;
	double* range = in->GetScalarRange();
	setColorLevel((range[1] + range[0]) * 0.5);
	setColorWindow(range[1] - range[0]);
	for (int i = 0; i < NUMOFVIEWERS; ++i) {

		viewers[i]->SetInputData(in);

		//viewers[i]->SetColorWindow(range[1] - range[0]);
		//viewers[i]->SetColorLevel((range[1] + range[0]) * 0.5);

		double defaultWindowLevel[2] = { range[1] - range[0], (range[1] + range[0]) * 0.5 };
		viewers[i]->SetDefaultWindowLevel(defaultWindowLevel);

		viewers[i]->SetSliceOrientation(i);
		viewers[i]->SetSlice(viewers[i]->GetSliceMax() / 2);
		viewers[i]->SetupInteractor(interactor[i]);

		style[i]->SetImageViewer(viewers[i]);
		style[i]->SetMode(MyVtkInteractorStyleImage::WindowLevelMode);

		interactor[i]->SetInteractorStyle(style[i]);

		slices[i] = viewers[i]->GetSlice();

		style[i]->AddObserver(MyVtkCommand::WindowLevelEvent2, callback);
		style[i]->AddObserver(MyVtkCommand::SliceChangeEvent, callback);


	}
	//colorLevel = viewers[0]->GetColorLevel();
	//colorWindow = viewers[0]->GetColorWindow();

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

	MyVtkInteractorStyleImage* style = static_cast<MyVtkInteractorStyleImage*>(caller);
	int orientation = style->getOrientation();
	cout << "orientatino:" << orientation << endl;
	if (controller->viewers.size() == 0) {
		return;
	}

	switch (event) {
	case MyVtkCommand::SliceChangeEvent:
		cout << "slice change" << endl;
		break;

	//case MyVtkCommand::WindowLevelEvent:
	//	cout << " window level1 " << endl;
	//	break;
	case MyVtkCommand::WindowLevelEvent2:
		controller->setColorLevel(style->GetWindowLevelCurrentPosition()[1]);
		controller->setColorWindow(style->GetWindowLevelCurrentPosition()[0]);
		controller->render();
		cout << " window level " << endl;
		break;
	default:
		return;
	}

}
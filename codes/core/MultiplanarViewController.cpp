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
		viewers[i]->SetSlice(slices[i]);
		cout << "slices:" << slices[i];
	}
	cout << endl;
}

void MultiplanarViewController::setColorWindow(double colorWindow)
{
	this->colorWindow = colorWindow;
	for (int i = 0; i < NUMOFVIEWERS; ++i) {
		viewers[i]->SetColorWindow(colorWindow);
	}
}

void MultiplanarViewController::setColorLevel(double colorLevel)
{
	this->colorLevel = colorLevel;
	for (int i = 0; i < NUMOFVIEWERS; ++i) {
		viewers[i]->SetColorLevel (colorLevel);
	}
}

void MultiplanarViewController::render()
{
	for (int i = 0; i < NUMOFVIEWERS; ++i) {
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
		style[i]->SetMode(MyVtkInteractorStyleImage::NavaigationMode);

		interactor[i]->SetInteractorStyle(style[i]);
		//interactor[i]->SetInteractorStyle(vtkInteractorStyleImage::New());


		slices[i] = viewers[i]->GetSlice();

		style[i]->AddObserver(MyVtkCommand::WindowLevelEvent2, callback);

		style[i]->AddObserver(MyVtkCommand::SliceChangeEvent, callback);
		style[i]->AddObserver(MyVtkCommand::ResetWindowLevelEvent, callback);
		style[i]->AddObserver(MyVtkCommand::NavigationEvent, callback);




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


	if (controller->viewers.size() == 0) {
		return;
	}
	MyVtkInteractorStyleImage* style = static_cast<MyVtkInteractorStyleImage*>(caller);
	int orientation = style->getOrientation();
	double* range = controller->getViewers(orientation)->GetInput()->GetScalarRange();
	cout << "orientatino:" << orientation << endl;
	int index[3];

	switch (event) {
	case MyVtkCommand::SliceChangeEvent:
		cout << "slice change" << endl;
		break;
	case MyVtkCommand::ResetWindowLevelEvent:
		controller->setColorWindow(range[1] - range[0]);
		controller->setColorLevel((range[1] + range[0]) * 0.5);
		cout << "reset" << endl;
		break;
	case MyVtkCommand::NavigationEvent:
		for (int i = 0; i < MultiplanarViewController::NUMOFVIEWERS; ++i) {
			index[i] =(int)( style->getIndex()[i]+ 0.5);
		}
		controller->setSlices(index);

		break;
	case MyVtkCommand::WindowLevelEvent2:
		controller->setColorLevel(controller->getViewers(orientation)->GetColorLevel());
		controller->setColorWindow(controller->getViewers(orientation)->GetColorWindow());
		cout << " window level " << endl;
		break;
	default:
		;
	}
	controller->render();

}
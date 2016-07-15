#include "MyVtkInteractorStyleImage.h"



vtkStandardNewMacro(MyVtkInteractorStyleImage);

void MyVtkInteractorStyleImage::SetImageViewer(vtkImageViewer2* imageViewer) 
{
	this->imageViewer = imageViewer;
	minSlice	  = imageViewer->GetSliceMin();
	maxSlice	  = imageViewer->GetSliceMax();
	slice		  = imageViewer->GetSlice();
	cout << minSlice << " " << maxSlice << endl;
	
	orientation = imageViewer->GetSliceOrientation();
	imageViewer->GetInput()->GetSpacing(spacing);
	imageViewer->GetInput()->GetOrigin(origin);
	imageViewer->GetInput()->GetExtent(extent);
}

void MyVtkInteractorStyleImage::SetMode(int m) 
{
	if( m>-1 && m<3)
		mode = (Mode)m;
}

void MyVtkInteractorStyleImage::SetMode(Mode m)
{
	mode = m;
}

void MyVtkInteractorStyleImage::setViewerSlice()
{
	if (this->HandleObservers && this->HasObserver(MyVtkCommand::SliceChangeEvent)) {

		InvokeEvent(MyVtkCommand::SliceChangeEvent, this);

	}
	imageViewer->SetSlice(slice);
}

void MyVtkInteractorStyleImage::setSlice(int slice)
{
	this->slice = slice;
}

double * MyVtkInteractorStyleImage::getIndex()
{
	return index;
}



void MyVtkInteractorStyleImage::WindowLevel()
{
	vtkRenderWindowInteractor *rwi = this->Interactor;

	this->WindowLevelCurrentPosition[0] = rwi->GetEventPosition()[0];
	this->WindowLevelCurrentPosition[1] = rwi->GetEventPosition()[1];

	if (this->HandleObservers &&
		this->HasObserver(vtkCommand::WindowLevelEvent))
	{
		this->InvokeEvent(vtkCommand::WindowLevelEvent, this);
	}
	
	int *size = this->CurrentRenderer->GetSize();
	double* range = imageViewer->GetInput()->GetScalarRange();
	double window = range[1] - range[0];
	double level = (range[1] + range[0]) * 0.5;

	// Compute normalized delta

	double dx = (this->WindowLevelCurrentPosition[0] -
		this->WindowLevelStartPosition[0]) * 4.0 / size[0];
	double dy = (this->WindowLevelStartPosition[1] -
		this->WindowLevelCurrentPosition[1]) * 4.0 / size[1];

	// Scale by current values

	if (fabs(window) > 0.01)
	{
		dx = dx * window;
	}
	else
	{
		dx = dx * (window < 0 ? -0.01 : 0.01);
	}
	if (fabs(level) > 0.01)
	{
		dy = dy * level;
	}
	else
	{
		dy = dy * (level < 0 ? -0.01 : 0.01);
	}

	// Abs so that direction does not flip

	if (window < 0.0)
	{
		dx = -1 * dx;
	}
	if (level < 0.0)
	{
		dy = -1 * dy;
	}

	// Compute new window level

	double newWindow = dx + window;
	double newLevel = level - dy;

	if (newWindow < 0.01)
	{
		newWindow = 0.01;
	}
	imageViewer->SetColorWindow(newWindow);
	imageViewer->SetColorLevel(newLevel);

	this->Interactor->Render();
	//if (this->HandleObservers && this->HasObserver(MyVtkCommand::WindowLevelEvent2))
	//{
	//	this->InvokeEvent(MyVtkCommand::WindowLevelEvent2, this);
	//}
}

MyVtkInteractorStyleImage::MyVtkInteractorStyleImage()
	:vtkInteractorStyleImage()
{
	slice = 0;
	minSlice = 0;
	maxSlice = 0;
	mode = WindowLevelMode;
	rclickDrag = false;
	lclickDrag = false;
	imageViewer = NULL;
}

MyVtkInteractorStyleImage::~MyVtkInteractorStyleImage()
{
}

void MyVtkInteractorStyleImage::MoveSliceForward() 
{
	//MainWindow* mainWnd = MainWindow::GetMainWindow();
	if(slice < maxSlice) 
	{
		slice += 1;
		setViewerSlice();
		cout << " slice " << slice << endl;
		cout << " maxslice " << maxSlice << endl;
		cout << " minslice " << minSlice << endl;

	}
}

void MyVtkInteractorStyleImage::MoveSliceBackward() 
{
	if(slice > minSlice) 
	{
		slice -= 1;
		setViewerSlice();
		cout << " slice " << slice << endl;
		cout << " maxslice " << maxSlice << endl;
		cout << " minslice " << minSlice << endl;

	}
}

void MyVtkInteractorStyleImage::OnMouseWheelForward() 
{
	//std::cout << "Scrolled mouse wheel forward." << std::endl;
	if(slice < maxSlice) {
		MoveSliceForward();
	}
	// don't forward events, otherwise the image will be zoomed 
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelForward();
}


void MyVtkInteractorStyleImage::OnMouseWheelBackward() 
{
	//std::cout << "Scrolled mouse wheel backward." << std::endl;
	if(slice > minSlice) {
		MoveSliceBackward();
	}
	// don't forward events, otherwise the image will be zoomed 
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelBackward();
}

void MyVtkInteractorStyleImage::OnLeftButtonUp()
{
	lclickDrag = false;

	switch (mode)
	{
	case MyVtkInteractorStyleImage::OtherMode:
		break;
	case MyVtkInteractorStyleImage::NavaigationMode:
		break;
	case MyVtkInteractorStyleImage::WindowLevelMode:
		vtkInteractorStyleImage::OnLeftButtonUp();
		break;
	default:
		vtkInteractorStyleImage::OnLeftButtonDown();
		break;
	}

}

void MyVtkInteractorStyleImage::OnLeftButtonDown()
{

	lclickDrag = true;
	//m_functioning = true;
	//isDraw = false;
	//MainWindow* mainWnd = MainWindow::GetMainWindow();
	switch (mode)
	{
	case MyVtkInteractorStyleImage::OtherMode:
		break;
	case MyVtkInteractorStyleImage::NavaigationMode:
		CalculateIndex(index);
		break;
	case MyVtkInteractorStyleImage::WindowLevelMode:
		vtkInteractorStyleImage::OnLeftButtonDown();
		break;
	default:
		vtkInteractorStyleImage::OnLeftButtonDown();
		break;
	}
}

void MyVtkInteractorStyleImage::OnMouseMove()
{
	if (lclickDrag) {
		switch (mode)
		{
		case MyVtkInteractorStyleImage::OtherMode:
			break;
		case MyVtkInteractorStyleImage::NavaigationMode:
			CalculateIndex(index);
			break;
		case MyVtkInteractorStyleImage::WindowLevelMode:
			vtkInteractorStyleImage::OnMouseMove();
			break;
		default:
			vtkInteractorStyleImage::OnMouseMove();
			break;
		}
	}
	else {
		vtkInteractorStyleImage::OnMouseMove();

	}
}

void MyVtkInteractorStyleImage::OnKeyPress() 
{
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();

	// Handle an arrow key
	if(key == "Up")
		MoveSliceForward();
	else if(key == "Down")
		MoveSliceBackward();

	// Forward events
	vtkInteractorStyleImage::OnKeyPress();
}
int MyVtkInteractorStyleImage::getOrientation()
{
	return this->orientation;
}

void MyVtkInteractorStyleImage::CalculateIndex(double* index)
{
	//Pick
	this->GetInteractor()->GetPicker()->Pick(	
		this->GetInteractor()->GetEventPosition()[0], 
		this->GetInteractor()->GetEventPosition()[1], 
		0,  // always zero.
		imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if(picked[0]==0.0&&picked[1]==0.0)	
		return;
    if (imageViewer->GetInput() != NULL){
	picked[orientation] = origin[orientation] + slice * spacing[orientation];
        for (int i =0;i<3;i++)
        {
            index[i] = (picked[i]-origin[i])/spacing[i];

		}
		cout << "index:" <<index[0] << " " << index[1] << " " << index[2] << endl;
    }

	if (this->HandleObservers && this->HasObserver(MyVtkCommand::NavigationEvent))
	{
		this->InvokeEvent(MyVtkCommand::NavigationEvent, this);
	}
	
}

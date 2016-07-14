#include "MultiWidgetsController.h"


MultiWidgetsController::MultiWidgetsController()
{
	for (int i = 0; i < 3; ++i) {
		renderers[i] = vtkSmartPointer<vtkRenderer>::New();
		renderers[i]->SetLayer(1);
	}
}

MultiWidgetsController::~MultiWidgetsController()
{
}

void MultiWidgetsController::setupRenderWindows(vtkRenderWindow ** renderWindow)
{
	for (int i = 0; i < NUMOFRENDERERWINDOWS; ++i) {
		this->renderWindow[i] = renderWindow[i];
		renderWindow[i]->SetNumberOfLayers(2);
		renderWindow[i]->GetRenderers()->GetFirstRenderer()->SetLayer(0);
		renderWindow[i]->AddRenderer(renderers[i]);
	}
	
}

void MultiWidgetsController::contourMotion()
{
	contour = vtkSmartPointer<vtkContourWidget>::New();
	contour->SetInteractor(renderWindow[0]->GetInteractor());
	contour->SetCurrentRenderer(renderers[0]);

	contourRep = vtkOrientedGlyphContourRepresentation::New();
	contourRep->SetRenderer(renderers[0]);
	contourRep->GetLinesProperty()->SetColor(255, 255, 0);
	
	
	vtkPolyData* cursorpolyData = contourRep->GetActiveCursorShape();

	//if (m_orientation == 0) {
	//	translation->RotateX(90);
	//	translation->RotateZ(90);
	//}
	//else if (m_orientation == 1) {
	//	translation->RotateX(90);
	//	translation->RotateY(90);
	//}


	contour->SetRepresentation(contourRep);
	contour->FollowCursorOn();
	contour->CreateDefaultRepresentation();
	contour->On();
	contour->EnabledOn();

	renderWindow[0]->Render();
}

void MultiWidgetsController::paintBrush()
{
	vtkSmartPointer<callback> callback1 =
		vtkSmartPointer<callback>::New();
	cout << "press" << endl;

	renderWindow[0]->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, callback1);

	//renderWindow[0]->HasObserver()
}

callback * callback::New()
{
	return new callback;
}

void callback::Execute(vtkObject * caller, unsigned long eventId, void * callData)
{
	switch (eventId)
	{
	case LeftButtonPressEvent:
		cout << "press" << endl;
		break;
	default:
		break;
	}
}

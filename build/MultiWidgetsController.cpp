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

void MultiWidgetsController::setRenderWindows(vtkRenderWindow ** renderWindow)
{
	for (int i = 0; i < NUMOFRENDERERWINDOWS; ++i) {
		this->renderWindow[i] = renderWindow[i];
		renderWindow[i]->SetNumberOfLayers(2);
		renderWindow[i]->GetRenderers()->GetFirstRenderer()->SetLayer(0);
		renderWindow[i]->AddRenderer(renderers[i]);
	}
	
}


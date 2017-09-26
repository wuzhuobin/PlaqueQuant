#include "InteractorStyleWindowLevelThreshold.h"

#include "ImageViewer.h"

#include <vtkLookupTable.h>
#include <vtkImageThreshold.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkImageBlend.h>
#include <vtkExtractVOI.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>

vtkStandardNewMacro(InteractorStyleWindowLevelThreshold);

void InteractorStyleWindowLevelThreshold::SetOutputLabel(int label)
{
	vtkLookupTable* lookupTable = GetImageViewer()->GetLookupTable();
	if (label >= lookupTable->GetNumberOfTableValues()) {
		return;
	}
	m_label = label;
}

void InteractorStyleWindowLevelThreshold::ThresholdToOverlay()
{
	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputData(GetImageViewer()->GetInput());
	extractVOI->SetVOI(GetImageViewer()->GetDisplayExtent());
	extractVOI->Update();



	vtkSmartPointer<vtkImageThreshold> threshold =
		vtkSmartPointer<vtkImageThreshold>::New();
	threshold->SetNumberOfThreads(16);
	threshold->SetInputConnection(extractVOI->GetOutputPort());
	//threshold->SetInputData(GetImageViewer()->GetInput());
	threshold->SetOutputScalarType(GetImageViewer()->GetOverlay()->GetScalarType());
	threshold->ThresholdByUpper(GetLevel()- GetWindow() * 0.5);
	//threshold->ThresholdBetween(GetLevel() - GetWindow()*0.5, GetLevel() + GetWindow()*0.5);
	threshold->ReplaceOutOn();
	threshold->SetOutValue(0);
	threshold->ReplaceInOn();
	threshold->SetInValue(m_label);
	threshold->Update();

	vtkSmartPointer<vtkImageBlend> imageBlend =
		vtkSmartPointer<vtkImageBlend>::New();
	imageBlend->AddInputData(GetImageViewer()->GetOverlay());
	imageBlend->AddInputConnection(threshold->GetOutputPort());
	imageBlend->SetOpacity(1, 1);
	imageBlend->Update();

	GetImageViewer()->GetOverlay()->ShallowCopy(imageBlend->GetOutput());
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStyleWindowLevelThreshold, GetImageViewer()->Render());
}

void InteractorStyleWindowLevelThreshold::SetThreshold(double lower, double upper)
{

	for (std::list<AbstractInteractorStyleImage*>::const_iterator cit =
		m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		InteractorStyleWindowLevelThreshold* windowLevel =
			InteractorStyleWindowLevelThreshold::SafeDownCast(*cit);
		if (windowLevel != nullptr && windowLevel->GetCustomEnabled() &&
			GetImageViewer()->GetInput() == windowLevel->GetImageViewer()->GetInput()) {
			windowLevel->SetThresholdByViewer(lower, upper);
		}
	}
}

void InteractorStyleWindowLevelThreshold::SetThresholdByViewer(double lower, double upper)
{
	double window = upper - lower;
	double level = (upper + lower) * 0.5;

	SetWindowByViewer(window);
	SetLevelByViewer(level);
}

void InteractorStyleWindowLevelThreshold::WindowLevel()
{
	int *size = this->GetImageViewer()->GetRenderWindow()->GetSize();
	double window = this->InitialWindow;
	double level = this->InitialLevel;

	// Compute normalized delta
	vtkRenderWindowInteractor *rwi = this->Interactor;

	this->WindowLevelCurrentPosition[0] = rwi->GetEventPosition()[0];
	this->WindowLevelCurrentPosition[1] = rwi->GetEventPosition()[1];

	double dx = 4.0 *
		(this->WindowLevelCurrentPosition[0] -
			this->WindowLevelStartPosition[0]) / size[0];
	double dy = 4.0 *
		(this->WindowLevelStartPosition[1] -
			this->WindowLevelCurrentPosition[1]) / size[1];

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
	double newLevel;
	newLevel = level - dy;

	// Stay away from zero and really

	if (fabs(newWindow) < 0.01)
	{
		newWindow = 0.01*(newWindow < 0 ? -1 : 1);
	}

	if (fabs(newLevel) < 0.01)
	{
		newLevel = 0.01*(newLevel < 0 ? -1 : 1);
	}
	SetThreshold(newLevel - 0.5*newWindow + 0.5, newLevel + 0.5*newWindow + 0.5);
}

void InteractorStyleWindowLevelThreshold::ResetWindowLevel()
{
	double *range = this->GetImageViewer()->GetInput()->GetScalarRange();
	SetThreshold(range[0], range[1]);
}


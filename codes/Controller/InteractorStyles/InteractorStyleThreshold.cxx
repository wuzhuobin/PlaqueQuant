#include "InteractorStyleThreshold.h"

#include <vtkObjectFactory.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageThreshold.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkImageMapToColors.h>
#include <vtkLookupTable.h>
#include <vtkExtractVOI.h>
#include <vtkImageBlend.h>
#include <vtkImageCast.h>
#include <vtkSmartPointer.h>

#include "ImageViewer.h"

vtkStandardNewMacro(InteractorStyleThreshold);

void InteractorStyleThreshold::SetThreshold(double lower, double upper)
{
	for (std::list<AbstractInteractorStyleImage*>::const_iterator cit =
		m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		InteractorStyleThreshold* threshold =
			InteractorStyleThreshold::SafeDownCast(*cit);
		if (threshold != nullptr && threshold->GetCustomEnabled() &&
			GetImageViewer()->GetInput() == threshold->GetImageViewer()->GetInput()) {
			threshold->SetThresholdByViewer(lower, upper);
		}
	}
}

void InteractorStyleThreshold::SetThresholdByViewer(double lower, double upper)
{
	m_threshold->ThresholdBetween(lower, upper);
	GetImageViewer()->Render();
}

void InteractorStyleThreshold::SetOutputLabel(int label)
{
	m_label = label;
	m_threshold->SetInValue(label);
	if (m_previewFlag) {
		GetImageViewer()->Render();
	}
}

void InteractorStyleThreshold::ThresholdToOverlay()
{
	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputConnection(m_cast->GetOutputPort());
	extractVOI->SetVOI(GetImageViewer()->GetDisplayExtent());
	extractVOI->Update();


	vtkSmartPointer<vtkImageBlend> imageBlend =
		vtkSmartPointer<vtkImageBlend>::New();
	imageBlend->AddInputData(m_tmpOverlay);
	imageBlend->AddInputConnection(extractVOI->GetOutputPort());
	imageBlend->SetOpacity(1, 1);
	imageBlend->Update();
	m_tmpOverlay->ShallowCopy(imageBlend->GetOutput());
}

void InteractorStyleThreshold::SetPreview(bool flag)
{
	m_previewFlag = flag;
	if (flag) {
		GetImageViewer()->GetOverlayImageMapToColors()->SetInputConnection(m_cast->GetOutputPort());
	}
	else {
		GetImageViewer()->GetOverlayImageMapToColors()->SetInputData(m_tmpOverlay);
	}
	GetImageViewer()->Render();
}

void InteractorStyleThreshold::PreviewOn()
{
	SetPreview(false);
}

void InteractorStyleThreshold::PreviewOff()
{
	SetPreview(false);
}

void InteractorStyleThreshold::SetCustomEnabled(bool flag)
{
	InteractorStyleWindowLevel::SetCustomEnabled(flag);
	if (flag) {
		if (!m_threshold) {
			m_threshold = vtkSmartPointer<vtkImageThreshold>::New();
			m_threshold->SetInputData(GetImageViewer()->GetInput());
			m_threshold->SetNumberOfThreads(16);
			//m_threshold->ThresholdBetween(m_lowerThreshold, m_upperThreshold);
			m_threshold->SetOutValue(0);
			m_threshold->ReplaceOutOn();
			m_threshold->SetInValue(m_label);
			m_threshold->ReplaceInOn();
		}
		if (!m_cast) {
			m_cast = vtkSmartPointer<vtkImageCast>::New();
			m_cast->SetInputConnection(m_threshold->GetOutputPort());
			m_cast->SetOutputScalarType(GetImageViewer()->GetOverlay()->GetScalarType());
			m_cast->SetNumberOfThreads(16);
		}

		// replace the original overlay with the threshold output and save it in a pointer
		m_tmpOverlay = GetImageViewer()->GetOverlay();
		SetPreview(m_previewFlag);
	}
	else {
		GetImageViewer()->GetOverlayImageMapToColors()->SetInputData(m_tmpOverlay);
		if(m_threshold){
			m_threshold = nullptr;
		}
		if (m_cast) {
			m_cast = nullptr;
		}
	}
}

void InteractorStyleThreshold::StartWindowLevel()
{
	// Compute normalized delta
	vtkRenderWindowInteractor *rwi = this->Interactor;

	this->WindowLevelStartPosition[0] = rwi->GetEventPosition()[0];
	this->WindowLevelStartPosition[1] = rwi->GetEventPosition()[1];


	this->InitialWindow = m_threshold->GetUpperThreshold() - m_threshold->GetLowerThreshold();
	this->InitialLevel = 0.5*(m_threshold->GetUpperThreshold() + m_threshold->GetLowerThreshold());
}

void InteractorStyleThreshold::WindowLevel()
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
	
	SetThreshold(newLevel - 0.5*newWindow, newLevel + 0.5*newWindow);
	//SetWindow(newWindow);
	//SetLevel(newLevel);
}

void InteractorStyleThreshold::ResetWindowLevel()
{
	double *range = this->GetImageViewer()->GetInput()->GetScalarRange();
	SetThreshold(range[0], range[1]);
}

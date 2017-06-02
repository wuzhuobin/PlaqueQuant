/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			Junior Research Assistant

Author:		Lok, Ka Hei Jason
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

This class allows interactive modification to window and level.


Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#include "InteractorStyleWindowLevel.h"

#include "ImageViewer.h"

#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageProperty.h>
#include <vtkImageActor.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkImageData.h>

vtkStandardNewMacro(InteractorStyleWindowLevel);

void InteractorStyleWindowLevel::SetWindow(double window)
{
	for (std::list<AbstractInteractorStyleImage*>::const_iterator cit =
		m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		InteractorStyleWindowLevel* windowLevel =
			InteractorStyleWindowLevel::SafeDownCast(*cit);
		if (windowLevel != nullptr && windowLevel->GetCustomEnabled() &&
			GetImageViewer()->GetInput() == windowLevel->GetImageViewer()->GetInput()) {
			windowLevel->SetWindowByViewer(window);
		}
	}
}

void InteractorStyleWindowLevel::SetLevel(double level)
{
	for (std::list<AbstractInteractorStyleImage*>::const_iterator cit =
		m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		InteractorStyleWindowLevel* windowLevel =
			InteractorStyleWindowLevel::SafeDownCast(*cit);
		if (windowLevel != nullptr && windowLevel->GetCustomEnabled() &&
			GetImageViewer()->GetInput() == windowLevel->GetImageViewer()->GetInput()) {
			windowLevel->SetLevelByViewer(level);
		}
	}
}

void InteractorStyleWindowLevel::SetWindowByViewer(double window)
{
	if (GetWindow() != window) {
		GetImageViewer()->SetColorWindow(window);
		GetImageViewer()->Render();
	}
}

void InteractorStyleWindowLevel::SetLevelByViewer(double level)
{
	if (GetLevel() != level) {
		GetImageViewer()->SetColorLevel(level);
		GetImageViewer()->Render();
	}
}

void InteractorStyleWindowLevel::SetWindowLevelByViewer(double window, double level)
{
	SetWindowByViewer(window);
	SetLevelByViewer(level);
}

InteractorStyleWindowLevel::InteractorStyleWindowLevel()
	:AbstractNavigation()
{
}

InteractorStyleWindowLevel::~InteractorStyleWindowLevel()
{

}

void InteractorStyleWindowLevel::StartWindowLevel()
{

	// Compute normalized delta
	vtkRenderWindowInteractor *rwi = this->Interactor;

	this->WindowLevelStartPosition[0] = rwi->GetEventPosition()[0];
	this->WindowLevelStartPosition[1] = rwi->GetEventPosition()[1];

	this->InitialWindow = this->GetImageViewer()->GetColorWindow();
	this->InitialLevel = this->GetImageViewer()->GetColorLevel();

	//if (this->State != VTKIS_NONE)
	//{
	//	return;
	//}
	//this->StartState(VTKIS_WINDOW_LEVEL);

	//// Get the last (the topmost) image
	//this->SetCurrentImageToNthImage(0);

	//if (this->HandleObservers &&
	//	this->HasObserver(vtkCommand::StartWindowLevelEvent))
	//{
	//	this->InvokeEvent(vtkCommand::StartWindowLevelEvent, this);
	//}
	//else
	//{
	//	if (this->CurrentImageProperty)
	//	{
	//		vtkImageProperty *property = this->CurrentImageProperty;
	//		this->WindowLevelInitial[0] = property->GetColorWindow();
	//		this->WindowLevelInitial[1] = property->GetColorLevel();
	//	}
	//}
}

void InteractorStyleWindowLevel::OnMouseMove()
{
	//vtkInteractorStyleImage::OnMouseMove();
	AbstractNavigation::OnMouseMove();
	if (m_leftFunctioning) {
		WindowLevel();
	}


}

void InteractorStyleWindowLevel::OnLeftButtonDown()
{
	//vtkInteractorStyleImage::OnLeftButtonDown();
	AbstractNavigation::OnLeftButtonDown();
	StartWindowLevel();

	//this->WindowLevelInitial[0] = m_imageViewer->GetImageActor()->
	//	GetProperty()->GetColorWindow();
	//this->WindowLevelInitial[1] = m_imageViewer->GetImageActor()->
	//	GetProperty()->GetColorLevel();
}

void InteractorStyleWindowLevel::OnLeftButtonUp()
{
	//vtkInteractorStyleImage::OnLeftButtonUp();
	AbstractNavigation::OnLeftButtonUp();
}

void InteractorStyleWindowLevel::WindowLevel()
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
	SetWindow(newWindow);
	SetLevel(newLevel);
}

void InteractorStyleWindowLevel::ResetWindowLevel()
{
	//this->GetImageViewer()->GetInputAlgorithm()->UpdateInformation();
	//vtkStreamingDemandDrivenPipeline::SetUpdateExtent(
	//	this->GetImageViewer()->GetInputInformation(),
	//	vtkStreamingDemandDrivenPipeline::GetWholeExtent(
	//		this->IV->GetInputInformation()));
	//this->GetImageViewer()->GetInputAlgorithm()->Update();
	double *range = this->GetImageViewer()->GetInput()->GetScalarRange();
	this->SetWindow(range[1] - range[0]);
	this->SetLevel(0.5 * (range[1] + range[0]));
}

//void InteractorStyleWindowLevel::OnKeyPress()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		SetWindowLevelByViewer(255,	127.5);
//		//AbstractNavigation::OnKeyPress();
//
//	}
//	else {
//		AbstractNavigation::OnKeyPress();
//	}
//
//}
//
//void InteractorStyleWindowLevel::OnKeyDown()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		//AbstractNavigation::OnKeyPress();
//
//	}
//	else {
//		AbstractNavigation::OnKeyDown();
//	}
//}
//
//void InteractorStyleWindowLevel::OnKeyRelease()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		//AbstractNavigation::OnKeyPress();
//
//	}
//	else {
//		AbstractNavigation::OnKeyRelease();
//	}
//}
//
//void InteractorStyleWindowLevel::OnKeyUp()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		//AbstractNavigation::OnKeyPress();
//
//	}
//	else {
//		AbstractNavigation::OnKeyUp();
//	}
//}
//
void InteractorStyleWindowLevel::OnChar()
{
	//vtkInteractorStyleImage::OnChar();
	char key = this->Interactor->GetKeyCode();
	switch (key)
	{
	case 'r':
	case 'R':
		ResetWindowLevel();
		//SetWindow(255);
		//SetLevel(122.5);
		//AbstractNavigation::OnChar();
		//SynchronalZooming();
		break;
	default:
		AbstractNavigation::OnChar();
		break;
	}
}


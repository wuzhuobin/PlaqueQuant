/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant
This class is based on vtkInteractorStyleSwitch, written to allow easy
switching between 2D interactors.
Wong Matthew Lun
Copyright (C) 2016
*/


#include <vtkImageInterpolator.h>
#include <vtkInteractorStyle.h>
#include "InteractorStyleSwitch.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

vtkStandardNewMacro(InteractorStyleSwitch);

InteractorStyleSwitch::InteractorStyleSwitch()
{
	InteractorStyleTesting = vtkInteractorStyleImage::New();
	WindowLevel = InteractorStyleWindowLevel::New();
	Navigation = InteractorStyleNavigation::New();
	PolygonDraw = InteractorStylePolygonDraw::New();
	PaintBrush = InteractorStylePaintBrush::New();
	ROI = InteractorStyleROI::New();
	Ruler = InteractorStyleRuler::New();
	SmartContour = InteractorStyleSmartContour::New();

	allStyles.push_back(InteractorStyleTesting);
	allStyles.push_back(WindowLevel);
	allStyles.push_back(Navigation);
	allStyles.push_back(PolygonDraw);
	allStyles.push_back(PaintBrush);
	allStyles.push_back(ROI);
	allStyles.push_back(Ruler);
	allStyles.push_back(SmartContour);

	this->CurrentStyle = 0;
}

InteractorStyleSwitch::~InteractorStyleSwitch()
{
	if (InteractorStyleTesting != NULL) {
		InteractorStyleTesting->Delete();
		InteractorStyleTesting = 0;
	}

	if (WindowLevel != NULL) {
		WindowLevel->Delete();
		WindowLevel = NULL;
	}

	if (Navigation != NULL) {
		Navigation->Delete();
		Navigation = NULL;
	}

	if (PolygonDraw != NULL) {
		PolygonDraw->Delete();
		PolygonDraw = NULL;
	}

	if (PaintBrush != NULL) {
		PaintBrush->Delete();
		PaintBrush = NULL;
	}

	if (ROI != NULL) {
		ROI->Delete();
		ROI = NULL;
	}
	if (Ruler != NULL) {
		Ruler->Delete();
		Ruler = NULL;
	}
	if (SmartContour != NULL) {
		SmartContour->Delete();
		SmartContour = NULL;
	}
	/* Do not modifiy the upper code£¡ */
	//for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
	//	it != allStyles.end(); ++it) {
	//	if ((*it) != NULL) {
	//		(*it)->Delete();
	//		(*it) = NULL;
	//	}
	//}
	allStyles.clear();

}

void InteractorStyleSwitch::InternalUpdate()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();

	if (this->CurrentStyle != this->PolygonDraw) 
		this->PolygonDraw->SetPolygonModeEnabled(false);
	if (this->CurrentStyle != this->PaintBrush)
		this->PaintBrush->SetPaintBrushModeEnabled(false);
	if (this->CurrentStyle != this->Ruler)
		this->Ruler->SetDistanceWidgetEnabled(false);
	if (this->CurrentStyle != this->ROI)
		this->ROI->SetPlaneWidgetEnabled(false);
	if (this->CurrentStyle != this->SmartContour)
		//this->SmartContour->SetSmartContourEnable(false);
		this->SmartContour->SetSmartContourEnable(false);


	// some special cases need to use InternalUpdate() to enabled
	if (this->CurrentStyle == this->ROI)
		this->ROI->SetPlaneWidgetEnabled(true);
	if (this->CurrentStyle == this->Ruler)
		this->Ruler->SetDistanceWidgetEnabled(true);
	if (this->CurrentStyle == this->SmartContour)
		//this->SmartContour->SetSmartContourEnable(true);
		this->SmartContour->SetSmartContourEnable(true);

}

void InteractorStyleSwitch::SetAutoAdjustCameraClippingRange(int value)
{
	if (value == this->AutoAdjustCameraClippingRange)
	{
		return;
	}

	if (value < 0 || value > 1)
	{
		vtkErrorMacro("Value must be between 0 and 1 for" <<
			" SetAutoAdjustCameraClippingRange");
		return;
	}
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		(*it)->SetAutoAdjustCameraClippingRange(value);
	}
	this->Modified();
}

void InteractorStyleSwitch::SetDefaultRenderer(vtkRenderer* renderer)
{
	vtkInteractorStyle::SetDefaultRenderer(renderer);

	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		(*it)->SetDefaultRenderer(renderer);
	}
}

void InteractorStyleSwitch::SetCurrentRenderer(vtkRenderer* renderer)
{
	vtkInteractorStyle::SetCurrentRenderer(renderer);
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		(*it)->SetCurrentRenderer(renderer);
	}
}

void InteractorStyleSwitch::SetImageViewer(MyImageViewer* imageViewer)
{
	//this->imageViewer = imageViewer;
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		AbstractInteractorStyleImage* _style = 
			dynamic_cast<AbstractInteractorStyleImage*>(*it);
		if (_style != NULL) {
			_style->SetImageViewer(imageViewer);
		}
	}
}

void InteractorStyleSwitch::AddSynchronalViewer(MyImageViewer * imageViewer)
{
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		AbstractInteractorStyleImage* _style =
			dynamic_cast<AbstractInteractorStyleImage*>(*it);
		if (_style != NULL) {
			_style->AddSynchronalViewer(imageViewer);
		}
	}
}

void InteractorStyleSwitch::SetSynchronalViewers(std::list<MyImageViewer*> synchronalViewers)
{
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		AbstractInteractorStyleImage* _style =
			dynamic_cast<AbstractInteractorStyleImage*>(*it);
		if (_style != NULL) {
			_style->SetSynchronalViewers(synchronalViewers);
		}
	}
}

void InteractorStyleSwitch::SetCurrentSlice(int slice)
{
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		AbstractInteractorStyleImage* _style =
			dynamic_cast<AbstractInteractorStyleImage*>(*it);
		if (_style != NULL) {
			_style->SetCurrentSlice(slice);
		}
	}
}

void InteractorStyleSwitch::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		AbstractInteractorStyleImage* _style =
			dynamic_cast<AbstractInteractorStyleImage*>(*it);
		if (_style != NULL) {
			_style->SetCurrentFocalPointWithImageCoordinate(i, j, k);
		}
	}
}

void InteractorStyleSwitch::SetEnabled(int i)
{
	Superclass::SetEnabled(i);
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		(*it)->SetEnabled(i);
	}
}

void InteractorStyleSwitch::SetEnabledOn()
{
	this->SetEnabled(1);
}

void InteractorStyleSwitch::SetEnabledOff()
{
	this->SetEnabled(0);
}


void InteractorStyleSwitch::SetInteractor(vtkRenderWindowInteractor *iren)
{
	if (iren == this->Interactor)
	{
		return;
	}
	// if we already have an Interactor then stop observing it
	if (this->Interactor)
	{
		this->Interactor->RemoveObserver(this->EventCallbackCommand);
	}
	this->Interactor = iren;
	// add observers for each of the events handled in ProcessEvents
	if (iren)
	{
		iren->AddObserver(vtkCommand::CharEvent,
			this->EventCallbackCommand,
			this->Priority);

		iren->AddObserver(vtkCommand::DeleteEvent,
			this->EventCallbackCommand,
			this->Priority);
	}
}



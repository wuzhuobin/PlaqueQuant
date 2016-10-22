/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor modified from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef INTERACTOR_STYLE__NAVIGATION_H
#define INTERACTOR_STYLE__NAVIGATION_H

#include "AbstractInteractorStyleImage.h"

#include <vtkEvent.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkCommand.h>


class MySeedRepresentation : public vtkSeedRepresentation
{
public:
	vtkTypeMacro(MySeedRepresentation, vtkSeedRepresentation);
	static MySeedRepresentation* New() { return new MySeedRepresentation; }
	
	vtkImageActorPointPlacer* GetImagePointPlacer() {
		return vtkImageActorPointPlacer::SafeDownCast(
			GetHandleRepresentation()->
			GetPointPlacer());
	}

protected:
	MySeedRepresentation() {
		SetHandleRepresentation(
			vtkSmartPointer<vtkPointHandleRepresentation3D>::New());
		GetHandleRepresentation()->SetPointPlacer(
			vtkSmartPointer<vtkImageActorPointPlacer>::New());
	}


};

class MySeedWidget : public vtkSeedWidget
{
public:
	vtkTypeMacro(MySeedWidget, vtkSeedWidget);
	static MySeedWidget* New() { return new MySeedWidget; }
protected:
	MySeedWidget() {
		this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent,
			vtkWidgetEvent::Move,
			this, MySeedWidget::MoveAction);
		this->CallbackMapper->SetCallbackMethod(vtkCommand::KeyPressEvent,
			vtkEvent::NoModifier, 127, 1, "Delete",
			vtkWidgetEvent::Delete,
			this, MySeedWidget::DeleteAction);
		this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent, 
			vtkWidgetEvent::Completed,
			this, MySeedWidget::CompletedAction);
	}
	static void MoveAction(vtkAbstractWidget *w);
	static void DeleteAction(vtkAbstractWidget *w);
	static void CompletedAction(vtkAbstractWidget *w) {};
};

class InteractorStyleSmartContour : public AbstractInteractorStyleImage
{
public:
	vtkTypeMacro(InteractorStyleSmartContour, AbstractInteractorStyleImage);
	static InteractorStyleSmartContour* New();

	void SetSmartContourEnable(bool flag);
	void SetFocalSeed(int i);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
	void ReloadSeedFromList();

protected:
	InteractorStyleSmartContour();
	~InteractorStyleSmartContour();

	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnChar();
	virtual void OnKeyPress();
	virtual void OnLeave();


private:
	//static vtkSmartPointer<vtkPoints> m_seeds;
	/**
	* Using a static list to save all seeds and it will be shared by all other
	* InteractorStyleSmartContour instances
	*/
	vtkSmartPointer<MySeedRepresentation> m_seedRep;
	vtkSmartPointer<MySeedWidget> m_seedWidget;
	/**
	 * Invalide pick or any other error? CalculateIndex() will set m_world[3] = 
	 * { 0 };
	 */
	double m_world[3] = { 0 };
	bool m_renderingFlag = false;

	int m_focalSeed = 2;

	void CalculateIndex();
	void UpdateSeedWidgetBefore();
	void UpdateSeedWidgetAfter();
	void UpdateFocalSeed();
	void ClearAllSeedWidget();

};

#endif // !INTERACTOR_STYLE__NAVIGATION_H

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


#ifndef __INTERACTOR_STYLE_SEEDS_PLACER_H__
#define __INTERACTOR_STYLE_SEEDS_PLACER_H__

#include "AbstractNavigation.h"

#include <vtkEvent.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetCallbackMapper.h>

#include "InteractorStyleSmartContour.h"
class InteractorStyleSeedsPlacer;
class SeedsPlacerRepresentation : public vtkSeedRepresentation
{
public:
	vtkTypeMacro(SeedsPlacerRepresentation, vtkSeedRepresentation);
	static SeedsPlacerRepresentation* New() { return new SeedsPlacerRepresentation; }
	
	vtkImageActorPointPlacer* GetImagePointPlacer() {
		return vtkImageActorPointPlacer::SafeDownCast(
			GetHandleRepresentation()->
			GetPointPlacer());
	}

protected:
	SeedsPlacerRepresentation() {
		SetHandleRepresentation(
			vtkSmartPointer<vtkPointHandleRepresentation3D>::New());
		GetHandleRepresentation()->SetPointPlacer(
			vtkSmartPointer<vtkImageActorPointPlacer>::New());
	}


};

class SeedsPlacerWidget : public vtkSeedWidget
{
public:
	vtkTypeMacro(SeedsPlacerWidget, vtkSeedWidget);
	static SeedsPlacerWidget* New() { return new SeedsPlacerWidget; }
protected:
	SeedsPlacerWidget();
	//static void MoveAction(vtkAbstractWidget *w){
	//	w->InvokeEvent(vtkCommand::LeftButtonPressEvent);
	//}
	//static void DeleteAction(vtkAbstractWidget *w) {}
	//static void CompletedAction(vtkAbstractWidget *w) {}

private:
	friend class InteractorStyleSeedsPlacer;
	InteractorStyleSeedsPlacer* seedsPlacer;
	static void CallbackSave(vtkObject* caller,
		unsigned long eid, void* clientdata, void *calldata);
	static int oldImagePos[3];
};

class InteractorStyleSeedsPlacer : public AbstractNavigation
{
public:                                                                                                                                                                             
	vtkTypeMacro(InteractorStyleSeedsPlacer, AbstractNavigation);
	static InteractorStyleSeedsPlacer* New();
	
	enum SeedsType {
		SegmentationSeeds = 0,
		ObliqueViewSeeds = 1
	};

	void SetCurrentSeedsType(SeedsType);
	SeedsType GetCurrentSeedsType();


	void SetSeedsPlacerEnable(bool flag);
	void SetFocalSeed(int i);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	virtual void ClearAllSeedWidget();
	virtual void GenerateWidgetFromSeeds();
	virtual void SaveWidgetToSeeds();
	virtual void DropSeed();

protected:
	InteractorStyleSeedsPlacer();
	~InteractorStyleSeedsPlacer();
	/**
	 * Supposed to only used in callback
	 */
	virtual void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);

	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnChar();
	virtual void OnKeyPress();
	virtual void OnLeave();



	/**
	* Using a static list to save all seeds and it will be shared by all other
	* InteractorStyleSeedsPlacer instances
	*/
	static std::list<int*> m_seeds[2];
	static int m_oldSeedsSize;
	void ClearAllSeeds();

	/* This property decide which type of seeds this placer is controlling */
	SeedsType m_currentSeedsType;
	vtkSmartPointer<SeedsPlacerWidget> m_seedWidget;

private:
	friend class SeedsPlacerWidget;
	vtkSmartPointer<SeedsPlacerRepresentation> m_seedRep;
	/**
	 * Invalide pick or any other error? CalculateIndex() will set m_world[3] = 
	 * { 0 };
	 */
	double m_world[3] = { 0 };

	void CalculateIndex();


};

#endif // !__INTERACTOR_STYLE_SEEDS_PLACER_H__

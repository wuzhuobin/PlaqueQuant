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


#ifndef INTERACTOR_STYLE_SWITCH_H
#define INTERACTOR_STYLE_SWITCH_H

#include <vtkSetGet.h>
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleSwitchBase.h>

#include <list>

#include "QInteractorStyleNavigation.h"
#include "QInteractorStyleWindowLevel.h"
#include "QInteractorStyleVesselSegmentation.h"
#include "QInteractorStyleLumenSeedsPlacer.h"
#include "QInteractorStylePaintBrush.h"
#include "InteractorStyleROI.h"
#include "QInteractorStyleRuler.h"
//#include "InteractorStyleSmartContour.h"
//#include "InteractorStyleSmartContour2.h"
#include "StyleSwitchMacro.h"

class InteractorStyleSwitch : public vtkInteractorStyleSwitchBase
{
public:
	vtkTypeMacro(InteractorStyleSwitch, vtkInteractorStyleSwitchBase);
	static InteractorStyleSwitch* New();

	vtkGetMacro(InteractorStyleTesting, vtkInteractorStyleImage*);
	vtkGetMacro(WindowLevel, QInteractorStyleWindowLevel*);
	vtkGetMacro(Navigation, QInteractorStyleNavigation*);
	vtkGetMacro(PolygonDraw, QInteractorStyleVesselSegmentation*);
	vtkGetMacro(SeedsPlacer, QInteractorStyleLumenSeedsPlacer*);
	vtkGetMacro(PaintBrush, QInteractorStylePaintBrush*);
	vtkGetMacro(ROI, InteractorStyleROI*);
	vtkGetMacro(Ruler, QInteractorStyleRuler*);
	//vtkGetMacro(SmartContour, InteractorStyleSmartContour*);
	//vtkGetMacro(SmartContour2, InteractorStyleSmartContour2*);

	void SetInteractor(vtkRenderWindowInteractor *iren);

	SetInteractorStyleMacro(InteractorStyleTesting);
	SetInteractorStyleMacro(Navigation);
	//void SetInteractorStyleToNavigation();
	SetInteractorStyleMacro(WindowLevel);
	SetInteractorStyleMacro(PolygonDraw);
	SetInteractorStyleMacro(SeedsPlacer);
	SetInteractorStyleMacro(PaintBrush);
	SetInteractorStyleMacro(ROI);
	SetInteractorStyleMacro(Ruler);
	//SetInteractorStyleMacro(SmartContour);
	//SetInteractorStyleMacro(SmartContour2);

	CurrentStyleMacro(InteractorStyleTesting);
	CurrentStyleMacro(Navigation);
	CurrentStyleMacro(WindowLevel);
	CurrentStyleMacro(PolygonDraw);
	CurrentStyleMacro(SeedsPlacer);
	CurrentStyleMacro(PaintBrush);
	CurrentStyleMacro(ROI);
	CurrentStyleMacro(Ruler);
	//CurrentStyleMacro(SmartContour);
	//CurrentStyleMacro(SmartContour2);

	virtual void SetDefaultRenderer(vtkRenderer* renderer);
	virtual void SetCurrentRenderer(vtkRenderer* renderer);
	virtual void SetImageViewer(MyImageViewer* m_imageViewer);

	virtual void SetEnabled(int);
	virtual void SetEnabledOn();
	virtual void SetEnabledOff();

protected:
	InteractorStyleSwitch();
	~InteractorStyleSwitch();

	void InternalUpdate();
	void SetAutoAdjustCameraClippingRange(int value);

private:

	vtkInteractorStyleImage* InteractorStyleTesting;
	QInteractorStyleNavigation*	Navigation;
	QInteractorStyleWindowLevel* WindowLevel;
	QInteractorStyleVesselSegmentation* PolygonDraw;
	QInteractorStyleLumenSeedsPlacer* SeedsPlacer;
	QInteractorStylePaintBrush*	PaintBrush;
	InteractorStyleROI* ROI;
	QInteractorStyleRuler* Ruler;
	//InteractorStyleSmartContour* SmartContour;
	//InteractorStyleSmartContour2* SmartContour2;
	vtkInteractorStyle*	CurrentStyle;
	std::list<vtkInteractorStyle*> allStyles;
};



#endif
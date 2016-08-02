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

#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleSwitchBase.h>
#include "InteractorStyleNavigation.h"
#include "InteractorStyleWindowLevel.h"
#include "InteractorStylePolygonDraw.h"
#include "InteractorStylePaintBrush.h"
#include "StyleSwitchMacro.h"

class InteractorStyleNavigation;
class InteractorStyleWindowLevel;
class InteractorStylePaintBrush;

class InteractorStyleSwitch : public vtkInteractorStyleSwitchBase
{
public:
	vtkTypeMacro(InteractorStyleSwitch, vtkInteractorStyleSwitchBase);
	static InteractorStyleSwitch* New();

	vtkGetMacro(WindowLevel, InteractorStyleWindowLevel*);
	vtkGetMacro(Navigation, InteractorStyleNavigation*);
	//vtkGetMacro(PolygonDraw, InteractorStylePolygonDraw*);
	vtkGetMacro(PaintBrush, InteractorStylePaintBrush*);

	void SetInteractor(vtkRenderWindowInteractor *iren);

	SetInteractorStyleMacro(Navigation);
	SetInteractorStyleMacro(WindowLevel);
	//SetInteractorStyleMacro(PolygonDraw);
	SetInteractorStyleMacro(PaintBrush);

	CurrentStyleMacro(Navigation);
	CurrentStyleMacro(WindowLevel);
	//CurrentStyleMacro(PolygonDraw);
	CurrentStyleMacro(PaintBrush);

	//bool CurrentStyleIsNavigation();
	//bool CurrentStyleIsWindowLevel();
	//bool CurrentStyleIsTransform();

	virtual void SetDefaultRenderer(vtkRenderer* renderer);
	virtual void SetCurrentRenderer(vtkRenderer* renderer);
	virtual void SetViewers(MyImageViewer* imageViewer);
	virtual void initializeQWidget(QSpinBox* sliceX, QSpinBox* sliceY, QSpinBox* sliceZ, 
		QDoubleSpinBox* window, QDoubleSpinBox* level, 
		QSpinBox* drawBrushSize,
		QComboBox* drawShape,
		QCheckBox* drawVolumetric,
		QCheckBox* drawIsotropic);

protected:
	InteractorStyleSwitch();
	~InteractorStyleSwitch();

	void InternalUpdate();
	void SetAutoAdjustCameraClippingRange(int value);


	InteractorStyleNavigation*	Navigation;
	InteractorStyleWindowLevel* WindowLevel;
	//InteractorStylePolygonDraw* PolygonDraw;
	InteractorStylePaintBrush*	PaintBrush;
	vtkInteractorStyle*			CurrentStyle;
};



#endif
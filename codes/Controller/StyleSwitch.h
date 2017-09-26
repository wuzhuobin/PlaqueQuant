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


#ifndef __STYLE_SWITCH_H__
#define __STYLE_SWITCH_H__

#include "InteractorStyleSwitch.h"

#include "QInteractorStyleNavigation.h"
#include "QInteractorStyleWindowLevel.h"
#include "QInteractorStyleWindowLevelThreshold.h"
#include "QInteractorStyleThreshold.h"
#include "QInteractorStylePaintBrush.h"
#include "QInteractorStyleVOI.h"
#include "QInteractorStyleLumenSeedsPlacer.h"
#include "QInteractorStyleVBDSmokerSeeds.h"
#include "QInteractorStyleTubularVOI.h"
#include "QInteractorStyleMaximumWallThickness.h"
#include "QInteractorStyleRuler.h"
#include "InteractorStylePolygonDraw.h"
#include "QInteractorStylePolygonDrawSeries.h"
#include "QInteractorStyleVesselSegmentation2.h"

class StyleSwitch : public InteractorStyleSwitch
{
public:
	static StyleSwitch* New() { return new StyleSwitch; }
	vtkTypeMacro(StyleSwitch, InteractorStyleSwitch);

	vtkGetMacro(WindowLevel, QInteractorStyleWindowLevel*);
	SetInteractorStyleMacro(WindowLevel);
	CurrentStyleMacro(WindowLevel);

	vtkGetMacro(WindowLevelThreshold, QInteractorStyleWindowLevelThreshold*);
	SetInteractorStyleMacro(WindowLevelThreshold);
	CurrentStyleMacro(WindowLevelThreshold);

	vtkGetMacro(Threshold, QInteractorStyleThreshold*);
	SetInteractorStyleMacro(Threshold);
	CurrentStyleMacro(Threshold);

	vtkGetMacro(Navigation, QInteractorStyleNavigation*);
	SetInteractorStyleMacro(Navigation);
	CurrentStyleMacro(Navigation);

	vtkGetMacro(PaintBrush, QInteractorStylePaintBrush*);
	SetInteractorStyleMacro(PaintBrush);
	CurrentStyleMacro(PaintBrush);

	vtkGetMacro(LumenSeedsPlacer, QInteractorStyleLumenSeedsPlacer*);
	SetInteractorStyleMacro(LumenSeedsPlacer);
	CurrentStyleMacro(LumenSeedsPlacer);

	vtkGetMacro(VOI, QInteractorStyleVOI*);
	SetInteractorStyleMacro(VOI);
	CurrentStyleMacro(VOI);

	vtkGetMacro(TubularVOI, QInteractorStyleTubularVOI*);
	SetInteractorStyleMacro(TubularVOI);
	CurrentStyleMacro(TubularVOI);

	vtkGetMacro(Ruler, QInteractorStyleRuler*);
	SetInteractorStyleMacro(Ruler);
	CurrentStyleMacro(Ruler);

	vtkGetMacro(MaximumWallThickness, QInteractorStyleMaximumWallThickness*);
	SetInteractorStyleMacro(MaximumWallThickness);
	CurrentStyleMacro(MaximumWallThickness);

	vtkGetMacro(PolygonDraw, InteractorStylePolygonDraw*);
	SetInteractorStyleMacro(PolygonDraw);
	CurrentStyleMacro(PolygonDraw);

	vtkGetMacro(PolygonDrawSeries, QInteractorStylePolygonDrawSeries*);
	SetInteractorStyleMacro(PolygonDrawSeries);
	CurrentStyleMacro(PolygonDrawSeries);

	vtkGetMacro(VesselSegmentation2, QInteractorStyleVesselSegmentation2*);
	SetInteractorStyleMacro(VesselSegmentation2);
	CurrentStyleMacro(VesselSegmentation2);

	vtkGetMacro(VBDSmoker, QInteractorStyleVBDSmokerSeeds*);
	SetInteractorStyleMacro(VBDSmoker);
	CurrentStyleMacro(VBDSmoker);


protected:
	StyleSwitch() {
		// The first instance of QInteractorStyleNavigation should have control 
		// of the ui
		Navigation = QInteractorStyleNavigation::New();
		allStyles.push_back(Navigation);
		WindowLevel = QInteractorStyleWindowLevel::New();
		allStyles.push_back(WindowLevel);
		WindowLevelThreshold = QInteractorStyleWindowLevelThreshold::New();
		allStyles.push_back(WindowLevelThreshold);
		Threshold = QInteractorStyleThreshold::New();
		allStyles.push_back(Threshold);
		//PolygonDraw = QInteractorStyleVesselSegmentation::New();
		LumenSeedsPlacer = QInteractorStyleLumenSeedsPlacer::New();
		allStyles.push_back(LumenSeedsPlacer);
		PaintBrush = QInteractorStylePaintBrush::New();
		allStyles.push_back(PaintBrush);
		VOI = QInteractorStyleVOI::New();
		allStyles.push_back(VOI);
		TubularVOI = QInteractorStyleTubularVOI::New();
		allStyles.push_back(TubularVOI);
		Ruler = QInteractorStyleRuler::New();
		allStyles.push_back(Ruler);
		PolygonDraw = InteractorStylePolygonDraw::New();
		allStyles.push_back(PolygonDraw);
		PolygonDrawSeries = QInteractorStylePolygonDrawSeries::New();
		allStyles.push_back(PolygonDrawSeries);
		VesselSegmentation2 = QInteractorStyleVesselSegmentation2::New();
		allStyles.push_back(VesselSegmentation2);
		MaximumWallThickness = QInteractorStyleMaximumWallThickness::New();
		allStyles.push_back(MaximumWallThickness);
		VBDSmoker = QInteractorStyleVBDSmokerSeeds::New();
		allStyles.push_back(VBDSmoker);
	}

	QInteractorStyleNavigation*	Navigation;
	QInteractorStyleWindowLevel* WindowLevel;
	QInteractorStyleThreshold* Threshold;
	QInteractorStyleWindowLevelThreshold* WindowLevelThreshold;
	QInteractorStyleLumenSeedsPlacer* LumenSeedsPlacer;
	QInteractorStylePaintBrush*	PaintBrush;
	QInteractorStyleVOI* VOI;
	QInteractorStyleTubularVOI* TubularVOI;
	QInteractorStyleRuler* Ruler;
	QInteractorStyleMaximumWallThickness* MaximumWallThickness;
	InteractorStylePolygonDraw* PolygonDraw;
	QInteractorStylePolygonDrawSeries* PolygonDrawSeries;
	QInteractorStyleVesselSegmentation2* VesselSegmentation2;
	QInteractorStyleVBDSmokerSeeds* VBDSmoker;

	//QInteractorStyleRuler* Ruler;
};



#endif
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

This class allows interactive segmentation on images with contour widget.

Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#pragma once

#include <QTime>

#include <vtkContourWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include "AbstractInteractorStyleImage.h"


class InteractorStylePolygonDraw : public AbstractInteractorStyleImage
{
public:
	vtkTypeMacro(InteractorStylePolygonDraw, AbstractInteractorStyleImage);
	static InteractorStylePolygonDraw* New();

	void SetPolygonModeEnabled(bool b);
	void SetVesselWallLabel(int vesselWallLabel);
	void SetLumenWallLabel(int lumenWallLabel);
	void EnableAutoLumenSegmentation(bool flag);
	void SetContourFilterGenerateValues(int generateValues);
	void GenerateLumenWallContourWidget();

protected:
	InteractorStylePolygonDraw();
	~InteractorStylePolygonDraw();

	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnMouseMove();
	virtual void OnKeyPress();


private:
	bool CheckDoubleClicked();
	void DisplayPolygon(vtkObject*, long unsigned, void*);
	void FillPolygon();


	QTime m_timer;
	int m_firstClickTimeStamp;
	int m_generateValue = 60;

	bool DOUBLE_CLICKED_FLAG;
	bool CONTOUR_IS_ON_FLAG;
	bool AUTO_LUMEN_SEGMENTATION_ENABLE_FLAG;

	int vesselWallLabel = 1;
	int lumenWallLabel = 1;

	vtkContourWidget* m_vesselWallContourWidget;
	vtkOrientedGlyphContourRepresentation* m_vesselWallContourRepresentation;
	vtkContourWidget* m_lumenWallContourWidget;
	vtkOrientedGlyphContourRepresentation* m_lumenWallContourRepresentation;
	vtkSmartPointer<vtkCallbackCommand> m_callbackFunction;

};
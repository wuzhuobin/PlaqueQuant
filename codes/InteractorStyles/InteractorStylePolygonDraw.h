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

#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>

#include "AbstractNavigation.h"


class InteractorStylePolygonDraw : public AbstractNavigation
{
public:

	vtkTypeMacro(InteractorStylePolygonDraw, AbstractNavigation);
	static InteractorStylePolygonDraw* New();

	virtual void SetPolygonModeEnabled(bool b);
	virtual void NewContour();

	virtual void SetContourLabel(int contourLabel);
	
	virtual void SetSmoothCurveEnable();
	virtual void SetPolygonEnable();
	virtual void SetLineInterpolator(int i);
	virtual void FillPolygon();
	virtual void FillPolygon(std::list<vtkSmartPointer<vtkContourWidget>>* contour, int label);

protected:
	InteractorStylePolygonDraw();
	~InteractorStylePolygonDraw();

	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnMouseMove();
	virtual void OnKeyPress();


	virtual void ClearCurrentContour();
	virtual void CleanAllConoturs();

	bool m_polygonDrawEnabledFlag = false;


	// defaule label
	int m_contourLabel = 1;

	vtkSmartPointer<vtkContourLineInterpolator> m_interpolator = nullptr;
	std::list<vtkSmartPointer<vtkContourWidget>> m_contours;

	vtkContourWidget* m_currentContour = nullptr;
	vtkOrientedGlyphContourRepresentation* m_currentContourRep = nullptr;

};
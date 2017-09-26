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
#ifndef __INTERACTOR_STYLE_POLYGON_DRAW_SERIES_H__
#define __INTERACTOR_STYLE_POLYGON_DRAW_SERIES_H__


#include "AbstractNavigation.h"

#include <vtkSmartPointer.h>
class ContourWidgetSeries;
class ContourWidgetSeriesOrientedGlyphRepresentation;
//class vtkContourWidget;
//class vtkOrientedGlyphContourRepresentation;
//class vtkContourRepresentation;
class vtkPolygon;
//class vtkContourLineInterpolator;

class InteractorStylePolygonDrawSeries : public AbstractNavigation
{
public:
	enum Interpolator
	{
		LINEAR = 0,
		BEZIER = 1,
		MAGNETIC = 2
	};

	vtkTypeMacro(InteractorStylePolygonDrawSeries, AbstractNavigation);
	static InteractorStylePolygonDrawSeries* New();

	virtual void SetCustomEnabled(bool b);

	virtual void SetContourLabel(int contourLabel);
	virtual void SetInterpolator(int interpolator);
	virtual void SetLinearInterpolator() { SetInterpolator(LINEAR); }
	virtual void SetBezierInterpolator() { SetInterpolator(BEZIER); }
	virtual void SetMagneticInterpolator() { SetInterpolator(MAGNETIC); }

	
	virtual void FillContours();

	//virtual void FillPolygon(
	//	std::list<vtkSmartPointer<vtkContourWidget>>* contour, 
	//	unsigned char label);
	//virtual void FillPolygon(
	//	std::list<vtkSmartPointer<vtkPolygon>>* contourPolygon, 
	//	unsigned char label);

	//virtual void FillPolygon(
	//	std::list<vtkSmartPointer<vtkContourWidget>>* contour, 
	//	unsigned char label,
	//	int slice);

	/**
	 * Because the index of the sliceOrientation is wrong, it need to be specified
	 */
	//virtual void FillPolygon(
	//	std::list<vtkSmartPointer<vtkPolygon>>* contourPolygon, 
	//	unsigned char label,
	//	int slice);
	
	/**
	* For using delete contours one by one
	* if all contour is deleted, it set m_currentContour = nullptr
	*/
	//virtual void CleanCurrentContour();
	//virtual void CleanAllContours();
	//virtual void SetAllContoursEnabled(int flag);

protected:
	InteractorStylePolygonDrawSeries();
	~InteractorStylePolygonDrawSeries();

	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnKeyPress();

	void FillContours(ContourWidgetSeriesOrientedGlyphRepresentation* rep);
	void FillPolygon(vtkPolygon* polygon, int colour);



	// defaule label
	int m_contourLabel = 0;
	int m_interpolator = 1;

	vtkSmartPointer<ContourWidgetSeries> m_contourSeries = nullptr;
	vtkSmartPointer<ContourWidgetSeriesOrientedGlyphRepresentation> m_contourSeriesRep = nullptr;

	//vtkSmartPointer<vtkContourLineInterpolator> m_interpolator = nullptr;
	//std::list<vtkSmartPointer<vtkContourWidget>> m_contours;

	//vtkContourWidget* m_currentContour = nullptr;
	//vtkOrientedGlyphContourRepresentation* m_currentContourRep = nullptr;

};


#endif // !__INTERACTOR_STYLE_POLYGON_DRAW_SERIES_H__

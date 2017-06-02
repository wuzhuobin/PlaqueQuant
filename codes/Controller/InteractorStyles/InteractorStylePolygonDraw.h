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
#ifndef __INTERACTOR_STYLE_POLYGON_DRAW_H__
#define __INTERACTOR_STYLE_POLYGON_DRAW_H__


#include "AbstractNavigation.h"

#include <vtkSmartPointer.h>

class vtkContourWidget;
class vtkOrientedGlyphContourRepresentation;
class vtkContourRepresentation;
class vtkPolygon;
class vtkContourLineInterpolator;


class InteractorStylePolygonDraw : public AbstractNavigation
{
public:

	vtkTypeMacro(InteractorStylePolygonDraw, AbstractNavigation);
	static InteractorStylePolygonDraw* New();

	virtual void SetCustomEnabled(bool b);
	/**
	 * For better performance, NewContour() will not invoke EnabledOn()
	 * it need to be enable manually
	 */
	virtual void NewContour();

	virtual void SetContourLabel(unsigned char contourLabel);
	
	virtual void SetSmoothCurveEnable();
	virtual void SetPolygonEnable();
	virtual void SetLineInterpolator(int i);
	virtual void FillPolygon();

	virtual void FillPolygon(
		std::list<vtkSmartPointer<vtkContourWidget>>* contour, 
		unsigned char label);
	virtual void FillPolygon(
		std::list<vtkSmartPointer<vtkPolygon>>* contourPolygon, 
		unsigned char label);

	//virtual void FillPolygon(
	//	std::list<vtkSmartPointer<vtkContourWidget>>* contour, 
	//	unsigned char label,
	//	int slice);

	/**
	 * Because the index of the sliceOrientation is wrong, it need to be specified
	 */
	virtual void FillPolygon(
		std::list<vtkSmartPointer<vtkPolygon>>* contourPolygon, 
		unsigned char label,
		int slice);
	
	/**
	* For using delete contours one by one
	* if all contour is deleted, it set m_currentContour = nullptr
	*/
	virtual void CleanCurrentContour();
	virtual void CleanAllContours();
	virtual void SetAllContoursEnabled(int flag);

protected:
	InteractorStylePolygonDraw();
	~InteractorStylePolygonDraw();

	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnMouseMove();
	virtual void OnKeyPress();



	//bool m_polygonDrawEnabledFlag = false;


	// defaule label
	unsigned char m_contourLabel = 1;

	vtkSmartPointer<vtkContourLineInterpolator> m_interpolator = nullptr;
	std::list<vtkSmartPointer<vtkContourWidget>> m_contours;

	vtkContourWidget* m_currentContour = nullptr;
	vtkOrientedGlyphContourRepresentation* m_currentContourRep = nullptr;

};


#endif // !__INTERACTOR_STYLE_POLYGON_DRAW_H__

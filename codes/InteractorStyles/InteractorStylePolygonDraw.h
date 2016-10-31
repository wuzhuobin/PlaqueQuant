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
#include <vtkRenderWindowInteractor.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include "AbstractNavigation.h"


class InteractorStylePolygonDraw : public AbstractNavigation
{
public:

	enum CONTOUR_TYPE {
		NO_SEG = 0,
		VESSEL_WALL = 1,
		LUMEN = 2
	};

	vtkTypeMacro(InteractorStylePolygonDraw, AbstractNavigation);
	static InteractorStylePolygonDraw* New();

	void SetPolygonModeEnabled(bool b);
	void SetVesselWallLabel(int vesselWallLabel);
	void SetLumenWallLabel(int lumenWallLabel);
	void SetNoSegLabel(int noSegLabel);


	void EnableAutoLumenSegmentation(bool flag);
	void SetContourFilterGenerateValues(int generateValues);
	void GenerateLumenWallContourWidget();
	void SetLineInterpolator(int i);
	void FillPolygon();

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

	void ClearCurrentContour();
	void ClearAllConoturs();


	int m_firstClickTimeStamp;
	int m_generateValue = 60;


	bool m_polygonDrawEnabledFlag = false;
	bool DOUBLE_CLICKED_FLAG;
	bool CONTOUR_IS_ON_FLAG;
	bool AUTO_LUMEN_SEGMENTATION_ENABLE_FLAG;

	
	// contour initial type
	int m_contourType = NO_SEG;
	// defaule label
	int vesselWallLabel = 2;
	int lumenWallLabel = 1;
	int noSegLabel = 1;

	vtkContourWidget* m_vesselWallContourWidget;
	vtkOrientedGlyphContourRepresentation* m_vesselWallContourRepresentation;
	vtkContourWidget* m_lumenWallContourWidget;
	vtkOrientedGlyphContourRepresentation* m_lumenWallContourRepresentation;

	vtkSmartPointer<vtkContourLineInterpolator> m_interpolator = nullptr;
	/**
	* When refering to using index to represnet the image
	* Always first vessel wall then lumen
	*/
	std::list<vtkSmartPointer<vtkContourWidget>> m_vesselWallContourWidgets;
	std::list<vtkSmartPointer<vtkContourWidget>> m_lumenWallContourWidgets;
	std::list<vtkSmartPointer<vtkContourWidget>> m_noSegmentation;

	vtkContourWidget* m_currentContour = nullptr;
	vtkOrientedGlyphContourRepresentation* m_currentContourRep = nullptr;

};
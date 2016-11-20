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

#include "InteractorStylePolygonDraw.h"

class InteractorStyleVesselSegmentation : public InteractorStylePolygonDraw
{
public:

	enum CONTOUR_TYPE {
		// inifinte contours
		CONTOUR = 0, 
		// inifinte vessel wall contours for doing lumen segmentation
		VESSEL_WALL = 1, 
		LUMEN = 2
	};

	vtkTypeMacro(InteractorStyleVesselSegmentation, AbstractNavigation);
	static InteractorStyleVesselSegmentation* New();

	virtual void SetPolygonModeEnabled(bool b);

	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	virtual void NewContour();
	virtual void NewContour(int type, QList<vtkSmartPointer<vtkPolyData>>* list);
	
	virtual void ReadFromPolydata();
	virtual void ReadFromPolydata(int type);
	virtual void WriteToPolydata();

	void SetVesselWallLabel(int vesselWallLabel);
	void SetLumenWallLabel(int lumenWallLabel);
	void SetGenerateValue(int value);

	virtual void SetSegmentationMode(int i);
	virtual void EnableNoSegmentation();
	virtual void EnableLumenSegmentation();
	virtual void EnableVesselWallSegmentation();

	virtual void FillPolygon();


	void SetContourFilterGenerateValues(int generateValues);
	void GenerateLumenPolydata();
	void GenerateVesselWallPolyData();

protected:
	InteractorStyleVesselSegmentation();
	~InteractorStyleVesselSegmentation();

	virtual void OnLeftButtonUp();
	virtual void OnMouseMove();
	virtual void OnKeyPress();


	virtual void CleanAllConoturs();
	virtual void CleanConoturs(int type);

	int m_generateValue = 60;

	// contour initial type
	int m_contourType = CONTOUR;
	// defaule label
	int m_vesselWallLabel = 2;
	int m_lumenWallLabel = 1;


	/**
	* When refering to using index to represnet the image
	* Always first vessel wall then lumen
	*/
	std::list<vtkSmartPointer<vtkContourWidget>> m_vesselWallContourWidgets;
	std::list<vtkSmartPointer<vtkContourWidget>> m_lumenWallContourWidgets;

};
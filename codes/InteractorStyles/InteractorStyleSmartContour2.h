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

#include "AbstractNavigation.h"

#include <vtkContourWidget.h>
#include <vtkRenderWindowInteractor.h>


class InteractorStyleSmartContour2 : public AbstractNavigation
{
public:
	enum CONTOUR_TYPE {
		VESSEL_WALL = 0,
		LUMEN = 1
	};
	vtkTypeMacro(InteractorStyleSmartContour2, AbstractNavigation);
	static InteractorStyleSmartContour2* New();

	void SetVesselWallImage(vtkSmartPointer<vtkImageData> vesslWall);
	void SetLumenImage(vtkSmartPointer<vtkImageData> lumen);

	void SetSmartContour2Enable(bool b);
	void SetVesselWallLabel(int vesselWallLabel);
	void SetLumenWallLabel(int lumenWallLabel);
	void FillPolygon();
	void FillAllPolygons();
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

protected:
	InteractorStyleSmartContour2();
	~InteractorStyleSmartContour2();

	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnMouseMove();
	virtual void OnKeyPress();


private:
	/**
	 * @static
	 * Factory Method
	 * Produce a special, well specified vtkContourWidget
	 * @param	type, for specify the colour of contourwidget's line
	 * @return	the product of vtkCountourWidget
	 */
	static vtkSmartPointer<vtkContourWidget> MyContourWidgetFactory(int type);
	std::vector<std::vector<vtkSmartPointer<vtkPolyData>>*> m_vesselWallPolyData;
	std::vector<std::vector<vtkSmartPointer<vtkPolyData>>*> m_lumenPolyData;
	void ClearPolyDataList();


	void GeneratePolyData();
	void GeneratePolyData(int slice);

	void GenerateContour();
	void ClearAllContour();

	bool m_ContourIsOnFlag = false;

	int vesselWallLabel = 2;
	int lumenLabel = 1;

	static void ResequenceLumenWallPolyData(vtkPolyData* lumenWallPolyData);


	/**
	 * When refering to using index to represnet the image
	 * Always first vessel wall then lumen
	 */
	std::vector<vtkSmartPointer<vtkContourWidget>> m_vesselWallContourWidgets;
	std::vector<vtkSmartPointer<vtkContourWidget>> m_lumenWallContourWidgets;

	/* This saves the contour widget of the last slice*/
	int m_lastSlice;

	vtkSmartPointer<vtkImageData> m_vesselWallImage = nullptr;
	vtkSmartPointer<vtkImageData> m_lumenImage = nullptr;

};
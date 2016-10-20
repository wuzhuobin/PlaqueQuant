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

#include "AbstractInteractorStyleImage.h"

#include <vtkContourWidget.h>
#include <vtkRenderWindowInteractor.h>


class InteractorStyleSmartContour2 : public AbstractInteractorStyleImage
{
public:
	enum CONTOUR_TYPE {
		LUMEN = 0,
		VESSEL_WALL = 1
	};
	vtkTypeMacro(InteractorStyleSmartContour2, AbstractInteractorStyleImage);
	static InteractorStyleSmartContour2* New();

	void SetLumenImage(vtkImageData* lumen);
	void SetVesselWallImage(vtkImageData* vesslWall);

	void SetSmartContour2Enable(bool b);
	void SetVesselWallLabel(int vesselWallLabel);
	void SetLumenWallLabel(int lumenWallLabel);
	void FillPolygon();
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

	void GenerateContourWidget();
	void ClearAllContour();

	bool m_ContourIsOnFlag = false;

	int vesselWallLabel = 0;
	int lumenWallLabel = 0;

	static void ResequenceLumenWallPolyData(vtkPolyData* lumenWallPolyData);


	/**
	 * When refering to using index to represnet the image
	 * Always first lumen than vessel wall
	 */
	std::list<vtkSmartPointer<vtkContourWidget>> m_lumenWallContourWidgets;
	std::list<vtkSmartPointer<vtkContourWidget>> m_vesselWallContourWidgets;

	vtkImageData* m_lumenImage = nullptr;
	vtkImageData* m_vesselWallImage = nullptr;

};
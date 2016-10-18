/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor modified from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef INTERACTOR_STYLE__NAVIGATION_H
#define INTERACTOR_STYLE__NAVIGATION_H

#include "AbstractInteractorStyleImage.h"

#include <vtkSeedWidget.h>


class InteractorStyleSmartContour : public AbstractInteractorStyleImage
{
public:
	vtkTypeMacro(InteractorStyleSmartContour, AbstractInteractorStyleImage);
	static InteractorStyleSmartContour* New();

	void SetSmartContourEnable(bool flag);

protected:
	InteractorStyleSmartContour();
	~InteractorStyleSmartContour();

	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnChar();
	virtual void OnKeyPress();

private:
	//static vtkSmartPointer<vtkPoints> m_seeds;
	/**
	* Using a static list to save all seeds and it will be shared by all other
	* InteractorStyleSmartContour instances
	*/
	static std::list<int*> m_seeds;
	/**
	 * Invalide pick or any other error? CalculateIndex() will set m_world[3] = 
	 * { VTK_INT_MIN };
	 */
	void CalculateIndex();
	void UpdateSeedWidgetBefore();
	void UpdateSeedWidgetAfter();
	void ClearAllSeeds();
	double m_world[3] = { 0 };

	vtkSmartPointer<vtkSeedWidget> m_seedWidget;
};

#endif // !INTERACTOR_STYLE__NAVIGATION_H

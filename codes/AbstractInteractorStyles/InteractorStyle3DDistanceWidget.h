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


#ifndef INTERACTOR_STYLE_3D_DISTANCE_WIDGET_H
#define INTERACTOR_STYLE_3D_DISTANCE_WIDGET_H

#include <vector>
#include <QTime>
#include "AbstractInteractorStyle3D.h"
#include "Distance3DWidget.h"

class InteractorStyle3DDistanceWidget : public AbstractInteractorStyle3D
{
public:
	vtkTypeMacro(InteractorStyle3DDistanceWidget, AbstractInteractorStyle3D);
	static InteractorStyle3DDistanceWidget* New();

protected:
	InteractorStyle3DDistanceWidget();
	~InteractorStyle3DDistanceWidget();
	
	virtual void OnMiddleButtonDown();
	virtual void OnKeyPress();

private:
	bool CalculateIndex();
	void UpdateStenosisValue();

	QTime m_clickTimer;
	std::vector<Distance3DWidget*> m_distanceWidgetList;
	std::vector<double*> m_coords;
	std::vector<vtkActor*> m_ABActors;
};


#endif // !INTERACTOR_STYLE_3D_NAVIGATION_H

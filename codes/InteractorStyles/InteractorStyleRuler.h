#ifndef __INTERACTORSTYLERULER_H
#define __INTERACTORSTYLERULER_H

#include "InteractorStyleNavigation.h"

#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation3D.h>

class InteractorStyleRuler :
	public InteractorStyleNavigation
{
public:
	vtkTypeMacro(InteractorStyleRuler, InteractorStyleNavigation);
	static InteractorStyleRuler* New();
	void SetDistanceWidgetEnabled(bool flag);
	

protected:
	InteractorStyleRuler();
	~InteractorStyleRuler();

private:
	bool m_distanceWidgetEnabledFlag = false;
	//vtkPointHandleRepresentation2D* m_pointHandleRepresentation2D = NULL;
	vtkDistanceRepresentation3D* m_distanceRepresentation3D = NULL;
	vtkDistanceWidget* m_distanceWidget = NULL;
};

#endif // !__INTERACTORSTYLERULER_H




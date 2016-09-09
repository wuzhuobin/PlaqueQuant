#ifndef __INTERACTORSTYLERULER_H
#define __INTERACTORSTYLERULER_H

#include "InteractorStyleNavigation.h"

#include <vtkPointHandleRepresentation2D.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkDistanceWidget.h>

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

	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnMouseWheelBackward();
	virtual void OnMouseWheelForward();
	virtual void OnMiddleButtonDown();


private:
	bool m_distanceWidgetEnabledFlag = false;
	//vtkPointHandleRepresentation2D* m_pointHandleRepresentation2D = NULL;
	//vtkDistanceRepresentation2D* m_distanceRepresentation2D = NULL;
	vtkDistanceWidget* m_distanceWidget = NULL;
};

#endif // !__INTERACTORSTYLERULER_H




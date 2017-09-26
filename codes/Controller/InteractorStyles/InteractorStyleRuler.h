#ifndef __INTERACTORSTYLERULER_H
#define __INTERACTORSTYLERULER_H

#include "InteractorStyleNavigation.h"

#include <vtkSmartPointer.h>

class vtkDistanceWidget;
class vtkPointSetToLabelHierarchy;
class vtkStringArray;
class vtkLabelPlacementMapper;



class InteractorStyleRuler :
	public InteractorStyleNavigation
{
public:
	vtkTypeMacro(InteractorStyleRuler, InteractorStyleNavigation);
	static InteractorStyleRuler* New();
	virtual void SetCustomEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

protected:
	InteractorStyleRuler();
	~InteractorStyleRuler();

	vtkDistanceWidget* m_distanceWidget = nullptr;
};

#endif // !__INTERACTORSTYLERULER_H




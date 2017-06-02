#ifndef __INTERACTOR_STYLE_SURFACE_CENTERLINE_WAYPOINT_H__
#define __INTERACTOR_STYLE_SURFACE_CENTERLINE_WAYPOINT_H__

#include "AbstractSurfaceCenterLine.h"


class InteractorStyleSurfaceCenterlineWaypoint: public AbstractSurfaceCenterLine
{
public:

	vtkTypeMacro(InteractorStyleSurfaceCenterlineWaypoint, AbstractSurfaceCenterLine);
	static InteractorStyleSurfaceCenterlineWaypoint* New();

	virtual void SetCustomEnabled(bool flag) override;

	virtual bool CreateCenterLine() override;


protected:
	InteractorStyleSurfaceCenterlineWaypoint();
	~InteractorStyleSurfaceCenterlineWaypoint();
private:

};



#endif // !__INTERACTOR_STYLE_SURFACE_CENTERLINE_WAYPOINT_H__
#ifndef __STYLE_SWITCH3D_H__
#define __STYLE_SWITCH3D_H__

#include "InteractorStyleSwitch3D.h"
#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"
#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"
#include "InteractorStyleSurfaceCenterLinePerpendicularMeasurement.h"
#include "InteractorStyleSurfaceCenterLineCurvedNavigation.h"
#include "InteractorStyleSurfaceCenterlineWaypoint.h"
#include "InteractorStyleSurfaceCenterLineStenosis.h"

class StyleSwitch3D: public InteractorStyleSwitch3D
{
public:
	static StyleSwitch3D* New() { return new StyleSwitch3D; }
	vtkTypeMacro(StyleSwitch3D, InteractorStyleSwitch3D);
	
	vtkGetMacro(CenterLine, InteractorStyleSurfaceCenterLineSimpleClipping*);
	SetInteractorStyle3DMacro(CenterLine);
	CurrentStyle3DMacro(CenterLine);

	vtkGetMacro(FindMaximumRadius, InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius*);
	SetInteractorStyle3DMacro(FindMaximumRadius);
	CurrentStyle3DMacro(FindMaximumRadius);

	vtkGetMacro(PerpendicularMeasurement, InteractorStyleSurfaceCenterLinePerpendicularMeasurement*);
	SetInteractorStyle3DMacro(PerpendicularMeasurement);
	CurrentStyle3DMacro(PerpendicularMeasurement);

	vtkGetMacro(CurvedNavigation, InteractorStyleSurfaceCenterLineCurvedNavigation*);
	SetInteractorStyle3DMacro(CurvedNavigation);
	CurrentStyle3DMacro(CurvedNavigation);

	vtkGetMacro(Waypoint, InteractorStyleSurfaceCenterlineWaypoint*);
	SetInteractorStyle3DMacro(Waypoint);
	CurrentStyle3DMacro(Waypoint);

	vtkGetMacro(Stenosis, InteractorStyleSurfaceCenterLineStenosis*);
	SetInteractorStyle3DMacro(Stenosis);
	CurrentStyle3DMacro(Stenosis);

	//virtual void SetCenterLine(vtkPolyData* centerline);

protected:
	StyleSwitch3D() {

		this->CenterLine = InteractorStyleSurfaceCenterLineSimpleClipping::New();
		allStyles.push_back(this->CenterLine);

		this->FindMaximumRadius = InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::New();
		allStyles.push_back(this->FindMaximumRadius);

		this->PerpendicularMeasurement = InteractorStyleSurfaceCenterLinePerpendicularMeasurement::New();
		allStyles.push_back(this->PerpendicularMeasurement);

		this->CurvedNavigation = InteractorStyleSurfaceCenterLineCurvedNavigation::New();
		allStyles.push_back(this->CurvedNavigation);

		this->Waypoint = InteractorStyleSurfaceCenterlineWaypoint::New();
		allStyles.push_back(this->Waypoint);

		this->Stenosis = InteractorStyleSurfaceCenterLineStenosis::New();
		allStyles.push_back(this->Stenosis);
	}

	InteractorStyleSurfaceCenterLineSimpleClipping* CenterLine;
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius* FindMaximumRadius;
	InteractorStyleSurfaceCenterLinePerpendicularMeasurement* PerpendicularMeasurement;
	InteractorStyleSurfaceCenterLineCurvedNavigation* CurvedNavigation;
	InteractorStyleSurfaceCenterlineWaypoint* Waypoint;
	InteractorStyleSurfaceCenterLineStenosis* Stenosis;

private:

};




#endif // !__STYLE_SWITCH3D_H__




#ifndef __STYLE_SWITCH3D_H__
#define __STYLE_SWITCH3D_H__

#include "InteractorStyleSwitch3D.h"
#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"
#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"
#include "InteractorStyleSurfaceCenterLinePerpendicularMeasurement.h"
#include "InteractorStyleSurfaceCenterLineCurvedNavigation.h"
#include "InteractorStyleSurfaceCenterlineWaypoint.h"
#include "QInteractorStyleSurfaceCenterLineStenosis.h"
#include "QInteractorStyleSurfaceCenterLineICDAStandard.h"
#include "QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter.h"
#include "QInteractorStyleSurfaceCenterLineVBDUboguStandard.h"

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

	vtkGetMacro(Stenosis, QInteractorStyleSurfaceCenterLineStenosis*);
	SetInteractorStyle3DMacro(Stenosis);
	CurrentStyle3DMacro(Stenosis);

	vtkGetMacro(ICDADiameter, QInteractorStyleSurfaceCenterLineICDAStandard*);
	SetInteractorStyle3DMacro(ICDADiameter);
	CurrentStyle3DMacro(ICDADiameter);

	vtkGetMacro(SmokerBADiameter, QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter*);
	SetInteractorStyle3DMacro(SmokerBADiameter);
	CurrentStyle3DMacro(SmokerBADiameter);

	vtkGetMacro(UboguMeasure, QInteractorStyleSurfaceCenterLineVBDUboguStandard*);
	SetInteractorStyle3DMacro(UboguMeasure);
	CurrentStyle3DMacro(UboguMeasure);

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

		this->Stenosis = QInteractorStyleSurfaceCenterLineStenosis::New();
		allStyles.push_back(this->Stenosis);

		this->ICDADiameter = QInteractorStyleSurfaceCenterLineICDAStandard::New();
		allStyles.push_back(this->ICDADiameter);

		this->SmokerBADiameter = QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter::New();
		allStyles.push_back(this->SmokerBADiameter);

		this->UboguMeasure = QInteractorStyleSurfaceCenterLineVBDUboguStandard::New();
		allStyles.push_back(this->UboguMeasure);
	}

	InteractorStyleSurfaceCenterLineSimpleClipping* CenterLine;
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius* FindMaximumRadius;
	InteractorStyleSurfaceCenterLinePerpendicularMeasurement* PerpendicularMeasurement;
	InteractorStyleSurfaceCenterLineCurvedNavigation* CurvedNavigation;
	InteractorStyleSurfaceCenterlineWaypoint* Waypoint;
	QInteractorStyleSurfaceCenterLineStenosis* Stenosis;
	QInteractorStyleSurfaceCenterLineICDAStandard* ICDADiameter;
	QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter* SmokerBADiameter;
	QInteractorStyleSurfaceCenterLineVBDUboguStandard* UboguMeasure;

private:

};




#endif // !__STYLE_SWITCH3D_H__



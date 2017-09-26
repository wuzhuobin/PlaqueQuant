#include "InteractorStyleSurfaceCenterLinePerpendicularMeasurement.h"

#include "CenterlineSurfaceViewer.h"
#include "PerpendicularMeasurementLineWidget.h"

#include <vtkObjectFactory.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdList.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
//#include <vtkTextActor.h>
//#include <vtkTextProperty.h>
#include <vtkLineRepresentation.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>



vtkStandardNewMacro(InteractorStyleSurfaceCenterLinePerpendicularMeasurement);

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	if (m_customFlag) {
		InitializeLinePerpendicularMeasurementWidget();
	}
	else {
		if (m_lineWidget2) {
			m_lineWidget2->SetInteractor(nullptr);
			m_lineWidget2->EnabledOff();
			m_lineWidget2 = nullptr;
		}
	}
	

}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::FindMaxiMumPerpendicularDistance()
{
	double pos1[3], pos2[3];
	this->m_lineWidget2->GetLineRepresentation()->GetPoint1WorldPosition(pos1);
	this->m_lineWidget2->GetLineRepresentation()->GetPoint2WorldPosition(pos2);
}

bool InteractorStyleSurfaceCenterLinePerpendicularMeasurement::CreateCenterLine(bool reClipSurface)
{
	bool flag = InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine(reClipSurface);
	if (flag) {
		CustomEnabledOff();
		CustomEnabledOn();
	}
	return flag;
}

InteractorStyleSurfaceCenterLinePerpendicularMeasurement::InteractorStyleSurfaceCenterLinePerpendicularMeasurement()
{

}

InteractorStyleSurfaceCenterLinePerpendicularMeasurement::~InteractorStyleSurfaceCenterLinePerpendicularMeasurement()
{
}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::InitializeLinePerpendicularMeasurementWidget()
{
	if (!GetCenterlineSurfaceViewer()->GetCenterline() ||
		GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
		vtkErrorMacro(<< "no centerline ");
		return;
	}

	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
		vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	pointPlacer->AddProp(GetCenterlineSurfaceViewer()->GetCenterlineActor());


	m_lineWidget2 = vtkSmartPointer<PerpendicularMeasurementLineWidget>::New();
	m_lineWidget2->SetInteractor(this->Interactor);
	m_lineWidget2->CreateDefaultRepresentation();
	m_lineWidget2->GetLineRepresentation()->GetPoint1Representation()->SetPointPlacer(pointPlacer);
	m_lineWidget2->GetLineRepresentation()->GetPoint2Representation()->SetPointPlacer(pointPlacer);
	m_lineWidget2->GetLineRepresentation()->SetPoint1WorldPosition(
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(0));
	m_lineWidget2->GetLineRepresentation()->SetPoint2WorldPosition(
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(1));
	m_lineWidget2->GetLineRepresentation()->SetDistanceAnnotationFormat("%-#11.2f mm");
	m_lineWidget2->GetDistanceWidget()->GetDistanceRepresentation()->SetLabelFormat("%-#11.2f mm");
	m_lineWidget2->SetLine(GetCenterlineSurfaceViewer()->GetCenterline());
	m_lineWidget2->EnabledOn();

}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	if (key == "Return") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress(); 
		FindMaxiMumPerpendicularDistance();
	}
	else {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
	}
}



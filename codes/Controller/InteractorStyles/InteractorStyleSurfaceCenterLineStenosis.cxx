#include "InteractorStyleSurfaceCenterLineStenosis.h"

#include "CenterlineSurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkActor.h>
#include <vtkHandleWidget.h>
#include <vtkKdTreePointLocator.h>
//#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdList.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkCoordinate.h>
#include <vtkImageData.h>



class InteractorStyleSurfaceCenterLineStenosisCallback : public vtkCommand {
public:
	static InteractorStyleSurfaceCenterLineStenosisCallback* New() { return new InteractorStyleSurfaceCenterLineStenosisCallback; }
	InteractorStyleSurfaceCenterLineStenosis* style;
	virtual void Execute(vtkObject*, unsigned long event, void *calldata) override {
		if (event == vtkCommand::InteractionEvent)
		{
			style->CalculateStenosis();
		}
	}
};

vtkStandardNewMacro(InteractorStyleSurfaceCenterLineStenosis);

void InteractorStyleSurfaceCenterLineStenosis::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	if (m_customFlag) {
		InitializeHandleWidgets();
		GetSurfaceViewer()->GetRenderer()->AddActor(m_measurementText);
	}
	else {
		for (int i = 0; i < NUM_OF_HANDLES; ++i) {
			// removing widgets
			if (m_handleWidgets[i]) {
				m_handleWidgets[i]->SetInteractor(nullptr);
				m_handleWidgets[i]->EnabledOff();
				m_handleWidgets[i]->RemoveAllObservers();
				m_handleWidgets[i] = nullptr;
			}

		}
		m_pointLocator = nullptr;
		GetSurfaceViewer()->GetRenderer()->RemoveActor(m_measurementText);
	}
	

}

InteractorStyleSurfaceCenterLineStenosis::InteractorStyleSurfaceCenterLineStenosis()
{
	m_measurementText =
		vtkSmartPointer<vtkTextActor>::New();
	//m_measurementText->SetPosition(5, 10);
	//m_measurementText->SetPosition2(10, 40);
	m_measurementText->GetTextProperty()->SetFontSize(15);
	m_measurementText->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
}

InteractorStyleSurfaceCenterLineStenosis::~InteractorStyleSurfaceCenterLineStenosis()
{
}



void InteractorStyleSurfaceCenterLineStenosis::InitializeHandleWidgets()
{

	if (!GetCenterlineSurfaceViewer()->GetCenterline() || 
		GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
		vtkErrorMacro( << "no centerline ");
		return;
	}

	m_pointLocator = vtkSmartPointer<vtkKdTreePointLocator>::New();
	m_pointLocator->SetDataSet(GetCenterlineSurfaceViewer()->GetCenterline());
	//m_pointLocator->BuildLocator();

	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
		vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	pointPlacer->AddProp(GetCenterlineSurfaceViewer()->GetCenterlineActor());

	//double* worldPos =
	//	m_triangulatedCenterLine->GetPoint(0);

	for (int i = 0; i < NUM_OF_HANDLES; ++i) {

		// the vtkSphereHandleRepresentation does not use the vtkPolygonalSurfacePointPlacer 
		// in its widgetInteraction function, but vtkPointHandleRepresentation3D does
		vtkSmartPointer<vtkPointHandleRepresentation3D> handleRep =
			vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
		handleRep->SetPointPlacer(pointPlacer);
		handleRep->SetWorldPosition(GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(i));

		vtkSmartPointer<InteractorStyleSurfaceCenterLineStenosisCallback> callback =
			vtkSmartPointer<InteractorStyleSurfaceCenterLineStenosisCallback>::New();
		callback->style = this;

		m_handleWidgets[i] = vtkSmartPointer<vtkHandleWidget>::New();
		m_handleWidgets[i]->SetRepresentation(handleRep);
		m_handleWidgets[i]->SetInteractor(this->Interactor);
		m_handleWidgets[i]->EnabledOn();
		m_handleWidgets[i]->AddObserver(vtkCommand::InteractionEvent, callback);

	}


}



bool InteractorStyleSurfaceCenterLineStenosis::CreateCenterLine(bool reClipSurface)
{
	bool flag = InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine(reClipSurface);
	if (flag) {
		CustomEnabledOff();
		CustomEnabledOn();
	}
	return flag;
}

void InteractorStyleSurfaceCenterLineStenosis::CalculateStenosis()
{
	vtkIdType seed1 = m_pointLocator->FindClosestPoint(
		m_handleWidgets[0]->GetHandleRepresentation()->GetWorldPosition());
	vtkIdType seed2 = m_pointLocator->FindClosestPoint(
		m_handleWidgets[1]->GetHandleRepresentation()->GetWorldPosition());
	
	//vtkSmartPointer<vtkDoubleArray> radius =
	//	vtkSmartPointer<vtkDoubleArray>::New();
	//radius->SetNumberOfValues(pathPointId->GetNumberOfIds());
	double radius1 = *static_cast<double*>(
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPointData()->GetArray(RADIUS)->GetVoidPointer(seed1));
	double radius2 = *static_cast<double*>(
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPointData()->GetArray(RADIUS)->GetVoidPointer(seed2));

	StenosisValue = { radius1 > radius2 ? 1. - radius2 / radius1 : 1 - radius1 / radius2 };
	StenosisValue *= 100;
	vtkSmartPointer<vtkCoordinate> coordinate =
		vtkSmartPointer<vtkCoordinate>::New();
	coordinate->SetCoordinateSystemToWorld();
	coordinate->SetValue(m_handleWidgets[0]->GetHandleRepresentation()->GetWorldPosition());
	int* displayCoordinate = coordinate->GetComputedDisplayValue(GetSurfaceViewer()->GetRenderer());

	char buff[100];
	sprintf_s(buff, "Stenosis: %.4f%%", StenosisValue);
	m_measurementText->SetInput(buff);
	m_measurementText->SetDisplayPosition(displayCoordinate[0], displayCoordinate[1]);
	//this->GetSurfaceViewer()->Render();
}

void InteractorStyleSurfaceCenterLineStenosis::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	if (key == "Tab") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
		InitializeHandleWidgets();
	}
	else if (key == "space") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
		InitializeHandleWidgets();
	}
	else {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
	}
}



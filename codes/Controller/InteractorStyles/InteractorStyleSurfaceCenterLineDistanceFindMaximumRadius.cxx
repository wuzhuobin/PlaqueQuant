#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"

#include "CenterlineSurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkCleanPolyData.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkTriangleFilter.h>
#include <vtkActor.h>
#include <vtkHandleWidget.h>
#include <vtkKdTreePointLocator.h>
#include <vtkDijkstraGraphGeodesicPath.h>
//#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdList.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkCoordinate.h>

#include <vtkDijkstraGraphInternals.h>
class vtkDijkstraGraphGeodesicPathDistance : public vtkDijkstraGraphGeodesicPath {

public:
	static vtkDijkstraGraphGeodesicPathDistance* New() { return new vtkDijkstraGraphGeodesicPathDistance(); }
	double GetGeodesicPathDistance(int vertexId) {
		try
		{
			return this->Internals->CumulativeWeights.at(vertexId);
		}
		catch (const std::out_of_range& e)
		{
			cerr << e.what();
			vtkErrorMacro(<< e.what());
			return VTK_DOUBLE_MAX;
		}

	}
	//vtkGetMacro(GeodesicPathDistance, double);
protected:
	double GeodesicPathDistance = 0;

};

class InteractorStyleSurfaceCenterLineDistanceFindMaximumRadiusCallback : public vtkCommand {
public:
	static InteractorStyleSurfaceCenterLineDistanceFindMaximumRadiusCallback* New() { return new InteractorStyleSurfaceCenterLineDistanceFindMaximumRadiusCallback; }
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius* style;
	virtual void Execute(vtkObject*, unsigned long event, void *calldata) override {
		if (event == vtkCommand::InteractionEvent)
		{
			style->FindMaximumRadius();
		}
	}
};

vtkStandardNewMacro(InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius);

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::SetCustomEnabled(bool flag)
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

InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius()
{
	m_measurementText =
		vtkSmartPointer<vtkTextActor>::New();
	//m_measurementText->SetPosition(5, 10);
	//m_measurementText->SetPosition2(10, 40);
	m_measurementText->GetTextProperty()->SetFontSize(15);
	m_measurementText->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
}

InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::~InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius()
{
}

//void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::CreateCenterLine()
//{
//	InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine();
//}

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::InitializeHandleWidgets()
{
	//vtkSmartPointer<vtkCleanPolyData> cleanPolyData =
	//	vtkSmartPointer<vtkCleanPolyData>::New();
	//cleanPolyData->SetInputData(m_centerLine);
	//cleanPolyData->PointMergingOn();
	////cleanPolyData->PointMergingOff();
	//cleanPolyData->Update();
	if (!GetCenterlineSurfaceViewer()->GetCenterline() || 
		GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
		vtkErrorMacro( << "no centerline ");
		return;
	}
	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	//triangleFilter->SetInputData(GetCenterlineSurfaceViewer()->GetCenterline());
	triangleFilter->SetInputConnection(GetCenterlineSurfaceViewer()->GetCleanPolyData()->GetOutputPort());
	triangleFilter->Update();
	m_triangulatedCenterLine = triangleFilter->GetOutput();

	m_pointLocator = vtkSmartPointer<vtkKdTreePointLocator>::New();
	m_pointLocator->SetDataSet(m_triangulatedCenterLine);
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
		handleRep->SetWorldPosition(m_triangulatedCenterLine->GetPoint(i));

		vtkSmartPointer<InteractorStyleSurfaceCenterLineDistanceFindMaximumRadiusCallback> callback =
			vtkSmartPointer<InteractorStyleSurfaceCenterLineDistanceFindMaximumRadiusCallback>::New();
		callback->style = this;

		m_handleWidgets[i] = vtkSmartPointer<vtkHandleWidget>::New();
		m_handleWidgets[i]->SetRepresentation(handleRep);
		m_handleWidgets[i]->SetInteractor(this->Interactor);
		m_handleWidgets[i]->EnabledOn();
		m_handleWidgets[i]->AddObserver(vtkCommand::InteractionEvent, callback);

	}



	//vtkSmartPointer<vtkPolyDataWriter> writer =
	//	vtkSmartPointer<vtkPolyDataWriter>::New();
	//writer->SetInputData(centerlinesFilter->GetOutput());
	//writer->SetFileName("C:/Users/user/Desktop/centerline.vtk");
	//writer->Write();
}



bool InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::CreateCenterLine(bool reClipSurface)
{
	bool flag = InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine(reClipSurface);
	if (flag) {
		CustomEnabledOff();
		CustomEnabledOn();
	}
	return flag;
}

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::FindMaximumRadius()
{
	vtkIdType seed1 = m_pointLocator->FindClosestPoint(
		m_handleWidgets[0]->GetHandleRepresentation()->GetWorldPosition());
	vtkIdType seed2 = m_pointLocator->FindClosestPoint(
		m_handleWidgets[1]->GetHandleRepresentation()->GetWorldPosition());
	vtkSmartPointer<vtkDijkstraGraphGeodesicPathDistance> dijkstra =
		vtkSmartPointer<vtkDijkstraGraphGeodesicPathDistance>::New();
	dijkstra->SetInputData(m_triangulatedCenterLine);
	dijkstra->SetStartVertex(seed1);
	//dijkstra->SetRepelVertices(repelVertices);
	//dijkstra->RepelPathFromVerticesOn();
	dijkstra->SetEndVertex(seed2);
	dijkstra->StopWhenEndReachedOn();
	dijkstra->Update();
	vtkIdList* pathPointId = dijkstra->GetIdList();
	//vtkSmartPointer<vtkDoubleArray> radius =
	//	vtkSmartPointer<vtkDoubleArray>::New();
	//radius->SetNumberOfValues(pathPointId->GetNumberOfIds());
	double maxRadius = VTK_DOUBLE_MIN;
	double minRadius = VTK_DOUBLE_MAX;
	double GeodesicPathDistance = 0;
	vtkIdType maxRadiusId = 0;
	vtkIdType minRadiusId = 0;
	for (vtkIdType id = 0; id < pathPointId->GetNumberOfIds(); ++id) {
		vtkIdType _pointId = pathPointId->GetId(id);
		double* value = static_cast<double*>(
			m_triangulatedCenterLine->GetPointData()->GetArray(RADIUS)->GetVoidPointer(_pointId));
		if (*value > maxRadius) {
			maxRadius = *value;
			maxRadiusId = _pointId;
		}
		if (*value < minRadius) {
			minRadius = *value;
			minRadiusId = _pointId;
		}
		//cout << "point id = " << id << endl;
		//double* point = m_centerLine->GetPoint(pathPointId->GetId(id));
		//cout << point[0] << ' ' << point[1] << ' ' << point[2] << endl;
	}
	cout << "maximum radius " << maxRadiusId << " is " << maxRadius << endl;
	cout << "minimum radius " << minRadiusId << " is " << minRadius << endl;
	GeodesicPathDistance = dijkstra->GetGeodesicPathDistance(seed2);
	cout << GeodesicPathDistance << endl;

	vtkSmartPointer<vtkCoordinate> coordinate =
		vtkSmartPointer<vtkCoordinate>::New();
	coordinate->SetCoordinateSystemToWorld();
	coordinate->SetValue(m_handleWidgets[0]->GetHandleRepresentation()->GetWorldPosition());
	int* displayCoordinate = coordinate->GetComputedDisplayValue(GetSurfaceViewer()->GetRenderer());

	char buff[100];
	sprintf_s(buff, "Maximum radius: %.2f mm\n Minimum radius: %.2f mm\n Centerline length: %.2f mm", maxRadius, minRadius, GeodesicPathDistance);
	m_measurementText->SetInput(buff);
	m_measurementText->SetDisplayPosition(displayCoordinate[0], displayCoordinate[1]);
	//this->GetSurfaceViewer()->Render();
}

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::OnKeyPress()
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



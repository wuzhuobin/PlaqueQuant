#include "InteractorStyleSurfaceCenterLineCurvedNavigation.h"

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
#include <vtkSplineFilter.h>
#include <vtkCoordinate.h>
#include <vtkImageViewer2.h>
#include <vtkImageData.h>
#include <vtkDijkstraGraphInternals.h>

#include <algorithm>

#include "AbstractNavigation.h"


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

class InteractorStyleSurfaceCenterLineCurvedNavigationCallback : public vtkCommand {
public:
	static InteractorStyleSurfaceCenterLineCurvedNavigationCallback* New() { return new InteractorStyleSurfaceCenterLineCurvedNavigationCallback; }
	InteractorStyleSurfaceCenterLineCurvedNavigation* style;
	virtual void Execute(vtkObject*, unsigned long event, void *calldata) override {
		if (event == vtkCommand::InteractionEvent)
		{
			style->Update2DViewers();
			style->UpdateRadiusLabel();
		}
	}
};

vtkStandardNewMacro(InteractorStyleSurfaceCenterLineCurvedNavigation);

void InteractorStyleSurfaceCenterLineCurvedNavigation::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	if (m_customFlag) {
		InitializeHandleWidgets();
		GetSurfaceViewer()->GetRenderer()->AddActor(m_measurementText);
		//InitializeReslicer();
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
		m_splinePointLocator = nullptr;
		m_centerlinePointLocator= nullptr;
		GetSurfaceViewer()->GetRenderer()->RemoveActor(m_measurementText);
		//m_imageActor = nullptr;
	}
	

}

InteractorStyleSurfaceCenterLineCurvedNavigation::InteractorStyleSurfaceCenterLineCurvedNavigation()
{
	m_measurementText =
		vtkSmartPointer<vtkTextActor>::New();
	m_measurementText->GetTextProperty()->SetFontSize(15);
	m_measurementText->GetTextProperty()->SetColor(1.0, 1.0, 1.0);

}

InteractorStyleSurfaceCenterLineCurvedNavigation::~InteractorStyleSurfaceCenterLineCurvedNavigation()
{
}

void InteractorStyleSurfaceCenterLineCurvedNavigation::InitializeHandleWidgets()
{

	if (!GetCenterlineSurfaceViewer()->GetCenterline() || 
		GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
		vtkErrorMacro(<< "no centerline ");
		return;
	}
	m_splinePointLocator = vtkSmartPointer<vtkKdTreePointLocator>::New();
	m_splinePointLocator->SetDataSet(GetCenterlineSurfaceViewer()->GetSplineFilter()->GetOutput());
	m_centerlinePointLocator= vtkSmartPointer<vtkKdTreePointLocator>::New();
	m_centerlinePointLocator->SetDataSet(GetCenterlineSurfaceViewer()->GetCenterline());

	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
		vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	pointPlacer->AddProp(GetCenterlineSurfaceViewer()->GetCenterlineActor());

	double* worldPos =
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(0);

	for (int i = 0; i < NUM_OF_HANDLES; ++i) {

		// the vtkSphereHandleRepresentation does not use the vtkPolygonalSurfacePointPlacer 
		// in its widgetInteraction function, but vtkPointHandleRepresentation3D does
		vtkSmartPointer<vtkPointHandleRepresentation3D> handleRep =
			vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
		handleRep->SetPointPlacer(pointPlacer);
		handleRep->SetWorldPosition(GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(i));

		vtkSmartPointer<InteractorStyleSurfaceCenterLineCurvedNavigationCallback> callback =
			vtkSmartPointer<InteractorStyleSurfaceCenterLineCurvedNavigationCallback>::New();
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
//#include <vtkNIFTIImageWriter.h>
//void InteractorStyleSurfaceCenterLineCurvedNavigation::InitializeReslicer()
//{
//	vtkSmartPointer<vtkvmtkCenterlineGeometry> centerlineGeometry =
//		vtkSmartPointer<vtkvmtkCenterlineGeometry>::New();
//	centerlineGeometry->SetInputData(m_centerLine);
//	centerlineGeometry->SetLengthArrayName("Length");
//	centerlineGeometry->SetCurvatureArrayName("Curvature");
//	centerlineGeometry->SetTorsionArrayName("Torsion");
//	centerlineGeometry->SetTortuosityArrayName("Tortuosity");
//	centerlineGeometry->SetFrenetTangentArrayName("FrenetTangent");
//	centerlineGeometry->SetFrenetNormalArrayName("FrenetNormal");
//	centerlineGeometry->SetFrenetBinormalArrayName("FrenetBinormal");
//	centerlineGeometry->LineSmoothingOff();
//	centerlineGeometry->Update();
//
//	vtkSmartPointer<vtkvmtkCenterlineAttributesFilter> centerlineAttributes =
//		vtkSmartPointer<vtkvmtkCenterlineAttributesFilter>::New();
//	centerlineAttributes->SetInputConnection(centerlineGeometry->GetOutputPort());
//	centerlineAttributes->SetAbscissasArrayName("Abscissas");
//	centerlineAttributes->SetParallelTransportNormalsArrayName("Normals");
//	centerlineAttributes->Update();
//
//
//
//	vtkSmartPointer<vtkvmtkCurvedMPRImageFilter> curvedMPRImageFilter =
//		vtkSmartPointer<vtkvmtkCurvedMPRImageFilter>::New();
//	curvedMPRImageFilter->SetInputData(m_surfaceViewer->GetInput());
//	curvedMPRImageFilter->SetCenterline(centerlineAttributes->GetOutput());
//	//centerlineAttributesFilter->SetInputConnection(triangleFilter->GetOutputPort());
//	curvedMPRImageFilter->SetParallelTransportNormalsArrayName("Normals");
//	curvedMPRImageFilter->SetFrenetTangentArrayName("FrenetTangent");
//	curvedMPRImageFilter->SetInplaneOutputSpacing(GetSpacing()[0], GetSpacing()[1]);
//	curvedMPRImageFilter->SetInplaneOutputSize(GetExtent()[1] - GetExtent()[0], GetExtent()[3] - GetExtent()[2]);
//	curvedMPRImageFilter->SetReslicingBackgroundLevel(0.0);
//	curvedMPRImageFilter->Update();
//	cout << GetOrigin()[0] << endl;
//	cout << GetOrigin()[1] << endl;
//	cout << GetOrigin()[2] << endl;
//	cout << curvedMPRImageFilter->GetOutputOrigin()[0] << endl;
//	cout << curvedMPRImageFilter->GetOutputOrigin()[1] << endl;
//	cout << curvedMPRImageFilter->GetOutputOrigin()[2] << endl;
//	//m_centerLine = curvedMPRImageFilter->GetCenterLine();
//
//	//vtkSmartPointer<vtkNIFTIImageWriter> nIFTIImageWriter =
//	//	vtkSmartPointer<vtkNIFTIImageWriter>::New();
//	//nIFTIImageWriter->SetInputData(curvedMPRImageFilter->GetOutput());
//	//nIFTIImageWriter->SetFileName("C:/Users/jieji/Desktop/curved.nii");
//	//nIFTIImageWriter->Update();
//
//	//vtkSmartPointer<vtkPolyDataWriter> w =
//	//	vtkSmartPointer<vtkPolyDataWriter>::New();
//	//w->SetInputData(centerlineAttributes->GetOutput());
//	//w->SetFileName("C:/Users/jieji/Desktop/centerline.vtk");
//	//w->Update();
//
//	m_imageActor = vtkSmartPointer<vtkImageActor>::New();
//	m_imageActor->GetMapper()->SetInputConnection(curvedMPRImageFilter->GetOutputPort());
//	m_imageActor->GetProperty()->SetLookupTable(m_surfaceViewer->GetLookupTable());
//	m_imageActor->GetProperty()->UseLookupTableScalarRangeOn();
//	m_surfaceViewer->GetRenderer()->AddActor(m_imageActor);
//}

bool InteractorStyleSurfaceCenterLineCurvedNavigation::CreateCenterLine(bool reClipSurface)
{
	bool flag = InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine(reClipSurface);
	if (flag) {
		CustomEnabledOff();
		CustomEnabledOn();
	}
	return flag;
}

void InteractorStyleSurfaceCenterLineCurvedNavigation::Update2DViewers()
{
	const double* worldPos = m_handleWidgets[0]->GetHandleRepresentation()->GetWorldPosition();
	
	int* extent;
	int i;
	int j;
	int k;

	//for (std::list<AbstractInteractorStyle*>::const_iterator cit = m_abstractInteractorStyles.cbegin();
	//	cit != m_abstractInteractorStyles.cend(); ++cit) {
	//	AbstractNavigation* style = dynamic_cast<AbstractNavigation*>(*cit);
	//	if (style) {
	//		extent = style->GetVtkImageViewer2()->GetInput()->GetExtent();
	//		break;
	//	}
	//}
	
	// the same as above
	std::list<AbstractInteractorStyle*>::const_iterator cit = std::find_if(
		m_abstractInteractorStyles.cbegin(), 
		m_abstractInteractorStyles.cend(),
		[](AbstractInteractorStyle* style)->bool {
		return dynamic_cast<AbstractNavigation*>(style); });

	extent = static_cast<AbstractNavigation*>(*cit)->GetVtkImageViewer2()->GetInput()->GetExtent();


	if (std::equal(extent, extent + 5, GetExtent())) {
		i = static_cast<int>((worldPos[0] - GetOrigin()[0]) / GetSpacing()[0] + 0.5);
		j = static_cast<int>((worldPos[1] - GetOrigin()[1]) / GetSpacing()[1] + 0.5);
		k = static_cast<int>((worldPos[2] - GetOrigin()[2]) / GetSpacing()[2] + 0.5);
	}
	else {
		i = static_cast<int>((GetExtent()[1] - GetExtent()[0])*0.5 + 0.5);
		j = static_cast<int>((GetExtent()[3] - GetExtent()[2])*0.5 + 0.5);
		k = m_splinePointLocator->FindClosestPoint(worldPos);
	}
	
	DYNAMIC_CAST_CONSTITERATOR(AbstractNavigation, SetCurrentFocalPointWithImageCoordinate(i, j, k));


}

void InteractorStyleSurfaceCenterLineCurvedNavigation::UpdateRadiusLabel()
{
	//int size[2] = { 50, 100 };
	double* worldPos = m_handleWidgets[0]->GetHandleRepresentation()->GetWorldPosition();
	vtkIdType id = m_centerlinePointLocator->FindClosestPoint(worldPos);
	double* value = static_cast<double*>(
		GetCenterlineSurfaceViewer()->GetCenterline()->
		GetPointData()->GetArray(RADIUS)->GetVoidPointer(id));

	vtkSmartPointer<vtkCoordinate> coordinate =
		vtkSmartPointer<vtkCoordinate>::New();
	coordinate->SetCoordinateSystemToWorld();
	coordinate->SetValue(worldPos);

	int* displayCoordinate = coordinate->GetComputedDisplayValue(GetSurfaceViewer()->GetRenderer());
	char buff[100];
	sprintf_s(buff, "Radius: %.2f mm", *value);
	m_measurementText->SetInput(buff);
	m_measurementText->SetDisplayPosition(displayCoordinate[0], displayCoordinate[1]);
	//m_measurementText->SetWidth(size[0]);
	//m_measurementText->SetHeight(size[1]);
}

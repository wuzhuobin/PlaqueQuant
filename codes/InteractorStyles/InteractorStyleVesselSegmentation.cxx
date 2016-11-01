/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant

Author:		Lok, Ka Hei Jason
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
M.Phil Student

This class allows interactive segmentation on images with contour widget.

Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#include <vtkContourWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolygon.h>
#include <vtkMath.h>
#include <vtkLinearContourLineInterpolator.h>
#include <vtkBezierContourLineInterpolator.h>
#include <vtkProperty.h>

#include "InteractorStylePolygonDraw.h"
#include "MyImageViewer.h"
#include "InteractorStyleVesselSegmentation.h"
#include "LumenSegmentation.h"

using namespace std;
vtkStandardNewMacro(InteractorStyleVesselSegmentation);

InteractorStyleVesselSegmentation::InteractorStyleVesselSegmentation()
	:InteractorStylePolygonDraw()
{
}

InteractorStyleVesselSegmentation::~InteractorStyleVesselSegmentation()
{
}


void InteractorStyleVesselSegmentation::SetPolygonModeEnabled(bool b)
{
	InteractorStylePolygonDraw::SetPolygonModeEnabled(b);
}

void InteractorStyleVesselSegmentation::NewContour()
{
	switch (m_contourType)
	{
	case CONTOUR:
		InteractorStylePolygonDraw::NewContour();
		break;
	case VESSEL_WALL:
		InteractorStylePolygonDraw::NewContour();
		m_currentContourRep->GetLinesProperty()->SetColor(0, 0, 255);
		break;
	case LUMEN:
		break;
	default:
		break;
	}
}

void InteractorStyleVesselSegmentation::SetVesselWallLabel(int vesselWallLabel)
{
	this->m_vesselWallLabel = vesselWallLabel;
}

void InteractorStyleVesselSegmentation::SetLumenWallLabel(int lumenWallLabel)
{
	this->m_lumenWallLabel = lumenWallLabel;
}

void InteractorStyleVesselSegmentation::SetSegmentationMode(int i)
{
	if (i < 0 || i > 2) {
		return;
	}
	m_contourType = i;
	SetPolygonModeEnabled(false);
	SetPolygonModeEnabled(true);
}

void InteractorStyleVesselSegmentation::EnableNoSegmentation()
{
	SetSegmentationMode(0);
}

void InteractorStyleVesselSegmentation::EnableLumenSegmentation()
{
	SetSegmentationMode(1);
}

void InteractorStyleVesselSegmentation::EnableVesselWallSegmentation()
{
	SetSegmentationMode(2);
}

void InteractorStyleVesselSegmentation::SetContourFilterGenerateValues(int generateValues)
{
	this->m_generateValue = generateValues;
}

void InteractorStyleVesselSegmentation::ClearAllConoturs()
{
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend();
		++cit) {
		(*cit)->EnabledOff();
	}
	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
		= m_lumenWallContourWidgets.cbegin(); cit != m_lumenWallContourWidgets.cend();
		++cit) {
		(*cit)->EnabledOff();

	}
	m_vesselWallContourWidgets.clear();
	m_lumenWallContourWidgets.clear();
	InteractorStylePolygonDraw::ClearAllConoturs();
	m_imageViewer->Render();
}

void InteractorStyleVesselSegmentation::GenerateLumenWallContourWidget()
{
	//if (m_vesselWallContourRepresentation == NULL || m_vesselWallContourWidget == NULL)
	//	return;

	//// if there are not points to close #Issue7
	//if (this->m_vesselWallContourRepresentation->GetNumberOfNodes() < 1)
	//	return;

	//this->m_vesselWallContourWidget->CloseLoop();

	//if (m_lumenWallContourWidget) {
	//	m_lumenWallContourWidget->Off();
	//	m_lumenWallContourWidget->SetRepresentation(NULL);
	//	m_lumenWallContourWidget->EnabledOff();
	//	m_lumenWallContourWidget->Delete();
	//	m_lumenWallContourWidget = NULL;
	//}


	//if (m_lumenWallContourRepresentation) {
	//	m_lumenWallContourRepresentation->Delete();
	//	m_lumenWallContourRepresentation = NULL;
	//}
	//if (this->CONTOUR_IS_ON_FLAG) {
	//	m_lumenWallContourRepresentation = vtkOrientedGlyphContourRepresentation::New();
	//	MyImageViewer* viewer2 = dynamic_cast<MyImageViewer*>(m_imageViewer);
	//	if (viewer2 != NULL) {
	//		m_lumenWallContourRepresentation->SetRenderer(viewer2->GetAnnotationRenderer());
	//	}
	//	else {
	//		m_lumenWallContourRepresentation->SetRenderer(m_imageViewer->GetRenderer());
	//	}
	//	m_lumenWallContourRepresentation->SetNeedToRender(true);
	//	m_lumenWallContourRepresentation->GetLinesProperty()->SetColor(255, 0, 0);
	//	m_lumenWallContourRepresentation->SetLineInterpolator(this->m_interpolator);
	//	m_lumenWallContourRepresentation->AlwaysOnTopOn();
	//	m_lumenWallContourRepresentation->BuildRepresentation();

	//	m_lumenWallContourWidget = vtkContourWidget::New();
	//	m_lumenWallContourWidget->SetInteractor(this->Interactor);
	//	m_lumenWallContourWidget->SetRepresentation(m_lumenWallContourRepresentation);
	//	if (viewer2 != NULL) {
	//		m_lumenWallContourWidget->SetDefaultRenderer(viewer2->GetAnnotationRenderer());
	//	}
	//	else {
	//		m_lumenWallContourWidget->SetDefaultRenderer(m_imageViewer->GetRenderer());
	//	}
	//	m_lumenWallContourWidget->FollowCursorOn();
	//	m_lumenWallContourWidget->ContinuousDrawOn();
	//	m_lumenWallContourWidget->On();
	//	m_lumenWallContourWidget->EnabledOn();

	//	vtkSmartPointer<LumenSegmentaiton> ls =
	//		vtkSmartPointer<LumenSegmentaiton>::New();
	//	ls->SetInputData(m_imageViewer->GetInput());
	//	ls->SetSlice(GetSlice());
	//	ls->SetGenerateValues(1, m_generateValue, m_generateValue);
	//	ls->SetVesselWallContourRepresentation(this->m_vesselWallContourRepresentation);
	//	ls->Update();

	//	if (ls->GetOutput() == nullptr)
	//		return;

	//	// This prevent crash when a loop is not found #Issue7
	//	if (ls->GetOutput()->GetNumberOfPoints() == 0)
	//		return;

	//	m_lumenWallContourWidget->Initialize(ls->GetOutput());
	//	m_lumenWallContourWidget->CloseLoop();
	//	m_imageViewer->Render();
	//}
}

void InteractorStyleVesselSegmentation::FillPolygon()
{
	InteractorStylePolygonDraw::FillPolygon(
		&m_lumenWallContourWidgets, m_lumenWallLabel);
	InteractorStylePolygonDraw::FillPolygon(
		&m_vesselWallContourWidgets, m_vesselWallLabel);
	InteractorStylePolygonDraw::FillPolygon();

}


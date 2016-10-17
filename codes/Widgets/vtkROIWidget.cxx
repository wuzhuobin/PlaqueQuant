#include <array>
#include <vector>
#include <vtkPlanes.h>
#include <vtkPolyData.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkBorderRepresentation.h>
#include <vtkObjectFactory.h>
#include <vtkProperty2D.h>
#include <vtkCamera.h>
#include <vtkMath.h>

#include "vtkROIWidget.h"
#include "MainWindow.h"

/// Implementation for ROIBorderWidget
vtkStandardNewMacro(vtkROIBorderWidget);

vtkROIBorderWidget::vtkROIBorderWidget() 
{
	this->Orientation = -1;
}

vtkROIBorderWidget::~vtkROIBorderWidget()
{
	if (this->WidgetRep != NULL) {
		this->WidgetRep->Delete();
		this->WidgetRep = NULL;
	}

}

void vtkROIBorderWidget::SelectRegion(double eventPos[2])
{
}

int vtkROIBorderWidget::SubclassSelectAction()
{
	if (this->WidgetRep->GetInteractionState() == vtkBorderRepresentation::Inside)
		return 1;
	else 
		return 0;
}

void vtkROIBorderWidget::SetCursor(int cState)
{
	Superclass::SetCursor(cState);

	if (cState == vtkBorderRepresentation::Inside)
	{
		if (reinterpret_cast<vtkBorderRepresentation*>(this->WidgetRep)->GetMoving())
		{
			this->RequestCursorShape(VTK_CURSOR_DEFAULT);
		}
		else
		{
			this->RequestCursorShape(VTK_CURSOR_DEFAULT);
		}
	}
}
void vtkROIBorderWidget::UpdateROIWidget()
{
	// if orientation not set
	if (this->Orientation == -1) {
		vtkErrorMacro("Orientation not set!");
		return;
	}
	/// Convert points of borderwidgets to real world coordinates
	// Conversion to world position
	double pt1[3], pt2[3];
	vtkROIWidget* parent = reinterpret_cast<ParentType*>(this->ROIParent);
	vtkBoxRepresentation* parentRep = reinterpret_cast<vtkBoxRepresentation*>(parent->GetRepresentation());
	vtkBorderRepresentation* borderRep = reinterpret_cast<vtkBorderRepresentation*>(this->WidgetRep);
	vtkRenderer* bwRen = this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
	vtkSmartPointer<vtkCoordinate> pt1Coord = borderRep->GetPositionCoordinate();
	vtkSmartPointer<vtkCoordinate> pt2Coord = borderRep->GetPosition2Coordinate();

	memcpy(pt1, borderRep->GetPositionCoordinate()->GetComputedWorldValue(bwRen), sizeof(double) * 3);
	memcpy(pt2, borderRep->GetPosition2Coordinate()->GetComputedWorldValue(bwRen), sizeof(double) * 3);

	int boundIndex[4];
	switch (this->Orientation)
	{
	case ORIENTATION_XY:
		boundIndex[0] = 0;
		boundIndex[1] = 1;
		boundIndex[2] = 2;
		boundIndex[3] = 3;
		break;
	case ORIENTATION_XZ:
		boundIndex[0] = 0;
		boundIndex[1] = 1;
		boundIndex[2] = 4;
		boundIndex[3] = 5;
		break;
	case ORIENTATION_YZ:
		boundIndex[0] = 2;
		boundIndex[1] = 3;
		boundIndex[2] = 4;
		boundIndex[3] = 5;
		break;
	}

	/*	P3---E2---P2
		|		  |
		E3		  E1
		|		  |
		P0---E0---P1     */
	// some corners are not of correct orientation
	/// Change bounds of boxwidget's representation based on interaction state
	bool reverseSides;
	int ptIndex;
	double boxBounds[6], oriBounds[6];
	memcpy(boxBounds, parentRep->GetBounds(), sizeof(double)*6);
	switch (borderRep->GetInteractionState())
	{
	case vtkBorderRepresentation::AdjustingE0:
		// Change lower y bound only
		ptIndex = int(boundIndex[2] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[3]] = pt1[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[2]] = pt1[ptIndex];
		break;
	case vtkBorderRepresentation::AdjustingE1:
		// Change upper x bound only
		ptIndex = int(boundIndex[1] / 2.);
		// determine 2D bound to 3D bound match up
		 reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D lower bound instead of  bound
			boxBounds[boundIndex[0]] = pt2[ptIndex];
		else				// Alter 3D upper bound 
			boxBounds[boundIndex[1]] = pt2[ptIndex];
		break;
	case vtkBorderRepresentation::AdjustingE2:
		// Change upper y bound only
		ptIndex = int(boundIndex[3] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[2]] = pt2[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[3]] = pt2[ptIndex];
		break;
	case vtkBorderRepresentation::AdjustingE3:
		// Change lower x bound only
		ptIndex = int(boundIndex[0] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[1]] = pt1[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[0]] = pt1[ptIndex];
		break;
	case vtkBorderRepresentation::AdjustingP0:
		// E0
		// Change lower y bound only
		ptIndex = int(boundIndex[2] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[3]] = pt1[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[2]] = pt1[ptIndex];

		// E3
		// Change lower x bound only
		ptIndex = int(boundIndex[0] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[1]] = pt1[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[0]] = pt1[ptIndex];
		break;
	case vtkBorderRepresentation::AdjustingP1:
		// E0
		// Change lower y bound only
		ptIndex = int(boundIndex[2] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[3]] = pt1[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[2]] = pt1[ptIndex];
		
		// E1		
		// Change upper x bound only
		ptIndex = int(boundIndex[1] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D lower bound instead of  bound
			boxBounds[boundIndex[0]] = pt2[ptIndex];
		else				// Alter 3D upper bound 
			boxBounds[boundIndex[1]] = pt2[ptIndex];
		break;
	case vtkBorderRepresentation::AdjustingP2:
		// E1
		// Change upper y bound only
		ptIndex = int(boundIndex[3] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[2]] = pt2[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[3]] = pt2[ptIndex];

		// E2
		// Change upper x bound only
		ptIndex = int(boundIndex[1] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D lower bound instead of  bound
			boxBounds[boundIndex[0]] = pt2[ptIndex];
		else				// Alter 3D upper bound 
			boxBounds[boundIndex[1]] = pt2[ptIndex];
		break;
	case vtkBorderRepresentation::AdjustingP3:
		// E2
		// Change upper y bound only
		ptIndex = int(boundIndex[3] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[2]] = pt2[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[3]] = pt2[ptIndex];

		// E3
		// Change lower x bound only
		ptIndex = int(boundIndex[0] / 2.);
		// determine 2D bound to 3D bound matchup
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[1]] = pt1[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[0]] = pt1[ptIndex];

		break;
	case vtkBorderRepresentation::Inside:
		// E0
		// Change lower y bound only
		ptIndex = int(boundIndex[2] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[3]] = pt1[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[2]] = pt1[ptIndex];

		// E1
		// Change upper x bound only
		ptIndex = int(boundIndex[1] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D lower bound instead of  bound
			boxBounds[boundIndex[0]] = pt2[ptIndex];
		else				// Alter 3D upper bound 
			boxBounds[boundIndex[1]] = pt2[ptIndex];

		// E2
		// Change upper y bound only
		ptIndex = int(boundIndex[3] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[2]] = pt2[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[3]] = pt2[ptIndex];

		// E3
		// Change lower x bound only
		ptIndex = int(boundIndex[0] / 2.);
		// determine 2D bound to 3D bound match up
		reverseSides = { pt1[ptIndex] > pt2[ptIndex] ? true : false }; // True means pt1 actually represent the upper 3D bound in 3D space
		if (reverseSides)	// Alter 3D upper bound instead of lower bound
			boxBounds[boundIndex[1]] = pt1[ptIndex];
		else				// Alter 3D lower bound 
			boxBounds[boundIndex[0]] = pt1[ptIndex];
		break;
	default:
		break;
	}



	// Because place widget is affected by place factor, temperally set it to 1
	double placefactor = parentRep->GetPlaceFactor();
	parentRep->SetPlaceFactor(1);
	parentRep->PlaceWidget(boxBounds);
	parentRep->Modified();
	parentRep->SetPlaceFactor(placefactor);
}

//////////////////////////////////////////////////////////////////////////
/// Implementation for ROIWidget
vtkStandardNewMacro(vtkROIWidget);

vtkROIWidget::vtkROIWidget() : vtkBoxWidget2()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();

	for (int i = 0; i < 3;i++)
	{
		this->m_planes[i] = NULL;
		this->m_borderWidgets[i] = vtkSmartPointer<vtkROIBorderWidget>::New();
		this->m_borderWidgets[i]->SetROIParent(this);
		this->m_borderWidgets[i]->SetOrientation(i);
		this->m_borderWidgets[i]->SelectableOff();
		this->m_borderRep[i] = vtkSmartPointer<vtkBorderRepresentation>::New();
		this->m_borderRep[i]->GetBorderProperty()->SetColor(0.8, 0.2, 0.1);
		this->m_borderRep[i]->GetBorderProperty()->SetLineStipplePattern(0xFCFC); // ------..------..
		this->m_borderRep[i]->GetBorderProperty()->SetLineWidth(2.);
	}

	/// Callback
	this->m_callback = vtkSmartPointer<vtkROIUpdateAllBorderWidgetCallBack>::New();
	this->m_callback->SetUpdateTarget(this);
	
	this->SetRotationEnabled(0);

	/// Box update interaction observer
	vtkSmartPointer<vtkROIUpdateAllBorderWidgetCallBack> callback = vtkSmartPointer<vtkROIUpdateAllBorderWidgetCallBack>::New();
	callback->SetUpdateTarget(this);
	this->AddObserver(vtkCommand::InteractionEvent, callback);
	
	/// Widgets update interaction observers
	vtkSmartPointer<vtkROIBroderWidgetCallBack> bwCallback[3];
	for (int i = 0; i < 3; i++)
	{
		bwCallback[i] = vtkSmartPointer<vtkROIBroderWidgetCallBack>::New();
		this->m_borderWidgets[i]->AddObserver(vtkCommand::InteractionEvent, bwCallback[i]);
	}
}

vtkROIWidget::~vtkROIWidget()
{
	for (int i = 0; i < 3;i++)
	{
		if (m_borderWidgets[i] == NULL)
			continue;

		/*if (this->m_borderWidgets[i]->GetInteractor()) {
			this->m_borderWidgets[i]->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->RemoveAllObservers();
		}*/

		this->m_borderWidgets[i]->Off();
		this->m_borderWidgets[i]->SetInteractor(NULL);
		this->m_borderWidgets[i]->SetCurrentRenderer(NULL);
		this->m_borderWidgets[i]->SetDefaultRenderer(NULL);
		this->m_borderWidgets[i] = NULL;
	}
}

void vtkROIWidget::SetBorderWidgetsInteractor(int index, vtkRenderWindowInteractor *iren)
{
	this->m_borderWidgets[index]->SetInteractor(iren);
	this->m_borderWidgets[index]->SetCurrentRenderer(iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
	this->m_borderWidgets[index]->SetDefaultRenderer(iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
	this->m_borderRep[index]->SetRenderer(iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	this->m_borderWidgets[index]->SetOrientation(index);

	// Observer for updating borderwidgets
	iren->AddObserver(vtkCommand::MouseWheelBackwardEvent, this->m_callback);
	iren->AddObserver(vtkCommand::MouseWheelForwardEvent, this->m_callback);

	vtkSmartPointer<vtkCamera> interactorCamera = iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
	if (!interactorCamera->HasObserver(vtkCommand::ModifiedEvent)) {
		interactorCamera->AddObserver(vtkCommand::ModifiedEvent, this->m_callback);
	}
}

void vtkROIWidget::SetPositionPointer(double* pos)
{
	this->m_cursorPos = pos;
}

void vtkROIWidget::UpdateBorderWidgets()
{
	//if (!this->m_planes[0] || !this->m_planes[1] || !this->m_planes[2]) {
	//	return;
	//}
	if (!this->Enabled)
		return;

	/// obtain box widget info
	vtkBoxRepresentation* boxRep = reinterpret_cast<vtkBoxRepresentation*>(this->WidgetRep);
	vtkSmartPointer<vtkPolyData> boxRepPoly = vtkSmartPointer<vtkPolyData>::New();

	double vect[3], center[3], corner1[3], corner2[3];
	boxRep->GetPolyData(boxRepPoly);
	boxRepPoly->GetPoint(0, corner1);
	boxRepPoly->GetPoint(6, corner2);

	/// Update border Widgets accordingly
	for (int i = 0; i < 3; i++)
	{
		// if interactor is not set
		if (!this->m_borderWidgets[i]->GetInteractor()) {
			continue;
		}

		// if border widgets are not enabled
		if (!this->m_borderWidgets[i]->GetEnabled()) {
			continue;
		}

		// if user is interacting with that borderwidget
		if (this->m_borderWidgets[i]->GetRepresentation()->GetInteractionState() == vtkBorderRepresentation::Inside) {
			continue;
		}

		// #vtkROIWidgetModified
		if (m_cursorPos[i] < corner1[i] || m_cursorPos[i] > corner2[i]) {
			this->m_borderRep[i]->GetBorderProperty()->SetOpacity(0.4);
			this->m_borderRep[i]->SetPickable(false);
			this->m_borderWidgets[i]->GetInteractor()->Render();
		}
		else {
			this->m_borderRep[i]->SetPickable(true);
			this->m_borderRep[i]->GetBorderProperty()->SetOpacity(1);
		}

		// Note that these vectors should only has two element useful, but declare for the purpose of subtraction.
		double displayPoint1[3], displayPoint2[3], normedPt1[3], normedPt2[3];
		vtkRenderer* bwRen = this->m_borderWidgets[i]->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
		bwRen->SetWorldPoint(corner2);
		bwRen->WorldToDisplay();
		bwRen->GetDisplayPoint(displayPoint1);

		bwRen->DisplayToNormalizedDisplay(displayPoint1[0], displayPoint1[1]);
		bwRen->NormalizedDisplayToViewport(displayPoint1[0], displayPoint1[1]);
		bwRen->ViewportToNormalizedViewport(displayPoint1[0], displayPoint1[1]);

		bwRen->SetWorldPoint(corner1);
		bwRen->WorldToDisplay();
		bwRen->GetDisplayPoint(displayPoint2);

		bwRen->DisplayToNormalizedDisplay(displayPoint2[0], displayPoint2[1]);
		bwRen->NormalizedDisplayToViewport(displayPoint2[0], displayPoint2[1]);
		bwRen->ViewportToNormalizedViewport(displayPoint2[0], displayPoint2[1]);

		// Make sure that pt1 is at the lower left corner wheras pt2 at the upper right corner in the view port coordinate
		normedPt1[0] = { displayPoint1[0] < displayPoint2[0] ? displayPoint1[0] : displayPoint2[0] };
		normedPt1[1] = { displayPoint1[1] < displayPoint2[1] ? displayPoint1[1] : displayPoint2[1] };
		normedPt2[0] = { displayPoint1[0] > displayPoint2[0] ? displayPoint1[0] : displayPoint2[0] };
		normedPt2[1] = { displayPoint1[1] > displayPoint2[1] ? displayPoint1[1] : displayPoint2[1] };
		vtkMath::Subtract(normedPt2, normedPt1, normedPt2);

		this->m_borderWidgets[i]->SetRepresentation(m_borderRep[i]);
		
		this->m_borderRep[i]->SetPosition(normedPt1);
		this->m_borderRep[i]->SetPosition2(normedPt2);
		this->m_borderRep[i]->Modified();

		this->m_borderWidgets[i]->Modified();
		this->m_borderWidgets[i]->GetInteractor()->Render();
	}
	this->Interactor->Render();
}

void vtkROIWidget::SetEnabled(int b)
{
	vtkBoxWidget2::SetEnabled(b);

	if (b) {

		for (int i = 0; i < 3; i++)
		{
			if (!m_borderWidgets[i]) continue;
			if (this->m_borderWidgets[i]->GetInteractor() != NULL) {
				this->m_borderWidgets[i]->On();
			}
		}

		this->UpdateBorderWidgets();
		for (int i = 0; i < 3; i++)
		{
			if (!m_borderWidgets[i]) continue;
			if (this->m_borderWidgets[i]->GetInteractor() != NULL)
				this->m_borderWidgets[i]->GetInteractor()->Render();
		}
	}
	else {
		for (int i = 0; i < 3;i++)
		{
			if (!m_borderWidgets[i]) continue;
			this->m_borderWidgets[i]->Off();
			//this->m_borderWidgets[i]->GetInteractor()->Render();
		}
		
	}
 	this->Interactor->Render();
}

void vtkROIWidget::EnabledOn()
{
	this->SetEnabled(true);
}

void vtkROIWidget::EnabledOff()
{
	this->SetEnabled(false);
}

void vtkROIWidget::SetSlicePlane(int index, vtkPlane *plane)
{
	this->m_planes[index] = plane;
}

void vtkROIBroderWidgetCallBack::Execute(vtkObject * caller, unsigned long, void *)
{
	vtkROIBorderWidget *borderWidget =
		reinterpret_cast<vtkROIBorderWidget*>(caller);

	borderWidget->UpdateROIWidget();
	borderWidget->GetROIParent()->UpdateBorderWidgets();
}

void vtkROIUpdateAllBorderWidgetCallBack::Execute(vtkObject * caller, unsigned long, void *)
{
	UpdateTarget->UpdateBorderWidgets();
}

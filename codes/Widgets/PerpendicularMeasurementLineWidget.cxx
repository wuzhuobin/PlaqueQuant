#include "PerpendicularMeasurementLineWidget.h"

#include <vtkObjectFactory.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkCommand.h>
#include <vtkWidgetEvent.h>
#include <vtkHandleWidget.h>
#include <vtkRenderWindow.h>
#include <vtkCallbackCommand.h>
#include <vtkDistanceWidget.h>
#include <vtkLineRepresentation.h>
#include <vtkPolyData.h>
#include <vtkLine.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkCleanPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkDijkstraGraphGeodesicPath.h>
#include <vtkKdTreePointLocator.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(PerpendicularMeasurementLineWidget);

void PerpendicularMeasurementLineWidget::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkLineWidget2::PrintSelf(os, indent);
	this->DistanceWidget->PrintSelf(os, indent);
}

void PerpendicularMeasurementLineWidget::SetEnabled(int enabling)
{
	vtkLineWidget2::SetEnabled(enabling);
	if (enabling) {
		this->DistanceWidget->CreateDefaultRepresentation();
		this->DistanceWidget->SetInteractor(this->Interactor);
		this->DistanceWidget->GetRepresentation()->SetRenderer(this->CurrentRenderer);
		this->DistanceWidget->SetWidgetStateToManipulate();
	}
}

void PerpendicularMeasurementLineWidget::CreateDefaultRepresentation()
{
	vtkLineWidget2::CreateDefaultRepresentation();
	this->GetLineRepresentation()->DistanceAnnotationVisibilityOn();
	this->DistanceWidget->CreateDefaultRepresentation();
}

void PerpendicularMeasurementLineWidget::SetLine(vtkPolyData * line)
{
	vtkSmartPointer<vtkCleanPolyData> cleanPolyData =
		vtkSmartPointer<vtkCleanPolyData>::New();
	cleanPolyData->SetInputData(line);
	cleanPolyData->PointMergingOn();
	cleanPolyData->Update();

	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputConnection(cleanPolyData->GetOutputPort());
	triangleFilter->Update();
	TriangulatedLine = triangleFilter->GetOutput();

	PointLocater = vtkSmartPointer<vtkKdTreePointLocator>::New();
	PointLocater->SetDataSet(TriangulatedLine);
	PointLocater->BuildLocator();
}

vtkDistanceWidget * PerpendicularMeasurementLineWidget::GetDistanceWidget()
{
	return this->DistanceWidget;
}

void PerpendicularMeasurementLineWidget::SetProcessEvents(int flag)
{
	vtkAbstractWidget::SetProcessEvents(flag);

	this->Point1Widget->SetProcessEvents(flag);
	this->Point2Widget->SetProcessEvents(flag);
}

PerpendicularMeasurementLineWidget::PerpendicularMeasurementLineWidget()
{
	// Define widget events
	// remove unnecessary event by a do nothing callback lambda function
	//this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent,
	//	vtkWidgetEvent::EndSelect,
	//	this, PerpendicularMeasurementLineWidget::EndSelectAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MiddleButtonPressEvent,
		vtkWidgetEvent::Translate,
		this, [](vtkAbstractWidget*) {});
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MiddleButtonReleaseEvent,
		vtkWidgetEvent::EndTranslate,
		this, [](vtkAbstractWidget*) {});
	this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent,
		vtkWidgetEvent::Scale,
		this, [](vtkAbstractWidget*) {});
	this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonReleaseEvent,
		vtkWidgetEvent::EndScale,
		this, [](vtkAbstractWidget*) {});
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent,
		vtkWidgetEvent::Move,
		this, PerpendicularMeasurementLineWidget::MoveAction);

	// disable the #LineHandle widget reponds to interaction
	this->LineHandle->ProcessEventsOff();

	DistanceWidget = vtkDistanceWidget::New();
	DistanceWidget->ProcessEventsOff();
}

PerpendicularMeasurementLineWidget::~PerpendicularMeasurementLineWidget()
{
	DistanceWidget->Delete();
}

//void PerpendicularMeasurementLineWidget::EndSelectAction(vtkAbstractWidget * w)
//{
//	vtkLineWidget2::EndSelectAction(w);
//	PerpendicularMeasurementLineWidget *self =
//		reinterpret_cast<PerpendicularMeasurementLineWidget*>(w);
//	if (self->WidgetState == vtkLineWidget2::Active && self->TriangulatedLine != nullptr)
//	{
//		double pos1[3], pos2[3];
//		self->GetLineRepresentation()->GetPoint1WorldPosition(pos1);
//		self->GetLineRepresentation()->GetPoint2WorldPosition(pos2);
//
//		vtkSmartPointer<vtkDijkstraGraphGeodesicPath> geodesicPath =
//			vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();
//		geodesicPath->SetInputData(self->TriangulatedLine);
//		geodesicPath->SetStartVertex(self->PointLocater->FindClosestPoint(pos1));
//		geodesicPath->SetEndVertex(self->PointLocater->FindClosestPoint(pos2));
//		geodesicPath->Update();
//
//		double perpendicularDistance = VTK_DOUBLE_MIN;
//		double perpendicularPoint1[3], perpendicularPoint2[3];
//		for (vtkIdType id = 0; id < geodesicPath->GetOutput()->GetNumberOfPoints(); ++id) {
//			double t;
//			double closest[3];
//			double dist0 = vtkLine::DistanceToLine(geodesicPath->GetOutput()->GetPoint(id), pos1, pos2, t, closest);
//			if (dist0 > perpendicularDistance) {
//				perpendicularDistance = dist0;
//				memcpy(perpendicularPoint1, geodesicPath->GetOutput()->GetPoint(id), sizeof(perpendicularPoint1));
//				memcpy(perpendicularPoint2, closest, sizeof(perpendicularPoint2));
//			}
//		}
//		cout << "farthest distance: " << sqrt(perpendicularDistance)<< endl;
//		self->DistanceWidget->EnabledOn();
//		self->DistanceWidget->GetDistanceRepresentation()->SetPoint1WorldPosition(perpendicularPoint1);
//		self->DistanceWidget->GetDistanceRepresentation()->SetPoint2WorldPosition(perpendicularPoint2);
//	}
//
//}

void PerpendicularMeasurementLineWidget::MoveAction(vtkAbstractWidget * w)
{
	PerpendicularMeasurementLineWidget *self = reinterpret_cast<PerpendicularMeasurementLineWidget*>(w);
	// compute some info we need for all cases
	int X = self->Interactor->GetEventPosition()[0];
	int Y = self->Interactor->GetEventPosition()[1];

	// See whether we're active
	if (self->WidgetState == vtkLineWidget2::Start)
	{
		self->Interactor->Disable(); //avoid extra renders
		self->Point1Widget->SetEnabled(0);
		self->Point2Widget->SetEnabled(0);
		self->LineHandle->SetEnabled(0);

		int oldState = self->WidgetRep->GetInteractionState();
		int state = self->WidgetRep->ComputeInteractionState(X, Y);
		int changed;
		// Determine if we are near the end points or the line
		if (state == vtkLineRepresentation::Outside)
		{
			changed = self->RequestCursorShape(VTK_CURSOR_DEFAULT);
		}
		else //must be near something
		{
			changed = self->RequestCursorShape(VTK_CURSOR_HAND);
			if (state == vtkLineRepresentation::OnP1)
			{
				self->Point1Widget->SetEnabled(1);
			}
			else if (state == vtkLineRepresentation::OnP2)
			{
				self->Point2Widget->SetEnabled(1);
			}
			// remove the OnLine interaction response
			//else //if ( state == vtkLineRepresentation::OnLine )
			//{
			//	self->LineHandle->SetEnabled(1);
			//	changed = 1; //movement along the line always needs render
			//}
		}
		self->Interactor->Enable(); //avoid extra renders
		if (changed || oldState != state)
		{
			self->Render();
		}
	}
	else //if ( self->WidgetState == vtkLineWidget2::Active )
	{
		// moving something
		double e[2];
		e[0] = static_cast<double>(X);
		e[1] = static_cast<double>(Y);
		self->InvokeEvent(vtkCommand::MouseMoveEvent, NULL); //handles observe this
		reinterpret_cast<vtkLineRepresentation*>(self->WidgetRep)->
			WidgetInteraction(e);
		self->InvokeEvent(vtkCommand::InteractionEvent, NULL);
		self->EventCallbackCommand->SetAbortFlag(1);
		self->Render();

		double pos1[3], pos2[3];
		self->GetLineRepresentation()->GetPoint1WorldPosition(pos1);
		self->GetLineRepresentation()->GetPoint2WorldPosition(pos2);

		vtkSmartPointer<vtkDijkstraGraphGeodesicPath> geodesicPath =
			vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();
		geodesicPath->SetInputData(self->TriangulatedLine);
		geodesicPath->SetStartVertex(self->PointLocater->FindClosestPoint(pos1));
		geodesicPath->SetEndVertex(self->PointLocater->FindClosestPoint(pos2));
		geodesicPath->StopWhenEndReachedOn();
		geodesicPath->Update();

		double perpendicularDistance = VTK_DOUBLE_MIN;
		double perpendicularPoint1[3], perpendicularPoint2[3];
		for (vtkIdType id = 0; id < geodesicPath->GetOutput()->GetNumberOfPoints(); ++id) {
			double t;
			double closest[3];
			double dist0 = vtkLine::DistanceToLine(geodesicPath->GetOutput()->GetPoint(id), pos1, pos2, t, closest);
			if (dist0 > perpendicularDistance) {
				perpendicularDistance = dist0;
				memcpy(perpendicularPoint1, geodesicPath->GetOutput()->GetPoint(id), sizeof(perpendicularPoint1));
				memcpy(perpendicularPoint2, closest, sizeof(perpendicularPoint2));
			}
		}
		cout << "farthest distance: " << sqrt(perpendicularDistance) << endl;
		self->DistanceWidget->EnabledOn();
		self->DistanceWidget->GetDistanceRepresentation()->SetPoint1WorldPosition(perpendicularPoint1);
		self->DistanceWidget->GetDistanceRepresentation()->SetPoint2WorldPosition(perpendicularPoint2);
	}
}

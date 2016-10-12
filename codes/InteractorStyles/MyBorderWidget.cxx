#include "MyBorderWidget.h"

#include <vtkBorderRepresentation.h>
#include <vtkProperty2D.h>

vtkStandardNewMacro(MyBorderWidget);

void MyBorderWidget::UpdateBorderWidget()
{

}

void MyBorderWidget::SetImageViewer(vtkImageViewer2 * viewer)
{
	m_imageViewer = viewer;
	
}

void MyBorderWidget::SetLowerLeft(double x, double y, double z)
{
	vtkRenderer* renderer = m_imageViewer->GetRenderer();
	renderer->SetWorldPoint(x, y, z, 1);
	renderer->WorldToView();
	//renderer->norma

	//GetBorderRepresentation()->GetPositionCoordinate()->SetValue(x, y ,z);
}

void MyBorderWidget::SetUpperRight(double x, double y, double z)
{
	vtkRenderer* renderer = m_imageViewer->GetRenderer();

	GetBorderRepresentation()->GetPosition2Coordinate()->SetValue(x, y, z);
}

MyBorderWidgetCallback * MyBorderWidget::GetBorderWidgetCallback()
{
	return m_callBack;
}

MyBorderWidget::MyBorderWidget()
{
	CreateDefaultRepresentation();
	GetBorderRepresentation()->GetBorderProperty()->SetColor(0.8, 0.2, 0.1);
	GetBorderRepresentation()->GetBorderProperty()->SetLineStipplePattern(0xFCFC); // ------..------..
	GetBorderRepresentation()->GetBorderProperty()->SetLineWidth(2.);

	SelectableOff();
	m_callBack = vtkSmartPointer<MyBorderWidgetCallback>::New();
	this->AddObserver(vtkCommand::InteractionEvent, m_callBack);
	m_callBack->AddBorderWidget(this);
}

MyBorderWidget::~MyBorderWidget()
{
}


void MyBorderWidgetCallback::Execute(vtkObject * caller, unsigned long ev, void *)
{
	MyBorderWidget* borderWidget = dynamic_cast<MyBorderWidget*>(caller);
	if (borderWidget == nullptr) 
		return;
	//if (1) {

	if (ev == InteractionEvent) {
		vtkBorderRepresentation* borderRep = borderWidget->GetBorderRepresentation();
		vtkCoordinate* pCoordinate = borderRep->GetPositionCoordinate();
		//const double* point1 = pCoordinate->GetComputedWorldValue(m_borderWidget->m_imageViewer->GetRenderer());
		vtkCoordinate* p2Coordinate = borderRep->GetPosition2Coordinate();
		//const double* point2 = p2Coordinate->GetComputedWorldValue(m_borderWidget->m_imageViewer->GetRenderer());
		cout << "Point1: ";
		//cout << point1[0] << ' ' << point1[1] << ' ' << point1[2] << endl;
		cout << pCoordinate->GetValue()[0] << pCoordinate->GetValue()[1] << pCoordinate->GetValue()[2] << endl;
		cout << "Point2: ";
		//cout << point2[0] << ' ' << point2[1] << ' ' << point2[2] << endl;
		cout << p2Coordinate->GetValue()[0] << p2Coordinate->GetValue()[1] << p2Coordinate->GetValue()[2] << endl;
		switch (borderWidget->m_imageViewer->GetSliceOrientation())
		{
		case vtkImageViewer2::SLICE_ORIENTATION_XY:
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_XZ:
			break;
		case vtkImageViewer2::SLICE_ORIENTATION_YZ:
			break;
		default:
			break;
		}
	}

}

void MyBorderWidgetCallback::SetMyBorderWidget(MyBorderWidget* myBorderWidget)
{
	m_borderWidget = myBorderWidget;
}

void MyBorderWidgetCallback::AddBorderWidget(MyBorderWidget * borderWidget)
{
	m_borderWidgets.push_back(borderWidget);
}

void MyBorderWidgetCallback::SetBorderWidgets(std::list<MyBorderWidget*> borderWidgets)
{
	m_borderWidgets = borderWidgets;
}

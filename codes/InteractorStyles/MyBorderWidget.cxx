#include "MyBorderWidget.h"


void MyBorderWidget::SetImageViewer(MyImageViewer * viewer)
{
	m_imageViewer = viewer;
}

MyBorderWidget::MyBorderWidget()
{
}

MyBorderWidget::~MyBorderWidget()
{
}

void MyBorderWidgetCallback::Execute(vtkObject * caller, unsigned long ev, void *)
{
}

void MyBorderWidgetCallback::AddBorderWidget(vtkBorderWidget * borderWidget)
{
	m_borderWidgets.push_back(borderWidget);
}

void MyBorderWidgetCallback::SetBorderWidgets(std::list<vtkBorderWidget*> borderWidgets)
{
	m_borderWidgets = borderWidgets;
}

#ifndef DISTANCE_3DWIDGET_H
#define DISTANCE_3DWIDGET_H

#include <vtkSmartPointer.h>
#include <vtkDistanceWidget.h>
#include <vtkRenderWindowInteractor.h>

class MyDistanceWidget;

class Distance3DWidget
{
public:
	Distance3DWidget();
	~Distance3DWidget();

	void Set3DRulerEnabled(bool);
	void SetInteractor(vtkRenderWindowInteractor*);
	vtkRenderWindowInteractor* GetInteractor();
	bool CalculateIndex();
private:
	vtkSmartPointer<vtkDistanceWidget> m_distanceWidget;
	int m_currentPos[3];
};

class MyDistanceWidget : public vtkDistanceWidget {

public:
	void AddPointAction(vtkAbstractWidget *w);

};

#endif
#ifndef VTK_ROI_WIDGET_H
#define VTK_ROI_WIDGET_H

#include <QObject>
#include <vtkSetGet.h>
#include <vtkCallbackCommand.h>
#include <vtkSmartPointer.h>
#include <vtkBoxRepresentation.h>
#include <vtkBorderRepresentation.h>
#include <vtkBorderWidget.h>
#include <vtkBoxWidget2.h>
#include <vtkPlaneCollection.h>
#include <vtkPlane.h>
#include <vtkSphereSource.h>

class vtkROIWidget;
class vtkROIUpdateAllBorderWidgetCallBack;

/// ROI border widget for 2D
class vtkROIBorderWidget : public vtkBorderWidget
{
private:
	typedef vtkROIWidget ParentType;

public:
	static vtkROIBorderWidget *New();
	vtkTypeMacro(vtkROIBorderWidget, vtkBorderWidget);

	vtkGetMacro(Orientation, int);
	vtkSetMacro(Orientation, int);

	vtkGetMacro(ROIParent, ParentType*);
	vtkSetMacro(ROIParent, ParentType*);

	enum {
		ORIENTATION_XY = 2,
		ORIENTATION_XZ = 1,
		ORIENTATION_YZ = 0
	};

	virtual void UpdateROIWidget();

protected:
	vtkROIBorderWidget();
	~vtkROIBorderWidget();


	virtual int SubclassSelectAction() override;
	virtual void SelectRegion(double eventPos[2]) override;
	virtual void SetCursor(int State) override;

private:
	ParentType* ROIParent;
	int Orientation;
};

//////////////////////////////////////////////////////////////////////////
/// ROI box widget for 3D
class vtkROIWidget : public QObject, public vtkBoxWidget2
{
	Q_OBJECT
public:
	static vtkROIWidget *New();

	vtkTypeMacro(vtkROIWidget, vtkBoxWidget2);

	virtual void SetPositionPointer(double* pos);

	virtual void UpdateBorderWidgets();
	virtual void SetBorderWidgetsInteractor(int, vtkRenderWindowInteractor*);

	virtual void SetEnabled(int);
	virtual void EnabledOn();
	virtual void EnabledOff();

	virtual void SetSlicePlane(int, vtkPlane*);

public slots:

signals:
	void signalROIBounds(double*);

protected:
	vtkROIWidget();
	~vtkROIWidget();

	vtkSmartPointer<vtkPlane>					m_planes[3];
	vtkSmartPointer<vtkROIBorderWidget>			m_borderWidgets[3];
	vtkSmartPointer<vtkBorderRepresentation>	m_borderRep[3];
	vtkSmartPointer<vtkROIUpdateAllBorderWidgetCallBack> m_callback;

	double* m_cursorPos;

private:

};

//////////////////////////////////////////////////////////////////////////
/// Call back for adjusting box widget according to border widget
class vtkROIBroderWidgetCallBack : public vtkCommand
{
public:
	static vtkROIBroderWidgetCallBack *New()
	{
		return new vtkROIBroderWidgetCallBack;
	}

	virtual void Execute(vtkObject *caller, unsigned long, void*);

	vtkROIBroderWidgetCallBack() {}
};

/// Call back for adjusting border widget according to box widget
class vtkROIUpdateAllBorderWidgetCallBack : public vtkCommand
{
public:
	static vtkROIUpdateAllBorderWidgetCallBack *New()
	{
		return new vtkROIUpdateAllBorderWidgetCallBack;
	}

	void SetUpdateTarget(vtkROIWidget* target) { UpdateTarget = target; }

	virtual void Execute(vtkObject *caller, unsigned long, void*);

	vtkROIUpdateAllBorderWidgetCallBack() {}
private:
	vtkROIWidget* UpdateTarget;
};
#endif
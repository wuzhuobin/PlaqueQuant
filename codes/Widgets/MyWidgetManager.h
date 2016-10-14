#ifndef __MYWIDGETMANAGER_H__
#define __MYWIDGETMANAGER_H__

#include <qobject.h>
#include <qlist.h>

#include <vtkAbstractWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkROIWidget.h"

class MyImageViewer;

class MyWidgetManager : public QObject
{
	Q_OBJECT
public:

	MyWidgetManager(QObject* parent = nullptr);
	~MyWidgetManager();


	void SetInteractor(vtkRenderWindowInteractor* interactor);
	void EnableROIWidget(
		vtkSmartPointer<MyImageViewer> viewer2D[3], 
		vtkRenderWindow* viewer3D, 
		double* placeBounds, 
		double* cursorPos);
	void EnableROIWidget();
	void DisableROIWidget();


	vtkROIWidget* GetROIWidget();

	
protected:

	QList<vtkAbstractWidget*> m_widgets;
	vtkSmartPointer<vtkROIWidget> m_roi;
	vtkRenderWindowInteractor* m_interactor;

private:
	

};



#endif // !__MYWIDGETMANAGER_H__———
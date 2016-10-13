#ifndef __MYWIDGETMANAGER_H__
#define __MYWIDGETMANAGER_H__

#include <qobject.h>
#include <qlist.h>

#include <vtkAbstractWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

#include "vtkROIWidget.h"

class MyWidgetManager : public QObject
{
	Q_OBJECT
public:

	MyWidgetManager(QObject* parent = nullptr);
	~MyWidgetManager();


	void SetInteractor(vtkRenderWindowInteractor* interactor);

	void EnableROIWidget();


	vtkROIWidget* GetROIWidget();

	
protected:

	QList<vtkAbstractWidget*> m_widgets;
	vtkSmartPointer<vtkROIWidget> m_roiWidget = nullptr;
	vtkRenderWindowInteractor* m_interactor = nullptr;

private:
	

};



#endif // !__MYWIDGETMANAGER_H__
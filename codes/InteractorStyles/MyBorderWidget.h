#ifndef __MyBorderWidget_H__
#define __MyBorderWidget_H__

#include <vtkBorderWidget.h>
#include <vtkCommand.h>
#include <vtkImageViewer2.h>

#include <list>

#include "MyImageViewer.h"

class MyBorderWidgetCallback;
class MyBorderWidget: public vtkBorderWidget
{
public:
	static MyBorderWidget *New();
	vtkTypeMacro(MyBorderWidget, vtkBorderWidget);

	void UpdateBorderWidget();
	void SetImageViewer(vtkImageViewer2* viewer);
	void SetLowerLeft(double x, double y, double z);
	void SetUpperRight(double x, double y, double z);
	MyBorderWidgetCallback* GetBorderWidgetCallback();

protected:
	MyBorderWidget();
	~MyBorderWidget();

private:
	friend class MyBorderWidgetCallback;
	vtkSmartPointer<MyBorderWidgetCallback> m_callBack = nullptr;
	vtkImageViewer2* m_imageViewer = nullptr;
};


class MyBorderWidgetCallback : public vtkCommand
{
public:
	static MyBorderWidgetCallback *New() {
		return new MyBorderWidgetCallback;
	};
	vtkTypeMacro(MyBorderWidgetCallback, vtkCommand);

	virtual void Execute(vtkObject *caller, unsigned long ev, void*);
	void SetMyBorderWidget(MyBorderWidget* myBorderWidget);

	void AddBorderWidget(MyBorderWidget* borderWidget);
	void SetBorderWidgets(std::list<MyBorderWidget*> borderWidgets);


private:
	std::list<MyBorderWidget*> m_borderWidgets;
	MyBorderWidget* m_borderWidget;

};



#endif // !__MyBorderWidget_H__
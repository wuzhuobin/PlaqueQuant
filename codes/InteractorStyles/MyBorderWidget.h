#ifndef __MyBorderWidget_H__
#define __MyBorderWidget_H__

#include <vtkBorderWidget.h>
#include <vtkCommand.h>

#include <list>

#include "MyImageViewer.h"

class MyBorderWidgetCallback;
class MyBorderWidget: public vtkBorderWidget
{
public:
	static MyBorderWidget *New();
	vtkTypeMacro(MyBorderWidget, vtkBorderWidget);

	void SetImageViewer(MyImageViewer* viewer);


protected:
	MyBorderWidget();
	~MyBorderWidget();

private:
	friend class MyBorderWidgetCallback;
	MyImageViewer* m_imageViewer;
};


class MyBorderWidgetCallback : public vtkCommand
{
public:
	static MyBorderWidgetCallback *New() {
		return new MyBorderWidgetCallback;
	};
	vtkTypeMacro(MyBorderWidgetCallback, vtkCommand);

	virtual void Execute(vtkObject *caller, unsigned long ev, void*);

	void AddBorderWidget(vtkBorderWidget* borderWidget);
	void SetBorderWidgets(std::list<vtkBorderWidget*> borderWidgets);


private:
	std::list<vtkBorderWidget*> m_borderWidgets;
};



#endif // !__MyBorderWidget_H__
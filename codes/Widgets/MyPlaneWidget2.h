#ifndef MyPlaneWidget2_H
#define MyPlaneWidget2_H

#include "vtkCommand.h"
#include "vtkCallbackCommand.h"
#include "vtkPlaneSource.h"
#include "vtkPlaneWidget.h"
#include "vtkProperty.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkImageData.h"
#include "vtkActor.h"
#include "MyImageViewer.h"

#include <list>

 class MyPlaneWidget2 : public vtkPlaneWidget
{
public:
	static MyPlaneWidget2 *New();
	vtkTypeMacro(MyPlaneWidget2, vtkPlaneWidget);

	virtual void initializeCustomFunction();
	void ReplaceWidget(double bds[6]);
	
	void SetDefaultBound(double* bound);
	void SetImageViewer(MyImageViewer* viewer);
	void SetCurrentBound(double* bound);
	void SetVisibility(bool b);	
	
	double* GetDefaultBound();	
	double* GetCurrentBound();
	MyImageViewer* GetImageViewer();

private:
	double m_deafultBound[6];
	double m_currentBound[6];
	MyImageViewer* m_2DImageViewer;
};

class MyPlaneWidget2Callback : public vtkCommand
{
public:
	MyPlaneWidget2Callback(){}

	static MyPlaneWidget2Callback *New(){return new MyPlaneWidget2Callback;}

	virtual void Execute(vtkObject *caller, unsigned long ev, void* );
	void SetPlaneWidget(MyPlaneWidget2* s, MyPlaneWidget2* c, MyPlaneWidget2* a);

private:
	//std::list<MyPlaneWidget2> 
	MyPlaneWidget2* m_planeWidget[3];

};


#endif //MyPlaneWidget2_H

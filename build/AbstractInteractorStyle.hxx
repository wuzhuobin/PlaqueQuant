#ifndef ABSTRACT_INTERACTOR_STYLE_H
#define ABSTRACT_INTERACTOR_STYLE_H

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPropPicker.h>


class AbstractInteractorStyle
{
public:
	AbstractInteractorStyle();
	~AbstractInteractorStyle();

	void SetViewers(MyViewer**);
	virtual void SetOrientation(int);
	
	MyViewer**	GetViewers();
	MyViewer*	GetViewer(int);
	int GetOrientation();

	vtkActor* PickActor(int x, int y);

protected:
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonDown();
	virtual void OnMiddleButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonUp();
	virtual void OnMiddleButtonUp();

	MyViewer*					m_viewers[4];
	
	int			m_orientation;

	bool m_rightFunctioning;
	bool m_leftFunctioning;
	bool m_middleFunctioning;
};

#endif //ABSTRACT_INTERACTOR_STYLE_H
/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract interactor class used in medical viewers.
This class contains methods of image related processes, including changing slice position,
zoomming, dragging...etc.


Wong Matthew Lun
Copyright (C) 2016
*/

#ifndef ABSTRACT_INTERACTOR_STYLE_IMAGE_H
#define ABSTRACT_INTERACTOR_STYLE_IMAGE_H


/// VTK
#include <vtkInteractorStyleImage.h>
#include <vtkObjectFactory.h>

#include <list>

#include "MyImageViewer.h"

class AbstractInteractorStyleImage : public vtkInteractorStyleImage
{
public:
	vtkTypeMacro(AbstractInteractorStyleImage, vtkInteractorStyleImage);
	static AbstractInteractorStyleImage *New();
	
	virtual void SetImageViewer(MyImageViewer* imageViewer);
	virtual void AddSynchronalViewer(MyImageViewer* imageViewer);
	virtual void SetSynchronalViewers(std::list<MyImageViewer*> synchronalViewers);
	virtual void SynchronizedZooming();
	virtual vtkActor* PickActor(int x, int y);
	virtual void SetCurrentSlice(int slice);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);


protected:
	AbstractInteractorStyleImage();
	~AbstractInteractorStyleImage();
	
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();
	virtual void OnMouseMove();
	virtual void OnChar();
	virtual void OnKeyPress();

	void MoveSliceForward();
	void MoveSliceBackward();

	MyImageViewer* m_imageViewer;
	std::list<MyImageViewer*> m_synchronalViewers;

	virtual int GetSlice();
	virtual int GetMinSlice();
	virtual int GetMaxSlice();
	virtual int GetSliceOrientation();
	virtual double* GetOrigin();
	virtual double* GetSpacing();
	virtual int* GetExtent();

	bool m_rightFunctioning;
	bool m_leftFunctioning;
	bool m_middleFunctioning;
};

#endif //ABSTRACT_INTERACTOR_STYLE_IMAGE_H
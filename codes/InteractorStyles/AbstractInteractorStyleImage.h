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


#include <vtkInteractorStyleImage.h>
#include <vtkImageViewer2.h>

#include "AbstractInteractorStyle.h"



#ifndef VIEWER_CONSTITERATOR(METHOD)
#define VIEWER_CONSTITERATOR(METHOD) \
for(std::list<vtkImageViewer2*>::const_iterator cit = \
	m_synchronalViewers.cbegin(); cit != m_synchronalViewers.cend(); ++cit){\
	(*cit)->##METHOD; \
}
#endif // !VIEWER_CONSTITERATOR(METHOD)

class AbstractInteractorStyleImage : public vtkInteractorStyleImage, 
	public AbstractInteractorStyle
{
public:
	vtkTypeMacro(AbstractInteractorStyleImage, vtkInteractorStyleImage);
	static AbstractInteractorStyleImage *New();
	
	virtual void SetImageViewer(vtkImageViewer2* imageViewer);
	virtual void SetCurrentSlice(int slice);
	virtual void EnableSynchronalZooming(bool flag);

protected:
	AbstractInteractorStyleImage();
	~AbstractInteractorStyleImage();

	virtual void AddSynchronalViewer(vtkImageViewer2* imageViewer);
	virtual void SynchronalZooming();
	
	void OnMouseWheelForward();
	void OnMouseWheelBackward();
	void OnLeftButtonDown();
	void OnLeftButtonUp();
	void OnLeftDoubleClick();
	void OnRightButtonDown();
	void OnRightButtonUp();
	void OnRightDoubleClick();
	void OnMiddleButtonDown();
	void OnMiddleButtonUp();
	void OnMiddleDoubleClick();
	void OnMouseMove();
	void OnChar();
	void OnKeyPress();

	virtual int GetSlice();
	virtual int GetMinSlice();
	virtual int GetMaxSlice();
	virtual int GetSliceOrientation();
	virtual double* GetOrigin();
	virtual double* GetSpacing();
	virtual int* GetExtent();

	static std::list<vtkImageViewer2*> m_synchronalViewers;
	vtkImageViewer2* m_imageViewer;

private:
	const static int RESET_PIXEL_DISTANCE = 5;
	bool CheckMoveDistance();
	bool m_synchronalZoomingFlag = true;
	int PreviousPosition[2] = { 0,0 };

};

#endif //ABSTRACT_INTERACTOR_STYLE_IMAGE_H
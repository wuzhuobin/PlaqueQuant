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

#ifndef __ABSTRACT_INTERACTOR_STYLE_IMAGE_H__
#define __ABSTRACT_INTERACTOR_STYLE_IMAGE_H__


#include "AbstractInteractorStyle.h"

#include <vtkInteractorStyleTrackballCamera.h>

#ifndef SAFE_DOWN_CAST_IMAGE_CONSTITERATOR
#define SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(STYLE_NAME, METHOD) \
for(std::list<AbstractInteractorStyleImage*>::const_iterator cit = \
	m_imageStyles.cbegin(); cit != m_imageStyles.cend(); ++cit){\
	STYLE_NAME* _style = STYLE_NAME::SafeDownCast(*cit); \
	if (_style != nullptr && _style->GetCustomEnabled()) { \
		_style->##METHOD; \
	} \
}
#endif // !SAFE_DOWN_CAST_IMAGE_CONSTITERATOR

class vtkImageViewer2;

class AbstractInteractorStyleImage : public vtkInteractorStyleTrackballCamera, 
	public AbstractInteractorStyle
{
public:
	vtkTypeMacro(AbstractInteractorStyleImage, vtkInteractorStyleTrackballCamera);
	//static AbstractInteractorStyleImage *New();
	
	virtual vtkImageViewer2* GetVtkImageViewer2();
	virtual void SetCurrentSlice(int slice);
	virtual void EnableSynchronalZooming(bool flag);

protected:
	AbstractInteractorStyleImage();
	virtual ~AbstractInteractorStyleImage();

	virtual void SynchronalZooming();
	
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnLeftDoubleClick();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnRightDoubleClick();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();
	virtual void OnMiddleDoubleClick();
	virtual void OnMouseMove();
	virtual void OnChar();
	virtual void OnKeyPress();


	int GetWindow();
	int GetLevel();
	int GetSlice();
	int GetMinSlice();
	int GetMaxSlice();
	int GetSliceOrientation();
	double* GetOrigin();
	double* GetSpacing();
	int* GetExtent();

	static std::list<AbstractInteractorStyleImage*> m_imageStyles;

	const static int RESET_PIXEL_DISTANCE = 5;
	bool CheckMoveDistance();
	bool m_synchronalZoomingFlag = true;
	int PreviousPosition[2] = { 0 };

};

#endif //__ABSTRACT_INTERACTOR_STYLE_IMAGE_H__
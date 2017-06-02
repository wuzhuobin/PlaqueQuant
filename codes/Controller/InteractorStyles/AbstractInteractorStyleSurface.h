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

#ifndef __ABSTRACT_INTERACTOR_STYLE_SURFACE_H__
#define __ABSTRACT_INTERACTOR_STYLE_SURFACE_H__


#include "AbstractInteractorStyle.h"

#include <vtkInteractorStyleTrackballCamera.h>

#ifndef SAFE_DOWN_CAST_SURFACE_CONSTITERATOR
#define SAFE_DOWN_CAST_SURFACE_CONSTITERATOR(STYLE_NAME, METHOD) \
for(std::list<AbstractInteractorStyleSurface*>::const_iterator cit = \
	m_surfaceStyles.cbegin(); cit != m_surfaceStyles.cend(); ++cit){\
	STYLE_NAME* _style = STYLE_NAME::SafeDownCast(*cit); \
	if (_style != nullptr && _style->GetCustomEnabled()) { \
		_style->##METHOD; \
	} \
}
#endif // !SAFE_DOWN_CAST_SURFACE_CONSTITERATOR


class SurfaceViewer;

class AbstractInteractorStyleSurface : public vtkInteractorStyleTrackballCamera,
	public AbstractInteractorStyle
{
public:
	vtkTypeMacro(AbstractInteractorStyleSurface, vtkInteractorStyleTrackballCamera);
	//static AbstractInteractorStyleSurface *New();
	
	virtual SurfaceViewer* GetSurfaceViewer();

protected:
	AbstractInteractorStyleSurface();
	virtual ~AbstractInteractorStyleSurface();

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

	double* GetOrigin();
	double* GetSpacing();
	int* GetExtent();

	static std::list<AbstractInteractorStyleSurface*> m_surfaceStyles;

	const static int RESET_PIXEL_DISTANCE = 5;
	bool CheckMoveDistance();
	int PreviousPosition[2] = { 0,0 };

private:

};

#endif //__ABSTRACT_INTERACTOR_STYLE_SURFACE_H__
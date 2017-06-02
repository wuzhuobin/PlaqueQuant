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

#ifndef __ABSTRACT_NAVIGATION_H__
#define __ABSTRACT_NAVIGATION_H__


#include "AbstractInteractorStyleImage.h"


class ImageViewer;

class AbstractNavigation : public AbstractInteractorStyleImage
{
public:
	vtkTypeMacro(AbstractNavigation, AbstractInteractorStyleImage);
	static AbstractNavigation *New();
	
	virtual ImageViewer* GetImageViewer();

	virtual void SetCurrentSlice(int slice);
	virtual void SetCurrentFocalPointWithImageCoordinate(int* ijk);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
	virtual void SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k);


protected:
	AbstractNavigation();
	virtual ~AbstractNavigation();

	
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnKeyPress();
	virtual void MoveSliceForward();
	virtual void MoveSliceBackward();


};

#endif //__ABSTRACT_NAVIGATION_H__
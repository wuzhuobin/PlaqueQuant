/*
Author:		Wong, Matthew Lun
Date:		22, February 2017
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

FileName: 	MyResliceMapper.h

Descriptions:
	This file is a test reslicer inheriting VTK original class
	but provide a function that allows the output of the data
	of that resliced part.

Wong Matthew Lun
Copyright (C) 2017
*/

#ifndef MY_RESLICE_MAPPER
#define MY_RESLICE_MAPPER

#include "vtkImageResliceMapper.h"
#include "vtkImageResliceToColors.h"
#include "vtkObjectFactory.h"

class MyResliceMapper : public vtkImageResliceMapper
{
public:
	static MyResliceMapper* New();
	vtkTypeMacro(MyResliceMapper, vtkImageResliceMapper);

	vtkGetMacro(ImageReslice, vtkImageResliceToColors*);


protected:
	MyResliceMapper();
	~MyResliceMapper();

private:
	
};

#endif

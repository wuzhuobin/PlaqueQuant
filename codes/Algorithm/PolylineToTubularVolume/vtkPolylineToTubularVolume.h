/*
Author:		Wong, Matthew Lun
Date:		27, February 2017
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

FileName: 	vtkPolylineToTubularVolume.h

Descriptions:
	This class creates a mask image according to the space of the input
polydata and the imagedata.

Wong Matthew Lun
Copyright (C) 2017
*/

#ifndef VTK_POLYLINE_TO_TUBULAR_VOLUME
#define	VTK_POLYLINE_TO_TUBULAR_VOLUME

#include "vtkPolyData.h"
#include "vtkImageData.h"
#include "vtkImageAlgorithm.h"
#include "vtkObjectFactory.h"

class vtkPolylineToTubularVolume : public vtkImageAlgorithm
{
public:
	static vtkPolylineToTubularVolume* New();
	vtkTypeMacro(vtkPolylineToTubularVolume, vtkImageAlgorithm);

	/* Set the radius of the tubular volume arround the polyline*/
	vtkSetClampMacro(TubeRadius, double, 1, sqrt(500));
	vtkGetMacro(TubeRadius, double);

	/* Set the reference polyline where a tubular volume is created
		surround it. */
	vtkSetMacro(Polyline, vtkPolyData*);
	vtkGetMacro(Polyline, vtkPolyData*);


protected:
	vtkPolylineToTubularVolume();
	~vtkPolylineToTubularVolume();

	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
	
	vtkPolyData* Polyline;
	double TubeRadius;

private:
	vtkPolylineToTubularVolume(const vtkPolylineToTubularVolume&);  // Not implemented.
	void operator=(const vtkPolylineToTubularVolume&);  // Not implemented.
};

#endif
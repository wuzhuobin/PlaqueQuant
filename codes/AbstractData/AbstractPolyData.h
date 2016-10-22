/*
Author:		Wong, Matthew Lun
Date:		26th, May 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract data class is designed to hold data structure typically used in a medical viewer.
This class handles all data type related to surface manipulation, which will eventually generate
a polydata. This class will also hold the actor.

Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef ABSTRACT_POLYDATA_H
#define ABSTRACT_POLYDATA_H


#include <QString>
#include <QObject>
#include <QFileInfo>

#include <vtkTriangleFilter.h>
#include <vtkSTLWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkAlgorithmOutput.h>
#include <vtkActor.h>
#include "AbstractData.h"

class AbstractPolyData :
	public AbstractData
{
public:
	AbstractPolyData();
	~AbstractPolyData();


	void SetColor(double*);
	void SetColor(double, double, double);
	void SetColor(vtkLookupTable*, double, double);
	void SetHoverColor(double*);
	void SetHoverColor(double, double, double);
	void SetSelectedColor(double*);
	void SetSelectedColor(double, double, double);
	void SetSurfaceRepresentation(int);
	
	virtual void SetActorState(int);
	/* Note that using this function may causes member user transform to detatch from member actor,
		if you dose not wish to do so,  you should use GetUserTransform() function to get member
		transform and modify it.*/
	virtual void SetUserTransform(vtkSmartPointer<vtkTransform>);
	virtual void SetVisibility(bool);
	virtual void SetOpacity(double);
	virtual void SetPickable(bool);

	virtual vtkSmartPointer<vtkTransform>	GetUserTransform();
	virtual vtkSmartPointer<vtkActor>	GetActor();
	
	vtkSmartPointer<vtkPolyData>	GetPolyData();
	vtkSmartPointer<vtkPolyData>	GetTransformedPolyData();
	double*							GetColor();
	double*							GetHoverColor();
	double*							GetSelectedColor();
	double							GetOpacity();
	int								GetActorState();
	bool							GetPickable();

	void SavePolyData(QString path);
	
	virtual void Update();


protected:
	/// Member functions
	/* Call this after polydata is prepared */
	virtual void InstallPipeline();

	virtual void InitializePorts();
	virtual void InitializeActors();


	/// Member variables
	enum ABSTRACT_POLY_DATA_ERROR {
		POLYDATA_MISSING_ERROR = 1,
		ACTORS_NOT_INITIALIZED_ERROR = 2,
		TRANSFORM_MISSING_ERROR = 3,
		UNKONW_ACTOR_REPRESENTATION_ERROR = 4, 
		WRITE_POLYDATA_ERROR = 5
	};

	vtkSmartPointer<vtkPolyData>					m_polyData;
	vtkSmartPointer<vtkActor>						m_actor;
	vtkSmartPointer<vtkTransform>					m_userTransform;
	vtkSmartPointer<vtkTransformPolyDataFilter>		m_transformFilter;

	double* m_color;
	double* m_hoverColor;
	double* m_selectedColor;
	double	m_opacity;
	bool	m_pickable;
};

#endif


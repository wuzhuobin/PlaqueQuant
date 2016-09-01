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

#include "AbstractPolyData.h"



AbstractPolyData::AbstractPolyData() : AbstractData()
{
	/// Default parameters
	// Colors
	this->m_color = (double*)malloc(sizeof(double) * 3);
	this->m_hoverColor = (double*)malloc(sizeof(double) * 3);
	this->m_selectedColor = (double*)malloc(sizeof(double) * 3);
	this->m_hoverColor[0] = 1;
	this->m_hoverColor[1] = 0;
	this->m_hoverColor[2] = 0;
	this->m_color[0] = 1;
	this->m_color[1] = 1;
	this->m_color[2] = 1;
	this->m_selectedColor[0] = 0.8;
	this->m_selectedColor[1] = 0;
	this->m_selectedColor[2] = 0;
	// Others
	this->m_opacity = 1;
	this->m_pickable = 0;
	this->m_actorState = IDLE;

	this->m_polyData = NULL;

	// Initialize variables
	this->m_actor = vtkSmartPointer<vtkActor>::New();
	this->m_userTransform = vtkSmartPointer<vtkTransform>::New();
	this->m_userTransform->PostMultiply();
	this->m_transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	this->m_actor->SetUserTransform(m_userTransform);
}


AbstractPolyData::~AbstractPolyData()
{
	free(m_color);
	free(m_hoverColor);
	free(m_selectedColor);
}

void AbstractPolyData::SetColor(double * color)
{
	memcpy(m_color, color, sizeof(double) * 3);
	int curstate = m_actorState;
	this->SetActorState(-1);
	this->SetActorState(curstate);
}

void AbstractPolyData::SetColor(double x1, double x2, double x3)
{
	double color[3] = { x1, x2, x3 };
	this->SetColor(color);
}

void AbstractPolyData::SetColor(vtkLookupTable *lut, double min, double max)
{
	vtkSmartPointer<vtkMapper> mapper = m_actor->GetMapper();
	mapper->SetScalarVisibility(true);
	//mapper->SetScalarRange(min, max);
	mapper->SetLookupTable(lut);
	mapper->SetUseLookupTableScalarRange(true);
	mapper->Update();
}

void AbstractPolyData::SetHoverColor(double * hoverColor)
{
	memcpy(m_hoverColor, hoverColor, sizeof(double) * 3);
	int curstate = m_actorState;
	this->SetActorState(-1);
	this->SetActorState(curstate);
}

void AbstractPolyData::SetHoverColor(double x1, double x2, double x3)
{
	double hoverColor[3] = { x1, x2, x3 };
	this->SetColor(hoverColor);
}


void AbstractPolyData::SetSelectedColor(double * selectedColor)
{
	memcpy(m_selectedColor, selectedColor, sizeof(double) * 3);
	int curstate = m_actorState;
	this->SetActorState(-1);
	this->SetActorState(curstate);
}

void AbstractPolyData::SetSelectedColor(double x1, double x2, double x3)
{
	double selectedColor[3] = { x1, x2, x3 };
	this->SetColor(selectedColor);
}

void AbstractPolyData::SetSurfaceRepresentation(int n)
{
	switch (n)
	{
	case VTK_SURFACE:
		this->m_actor->GetProperty()->SetRepresentationToSurface();
		break;
	case VTK_WIREFRAME:
		this->m_actor->GetProperty()->SetRepresentationToWireframe();
		break;
	case VTK_POINTS:
		this->m_actor->GetProperty()->SetRepresentationToPoints();
		break;
	default:
		cerr << "Unknown representation type called!\n";
		throw UNKONW_ACTOR_REPRESENTATION_ERROR;
	}
}


void AbstractPolyData::SetUserTransform(vtkSmartPointer<vtkTransform> inTransform)
{
	if (!m_actor) {
		throw ACTORS_NOT_INITIALIZED_ERROR;
	}

	if (!m_polyData) {
		throw POLYDATA_MISSING_ERROR;
	}

	
	m_userTransform = inTransform;

	// Connect to actor 
	m_actor->SetUserTransform(m_userTransform);
}

void AbstractPolyData::SetVisibility(bool vis)
{
	AbstractData::SetVisibility(vis);
	m_actor->SetVisibility(vis);
}

void AbstractPolyData::SetOpacity(double opacity)
{
	if (m_actor) {
		m_actor->GetProperty()->SetOpacity(opacity);
	}
}

void AbstractPolyData::SetActorState(int state)
{
	if (this->m_actorState != state) {
		this->m_actorState = state;
		try {
			switch (state)
			{
			case HOVERED:
				this->m_actor->GetProperty()->SetColor(this->m_hoverColor);
				break;
			case SELECTED:
				this->m_actor->GetProperty()->SetColor(this->m_selectedColor);
				break;
			case IDLE:
				this->m_actor->GetProperty()->SetColor(this->m_color);
				break;
			default:
				break;
			}
		}
		catch (...) {
			return;
		}
	}
}

void AbstractPolyData::SetPickable(bool val)
{
	if (m_actor) {
		m_actor->SetPickable(val);
	}
}

vtkSmartPointer<vtkTransform> AbstractPolyData::GetUserTransform()
{
	return m_userTransform;
}

vtkSmartPointer<vtkActor> AbstractPolyData::GetActor()
{
	if (this->m_actor->GetMapper())
		return this->m_actor;
	else {
		// Hook actor to polydata mapper if it is not hooked
		if (!m_polyData) {
			throw POLYDATA_MISSING_ERROR;
			return nullptr;
		}

		this->InitializeActors();
		return this->m_actor;
	}
}

vtkSmartPointer<vtkPolyData> AbstractPolyData::GetPolyData()
{
	return m_polyData;
}

vtkSmartPointer<vtkPolyData> AbstractPolyData::GetTransformedPolyData()
{
	if (!m_polyData)
		throw POLYDATA_MISSING_ERROR;

	if (!m_userTransform)
		throw TRANSFORM_MISSING_ERROR;
	
	
	m_transformFilter->Update();
	return m_transformFilter->GetOutput();
}

double * AbstractPolyData::GetColor()
{
	return m_color;
}

double * AbstractPolyData::GetHoverColor()
{
	return m_hoverColor;
}

double * AbstractPolyData::GetSelectedColor()
{
	return m_selectedColor;
}

double AbstractPolyData::GetOpacity()
{
	return m_opacity;
}

int AbstractPolyData::GetActorState()
{
	return m_actorState;
}

bool AbstractPolyData::GetPickable()
{
	return m_pickable;
}

void AbstractPolyData::SavePolyData(QString path)
{
	QFileInfo fileInfo(path);

	// Setup a triangle filter here to preventbroken polydata
	vtkSmartPointer<vtkTriangleFilter> triFilter = vtkSmartPointer<vtkTriangleFilter>::New();
	triFilter->SetInputData(this->m_polyData);
	triFilter->Update();

	vtkSmartPointer<vtkWriter> writer;

	if (fileInfo.suffix() == "stl")
	{
		vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
		stlWriter->SetFileName(path.toStdString().c_str());
		stlWriter->SetInputData(triFilter->GetOutput());
		if (!stlWriter->Write()) {
			throw WRITE_POLYDATA_ERROR;
		}

		
	}
	else
	{
		vtkSmartPointer<vtkXMLPolyDataWriter> vtpWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
		vtpWriter->SetFileName(path.toStdString().c_str());
		vtpWriter->SetInputData(triFilter->GetOutput());
		if (!vtpWriter->Write()) {
			throw WRITE_POLYDATA_ERROR;
		}
	}
}

void AbstractPolyData::Update()
{
	if (!m_polyData)
		throw POLYDATA_MISSING_ERROR;
	if (!m_actor)
		throw ACTORS_NOT_INITIALIZED_ERROR;
	if (!m_transformFilter)
		// Return if nothing to update
		return;

	m_transformFilter->Update();	
}

void AbstractPolyData::InstallPipeline()
{
	this->InitializePorts();
	this->InitializeActors();
}

void AbstractPolyData::InitializePorts()
{
	if (!m_polyData)
		throw POLYDATA_MISSING_ERROR;

	if (!m_userTransform)
		throw TRANSFORM_MISSING_ERROR;

	m_transformFilter->SetInputData(m_polyData);
	m_transformFilter->SetTransform(m_userTransform);
	m_transformFilter->Update();
}

/* Call this function after setting polydata in subclasses*/
void AbstractPolyData::InitializeActors()
{
	// Throw error if polydata is not prepared
	if (!m_polyData) {
		throw POLYDATA_MISSING_ERROR;
	}

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(m_polyData);
	mapper->Update();

	//m_actor->SetSourceData(this);
	this->m_actor->SetMapper(mapper);

	// Initialize IDLE state
	this->m_actor->GetProperty()->SetColor(m_color);
}


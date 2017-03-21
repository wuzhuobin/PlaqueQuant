#include "SurfaceCreator.h"

SurfaceCreator::SurfaceCreator(QObject* parent) : QObject(parent)
{
	//Default value
	m_resample				= NULL;
	m_surface				= NULL;
	m_connectivityFilter	= NULL;
	m_smoother				= NULL;

	m_input		= NULL;
	m_output	= NULL;
	m_factor	= 0.4;
	m_smoothFactor = 15;
	m_value		= -140; 
	m_discrete	= false;
	m_largest	= false;
}

SurfaceCreator::~SurfaceCreator(void)
{
	if (m_resample) m_resample->Delete();
	if (m_surface) m_surface->Delete();
	if (m_connectivityFilter) m_connectivityFilter->Delete();
	if (m_smoother) m_smoother->Delete();
}

void SurfaceCreator::SetDiscrete(bool b)
{
	m_discrete = b;
}

void SurfaceCreator::SetSmoothIteration(int val)
{
	this->m_smoothFactor = val;
}

void SurfaceCreator::SetResamplingFactor(double factor)
{
	m_factor = factor;
}

void SurfaceCreator::SetLargestConnected(bool b)
{
	m_largest = b;
}

void SurfaceCreator::SetInput(vtkImageData* input)
{
	m_input = input;
}

void SurfaceCreator::SetValue(int value)
{
	m_value = value;
}

bool SurfaceCreator::Update()
{	
	if (m_input==NULL) return true;

	double *range;
	if (this->m_factor != 1)
	{
		m_resample = vtkImageResample::New();
		m_resample->SetAxisMagnificationFactor(0, m_factor);
		m_resample->SetAxisMagnificationFactor(1, m_factor);
		m_resample->SetAxisMagnificationFactor(2, m_factor);
		m_resample->SetInputData(m_input);
		m_resample->Update();

		range = m_resample->GetOutput()->GetScalarRange();
	}
	else {
		range = m_input->GetScalarRange();
	}

	if (m_discrete)
	{
		int numValue = vtkMath::Round(range[1] - range[0]) +1;
		
		m_surface = vtkDiscreteMarchingCubes::New();
		m_surface->SetNumberOfContours(numValue);
		m_surface->GenerateValues(numValue, range[0], range[1]);
		m_surface->SetComputeScalars(1);
		
	}
	else
	{
		m_surface = vtkMarchingCubes::New();
		m_surface->SetValue(0, m_value);
	}

	if (this->m_factor != 1)
		m_surface->SetInputData(m_resample->GetOutput());
	else
		m_surface->SetInputData(m_input);
	
	//m_surface->SetInput(m_input);
	m_surface->ComputeNormalsOff();
	m_surface->ComputeGradientsOff();
	if (m_discrete)
		m_surface->ComputeScalarsOn();
	else
		m_surface->ComputeScalarsOff();
	m_surface->Update();
	
	vtkPolyData* tempPolyData = m_surface->GetOutput();

	if (m_largest)
	{
		m_connectivityFilter = vtkPolyDataConnectivityFilter::New();
		m_connectivityFilter->SetInputData(m_surface->GetOutput());
		m_connectivityFilter->SetExtractionModeToLargestRegion();
		m_connectivityFilter->Update();

		tempPolyData = m_connectivityFilter->GetOutput();
	}
	
	if (this->m_smoothFactor > 0)
	{
		m_smoother = vtkWindowedSincPolyDataFilter::New();
		m_smoother->SetInputData(tempPolyData);
		m_smoother->SetNumberOfIterations(this->m_smoothFactor);
		m_smoother->BoundarySmoothingOff();
		m_smoother->FeatureEdgeSmoothingOff();
		m_smoother->SetFeatureAngle(120.0);
		m_smoother->SetPassBand(.001);
		m_smoother->Update();

		m_output = m_smoother->GetOutput();
	}
	else {
		m_output = m_surface->GetOutput();
	}

	if (m_output==NULL) return true;

	return false;
}

vtkPolyData* SurfaceCreator::GetOutput()
{
	return m_output;
}


  



  



  



  



  



  




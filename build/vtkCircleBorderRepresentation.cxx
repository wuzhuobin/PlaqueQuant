#include "vtkCircleBorderRepresentation.h"
#include "vtkRenderer.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkMath.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkActor2D.h"
#include "vtkProperty2D.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkWindow.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkCircleBorderRepresentation);

//-------------------------------------------------------------------------
vtkCircleBorderRepresentation::vtkCircleBorderRepresentation()
{
  this->InteractionState = vtkCircleBorderRepresentation::Outside;
  this->SetShowBorderToOn();
  //this->ShowBorder = BORDER_ON;
  this->ProportionalResize = 0;
  this->Tolerance = 3;
  this->SelectionPoint[0] = this->SelectionPoint[1] = 0.0;

  // Initial positioning information
  this->Negotiated = 0;
  //this->PositionCoordinate = vtkCoordinate::New();
  this->PositionCoordinate->SetCoordinateSystemToNormalizedViewport();
  this->PositionCoordinate->SetValue(0.05, 0.05);
  //this->Position2Coordinate = vtkCoordinate::New();
  this->Position2Coordinate->SetCoordinateSystemToNormalizedViewport();
  this->Position2Coordinate->SetValue(0.1, 0.1); //may be updated by the subclass
  this->Position2Coordinate->SetReferenceCoordinate(this->PositionCoordinate);

  // Create the geometry in canonical coordinates
  //this->BWPoints = vtkPoints::New();
  this->BWPoints->Reset();
  this->BWPoints->SetDataTypeToDouble();
  this->BWPoints->SetNumberOfPoints(m_numberofpoints);

  vtkCellArray *outline  = vtkCellArray::New();
  vtkIdType* lineIndices = new vtkIdType[m_numberofpoints + 1];
  for (unsigned int i = 0; i < m_numberofpoints; i++)
  {
	  const double angle = 2.0 * vtkMath::Pi() * static_cast<double>(i) /
		  static_cast<double>(m_numberofpoints);
	  BWPoints->InsertPoint(static_cast<vtkIdType>(i), cos(angle), sin(angle), 0.);
	  lineIndices[i] = static_cast<vtkIdType>(i);
  }
  lineIndices[m_numberofpoints] = 0;
  outline->InsertNextCell(m_numberofpoints + 1, lineIndices);
  delete[] lineIndices;

  //this->BWPolyData = vtkPolyData::New();
  this->BWPolyData->SetPoints(this->BWPoints);
  this->BWPolyData->SetLines(outline);
  outline->Delete();

  //this->BWTransform = vtkTransform::New();
  //this->BWTransformFilter = vtkTransformPolyDataFilter::New();
  this->BWTransformFilter->SetTransform(this->BWTransform);
  this->BWTransformFilter->SetInputData(this->BWPolyData);

  //this->BWMapper = vtkPolyDataMapper2D::New();
  this->BWMapper->SetInputConnection(this->BWTransformFilter->GetOutputPort());
  this->BWActor = vtkActor2D::New();
  this->BWActor->SetMapper(this->BWMapper);
  
  //this->BorderProperty = vtkProperty2D::New();
  this->BWActor->SetProperty(this->BorderProperty);

  this->MinimumSize[0] = 1;
  this->MinimumSize[1] = 1;
  this->MaximumSize[0] = 100000;
  this->MaximumSize[1] = 100000;

  this->Moving = 0;
}

vtkCircleBorderRepresentation::~vtkCircleBorderRepresentation()
{
//	this->PositionCoordinate->Delete();
//	this->Position2Coordinate->Delete();
//
//	this->BWPoints->Delete();
//	this->BWTransform->Delete();
//	this->BWTransformFilter->Delete();
//	this->BWPolyData->Delete();
//	this->BWMapper->Delete();
//	this->BWActor->Delete();
//	this->BorderProperty->Delete();
}

void vtkCircleBorderRepresentation::NegotiateLayout()
{
	double size[2];
	this->GetSize(size);

	// Update the initial border geometry
	for (unsigned int i = 0; i < m_numberofpoints; i++)
	{
		const double angle = 2.0 * vtkMath::Pi() * static_cast<double>(i) / static_cast<double>(m_numberofpoints);
		this->BWPoints->SetPoint(i, cos(angle)*size[0], sin(angle)*size[1], 0.);
	}
}
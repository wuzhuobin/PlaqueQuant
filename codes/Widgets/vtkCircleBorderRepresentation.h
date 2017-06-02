#ifndef __vtkCircleBorderRepresentation_h
#define __vtkCircleBorderRepresentation_h

#include "vtkCoordinate.h" //Because of the viewport coordinate macro
#include "vtkBorderRepresentation.h"

class vtkPoints;
class vtkPolyData;
class vtkTransform;
class vtkTransformPolyDataFilter;
class vtkPolyDataMapper2D;
class vtkActor2D;
class vtkProperty2D;

class vtkCircleBorderRepresentation : public vtkBorderRepresentation
{
public:
  // Description:
  // Instantiate this class.
  static vtkCircleBorderRepresentation *New();
  vtkCircleBorderRepresentation();
  ~vtkCircleBorderRepresentation();

  // Description:
  // Define standard methods.
  vtkTypeMacro(vtkCircleBorderRepresentation,vtkBorderRepresentation);

protected:
	virtual void NegotiateLayout();
  
private:
  vtkCircleBorderRepresentation(const vtkCircleBorderRepresentation&);  //Not implemented
  void operator=(const vtkCircleBorderRepresentation&);  //Not implemented

  const unsigned int m_numberofpoints = 50;
};

#endif

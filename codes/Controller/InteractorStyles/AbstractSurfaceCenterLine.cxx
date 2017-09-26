#include "AbstractSurfaceCenterLine.h"

#include "CenterlineSurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>


void AbstractSurfaceCenterLine::SetCustomEnabled(bool flag)
{
	AbstractInteractorStyleSurface::SetCustomEnabled(flag);
	if (m_customFlag) {
		// only create centerline when there is no centerline, if there is a 
		// centerline, it won't create
		if (!GetCenterlineSurfaceViewer()->GetCenterline() ||
			GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
			CreateCenterLine();
			
		}
	}
	GetCenterlineSurfaceViewer()->GetCenterlineActor()->SetVisibility(m_customFlag);


}

CenterlineSurfaceViewer * AbstractSurfaceCenterLine::GetCenterlineSurfaceViewer()
{
	return CenterlineSurfaceViewer::SafeDownCast(m_viewer);
}

AbstractSurfaceCenterLine::AbstractSurfaceCenterLine()
{
}

AbstractSurfaceCenterLine::~AbstractSurfaceCenterLine()
{
}


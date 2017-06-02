#include "InteractorStyleSurfaceCenterlineWaypoint.h"

#include "QInteractorStyleTubularVOI.h"
#include "CenterlineSurfaceViewer.h"

#include <vtkPolyData.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(InteractorStyleSurfaceCenterlineWaypoint);

void InteractorStyleSurfaceCenterlineWaypoint::SetCustomEnabled(bool flag)
{
	AbstractSurfaceCenterLine::SetCustomEnabled(flag);
}

bool InteractorStyleSurfaceCenterlineWaypoint::CreateCenterLine()
{

	std::list<AbstractInteractorStyle*>::const_iterator cit = std::find_if(
		m_abstractInteractorStyles.cbegin(),
		m_abstractInteractorStyles.cend(),
		[](AbstractInteractorStyle* style)->bool {
		return dynamic_cast<QInteractorStyleTubularVOI*>(style); });

	GetCenterlineSurfaceViewer()->GetCenterline()->ShallowCopy(
		static_cast<QInteractorStyleTubularVOI*>(*cit)->UpdateSpline());
	if (!GetCenterlineSurfaceViewer()->GetCenterline() ||
		GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
		vtkErrorMacro(<< "No waypoint, no centerline");
		return false;
	}
	GetCenterlineSurfaceViewer()->Render();
	return true;
}

InteractorStyleSurfaceCenterlineWaypoint::InteractorStyleSurfaceCenterlineWaypoint()
{
}

InteractorStyleSurfaceCenterlineWaypoint::~InteractorStyleSurfaceCenterlineWaypoint()
{
}

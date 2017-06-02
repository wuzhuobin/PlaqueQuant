#ifndef __CENTERLINE_SURFACE_VIEWER_H__
#define __CENTERLINE_SURFACE_VIEWER_H__

#include "SurfaceViewer.h"

class vtkActor;
class vtkPolyDataMapper;
class vtkPolyData;
class vtkCleanPolyData;
//class vtkKdTreePointLocator;
class vtkTubeFilter;
class vtkSplineFilter;



class CenterlineSurfaceViewer: public SurfaceViewer
{
public:

	static CenterlineSurfaceViewer* New();
	vtkTypeMacro(CenterlineSurfaceViewer, SurfaceViewer);
	void PrintSelf(ostream& os, vtkIndent indent);


	// Description:
	// Set/Get the input image to the viewer.
	virtual void SetCenterline(vtkPolyData* centerline);
	virtual vtkPolyData* GetCenterline();

	// Description:
	// Get the internal render window, renderer, image actor, and
	// image map instances.
	vtkGetObjectMacro(CenterlineActor, vtkActor);
	vtkGetObjectMacro(CleanPolyData, vtkCleanPolyData);
	vtkGetObjectMacro(TubeFilter, vtkTubeFilter);
	//vtkGetObjectMacro(KdTreePointLocator, vtkKdTreePointLocator);
	vtkGetObjectMacro(SplineFilter, vtkSplineFilter);

protected:
	CenterlineSurfaceViewer();
	~CenterlineSurfaceViewer();

	vtkActor* CenterlineActor = nullptr;
	vtkPolyDataMapper* CenterlineMapper = nullptr;
	vtkCleanPolyData* CleanPolyData = nullptr;
	vtkSplineFilter* SplineFilter = nullptr;
	vtkTubeFilter* TubeFilter = nullptr;
	//vtkKdTreePointLocator* KdTreePointLocator = nullptr;

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();

private:

};


#endif // !__CENTERLINE_SURFACE_VIEWER_H__

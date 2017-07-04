#ifndef __SURFACE_VIEWER_H__
#define __SURFACE_VIEWER_H__

#include <vtkObject.h>

class vtkImageData;
class vtkAlgorithmOutput;
class vtkAlgorithm;
class vtkInformation;
class vtkActor;
class vtkPolyDataMapper;
class vtkRenderWindow;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkInteractorStyle;
class vtkDiscreteMarchingCubes;
class vtkLookupTable;
class vtkImageResample;
class vtkWindowedSincPolyDataFilter;
class vtkDepthSortPolyData;
class vtkAxesActor;
class vtkOrientationMarkerWidget;

class SurfaceViewer: public vtkObject
{
public:
	static SurfaceViewer* New();
	vtkTypeMacro(SurfaceViewer, vtkObject);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Get the name of rendering window.
	virtual const char *GetWindowName();

	// Description:
	// Render the resulting image.
	virtual void Render(void);

	// Description:
	// Set/get the slice orientation
	//BTX
	enum
	{
		ORIENTATION_YZ = 0,
		ORIENTATION_XZ = 1,
		ORIENTATION_XY = 2
	};
	//ETX
	vtkGetMacro(Orientation, int);
	virtual void SetOrientation(int orientation);
	virtual void SetOrientationToXY()
	{
		this->SetOrientation(ORIENTATION_XY);
	};
	virtual void SetOrientationToYZ()
	{
		this->SetOrientation(ORIENTATION_YZ);
	};
	virtual void SetOrientationToXZ()
	{
		this->SetOrientation(ORIENTATION_XZ);
	};
	// Description:
	// Set/Get the input image to the viewer.
	virtual void SetInputData(vtkImageData *in);
	virtual vtkImageData *GetInput();

	// Description:
	// Update the display extent manually so that the proper slice for the
	// given orientation is displayed. It will also try to set a
	// reasonable camera clipping range.
	// This method is called automatically when the Input is changed, but
	// most of the time the input of this class is likely to remain the same,
	// i.e. connected to the output of a filter, or an image reader. When the
	// input of this filter or reader itself is changed, an error message might
	// be displayed since the current display extent is probably outside
	// the new whole extent. Calling this method will ensure that the display
	// extent is reset properly.
	virtual void UpdateDisplayExtent();

	// Description:
	// These are here when using a Tk window.
	virtual void SetDisplayId(void *a);
	virtual void SetWindowId(void *a);
	virtual void SetParentId(void *a);

	// Description:
	// Set/Get the position in screen coordinates of the rendering window.
	virtual int* GetPosition();
	virtual void SetPosition(int a, int b);
	virtual void SetPosition(int a[2]) { this->SetPosition(a[0], a[1]); }

	// Description:
	// Set/Get the size of the window in screen coordinates in pixels.
	virtual int* GetSize();
	virtual void SetSize(int a, int b);
	virtual void SetSize(int a[2]) { this->SetSize(a[0], a[1]); }

	virtual void SetLookupTable(vtkLookupTable* lookupTable);
	vtkGetObjectMacro(LookupTable, vtkLookupTable);

	// Description:
	// Get the internal render window, renderer, image actor, and
	// image map instances.
	vtkGetObjectMacro(RenderWindow, vtkRenderWindow);
	vtkGetObjectMacro(Renderer, vtkRenderer);
	vtkGetObjectMacro(SurfaceActor, vtkActor);
	vtkGetObjectMacro(ImageResample, vtkImageResample);
	vtkGetObjectMacro(MarchingCubes, vtkDiscreteMarchingCubes);
	vtkGetObjectMacro(WindowedSincPolyDataFilter, vtkWindowedSincPolyDataFilter);
	vtkGetObjectMacro(DepthSortPolyData, vtkDepthSortPolyData);
	vtkGetObjectMacro(InteractorStyle, vtkInteractorStyle);
	// Description:
	// Set your own renderwindow and renderer
	virtual void SetRenderWindow(vtkRenderWindow *arg);
	virtual void SetRenderer(vtkRenderer *arg);

	// Description:
	// Attach an interactor for the internal render window.
	virtual void SetupInteractor(vtkRenderWindowInteractor* arg);

	vtkBooleanMacro(EnableDepthSorting, bool);
	virtual void SetEnableDepthSorting(bool flag);


	vtkBooleanMacro(EnableDepthPeeling, bool);
	virtual void SetEnableDepthPeeling(bool flag);


	// Description:
	// Create a window in memory instead of on the screen. This may not
	// be supported for every type of window and on some windows you may
	// need to invoke this prior to the first render.
	virtual void SetOffScreenRendering(int flag);
	virtual int GetOffScreenRendering();
	vtkBooleanMacro(OffScreenRendering, int);


protected:

	SurfaceViewer();
	~SurfaceViewer();

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();
	virtual void UpdateOrientation();

	vtkImageResample* ImageResample = nullptr;
	vtkDiscreteMarchingCubes* MarchingCubes = nullptr;
	vtkWindowedSincPolyDataFilter* WindowedSincPolyDataFilter = nullptr;
	vtkDepthSortPolyData* DepthSortPolyData = nullptr;
	vtkRenderWindow* RenderWindow = nullptr;
	vtkRenderer* Renderer = nullptr;
	vtkActor* SurfaceActor = nullptr;
	vtkPolyDataMapper* SurfaceMapper = nullptr;
	vtkRenderWindowInteractor* Interactor = nullptr;
	vtkInteractorStyle* InteractorStyle = nullptr;
	vtkAxesActor* AxesActor = nullptr;
	vtkOrientationMarkerWidget* OrientationMarkerWidget = nullptr;

	vtkLookupTable* LookupTable = nullptr;

	int Orientation = ORIENTATION_XY;
	bool FirstRender = true;
	// MaxNoOfPeels maximum number of depth peels(multi - pass rendering)
	int MaxNoOfPeels = 9999;
	// OcclusionRatio the occlusion ration(0.0 means a perfect image,
	// >0.0 means a non - perfect image which in general results in faster rendering)
	double OcclusionRatio = 0.0;

	vtkAlgorithm* GetInputAlgorithm();
	vtkInformation* GetInputInformation();

private:
	SurfaceViewer(const SurfaceViewer&);  // Not implemented.
	void operator=(const SurfaceViewer&);  // Not implemented.
	void SetInputConnection(vtkAlgorithmOutput* input); // not implemented

};




#endif // !__SURFACE_VIEWER_H__
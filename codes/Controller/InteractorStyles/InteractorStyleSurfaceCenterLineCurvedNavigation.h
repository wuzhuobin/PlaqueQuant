#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_CURVED_NAVIGATION_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_CURVED_NAVIGATION_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

//class vtkSeedWidget;
class vtkHandleWidget;
class vtkKdTreePointLocator;
class vtkPolyData;
class vtkImageActor;
class vtkTextActor;
class InteractorStyleSurfaceCenterLineCurvedNavigationCallback;

class InteractorStyleSurfaceCenterLineCurvedNavigation :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	const static unsigned int NUM_OF_HANDLES = 1;

	vtkTypeMacro(InteractorStyleSurfaceCenterLineCurvedNavigation,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLineCurvedNavigation* New();

	virtual void SetCustomEnabled(bool flag) override;
	/**
	* @override
	* @brief	Create ceterline or re-create ceterline depending on reClipSurface flag
	* @param	reclipSurface if true, re-clip, otherwise change source id
	* @return	true, creat succeed, false, create failed.
	* create or re-create, called by 2 buttons' callback and #CreateCenterline(), by
	* invoking #CustomEnabledOff(), #CustomEnabledOn() to re-initialize widget's functions.
	*/
	virtual bool CreateCenterLine(bool reClipSurface) override;

	virtual void Update2DViewers();

	virtual void UpdateRadiusLabel();

protected:

	InteractorStyleSurfaceCenterLineCurvedNavigation();
	virtual ~InteractorStyleSurfaceCenterLineCurvedNavigation() override;

	virtual void InitializeHandleWidgets();

	vtkSmartPointer<vtkKdTreePointLocator> m_splinePointLocator = nullptr;
	vtkSmartPointer<vtkKdTreePointLocator> m_centerlinePointLocator = nullptr;
	vtkSmartPointer<vtkHandleWidget> m_handleWidgets[NUM_OF_HANDLES] = {nullptr};
	vtkSmartPointer<vtkTextActor> m_measurementText = nullptr;


private:

};



#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_CURVED_NAVIGATION_H__

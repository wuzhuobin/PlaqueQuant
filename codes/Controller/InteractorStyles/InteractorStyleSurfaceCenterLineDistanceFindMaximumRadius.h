#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

//class vtkSeedWidget;
class vtkHandleWidget;
class vtkKdTreePointLocator;
class vtkPolyData;
class vtkTextActor;
class InteractorStyleSurfaceCenterLineDistanceFindMaximumRadiusCallback;

class InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	const static unsigned int NUM_OF_HANDLES = 2;

	vtkTypeMacro(InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius* New();

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

	virtual void FindMaximumRadius();


protected:

	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius();
	virtual ~InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius() override;

	virtual void InitializeHandleWidgets();

	virtual void OnKeyPress() override;


	vtkSmartPointer<vtkKdTreePointLocator> m_pointLocator = nullptr;
	vtkSmartPointer<vtkHandleWidget> m_handleWidgets[NUM_OF_HANDLES] = {nullptr};
	vtkSmartPointer<vtkPolyData> m_triangulatedCenterLine = nullptr;
	vtkSmartPointer<vtkTextActor> m_measurementText = nullptr;

	double m_maxRadius = 0;
	double m_minRadius = 0;
	double m_distance = 0;


private:

};



#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__

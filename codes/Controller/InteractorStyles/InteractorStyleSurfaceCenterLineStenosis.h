#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_STENOSIS_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_STENOSIS_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

//class vtkSeedWidget;
class vtkHandleWidget;
class vtkKdTreePointLocator;
class vtkPolyData;
class vtkTextActor;
class InteractorStyleSurfaceCenterLineStenosisCallback;

class InteractorStyleSurfaceCenterLineStenosis :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	const static unsigned int NUM_OF_HANDLES = 2;

	vtkTypeMacro(InteractorStyleSurfaceCenterLineStenosis,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLineStenosis* New();

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

	virtual void CalculateStenosis();

	vtkGetMacro(StenosisValue, double);

protected:

	InteractorStyleSurfaceCenterLineStenosis();
	virtual ~InteractorStyleSurfaceCenterLineStenosis() override;

	virtual void InitializeHandleWidgets();

	virtual void OnKeyPress() override;


	vtkSmartPointer<vtkKdTreePointLocator> m_pointLocator = nullptr;
	vtkSmartPointer<vtkHandleWidget> m_handleWidgets[NUM_OF_HANDLES];
	vtkSmartPointer<vtkPolyData> m_triangulatedCenterLine = nullptr;
	vtkSmartPointer<vtkTextActor> m_measurementText = nullptr;

	double StenosisValue = VTK_DOUBLE_MAX;

private:

};



#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_STENOSIS_H__

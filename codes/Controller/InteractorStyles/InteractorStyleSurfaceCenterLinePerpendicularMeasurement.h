#ifndef __INTERACTOR_STYLE_SURFAEC_CENTER_LINE_PERPENDICULAT_MEASUREMENT_H__
#define __INTERACTOR_STYLE_SURFAEC_CENTER_LINE_PERPENDICULAT_MEASUREMENT_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

class vtkLineWidget2;
class vtkTextActor;
class PerpendicularMeasurementLineWidget;

class InteractorStyleSurfaceCenterLinePerpendicularMeasurement :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	vtkTypeMacro(InteractorStyleSurfaceCenterLinePerpendicularMeasurement,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLinePerpendicularMeasurement* New();

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
	virtual void FindMaxiMumPerpendicularDistance();

protected:

	InteractorStyleSurfaceCenterLinePerpendicularMeasurement();
	virtual ~InteractorStyleSurfaceCenterLinePerpendicularMeasurement() override;

	virtual void InitializeLinePerpendicularMeasurementWidget();

	virtual void OnKeyPress() override;

	vtkSmartPointer<PerpendicularMeasurementLineWidget> m_lineWidget2 = nullptr;


private:

};



#endif // !__INTERACTOR_STYLE_SURFAEC_CENTER_LINE_PERPENDICULAT_MEASUREMENT_H__

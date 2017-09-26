#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__

#include "AbstractSurfaceCenterLine.h"
#include <vtkSmartPointer.h>


class vtkPolyData;
class vtkIdList;
class vtkButtonWidget;
class vtkTexturedButtonRepresentation2D;

class InteractorStyleSurfaceCenterLineSimpleClipping: 
	public AbstractSurfaceCenterLine
{
public:

	enum
	{
		CENTER_LINE_ORIENTATION_YZ = 0,
		CENTER_LINE_ORIENTATION_XZ = 1,
		CENTER_LINE_ORIENTATION_XY = 2
	};
	static const char RADIUS[];


	vtkTypeMacro(InteractorStyleSurfaceCenterLineSimpleClipping, AbstractSurfaceCenterLine);
	static InteractorStyleSurfaceCenterLineSimpleClipping* New();

	virtual void SetCustomEnabled(bool flag) override;
	virtual void SetCenterlineOrientation(int Orientation);
	virtual int GetCenterlineOrientation();
	virtual void SetClipingDistantce(int Distance);
	virtual int GetClipingDistance();
	virtual void SetLumenLabel(int label);
	virtual int GetLumenDistance();

	virtual bool CreateCenterLine() override;
	/**
	* @overload
	* @brief	Create ceterline or re-create ceterline depending on reClipSurface flag
	* @param	reclipSurface if true, re-clip, otherwise change source id
	* @return	true, creat succeed, false, create failed. 
	* create or re-create, called by 2 buttons' callback and #CreateCenterline()
	*/
	virtual bool CreateCenterLine(bool reClipSurface);
	virtual void ClipAndCap();

protected:
	InteractorStyleSurfaceCenterLineSimpleClipping();
	virtual ~InteractorStyleSurfaceCenterLineSimpleClipping() override;

	
	virtual void OnKeyPress() override;

	int m_lumenLabel = 2;
	int m_centerLineOrientation = CENTER_LINE_ORIENTATION_XY;
	int m_ClipingDistance = 3;
	vtkIdType m_sourceIdId = 0;

	vtkSmartPointer<vtkIdList> m_capCenterIds = nullptr;
	vtkSmartPointer<vtkPolyData> m_ClipAndCapSurface = nullptr;
	vtkSmartPointer<vtkButtonWidget> m_reClipButtonWidget = nullptr;
	vtkSmartPointer<vtkTexturedButtonRepresentation2D> m_reClipButtonRep = nullptr;
	vtkSmartPointer<vtkButtonWidget> m_ChangeSourceButtonWidget = nullptr;
	vtkSmartPointer<vtkTexturedButtonRepresentation2D> m_ChangeSourceButtonRep = nullptr;


private:

};




#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__
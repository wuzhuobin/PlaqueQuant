#ifndef __INTERACTOR_STYLE_VOI2_H__
#define __INTERACTOR_STYLE_VOI2_H__

#include "InteractorStyleNavigation.h"
class vtkImageCroppingRegionsWidget;
class InteractorStyleVOI2: public InteractorStyleNavigation
{
public:
	vtkTypeMacro(InteractorStyleVOI2, InteractorStyleNavigation);
	static InteractorStyleVOI2* New();
	void PrintSelf(ostream& os, vtkIndent indent);


	virtual void SetCustomEnabled(bool flag) override;

protected:
	InteractorStyleVOI2();
	~InteractorStyleVOI2();



	vtkImageCroppingRegionsWidget* m_croppingRegionWidget = nullptr;

private:
	InteractorStyleVOI2(const InteractorStyleVOI2&);  // Not implemented.
	void operator=(const InteractorStyleVOI2&);  // Not implemented.
};



#endif // !__INTERACTOR_STYLE_VOI2_H__

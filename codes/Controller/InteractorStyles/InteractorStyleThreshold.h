#ifndef __INTERACTOR_STYLE_THRESHOLD_H__
#define __INTERACTOR_STYLE_THRESHOLD_H__

#include "InteractorStyleWindowLevel.h"

#include <vtkSmartPointer.h>

class vtkImageThreshold;
class vtkImageData;
class vtkImageCast;

class InteractorStyleThreshold : public InteractorStyleWindowLevel 
{
public:
	static InteractorStyleThreshold* New();
	vtkTypeMacro(InteractorStyleThreshold, InteractorStyleWindowLevel);

	virtual void SetThreshold(double lower, double upper);

	virtual void SetThresholdByViewer(double lower, double upper);

	virtual void SetOutputLabel(int label);

	virtual void ThresholdToOverlay();

	virtual void SetPreview(bool flag);
	virtual void PreviewOn();
	virtual void PreviewOff();

protected:
	virtual void SetCustomEnabled(bool flag);

	virtual void StartWindowLevel() override;
	virtual void WindowLevel() override;
	virtual void ResetWindowLevel() override;

	vtkSmartPointer<vtkImageThreshold> m_threshold = nullptr;
	vtkSmartPointer<vtkImageCast> m_cast = nullptr;
	vtkImageData* m_tmpOverlay = nullptr;

	bool m_previewFlag = false;

	int m_label = 0;

};


#endif // !__INTERACTOR_STYLE_THRESHOLD_H__

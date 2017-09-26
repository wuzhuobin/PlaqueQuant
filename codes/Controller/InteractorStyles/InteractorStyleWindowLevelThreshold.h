#ifndef __INTERACTOR_STYLE_WINDOW_LEVEL_THRESHOLD_H__
#define __INTERACTOR_STYLE_WINDOW_LEVEL_THRESHOLD_H__

#include "InteractorStyleWindowLevel.h"

class InteractorStyleWindowLevelThreshold : public InteractorStyleWindowLevel
{
public:
	vtkTypeMacro(InteractorStyleWindowLevelThreshold, InteractorStyleWindowLevel);
	static InteractorStyleWindowLevelThreshold* New();

	virtual void SetOutputLabel(int label);
	virtual void ThresholdToOverlay();
	virtual void SetThreshold(double lower, double upper);

	virtual void SetThresholdByViewer(double lower, double upper);


protected: 


	virtual void WindowLevel() override;
	virtual void ResetWindowLevel();

	unsigned char m_label = 0;


};


#endif // !__INTERACTOR_STYLE_WINDOW_LEVEL_THRESHOLD_H__

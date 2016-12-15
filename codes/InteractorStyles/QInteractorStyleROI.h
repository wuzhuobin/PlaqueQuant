#ifndef __INTERACTOR_STYLE_ROI2_H__
#define __INTERACTOR_STYLE_ROI2_H__

#include "QAbstractNavigation.h"
#include "InteractorStyleNavigation.h"
#include "vtkROIWidget.h"

namespace Ui { class QInteractorStyleROI; }

class QInteractorStyleROI :	
	public QAbstractNavigation,	public InteractorStyleNavigation
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleROI);
public:
	vtkTypeMacro(QInteractorStyleROI, InteractorStyleNavigation);
	static QInteractorStyleROI* New();
	virtual void SetROIWidgetEnabled(bool flag);

public slots:
	virtual void slotUpdateROISpinBoxes(double* values);
	virtual void ExtractVOI();
	virtual void ResetVOI();

protected:
	QInteractorStyleROI(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleROI();


private:
	// initialization value is nullptr, if there is an instance of QInteractorStyleROI
	// m_roi will be created
	static vtkSmartPointer<vtkROIWidget> m_roi;
	static vtkSmartPointer<vtkRenderWindow> m_renderWindow;
	// for differentiate itself 
	int m_uniqueROIId = 0;
	Ui::QInteractorStyleROI* ui = nullptr;
};

#endif // !__INTERACTOR_STYLE_ROI2_H__

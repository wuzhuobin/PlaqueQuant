#ifndef __QINTERACTOR_STYLE_SURFACE_CENTERLINE_VBD_UBOGU_STANDARD_H__
#define __QINTERACTOR_STYLE_SURFACE_CENTERLINE_VBD_UBOGU_STANDARD_H__

#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"
#include "QAbstractInteractorStyle.h"

#include <vtkSmartPointer.h>

namespace Ui { class QInteractorStyleSurfaceCenterLineVBDUboguStandard; }
class PerpendicularMeasurementLineWidget;

class QInteractorStyleSurfaceCenterLineVBDUboguStandard:
	public QAbstractInteractorStyle, public InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius

{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleSurfaceCenterLineVBDUboguStandard);
public:

	enum MEASURE_MODE
	{
		LENGTH_OF_BASILAR_ARTERY = 0,
		LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY = 1,
		LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY = 2,
		DISTANCE_BETWEEN_CENTERLINE_AND_THE_LEAD = 3,
		DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT = 4,
		DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT = 5,
		MAXIMUM_DIAMETER_OF_BASILAR_ARTERY = 6,
		MAXIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY = 7,
		MAXIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY = 8
	};

	static QInteractorStyleSurfaceCenterLineVBDUboguStandard* New();
	vtkTypeMacro(QInteractorStyleSurfaceCenterLineVBDUboguStandard, InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius);


	public slots:
	virtual void SetCustomEnabled(bool flag) override;
	virtual void setCurrentMode(int mode);
	virtual void setCurrentModeToLengthOfBasilarArtery() { setCurrentMode(LENGTH_OF_BASILAR_ARTERY); }
	virtual void setCurrentModeToLengthOfIntracranialSegmentOfLeftVertebralArtery() { setCurrentMode(LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY); }
	virtual void setCurrentModeToLengthOfIntracranialSegmentOfRightVertebralArtery() { setCurrentMode(LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY); }
	virtual void setCurrentModeToDistanceBetweenCenterlineAndTheLead() { setCurrentMode(DISTANCE_BETWEEN_CENTERLINE_AND_THE_LEAD); }
	virtual void setCurrentModeToDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeft() { setCurrentMode(DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT); }
	virtual void setCurrentModeToDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRight() { setCurrentMode(DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT); }
	virtual void setCurrentModeToMaximumDiameterOfBasilarArtery() { setCurrentMode(MAXIMUM_DIAMETER_OF_BASILAR_ARTERY); }
	virtual void setCurrentModeToMaximumDiameterOfLeftVertebralArtery() { setCurrentMode(MAXIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY); }
	virtual void setCurrentModeToMaximumDiameterOfRightVertebralArtery() { setCurrentMode(MAXIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY); }
	virtual void FindMaximumRadius() override;
	virtual void FindMaxiMumPerpendicularDistance();

signals:
	void valueChangedLengthOfBasilarArtery(double);
	void valueChangedLengthOfIntracranialSegmentOfLeftVertebralArtery(double);
	void valueChangedLengthOfIntracranialSegmentOfRightVertebralArtery(double);
	void valueChangedDistanceBetweenCenterlineAndTheLead(double);
	void valueChangedDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeft(double);
	void valueChangedDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRight(double);
	void valueChangedMaximumDiameterOfBasilarArtery(double);
	void valueChangedMaximumDiameterOfLeftVertebralArtery(double);
	void valueChangedMaximumDiameterOfRightVertebralArtery(double);
protected:
	QInteractorStyleSurfaceCenterLineVBDUboguStandard(int uiType = MULTIPLE_UI, QWidget* parent = Q_NULLPTR);
	virtual ~QInteractorStyleSurfaceCenterLineVBDUboguStandard() override;

	virtual void uniqueDisable() override {};
	virtual void uniqueEnable() override {};
	virtual void uniqueCall() override {};

	virtual void InitializeLinePerpendicularMeasurementWidget();
	void initialization();
	int m_currentMode = 0;

	vtkSmartPointer<PerpendicularMeasurementLineWidget> m_lineWidget = nullptr;

private:
	Ui::QInteractorStyleSurfaceCenterLineVBDUboguStandard* ui = nullptr;
};

#endif // !__QINTERACTOR_STYLE_SURFACE_CENTERLINE_VBD_UBOGU_STANDARD_H__

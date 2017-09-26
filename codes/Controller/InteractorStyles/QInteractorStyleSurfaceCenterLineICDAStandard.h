#ifndef __QINTERACTOR_STYLE_SURFACE_CENTERLINE_ICDA_STANDARD_H__
#define __QINTERACTOR_STYLE_SURFACE_CENTERLINE_ICDA_STANDARD_H__

#include "QAbstractInteractorStyle.h"
#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"

namespace Ui { class QInteractorStyleSurfaceCenterLineICDAStandard; }

class QInteractorStyleSurfaceCenterLineICDAStandard: 
	public QAbstractInteractorStyle, public InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius 
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleSurfaceCenterLineICDAStandard);

public:

	enum MEASURE_MODE
	{
		LEFT_CAVERNOUS_SEGMENT_OF_INTERNAL_CAROTID_ARTERY = 0,
		RIGHT_CAVERNOUS_SEGMENT_OF_INTERNAL_CAROTID_ARTERY = 1,
		LEFT_MIDDLE_CEREBRAL_ARTERY = 2,
		RIGHT_MIDDLE_CEREBRAL_ARTERY = 3,
		LEFT_ANTERIOR_CEREBRAL_ARTERY = 4,
		RIGHT_ANTERIOR_CEREBRAL_ARTERY = 5
	};



	static QInteractorStyleSurfaceCenterLineICDAStandard* New();
	vtkTypeMacro(QInteractorStyleSurfaceCenterLineICDAStandard, InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius);


	virtual void FindMaximumRadius() override;

	public slots:

	virtual void setCurrentMode(int mode);
	virtual void SetCurrentModeToLeftCavernousSegmentOfInternalCarotidArtery() { setCurrentMode(LEFT_CAVERNOUS_SEGMENT_OF_INTERNAL_CAROTID_ARTERY); }
	virtual void SetCurrentModeToRightCavernousSegmentOfInternalCarotidArtery() { setCurrentMode(RIGHT_CAVERNOUS_SEGMENT_OF_INTERNAL_CAROTID_ARTERY); }
	virtual void SetCurrentModeToLeftMiddleCerebralArtery() { setCurrentMode(LEFT_MIDDLE_CEREBRAL_ARTERY); }
	virtual void SetCurrentModeToRightMiddleCerebralArtery() { setCurrentMode(RIGHT_MIDDLE_CEREBRAL_ARTERY); }
	virtual void SetCurrentModeToLeftAnteriorCerebralArtery() { setCurrentMode(LEFT_ANTERIOR_CEREBRAL_ARTERY); }
	virtual void SetCurrentModeToRightAnteriorCerebralArtery() { setCurrentMode(RIGHT_ANTERIOR_CEREBRAL_ARTERY); }

signals:

	void diameterChangedLeftCavernousSegmentOfInternalCarotidArtery(double diameter);
	void diameterChangedRightCavernousSegmentOfInternalCarotidArtery(double diameter);
	void diameterChangedLeftMiddleCerebralArtery(double diameter);
	void diameterChangedRightMiddleCerebralArtery(double diameter);
	void diameterChangedLeftAnteriorCerebralArtery(double diameter);
	void diameterChangedRightAnteriorCerebralArtery(double diameter);


protected:
	QInteractorStyleSurfaceCenterLineICDAStandard(int uiType = MULTIPLE_UI, QWidget* parent = Q_NULLPTR);
	virtual ~QInteractorStyleSurfaceCenterLineICDAStandard() override;



	void initialization();
	virtual void uniqueDisable() override {};
	virtual void uniqueEnable() override {};
	virtual void uniqueCall() override {};



	int m_currentMode = 0;
	
private:
	Ui::QInteractorStyleSurfaceCenterLineICDAStandard* ui = nullptr;
};



#endif // !__QINTERACTOR_STYLE_SURFACE_CENTERLINE_ICDA_STANDARD_H__


#ifndef __QINTERACTOR_STYLE_SURFACE_CENTERLINE_SMOKER_BA_DIAMETER_H__
#define __QINTERACTOR_STYLE_SURFACE_CENTERLINE_SMOKER_BA_DIAMETER_H__

#include "QAbstractInteractorStyle.h"
#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"

namespace Ui { class QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter; }

class QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter:
	public QAbstractInteractorStyle, public InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter);
public:

	static QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter* New();
	vtkTypeMacro(QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter, InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius);

public slots:
	virtual void FindMaximumRadius() override;
	
signals:
	void diameterChanged(double diameter);

protected:
	QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter(int uiType = MULTIPLE_UI, QWidget* parent = Q_NULLPTR);
	virtual ~QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter() override;

	virtual void uniqueDisable() override {};
	virtual void uniqueEnable() override {};
	virtual void uniqueCall() override {};
	void initialization();

private:
	///< ui thing. For integrity.
	Ui::QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter* ui = nullptr;
};



#endif // !__QINTERACTOR_STYLE_SURFACE_CENTERLINE_SMOKER_BA_DIAMETER_H__

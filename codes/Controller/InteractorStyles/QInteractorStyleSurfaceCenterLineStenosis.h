#ifndef __QINTERACTOR_STYLE_SURFACE_CENTER_LINE_STENOSIS_H__
#define __QINTERACTOR_STYLE_SURFACE_CENTER_LINE_STENOSIS_H__

#include "InteractorStyleSurfaceCenterLineStenosis.h"
#include "QAbstractInteractorStyle.h"

namespace Ui { typedef QAbstractInteractorStyle QInteractorStyleSurfaceCenterLineStenosis; }

class QInteractorStyleSurfaceCenterLineStenosis :
	public QAbstractInteractorStyle,
	public InteractorStyleSurfaceCenterLineStenosis
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleSurfaceCenterLineStenosis);
public:
	vtkTypeMacro(QInteractorStyleSurfaceCenterLineStenosis,
		InteractorStyleSurfaceCenterLineStenosis);
	static QInteractorStyleSurfaceCenterLineStenosis* New();

	public slots:
	virtual void CalculateStenosis();

signals:
	void calculatedStenosis(double stenosis);

protected:

	QInteractorStyleSurfaceCenterLineStenosis(int uiType = NO_UI, QWidget* parent = nullptr);
	virtual ~QInteractorStyleSurfaceCenterLineStenosis() override;

	virtual void uniqueDisable() override {};
	virtual void uniqueEnable() override {};
	virtual void uniqueCall() override {};
private:
	///< ui thing. For integrity.
	Ui::QInteractorStyleSurfaceCenterLineStenosis* ui = nullptr;
};
#endif // !__QINTERACTOR_STYLE_SURFACE_CENTER_LINE_STENOSIS_H__

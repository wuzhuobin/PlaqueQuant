#ifndef __QINTERACTOR_STYLE_MAXIMUM_WALL_THICKNESS_H__
#define __QINTERACTOR_STYLE_MAXIMUM_WALL_THICKNESS_H__

#include "QInteractorStyleRuler.h"

namespace Ui { class QInteractorStyleMaximumWallThickness; }

class vtkActor;
class vtkActor2D;

class QInteractorStyleMaximumWallThickness : public QInteractorStyleRuler
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleMaximumWallThickness);
public:
	vtkTypeMacro(QInteractorStyleMaximumWallThickness, QInteractorStyleRuler);
	static QInteractorStyleMaximumWallThickness* New();
public slots:
	virtual void SetCustomEnabled(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	virtual void EnableMaximumWallThickneesLabel(bool flag);

	virtual void UpdateMaximumWallThicknessLabel();

protected:
	QInteractorStyleMaximumWallThickness(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleMaximumWallThickness ();

	void initialization();

	vtkSmartPointer<vtkActor> m_lineActor = nullptr;
	vtkSmartPointer<vtkActor2D> m_labelActor = nullptr;
	//vtkSmartPointer<vtkRenderer>
	bool m_MaximumWallThickneesLabelFlag = false;

private:


	Ui::QInteractorStyleMaximumWallThickness* ui = nullptr;
};

#endif // !__QINTERACTOR_STYLE_MAXIMUM_WALL_THICKNESS_H__




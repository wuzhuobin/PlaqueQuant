#pragma once
#include "QAbstractNavigation.hpp"
#include "InteractorStyleWindowLevel.h"
namespace Ui {class QInteractorStyleWindowLevel;}

class QInteractorStyleWindowLevel : public QAbstractNavigation,
	public InteractorStyleWindowLevel
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleWindowLevel);

public:
	vtkTypeMacro(QInteractorStyleWindowLevel, InteractorStyleWindowLevel)
	static QInteractorStyleWindowLevel* New();
	virtual void Initialization();
	virtual void SetWindowLevelModeEnabled(bool flag);

	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
protected:
	QInteractorStyleWindowLevel(int uiType = -1, QWidget * parent = Q_NULLPTR);
	~QInteractorStyleWindowLevel();



private:
	Ui::QInteractorStyleWindowLevel *ui = nullptr;
};

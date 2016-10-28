#pragma once
#include "QAbstractNavigation.hpp"
#include "InteractorStyleNavigation.h"

namespace Ui { class QInteractorStyleNavigation;}

class QInteractorStyleNavigation : public QAbstractNavigation, 
	public InteractorStyleNavigation
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleNavigation);

public:
	vtkTypeMacro(QInteractorStyleNavigation, InteractorStyleNavigation);
	static QInteractorStyleNavigation* New();

	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);


protected:
	QInteractorStyleNavigation(int uiType = -1, QWidget * parent = Q_NULLPTR);
	~QInteractorStyleNavigation();
private:
	Ui::QInteractorStyleNavigation* ui = nullptr;
};

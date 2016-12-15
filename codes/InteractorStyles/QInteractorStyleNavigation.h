﻿#pragma once
#include "QAbstractNavigation.h"
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
	virtual void SetNavigationModeEnabled(bool flag);

	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

protected:
	QInteractorStyleNavigation(int uiType = NO_UI, QWidget * parent = Q_NULLPTR);
	~QInteractorStyleNavigation();
	virtual void UniqueEnable(bool flag);

private:
	Ui::QInteractorStyleNavigation* ui = nullptr;
	int m_oldExtent[6] = { -1 };
};

#pragma once
#include "QAbstractInteractorStyle.h"
#include "InteractorStyleNavigation.h"
#include "ui_QAbstractNavigation.h"

namespace Ui { class QAbstractNavigation;}

class QAbstractNavigation : public QAbstractInteractorStyle, 
	public InteractorStyleNavigation
{
	Q_OBJECT

public:
	vtkTypeMacro(QAbstractNavigation, InteractorStyleNavigation);
	static QAbstractNavigation* New();
	QSETUP_UI(QAbstractNavigation);


protected:
	QAbstractNavigation(int uiType = 0, QWidget * parent = Q_NULLPTR);
	~QAbstractNavigation();
private:
	Ui::QAbstractNavigation* ui = nullptr;
};

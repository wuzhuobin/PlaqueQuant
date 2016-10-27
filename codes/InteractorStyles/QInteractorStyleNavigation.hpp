#pragma once
#include "QAbstractInteractorStyle.h"
#include "InteractorStyleNavigation.h"
#include "ui_qinteractorstylenavigation.h"

namespace Ui { class QInteractorStyleNavigation;}

class QInteractorStyleNavigation : public QAbstractInteractorStyle, 
	public InteractorStyleNavigation
{
	Q_OBJECT

public:
	vtkTypeMacro(QInteractorStyleNavigation, InteractorStyleNavigation);
	static QInteractorStyleNavigation* New();
	QSETUP_UI(QInteractorStyleNavigation);


protected:
	QInteractorStyleNavigation(int uiType = 0, QWidget * parent = Q_NULLPTR);
	~QInteractorStyleNavigation();
private:
	Ui::QInteractorStyleNavigation* ui = nullptr;
};

#include "ui_QInteractorStyleRuler.h"
#include "ui_QAbstractNavigation.h"


#include "QInteractorStyleRuler.h"

#include <vtkObjectFactory.h>

vtkStandardNewMacro(QInteractorStyleRuler);
QSETUP_UI_SRC(QInteractorStyleRuler);

void QInteractorStyleRuler::SetCustomEnabled(bool flag)
{
	InteractorStyleRuler::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleRuler::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleRuler::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}


QInteractorStyleRuler::QInteractorStyleRuler(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleRuler::~QInteractorStyleRuler()
{
	QDELETE_UI();
}

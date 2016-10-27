#include "QInteractorStyleNavigation.hpp"

vtkStandardNewMacro(QInteractorStyleNavigation);
SETUP_UI_LIST(QInteractorStyleNavigation);

QInteractorStyleNavigation::QInteractorStyleNavigation(int uiType, QWidget * parent)
{
	setupUi();
	show();
}

QInteractorStyleNavigation::~QInteractorStyleNavigation() {
	DELETE_UI();
}

#include "QInteractorStyleSurfaceCenterLineStenosis.h"
#include "ui_QAbstractInteractorStyle.h"
#include "CenterlineSurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkActor.h>
#include <vtkHandleWidget.h>
#include <vtkKdTreePointLocator.h>
//#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdList.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkCoordinate.h>
#include <vtkImageData.h>
#include <vtkDoubleArray.h>
vtkStandardNewMacro(QInteractorStyleSurfaceCenterLineStenosis);
QSETUP_UI_SRC(QInteractorStyleSurfaceCenterLineStenosis);

void QInteractorStyleSurfaceCenterLineStenosis::CalculateStenosis()
{
	InteractorStyleSurfaceCenterLineStenosis::CalculateStenosis();
	emit calculatedStenosis(StenosisValue);
}

QInteractorStyleSurfaceCenterLineStenosis::QInteractorStyleSurfaceCenterLineStenosis(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleSurfaceCenterLineStenosis::~QInteractorStyleSurfaceCenterLineStenosis()
{
	QDELETE_UI();
}

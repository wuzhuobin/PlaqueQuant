#include "QInteractorStylePaintBrush.h"
#include "ui_QAbstractNavigation.h"
#include "ui_QInteractorStylePaintBrush.h"

vtkStandardNewMacro(QInteractorStylePaintBrush);
QSETUP_UI_SRC(QInteractorStylePaintBrush);

void QInteractorStylePaintBrush::SetPaintBrushModeEnabled(bool flag)
{
	InteractorStylePaintBrush::SetPaintBrushModeEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStylePaintBrush::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStylePaintBrush::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStylePaintBrush::SetPaintBrushLabel(int paintBrushLabel)
{
	InteractorStylePaintBrush::SetPaintBrushLabel(paintBrushLabel + 1);
}

void QInteractorStylePaintBrush::SetDrawOpacity(int opacity)
{
	InteractorStylePaintBrush::SetDrawOpacity(opacity * 2.55);
}

void QInteractorStylePaintBrush::SetBrushShape(int brushShape)
{
	InteractorStylePaintBrush::SetBrushShape(brushShape);
}

void QInteractorStylePaintBrush::SetBrushSize(int size)
{
	InteractorStylePaintBrush::SetBrushSize(size);
}

void QInteractorStylePaintBrush::EnableEraserMode(bool flag)
{
	InteractorStylePaintBrush::EnableEraserMode(flag);
}

QInteractorStylePaintBrush::QInteractorStylePaintBrush(int uiType, QWidget * parent)
{
	QNEW_UI();
	// brush size
	connect(ui->BrushSizeSpinBox, SIGNAL(valueChanged(int)), 
		this, SLOT(SetBrushSize(int)));
	// brush label
	connect(ui->labelComboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetPaintBrushLabel(int)));
	// set brushShape
	connect(ui->BrushComBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetBrushShape(int)));
	// set brushEraserMode
	connect(ui->eraserCheckBox, SIGNAL(toggled(bool)),
		this, SLOT(EnableEraserMode(bool)));
	// set overlay opacity
	connect(ui->opacitySpinBox, SIGNAL(valueChanged(int)), 
		this, SLOT(SetDrawOpacity(int)));
	// set brushEraserMode
	connect(ui->eraserCheckBox, SIGNAL(toggled(bool)),
		this, SLOT(EnableEraserMode(bool)));

}

QInteractorStylePaintBrush::~QInteractorStylePaintBrush()
{
	QDELETE_UI();
}

//void QInteractorStylePaintBrush::uniqueInvoke(bool flag)
//{
//	QAbstractNavigation::uniqueInvoke(flag);
//	
//	if (flag && flag != initializationFlag) {
//		// turn on codes
//
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//	}
//	// turn off
//	if (!flag && flag != initializationFlag) {
//		// turn off codes
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//
//	}
//	if (flag != initializationFlag) {
//
//	}
//	initializationFlag = flag;
//}

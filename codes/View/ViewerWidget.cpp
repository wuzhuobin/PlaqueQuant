#include "ViewerWidget.h"

#include "ui_ViewerWidget.h"

ViewerWidget::ViewerWidget(QWidget * parent)
	:QDockWidget(parent)
{

	setWidget(new QWidget(this));
	ui = new Ui::ViewerWidget;
	ui->setupUi(widget());
	setFeatures(DockWidgetFloatable | DockWidgetMovable);
	setAllowedAreas(Qt::AllDockWidgetAreas);

	connect(ui->pushButtonMaximum, SIGNAL(toggled(bool)),
		this, SLOT(showMaximized(bool)));
	//connect(ui->pushButtonRestore, SIGNAL(toggled(bool)),
	//	this, SLOT(showNormal(bool)));
}

void ViewerWidget::showMaximized(bool flag) 
{
	if (flag && !isMaximized()) {
		QDockWidget::showMaximized();
	}
	else {
		QDockWidget::showNormal();
	}
}

Ui::ViewerWidget * ViewerWidget::getUi()
{
	return this->ui;
}


//void ViewerWidget::resizeEvent(QResizeEvent * event)
//{
//	if (!isMaximized()) {
//		ui->pushButtonMaximum->setChecked(false);
//	}
//	QDockWidget::resizeEvent(event);
//}

void ViewerWidget::moveEvent(QMoveEvent * event)
{
	ui->pushButtonMaximum->setEnabled(isFloating());
	ui->pushButtonRestore->setEnabled(!isFloating());
	ui->pushButtonMaximum->setChecked(isMaximized());
	QDockWidget::moveEvent(event);
}

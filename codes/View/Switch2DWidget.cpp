#include "ui_Switch2DWidget.h"
#include <QSpinBox>
#include <QTextBrowser>
#include <qtablewidget.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Switch2DWidget.h"

#include "ui_LabelWidget.h"

#define setValueConnection(qtObj, filterName, valueName, dataType) \
connect(ui->##qtObj , SIGNAL(valueChanged(##dataType)), core , SLOT(slotSet##filterName##valueName(##dataType)))


Switch2DWidget::Switch2DWidget(QWidget *parent) :
	QWidget(parent)
{

	ui = new Ui::Switch2DWidget;
    ui->setupUi(this);

	ui->styleStackedWidget->setDisabled(true);
}

Switch2DWidget::~Switch2DWidget()
{
    delete ui;
}

void Switch2DWidget::ClearWidget()
{
}

void Switch2DWidget::addWidget(QWidget * style)
{
	ui->styleStackedWidget->addWidget(style);
}

void Switch2DWidget::setWidget(QWidget * style)
{
	int index = ui->styleStackedWidget->indexOf(style);
	if (index < 0) {
		ui->styleStackedWidget->setDisabled(true);
	}
	else {
		ui->styleStackedWidget->setEnabled(true);
		ui->styleStackedWidget->setCurrentIndex(index);
	}
}


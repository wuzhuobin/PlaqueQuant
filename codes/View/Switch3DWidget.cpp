#include "ui_Switch3DWidget.h"
#include <QSpinBox>
#include <QTextBrowser>
#include <qtablewidget.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Switch3DWidget.h"

#include "ui_LabelWidget.h"

#define setValueConnection(qtObj, filterName, valueName, dataType) \
connect(ui->##qtObj , SIGNAL(valueChanged(##dataType)), core , SLOT(slotSet##filterName##valueName(##dataType)))


Switch3DWidget::Switch3DWidget(QWidget *parent) :
	QWidget(parent)
{

	ui = new Ui::Switch3DWidget;
    ui->setupUi(this);

	ui->styleStackedWidget->setDisabled(true);
}

Switch3DWidget::~Switch3DWidget()
{
    delete ui;
}

void Switch3DWidget::ClearWidget()
{
}

void Switch3DWidget::addWidget(QWidget * style)
{
	ui->styleStackedWidget->addWidget(style);
}

void Switch3DWidget::setWidget(QWidget * style)
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


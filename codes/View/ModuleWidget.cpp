#include "ui_ModuleWidget.h"
#include <QSpinBox>
#include <QTextBrowser>
#include <qtablewidget.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ModuleWidget.h"

#include "ui_LabelWidget.h"

#define setValueConnection(qtObj, filterName, valueName, dataType) \
connect(ui->##qtObj , SIGNAL(valueChanged(##dataType)), core , SLOT(slotSet##filterName##valueName(##dataType)))


ModuleWidget::ModuleWidget(QWidget *parent) :
	QWidget(parent)
{

	ui = new Ui::ModuleWidget;
    ui->setupUi(this);

	ui->styleStackedWidget->setDisabled(true);
}

ModuleWidget::~ModuleWidget()
{
    delete ui;
}

void ModuleWidget::ClearWidget()
{
}

void ModuleWidget::addWidget(QWidget * style)
{
	ui->styleStackedWidget->addWidget(style);
}

void ModuleWidget::setWidget(QWidget * style)
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

//void ModuleWidget::slotChangeOpacity(int opacity)
//{
//	int layer = ui->labelComboBox->currentIndex() + 1;
//	emit signalChangeOpacity(layer, ui->opacitySpinBox->value());
//	//m_mainWnd->m_core->slotChangeOpacity(layer, opacity);
//}
//
//void ModuleWidget::slotUpdateOpacity(int opacity)
//{
//	//int layer = ui->labelComboBox->currentIndex() + 1;
//	//double rgba[4];
//	//m_mainWnd->m_core->GetLookupTable()->GetIndexedColor(layer, rgba);
//	ui->opacitySpinBox->setValue(opacity * 100+0.5);
//
//}

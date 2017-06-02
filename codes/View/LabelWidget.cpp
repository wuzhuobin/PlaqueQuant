#include"ui_LabelWidget.h"
#include "LabelWidget.h"


LabelWidget::LabelWidget(QWidget * parent)
{
	ui = new Ui::LabelWidget;
	ui->setupUi(this);


	connect(ui->spinBoxOpacity, SIGNAL(valueChanged(int)),
		this, SLOT(slotOpacityChange(int)));
	connect(ui->comboBoxLabel, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotCurrentIndexUpdate(int)));
	slotCurrentIndexUpdate(0);
	slotSetOpacity(0);
}

LabelWidget::~LabelWidget()
{
	delete ui;
}

void LabelWidget::slotCurrentIndexUpdate(int color)
{
	if (color == 0) {
		ui->sliderOpacity->setDisabled(true);
		ui->spinBoxOpacity->setDisabled(true);
	}
	else {
		ui->sliderOpacity->setEnabled(true);
		ui->spinBoxOpacity->setEnabled(true);
	}
	emit signalOpacityRequested(color);
}

void LabelWidget::slotSetOpacity(double opacity)
{
	opacity = static_cast<int>(opacity * 100 + 0.5);
	if (opacity != ui->spinBoxOpacity->value()) {
		ui->spinBoxOpacity->setValue(opacity);
	}
}


void LabelWidget::slotOpacityChange(int opacity) 
{
	emit signalOpacityChanged(ui->comboBoxLabel->currentIndex(),
		opacity * 0.01);
}
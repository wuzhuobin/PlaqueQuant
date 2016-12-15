#include"ui_LabelWidget.h"
#include "LabelWidget.h"


LabelWidget::LabelWidget(QWidget * parent)
{
	ui = new Ui::LabelWidget;
	ui->setupUi(this);




}

LabelWidget::~LabelWidget()
{
	delete ui;
}

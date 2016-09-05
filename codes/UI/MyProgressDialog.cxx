#include "MyProgressDialog.h"
#include <QLabel>
#include <QProgressBar>

MyProgressDialog::MyProgressDialog(QWidget *parent): QProgressDialog(parent,Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowStaysOnTopHint)
{
	QProgressBar* bar = new QProgressBar(this);
	bar->setTextVisible(false);

	this->setWindowTitle("Please Wait");
	this->setLabelText("Running algorithm...");
	this->setWindowModality(Qt::WindowModal);
	this->setCancelButton(0);
	this->setBar(bar);
	this->setMinimumDuration(0);
	this->setRange(0,0);
	this->setValue(0);
}

MyProgressDialog::~MyProgressDialog()
{

}
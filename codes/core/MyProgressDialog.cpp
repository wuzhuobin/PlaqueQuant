#include "MyProgressDialog.h"
#include <QLabel>
#include <QProgressBar>

MyProgressDialog::MyProgressDialog(QWidget *parent): QProgressDialog(parent)
{
	QProgressBar* bar = new QProgressBar;
	bar->setTextVisible(false);

	this->setWindowTitle("Please Wait");
	this->setLabelText("Running algorithm...");
	this->setWindowModality(Qt::WindowModal);
	this->setWindowFlags(Qt::CustomizeWindowHint);
	this->setCancelButton(0);
	this->setBar(bar);
	this->setMinimumDuration(0);
	this->setRange(0,0);
	this->setValue(0);
}

MyProgressDialog::~MyProgressDialog()
{

}
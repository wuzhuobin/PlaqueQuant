#include "InfoDialog.h"
#include "ui_InfoDialog.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
	ui->Infotag->setCurrentIndex(0);
	//Signal and slots
	//connect(ui->actionWindowLevel,				SIGNAL(triggered()), this, SLOT(slotWindowLevelMode()));
	connect(ui->SkipBtn, SIGNAL(clicked()), this, SLOT(slotClose()));
	connect(ui->Nextp0Btn, SIGNAL(clicked()), this, SLOT(slotNext()));
	connect(ui->Nextp1Btn, SIGNAL(clicked()), this, SLOT(slotNext()));
	connect(ui->Backp1Btn, SIGNAL(clicked()), this, SLOT(slotBack()));
	connect(ui->Nextp2Btn, SIGNAL(clicked()), this, SLOT(slotNext()));
	connect(ui->Backp2Btn, SIGNAL(clicked()), this, SLOT(slotBack()));
	connect(ui->Nextp3Btn, SIGNAL(clicked()), this, SLOT(slotNext()));
	connect(ui->Backp3Btn, SIGNAL(clicked()), this, SLOT(slotBack()));
	connect(ui->FinishBtn, SIGNAL(clicked()), this, SLOT(slotClose()));
}


void Dialog::slotClose(){
	this->close();
}

void Dialog::slotNext(){
	int index = ui->Infotag->currentIndex();
	ui->Infotag->setCurrentIndex(index+1);
}

void Dialog::slotBack(){
	int index = ui->Infotag->currentIndex();
	ui->Infotag->setCurrentIndex(index-1);
}

Dialog::~Dialog()
{
	delete ui;
}
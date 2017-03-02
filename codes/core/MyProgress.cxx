#include "DialogOpenImage.h"
#include "ui_DialogOpenImage.h"
#include <QFileDialog>


DialogOpenImage::DialogOpenImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpenImage)
{
    ui->setupUi(this);

	connect(this->ui->browseBtn,SIGNAL(clicked()), this, SLOT(slotBrowse()));

}

DialogOpenImage::~DialogOpenImage()
{
    delete ui;
}

void DialogOpenImage::slotBrowse()
{
	//Dialog Open Directory
	QString dir = QFileDialog::getExistingDirectory(this, 
					tr("Open Image Directory"),QDir::currentPath(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
	
	if(dir=="") 
		return;

	dir.replace("\\","/");	

	ui->imageDirLineEdit->setText(dir);

}

QString DialogOpenImage::getDirectory()
{
	return ui->imageDirLineEdit->text();
}
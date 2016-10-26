#include <QFileDialog>
#include <QMessageBox>
#include "keyconfirm.hpp"
#include "ui_keyconfirm.h"

keyConfirm::keyConfirm(QWidget * parent) : ui(new Ui::keyConfirm) {
	ui->setupUi(this);

	connect(this->ui->pushBtnExit, SIGNAL(clicked()), this, SLOT(close()));
	connect(this->ui->pushBtnBrowse, SIGNAL(clicked()), this, SLOT(slotSelectLicenseFile()));
	char mac[18];
	getMacAdd(mac);


	this->ui->macLineEdit->setText(mac);
}

keyConfirm::~keyConfirm() {
	
}

bool keyConfirm::checkLicence()
{
	QString path = QCoreApplication::applicationDirPath();
	QFile licence(path + QString("/license.dat"));
	if(!licence.exists())	return false;
	if (!licence.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
	QString key = licence.readLine();
	key.remove("\n");
	MacEncoding mac( ui->macLineEdit->text().toStdString());
	if (key == QString::fromStdString(mac.encodeString())) return true;
	return false;
}

void keyConfirm::slotSelectLicenseFile()
{
	QString path = QFileDialog::getOpenFileName(this, QString(tr("Open License File")),
			".", tr("license file (license.dat)"));

	QString exePath = QCoreApplication::applicationDirPath();
	QString targetPath = exePath + QString("/license.dat");

	if (QFile(targetPath).exists()) {
		QMessageBox msgBox(QMessageBox::Warning, "Replace License", "Do you want to replace your existing license?");
		msgBox.addButton(QMessageBox::Yes);
		msgBox.addButton(QMessageBox::No);
		
		int reply = msgBox.exec();
		if (QMessageBox::Yes) {
			if (!QFile(targetPath).remove()) {
				QMessageBox warning(QMessageBox::Critical, "License Add Failed", "Replacing file to home directory failed, please open the application with administration right and try again!");
				warning.exec();
				return;
			}
		}
		else {
			return;
		}
	}

	if (!QFile::copy(path, targetPath)) {
		QMessageBox warning(QMessageBox::Critical, "License Add Failed", "Copying file to home directory failed, please open the application with administration right and try again!");
		warning.exec();
	}
	else {
		if (!this->checkLicence()) {
			QMessageBox warning(QMessageBox::Critical, "Incorrect License", "Your license file is incorrect!");
			warning.exec();
		} 
		else {
			QMessageBox msgbox(QMessageBox::Information, "Success", "You license had been added! Please restart this application!");
			msgbox.exec();
		}
	}
}


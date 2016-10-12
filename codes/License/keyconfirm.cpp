#include "keyconfirm.hpp"
#include "ui_keyconfirm.h"

keyConfirm::keyConfirm(QWidget * parent) : ui(new Ui::keyConfirm) {
	ui->setupUi(this);

	this->connect(this->ui->exitPushButton, SIGNAL(clicked()), this, SLOT(close()));
	char mac[18];
	getMacAdd(mac);

	this->ui->macLineEdit->setText(mac);
}

keyConfirm::~keyConfirm() {
	
}

bool keyConfirm::checkLicence()
{
	// obtain executable path
#ifdef _WIN64 || _WIN32
	// The following dosen't work for linux system
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);

	QString qpath = QString::fromStdWString(path);
	QString fileName = qpath.split(QString("\\")).last();

#else 
	WCHAR path = "./license.dat";
#endif
	QFile licence(QString::fromStdWString(path).replace(fileName, QString("license.dat")));
	qDebug() << QString::fromStdWString(path).replace(fileName, QString("license.dat"));
	if(!licence.exists())	return false;
	if (!licence.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
	QString key = licence.readLine();
	key.remove("\n");
	qDebug() << ui->macLineEdit->text();
	qDebug() << key;
	MacEncoding mac( ui->macLineEdit->text().toStdString());
	if (key == QString::fromStdString(mac.encodeString())) return true;
	return false;
}

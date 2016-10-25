#pragma once
#include <QWidget>
#include <QFile>
#include <QDebug>
#include "MacEncoding.h"
#include "getmacadd.h"
/**
 * @class keyConfirm
 * check the licence file is correct or not
 * @author
 * @version
 */

namespace Ui {
	class keyConfirm;
}


class keyConfirm : public QWidget {
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param parent the parent
	 *
	 */
	keyConfirm(QWidget * parent = Q_NULLPTR);
	/**
	 * Destructor
	 */
	~keyConfirm();
	/**
	 * check the licence.bat in ./licence.bat
	 * if it exists and is correct, return true
	 * @return true if correct
	 *         false if incorrect
	 */
	bool checkLicence();

public slots:
	void slotSelectLicenseFile();


private:
	// ui
	Ui::keyConfirm* ui;
};

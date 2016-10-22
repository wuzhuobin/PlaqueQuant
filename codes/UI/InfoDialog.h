#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
	class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();
public slots:
	virtual void slotClose();
	virtual void slotNext();
	virtual void slotBack();
private:
	Ui::Dialog *ui;
};

#endif //INFODIALOG_H
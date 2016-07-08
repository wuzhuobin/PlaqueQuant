#ifndef DIALOGOPENIMAGE_H
#define DIALOGOPENIMAGE_H

#include <QDialog>
#include "Define.h"

namespace Ui {
class DialogOpenImage;
}

class DialogOpenImage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpenImage(QWidget *parent = 0);
    ~DialogOpenImage();

	QString getDirectory();

private:
    Ui::DialogOpenImage *ui;

public slots:
	virtual void slotBrowse();


};

#endif // DIALOGOPENIMAGE_H

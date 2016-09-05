#ifndef MyProgressDialog_h
#define MyProgressDialog_h

#include <QObject>
#include <QWidget>
#include <QFlags>
#include <QProgressDialog>

class MyProgressDialog : public QProgressDialog
{
	Q_OBJECT

public:
	//Constructor, destructor
	MyProgressDialog ( QWidget * parent = 0);
	virtual ~MyProgressDialog();

protected:
};

#endif



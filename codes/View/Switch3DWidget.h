#ifndef __SWITCH3D_WIDGET_H__
#define __SWITCH3D_WIDGET_H__

#include <QWidget>



namespace Ui { class Switch3DWidget; }

class Switch3DWidget: public QWidget
{
    Q_OBJECT

public:
    explicit Switch3DWidget(QWidget *parent = 0);
    ~Switch3DWidget();

	void ClearWidget();
	void addWidget(QWidget* style);
	void setWidget(QWidget* style);
	Ui::Switch3DWidget* getUi() { return ui; }

    
private:

    Ui::Switch3DWidget *ui;
	
};

#endif // __SWITCH3D_WIDGET_H__

#ifndef __SWITCH2D_WIDGET_H__
#define __SWITCH2D_WIDGET_H__

#include <QWidget>



namespace Ui { class Switch2DWidget; }

class Switch2DWidget: public QWidget
{
    Q_OBJECT

public:
    explicit Switch2DWidget(QWidget *parent = 0);
    ~Switch2DWidget();

	void ClearWidget();
	void addWidget(QWidget* style);
	void setWidget(QWidget* style);
	Ui::Switch2DWidget* getUi() { return ui; }

    
private:

    Ui::Switch2DWidget *ui;
	
};

#endif // __SWITCH2D_WIDGET_H__

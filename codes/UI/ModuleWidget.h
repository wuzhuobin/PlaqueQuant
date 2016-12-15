#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDir>
#include <QButtonGroup>


class MainWindow;
namespace Ui { class ModuleWidget; }

class ModuleWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ModuleWidget(QWidget *parent = 0);
    ~ModuleWidget();

	void ClearWidget();
	void addWidget(QWidget* style);
	void setWidget(QWidget* style);

public slots:
	void slotChangeOpacity(int opactiy);
	void slotUpdateOpacity();



protected:
    
private:

    Ui::ModuleWidget *ui;

	friend class MainWindow;
	MainWindow* m_mainWnd;
	
};

#endif // MODULEWIDGET_H

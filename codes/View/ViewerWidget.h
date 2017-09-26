#ifndef __VIEWER_WIDGET_H__
#define __VIEWER_WIDGET_H__

#include <qdockwidget.h>


namespace Ui { class ViewerWidget; }

class ViewerWidget: public QDockWidget
{
	Q_OBJECT;
public:
	ViewerWidget(QWidget* parent = nullptr);
	Ui::ViewerWidget* getUi();

public slots:
	void showMaximized(bool flag);

protected:
	//virtual void resizeEvent(QResizeEvent* event) override;
	virtual void moveEvent(QMoveEvent* event)override;


private:
	Ui::ViewerWidget* ui = nullptr;
};


#endif // !__VIEWER_WIDGET_H__

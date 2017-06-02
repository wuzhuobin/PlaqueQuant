#ifndef __LABEL_WIDGET_H__
#define __LABEL_WIDGET_H__

#include <qwidget.h>
namespace Ui { class LabelWidget; }
class LabelWidget: public QWidget
{
	Q_OBJECT;
public:
	LabelWidget(QWidget* parent = nullptr);
	~LabelWidget();


public slots:
	void slotSetOpacity(double opacity);

signals:
	void signalOpacityChanged(int color, double opacity);
	void signalOpacityRequested(int color);

protected slots:
	void slotOpacityChange(int opacity);
	void slotCurrentIndexUpdate(int color);

private:
	Ui::LabelWidget* ui = nullptr;
};

#endif // !__LABEL_WIDGET_H__

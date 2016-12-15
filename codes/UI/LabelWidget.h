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

private:
	Ui::LabelWidget* ui = nullptr;
};

#endif // !__LABEL_WIDGET_H__

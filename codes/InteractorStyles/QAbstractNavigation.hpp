#pragma once
#include "QAbstractInteractorStyle.h"
#include "ui_QAbstractNavigation.h"

namespace Ui { class QAbstractNavigation;}

class QAbstractNavigation : public QAbstractInteractorStyle
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QAbstractNavigation);
public:
	virtual void SetCurrentFocalPointWithImageCoordinate(int* ijk);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
	virtual void Initialization();
public slots:
	virtual void slotChangeSlice();



protected:
	/**
	 * 
	 */
	QAbstractNavigation(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	~QAbstractNavigation();
private:
	Ui::QAbstractNavigation* ui = nullptr;
};

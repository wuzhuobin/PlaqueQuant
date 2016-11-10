#pragma once
#include "QAbstractInteractorStyle.h"

namespace Ui { class QAbstractNavigation;}

class QAbstractNavigation : public QAbstractInteractorStyle
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QAbstractNavigation);
public:
	virtual void SetCurrentFocalPointWithImageCoordinate(int* ijk);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
public slots:
	virtual void slotChangeSlice();



protected:
	/**
	 * 
	 */
	QAbstractNavigation(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	~QAbstractNavigation();
	virtual void UniqueEnable(bool flag);

private:
	Ui::QAbstractNavigation* ui = nullptr;
};

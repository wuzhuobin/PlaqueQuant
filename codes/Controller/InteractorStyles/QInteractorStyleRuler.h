#ifndef __QInteractorStyleRuler_H__
#define __QInteractorStyleRuler_H__

#include "InteractorStyleRuler.h"
#include "QAbstractNavigation.h"

namespace Ui { class QInteractorStyleRuler; }


class QInteractorStyleRuler : public QAbstractNavigation,
	public InteractorStyleRuler
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleRuler);
public:
	vtkTypeMacro(QInteractorStyleRuler, InteractorStyleRuler);
	static QInteractorStyleRuler* New();
public slots:
	virtual void SetCustomEnabled(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);


protected:
	QInteractorStyleRuler(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	virtual ~QInteractorStyleRuler ();


private:


	Ui::QInteractorStyleRuler* ui = nullptr;
};

#endif // !__QInteractorStyleRuler_H__




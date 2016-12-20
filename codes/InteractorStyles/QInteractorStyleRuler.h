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
	void SetDistanceWidgetEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);


public slots:
	void EnableMaximumWallThickneesLabel(bool flag);
protected:
	QInteractorStyleRuler(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleRuler ();

	virtual void uniqueInvoke(bool flag);

private:


	Ui::QInteractorStyleRuler* ui = nullptr;
};

#endif // !__QInteractorStyleRuler_H__




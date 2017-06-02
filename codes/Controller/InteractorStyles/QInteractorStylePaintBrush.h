#include "InteractorStylePaintBrush.h"
#include "QAbstractNavigation.h"

namespace Ui { class QInteractorStylePaintBrush; }

class QInteractorStylePaintBrush: public QAbstractNavigation,
	public InteractorStylePaintBrush
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStylePaintBrush);
public:
	vtkTypeMacro(QInteractorStylePaintBrush, InteractorStylePaintBrush);
	static QInteractorStylePaintBrush* New();
public slots:
	virtual void SetCustomEnabled(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	virtual void SetPaintBrushLabel(int paintBrushLabel);
	virtual void SetDrawOpacity(int opacity);

	virtual void SetBrushShape(int brushShape);
	virtual void SetBrushSize(int size);
	virtual void EnableEraserMode(bool flag);


protected:
	QInteractorStylePaintBrush(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStylePaintBrush();

	void uniqueInitialization();
	void initialization();
	//virtual void uniqueInvoke(bool flag);

private:
	Ui::QInteractorStylePaintBrush* ui = nullptr;

};


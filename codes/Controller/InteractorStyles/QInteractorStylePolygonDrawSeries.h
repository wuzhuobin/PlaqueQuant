#ifndef __QINTERACTOR_STYLE_POLYGON_DRAW_SERIES_H__
#define __QINTERACTOR_STYLE_POLYGON_DRAW_SERIES_H__
#include "QAbstractNavigation.h"

#include "InteractorStylePolygonDrawSeries.h"

#include <vtkSmartPointer.h>

#include <qsharedpointer.h>

namespace Ui {class QInteractorStylePolygonDrawSeries;}
class vtkPolyData;
class ContourWidgetSeries;
class ContourWidgetSeriesOrientedGlyphRepresentation;
class QInteractorStylePolygonDrawSeriesCallback;

class QInteractorStylePolygonDrawSeries : public QAbstractNavigation, 
	public InteractorStylePolygonDrawSeries
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStylePolygonDrawSeries);

public:
	vtkTypeMacro(QInteractorStylePolygonDrawSeries, InteractorStylePolygonDrawSeries);
	static QInteractorStylePolygonDrawSeries* New();

	typedef QList<vtkSmartPointer<vtkPolyData>> PolyDataList;
	typedef QSharedPointer<PolyDataList> ContourMapElementPtr;
	typedef QMap<int, ContourMapElementPtr> ContourMap;

	static const char* COLOUR;
	static const char* SLICE_ORIENTATION;
	static const char* INTERPOLATOR;

public slots:
	virtual void SetCustomEnabled(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
	virtual void SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k) override;

	virtual void SetLinearInterpolator() { SetInterpolator(LINEAR); }
	virtual void SetBezierInterpolator() { SetInterpolator(BEZIER); }
	virtual void SetMagneticInterpolator() { SetInterpolator(MAGNETIC); }
	virtual void SetContourLabel(int contourLabel);

	virtual void SaveContoursToContourMap();
	virtual void LoadContoursFromContourMap();

	virtual void CleanOne();

	virtual void CleanAll();

	virtual void FillContours() override;

	virtual void SetFillSliceBegin(int slice);
	virtual void SetFillSliceEnd(int slice);
	virtual void FillSlices();



protected:
	QInteractorStylePolygonDrawSeries(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	~QInteractorStylePolygonDrawSeries();

	void uniqueInitialization();
	void initialization();

	void SaveContoursToContourMap(
		int slice,
		ContourWidgetSeries* contourWidgetSeries,
		ContourMap& contourMap);
	void LoadContoursFromContourMap(
		int slice,
		ContourWidgetSeries* contourWidgetSeries,
		ContourMap& contourMap);

	virtual void OnEnter() override;



	vtkSmartPointer<QInteractorStylePolygonDrawSeriesCallback> m_callback = nullptr;

	static ContourMap m_normalYZ;
	static ContourMap m_normalXZ;
	static ContourMap m_normalXY;

	int m_fillSliceBegin = 0;
	int m_fillSliceEnd = 0;

private:
	Ui::QInteractorStylePolygonDrawSeries *ui = nullptr;
};

#endif // __QINTERACTOR_STYLE_POLYGON_DRAW_SERIES_H__
#ifndef __QINTERACTOR_STYLE_VESSEL_SEGMENTATION2_H__
#define __QINTERACTOR_STYLE_VESSEL_SEGMENTATION2_H__


#include "QInteractorStylePolygonDrawSeries.h"

namespace Ui {class QInteractorStyleVesselSegmentation2;}
class vtkPolyData;
class ContourWidgetSeries;
class ContourWidgetSeriesOrientedGlyphRepresentation;
class QInteractorStyleVesselSegmentation2Callback;

class QInteractorStyleVesselSegmentation2 : public QInteractorStylePolygonDrawSeries
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleVesselSegmentation2);

public:
	vtkTypeMacro(QInteractorStyleVesselSegmentation2, QInteractorStylePolygonDrawSeries);
	static QInteractorStyleVesselSegmentation2* New();

	enum SegmentationMode
	{
		NORMAL = 0, 
		LUMEN_SEGMENTATION = 1,
		VESSEL_WALL_SEGMENTATION = 2
	};

public slots:
	virtual void SetCustomEnabled(bool flag);

	virtual void SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k) override;


	virtual void SetSegmentationMode(int i);
	virtual void SetSegmentationModeToNormal() { SetSegmentationMode(NORMAL); }
	virtual void SetSegmentationModeToLumenSegmentation() { SetSegmentationMode(LUMEN_SEGMENTATION); }
	virtual void SetSegmentationModeToVesselSegmentation() { SetSegmentationMode(VESSEL_WALL_SEGMENTATION); }

	virtual void SaveContoursToContourMap() override;
	virtual void LoadContoursFromContourMap() override;

	virtual void CleanAll() override;

	virtual void FillContours() override;
	virtual void Interpolate();
	//virtual void FillSlices() override;


	virtual void AutoVesselWallSegmentation();
	virtual void setVesselWallSegmentationRadius(int value);

	virtual void AutoLumenSegmentation();
	virtual void SetLumenSegmentationValue(int value);


protected:
	QInteractorStyleVesselSegmentation2(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	~QInteractorStyleVesselSegmentation2();

	void uniqueInitialization();
	void initialization();

	virtual void uniqueEnable() override;

	virtual void OnEnter() override;
	virtual void OnMouseMove() override;
	virtual void OnLeftButtonDown() override;
	virtual void OnLeftButtonUp() override;

	vtkSmartPointer<ContourWidgetSeries> m_lumenContours = nullptr;
	vtkSmartPointer<ContourWidgetSeries> m_vesselWallContours = nullptr;
	vtkSmartPointer<ContourWidgetSeriesOrientedGlyphRepresentation> m_lumenContoursRep = nullptr;
	vtkSmartPointer<ContourWidgetSeriesOrientedGlyphRepresentation> m_vesselWallContoursRep = nullptr;

	friend class QInteractorStyleVesselSegmentation2Callback;
	vtkSmartPointer<QInteractorStyleVesselSegmentation2Callback> m_callback = nullptr;

	static ContourMap m_normalYZ;
	static ContourMap m_normalXZ;
	static ContourMap m_normalXY;
	static ContourMap m_lumen;
	static ContourMap m_vesselWall;


	int m_currentMode = 0;
	unsigned char m_vesselWallLabel = 1;
	unsigned char m_lumenWallLabel = 2;
	int m_vesselWallSegmentationRadius = 5;
	int m_lumenSegmentationValue = 60;
private:
	Ui::QInteractorStyleVesselSegmentation2 *ui = nullptr;
};

#endif // __QINTERACTOR_STYLE_VESSEL_SEGMENTATION2_H__
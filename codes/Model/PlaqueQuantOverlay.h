#ifndef __PLAQUE_QUANT_OVERLAY_H__
#define __PLAQUE_QUANT_OVERLAY_H__

#include "Overlay.h"
#include <qmap.h>
#include <qsharedpointer.h>

class vtkPolyData;
class PlaqueQuantOverlay : public Overlay 
{
	Q_OBJECT;

public:


	static const int NUMBER_OF_COLOR = 7;
	const int* m_overlayColor[NUMBER_OF_COLOR];
	const int zeros[4] = { 0,0,0,0 };
	const int vesselWall[4] = { 0, 0, 255, 255 };
	const int lumen[4] = { 255, 0, 0, 255 };
	const int calcification[4] = { 0,255,0,255 };
	const int hemorrhage[4] = { 255, 255, 0,255 };
	const int lrnc[4] = { 0, 255, 255 ,255 };
	const int lm[4] = { 255, 0, 255 ,255 };


	PlaqueQuantOverlay(QObject* parent = nullptr);
	PlaqueQuantOverlay(OverlayImageData::itkImageType::Pointer data, QObject* parent = nullptr);
	PlaqueQuantOverlay(OverlayImageData* data, QObject* parent = nullptr);

	virtual vtkPolyData* getCenterLine();

	virtual void updatedOverlay() override;


	// total plaque, vessel wall, lumen, calcification, hemorrhage, lrnc, lm,
	double Measurements3D[7] = { 0 };
	// vessel wall, lumen, NMI
	//double Measurements2D[3] = { 0 };
	QMap<int, QSharedPointer<double>> Measurements2D;

	virtual int getCurrentSlice();
public slots:
	virtual void updateMeasurement3D();
	virtual void setCurrentSlice(int slice);
	virtual void updateMeasurement2D(int slice);

protected:
	vtkSmartPointer<vtkPolyData> m_centerline = nullptr;
	int currentSlice = 0;

};



#endif // !__PLAQUE_QUANT_OVERLAY_H__

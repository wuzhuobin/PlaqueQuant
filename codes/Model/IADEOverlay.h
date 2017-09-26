#ifndef __IADE_OVERLAY_H__
#define __IADE_OVERLAY_H__

#include "Overlay.h"
#include <qmap.h>
#include <qsharedpointer.h>

class vtkPolyData;
class IADEOverlay : public Overlay 
{
	Q_OBJECT;

public:


	static const int NUMBER_OF_COLOR = 3;
	const int* m_overlayColor[NUMBER_OF_COLOR];
	const int zeros[4] = { 0,0,0,0 };
	const int vesselWall[4] = { 0, 0, 255, 255 };
	const int lumen[4] = { 255, 0, 0, 255 };



	IADEOverlay(QObject* parent = nullptr);
	IADEOverlay(OverlayImageData::itkImageType::Pointer data, QObject* parent = nullptr);
	IADEOverlay(OverlayImageData* data, QObject* parent = nullptr);

	virtual vtkPolyData* getCenterLine();

	virtual void updatedOverlay() override;


	// total plaque, vessel wall, lumen, calcification, hemorrhage, lrnc, lm,
	double Measurements3D[7] = { 0 };
	// vessel wall, lumen, NMI
	//double Measurements2D[3] = { 0 };
	QMap<int, QSharedPointer<double>> Measurements2D;

	virtual int getCurrentSlice();
public slots:
	virtual void updateMeasurement3D() {};
	virtual void setCurrentSlice(int slice);
	virtual void updateMeasurement2D(int slice) {};

protected:
	vtkSmartPointer<vtkPolyData> m_centerline = nullptr;
	int currentSlice = 0;

};



#endif // !__IADE_OVERLAY_H__

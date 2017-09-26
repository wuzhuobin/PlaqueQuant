#include "Measurement.h"

#include <qsharedpointer.h>
#include <qmap.h>
class QInteractorStyleSurfaceCenterLineStenosis;
class PlaqueQuantMeasurement: public Measurement
{
	Q_OBJECT;
public:
	PlaqueQuantMeasurement(QObject* parent);
	~PlaqueQuantMeasurement();


public slots:
	virtual void update() override;
	virtual void updateMeasurement3D();
	virtual void updateMeasurement2D();
	virtual void updateMaximumWallThickness(int slice);
	virtual void updateStenosis(double stenosis);

signals:
	void signalMeasurement2D(double*);
	void signalMeasurement3D(double*);
	void signalStenosis(double);
	//void signalMeasurementMaximumWallThickness(double);

protected:
	int m_currentSlice = 0;
	// total plaque, vessel wall, lumen, calcification, hemorrhage, lrnc, lm,
	double m_measurements3D[7] = { 0 };
	// vessel wall, lumen, NMI
	//double m_measurements2D[4] = { 0 };
	QMap<int, QSharedPointer<double>> m_measurements2DMap;
	double m_stenosis = 0;

};

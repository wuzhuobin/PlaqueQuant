#if !defined(__IADE_MEASUREMENT_H__)
#define __IADE_MEASUREMENT_H__

#include "Measurement.h"
 
class IADEMeasurement: public Measurement
{

	Q_OBJECT;
public:

	IADEMeasurement(QObject* parent = nullptr);
	virtual ~IADEMeasurement() override;

public slots:

	virtual void updateICDAStandard();
	virtual void updateVBDSmoker();
	virtual void updateVBDUbogu();

};

#endif // !defined(__IADE_MEASUREMENT_H__)

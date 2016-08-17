#ifndef MEASUREMENTFOR3D_H
#define MEASUREMENTFOR3D_H

#include <vtkImageData.h>
#include <vtkLookupTable.h>


class MeasurementFor3D
{
public:
	MeasurementFor3D();
	~MeasurementFor3D();

	void SetInputData(vtkImageData* overlay);
	void SetLookupTable(vtkLookupTable* lookupTable);
	void Update();
	void GetVolumes(double* volumes);

private:

	//double volumeOfLumen = 0;
	//double volumeOfVesselWall = 0;
	//double volumeOfCalcification = 0;
	//double volumeOfHemorrhage = 0;
	//double volumeOfLRNC = 0;
	//double volumeOfLM = 0;
	// Same as above 
	double* volumes;

	vtkImageData* overlay;
	vtkLookupTable* lookupTable;

};



#endif // MEASUREMENTFOR3D_H
#ifndef MEASUREMENTS_FOR_2D_H
#define MEASUREMENTS_FOR_2D_H

#include <vector>
#include <vtkObjectFactory.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

class MeasurementFor2D : public vtkObject
{
public:
	vtkTypeMacro(MeasurementFor2D, vtkObject);
	static MeasurementFor2D* New();

	struct Measurements2D
	{
		double LumenArea;
		double VesselWallArea;
		double NMI;
		double MaximumWallThickness;
	};

	void SetSliceImage(vtkImageData*);

	vtkSetMacro(SliceNumber, int);
	vtkGetMacro(SliceNumber, int);

protected:
	MeasurementFor2D();
	~MeasurementFor2D();

private:
	typedef std::pair<int, int> CoordIndex;

	int CountCluster(int, int);
	bool CheckCoordIndexInList(CoordIndex);

	vtkImageData* SliceImage;

	int SliceNumber;
	int ImageExtent[6];

	std::vector<CoordIndex> m_countedList;



};


#endif
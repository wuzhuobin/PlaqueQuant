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
		//double MaximumWallThickness;
	};


	Measurements2D GetOutput(int);
	void SetSliceImage(vtkImageData* );
	void Update();

	vtkSetMacro(SliceNumber, int);
	vtkGetMacro(SliceNumber, int);

	enum ERROR_CODE {
		ERROR_OUTPUT_NOT_EXIST = 1,
		ERROR_INPUT_NOT_A_SLICE = 6
	};

protected:
	MeasurementFor2D();
	~MeasurementFor2D();

private:
	typedef std::pair<int, int> CoordIndex;
	
	void CountLabels();

	int CountCluster(int, int);
	bool CheckCoordIndexInList(CoordIndex);

	vtkSmartPointer<vtkImageData> SliceImage = NULL;

	int SliceNumber;
	int ImageExtent[6];

	std::vector<CoordIndex> m_countedList;
	std::vector<Measurements2D> m_outputList;
};


#endif
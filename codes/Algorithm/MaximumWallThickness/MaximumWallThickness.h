#ifndef MAXIMUMWALLTHICKNESS_H
#define MAXIMUMWALLTHICKNESS_H

#include <vtkObject.h>
#include <vtkSmartPointer.h>

#include <vector>
class vtkPolyData;
class vtkImageData;
class vtkContourFilter;

class MaximumWallThickness : public vtkObject
{
public:
	vtkTypeMacro(MaximumWallThickness, vtkObject);
	static MaximumWallThickness* New();

	const static int EDGENUM = 2;

	MaximumWallThickness();
	~MaximumWallThickness();


	typedef std::pair<vtkPolyData*, vtkPolyData*> LoopPair;
	struct DistanceLoopPair
	{
		LoopPair			LoopPair;
		double				Distance;
		std::pair<int, int> PIDs;
	};
	/* Do not modify the value, read only! */
	std::vector<DistanceLoopPair>	GetDistanceLoopPairVect();

	/* Two ways to specify the region of interest
	1. Set image slice number, then default extent is extracted from Overlay of mainwnd
	2. Directly set slice image
	*/
	void SetLumemIntensity(int i);
	void SetVesselIntensity(int i);
	void SetImage(vtkImageData*);
	void SetSliceNumber(int);
	void SetSliceImage(vtkImageData*);


	void Update();

	enum ERROR_CODE {
		ERROR_VALUE_TRANSFORM = 1,
		ERROR_EXTRACT_SLICE = 2,
		ERROR_EXTRACT_LOOP = 3,
		ERROR_UNDEFINED_BRANCH = 4,
		ERROR_THICKNESS_CAL = 5,
		ERROR_INPUT_NOT_A_SLICE = 6,
		ERROR_NO_SEGMENTATION_FOUND = 7
	};

private:
	int CheckNumberOfBranches();

	bool ValueTransform();
	bool ExtractSlice();
	bool ExtractLoops();
	bool EdgeDetection();
	bool ThicknessCal();

	vtkSmartPointer<vtkImageData> m_image;
	vtkSmartPointer<vtkImageData> m_sliceImage;
	vtkSmartPointer<vtkImageData> m_edgeImage[EDGENUM];
	vtkSmartPointer<vtkContourFilter> m_contourFilter;

	std::vector<LoopPair> m_loopPairVect;
	std::vector<DistanceLoopPair> m_distanceVect;

	int m_sliceNumber;
	int m_extent[6];
	int m_lumenIntensity = 1;
	int m_vesselIntensity = 2;

};

#endif // !MAXIUMWALLTHICKNESS_H

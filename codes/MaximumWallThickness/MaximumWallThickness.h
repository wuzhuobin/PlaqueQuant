#ifndef MAXIMUMWALLTHICKNESS_H
#define MAXIMUMWALLTHICKNESS_H
#endif // !MAXIUMWALLTHICKNESS_H

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkImageShiftScale.h>
#include <vtkImageTHreshold.h>
#include <vtkExtractVOI.h>
#include <vtkThreshold.h>
#include <vtkImageSobel2D.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageMathematics.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageGradient.h>
#include <vtkImageMagnitude.h>
#include <vtkImageNonMaximumSuppression.h>
#include <vtkImageConstantPad.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkLinkEdgels.h>
#include <vtkGeometryFilter.h>
#include <vtkSubPixelPositionEdgels.h>
#include <vtkStripper.h>
#include <vtkContourFilter.h>

#include <utility>
#include <math.h>
#include <list>
#include <iostream>


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
		1. Set image slice number, then default extent is extracted from SegmentationOverlay of mainwnd
		2. Directly set slice image
	*/
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

		ERROR_INPUT_NOT_A_SLICE = 6
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

};



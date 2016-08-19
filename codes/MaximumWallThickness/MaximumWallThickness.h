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

#include <utility>
#include <math.h>
#include <list>
#include <iostream>


class EdgePoint;
class MaximumWallThickness
{
public:

	const static int EDGENUM = 2;

	MaximumWallThickness(vtkImageData* image, int internalEdgeValue = 1, int externalEdgeValue = 0);
	~MaximumWallThickness();

	bool valueTransform();
	bool extractVOI();
	bool thresholdImage();
	bool edgeDetection();
	bool thicknessCal();
	bool thicknessCal2();
	bool output();
	bool setExtent(int* extent);
	const int* getExtent();

//private:

	vtkSmartPointer<vtkImageData> image;
	vtkSmartPointer<vtkImageData> edgeImage[EDGENUM];


	int edgeValue[EDGENUM];
	int* extent;
	


	std::list<std::pair<double, std::pair<EdgePoint, EdgePoint>>> maximumWallThickness;
	EdgePoint* centers[EDGENUM];
	std::list<EdgePoint>* edgePoints[EDGENUM];

};

class EdgePoint : public std::pair<double, double> {

public:
	double x = this->first;
	double y = this->second;

};


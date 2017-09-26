#ifndef __VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_H__
#define __VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_H__

#include <vtkPolyDataAlgorithm.h>
#include <vtkSetGet.h>
#include <vtkSmartPointer.h>


class vtkImageData;

class VesselWallHoughTransformCircles : public vtkPolyDataAlgorithm
{
public:


	static VesselWallHoughTransformCircles* New();
	vtkTypeMacro(VesselWallHoughTransformCircles, vtkPolyDataAlgorithm);

	vtkSetMacro(LumenLabel, unsigned int);
	vtkGetMacro(LumenLabel, unsigned int);

	vtkSetMacro(VesselWallLabel, unsigned int);
	vtkGetMacro(VesselWallLabel, unsigned int);

	vtkSetMacro(InnerRadius, unsigned int);
	vtkGetMacro(InnerRadius, unsigned int);

	vtkSetMacro(OuterRadius, unsigned int);
	vtkGetMacro(OuterRadius, unsigned int);


	// Description:
	// Set/Get the i-j-k index range which define a plane on which to generate
	// contour lines. Using this ivar it is possible to input a 3D volume
	// directly and then generate contour lines on one of the i-j-k planes, or
	// a portion of a plane.
	vtkSetVectorMacro(ImageRange, int, 6);
	vtkGetVectorMacro(ImageRange, int, 6);

	typedef std::vector<vtkSmartPointer<vtkPolyData>> CircleVector;
	virtual CircleVector* GetCircles() { return &this->Circles; }

protected:
	VesselWallHoughTransformCircles();
	virtual ~VesselWallHoughTransformCircles();

	// Description:
	// This is called by the superclass.
	// This is the method you should override.
	virtual int RequestData(vtkInformation* request,
		vtkInformationVector** inputVector,
		vtkInformationVector* outputVector);
	virtual int FillInputPortInformation(int port, vtkInformation *info);


	template<typename TPixelType>
	void itkProcess(
		vtkImageData* input0, 
		vtkImageData* overlay, 
		vtkPolyData* output, 
		int* roi, int plane);


	void SetImageRange(int imin, int imax, int jmin, int jmax,
		int kmin, int kmax);





	unsigned int LumenLabel = 2;
	unsigned int VesselWallLabel = 1;
	unsigned int InnerRadius = 2;
	unsigned int OuterRadius = 4;
	int ImageRange[6] = { 0 };
	CircleVector Circles;


private:
	VesselWallHoughTransformCircles(const VesselWallHoughTransformCircles&);  // Not implemented.
	void operator=(const VesselWallHoughTransformCircles&);  // Not implemented.





};



#endif // !__VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_H__

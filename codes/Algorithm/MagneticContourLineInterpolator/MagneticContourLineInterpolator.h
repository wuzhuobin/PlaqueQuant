#ifndef __MAGNETIC_CONTOUR_LINE_INTERPOLATOR_H__
#define __MAGNETIC_CONTOUR_LINE_INTERPOLATOR_H__

#include <vtkDijkstraImageContourLineInterpolator.h>

class vtkImageGradientMagnitude;
class vtkImageShiftScale;
class vtkImageGaussianSmooth;

class MagneticContourLineInterpolator: public vtkDijkstraImageContourLineInterpolator
{
public:
	vtkTypeMacro(MagneticContourLineInterpolator, vtkDijkstraImageContourLineInterpolator);
	void PrintSelf(ostream& os, vtkIndent indent);
	static MagneticContourLineInterpolator *New();

	// Description:
	// Subclasses that wish to interpolate a line segment must implement this.
	// For instance vtkBezierContourLineInterpolator adds nodes between idx1
	// and idx2, that allow the contour to adhere to a bezier curve.
	virtual int InterpolateLine(vtkRenderer *ren,
		vtkContourRepresentation *rep,
		int idx1, int idx2) override;

	
	vtkGetObjectMacro(ImageGradientMagnitude, vtkImageGradientMagnitude);
	vtkGetObjectMacro(ImageShiftScale, vtkImageShiftScale);
	vtkGetObjectMacro(ImageGaussianSmooth, vtkImageGaussianSmooth);

protected:

	MagneticContourLineInterpolator();
	virtual ~MagneticContourLineInterpolator() override;

	vtkImageGradientMagnitude* ImageGradientMagnitude = nullptr;
	vtkImageShiftScale* ImageShiftScale = nullptr;
	vtkImageGaussianSmooth* ImageGaussianSmooth = nullptr;

private:
	MagneticContourLineInterpolator(
		const MagneticContourLineInterpolator&);  //Not implemented
	void operator=(const
		MagneticContourLineInterpolator&);  //Not implemented
};



#endif // !__MAGNETIC_CONTOUR_LINE_INTERPOLATOR_H__

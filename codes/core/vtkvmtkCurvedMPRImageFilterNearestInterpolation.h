#ifndef VTK_VMTK_CURVEDMPRIMAGEFILTER_NEAREST_INTERPOLATION
#define VTK_VMTK_CURVEDMPRIMAGEFILTER_NEAREST_INTERPOLATION

#include <vtkvmtkCurvedMPRImageFilter.h>

class vtkvmtkCurvedMPRImageFilterNearestInterpolation : public vtkvmtkCurvedMPRImageFilter
{
public:
	static vtkvmtkCurvedMPRImageFilterNearestInterpolation* New() {
		return new vtkvmtkCurvedMPRImageFilterNearestInterpolation();
	}
	vtkTypeMacro(vtkvmtkCurvedMPRImageFilterNearestInterpolation, vtkvmtkCurvedMPRImageFilter);
	// Description:
	// This method is called by the superclass and performs the actual computation of the MPR image
	virtual int RequestData(
		vtkInformation * vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector);


protected:
	template<class T>
	void FillSlice(T* outReslicePtr, T* outputImagePtr, int* resliceUpdateExtent, int* outExtent, vtkIdType* outputInc, int slice)
	{
		//write result to the new dataset
		int size = (outExtent[1] - outExtent[0] + 1)*(outExtent[3] - outExtent[2] + 1);
		for (int i = 0; i < size; i++)
		{
			outputImagePtr[i + (slice - outExtent[4]) * outputInc[2]] = outReslicePtr[i];
		}
	}

};

#endif // !__DATA_PROCESSOR_H__

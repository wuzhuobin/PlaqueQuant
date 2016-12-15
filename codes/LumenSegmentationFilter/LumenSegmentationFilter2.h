#ifndef __LUMEN_SEGMENTATION_FILTER2_H__
#define __LUMEN_SEGMENTATION_FILTER2_H__

#include <vtkPolyDataAlgorithm.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataCollection.h>
#include <vtkOrientedGlyphContourRepresentation.h>



class LumenSegmentationFilter2 : public vtkPolyDataAlgorithm
{
public:
	vtkTypeMacro(LumenSegmentationFilter2, vtkPolyDataAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent);

	static LumenSegmentationFilter2 *New();

	void SetSlice(int slice);
	/**
	 * Set GenerateValuse of the vtkPolyDataContourFilter
	 * placing 3 values in an array and set as the input of void SetGenerateValues(double* generateValues);
	 * @param	generateValues0 1st generateValue of contourFilter
	 * @param	generateValues1 2nd
	 * @param	generateValues2 3rd
	 */
	void SetGenerateValues(double generateValues0, double generateValues1, double generateValues2);
	void SetGenerateValues(double* generateValues);

	void SetVesselWallContourRepresentation(vtkContourRepresentation* vesselWallContourRepresentation);


protected:
	LumenSegmentationFilter2();
	~LumenSegmentationFilter2() {};

	virtual int RequestData(vtkInformation* request,
		vtkInformationVector** inputVector,
		vtkInformationVector* outputVector);
	virtual int FillInputPortInformation(int port, vtkInformation *info);

	vtkContourRepresentation* vesselWallContourRepresentation = nullptr;

	double generateValues[3] = { 0 };
	int VOI[6] = { 0 };

private:
	LumenSegmentationFilter2(const LumenSegmentationFilter2&);  // Not implemented.
	void operator=(const LumenSegmentationFilter2&);  // Not implemented.

};

#endif
#ifndef __LUMENSEGMENTATION_H
#define __LUMENSEGMENTATION_H

#include <vtkPolyDataAlgorithm.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataCollection.h>
#include <vtkOrientedGlyphContourRepresentation.h>



class LumenSegmentationFilter : public vtkPolyDataAlgorithm
{
public:
	vtkTypeMacro(LumenSegmentationFilter, vtkPolyDataAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent);

	static LumenSegmentationFilter *New();

	//void SetInputData(vtkImageData* input);

	//vtkPolyData* GetOutput();

	void SetVOI(int* VOI);
	void SetVOI(int extent0, int extent1, int extent2, 
		int extent3, int extent4, int extent5);
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

	static void ReorderPolyData(vtkPolyData* lumenWallPolyData);


protected:
	LumenSegmentationFilter();
	~LumenSegmentationFilter() {};

	virtual int RequestData(vtkInformation* request,
		vtkInformationVector** inputVector,
		vtkInformationVector* outputVector);
	virtual int FillInputPortInformation(int port, vtkInformation *info);

	vtkContourRepresentation* vesselWallContourRepresentation = nullptr;


	//vtkSmartPointer<vtkPolyData> m_contour = NULL;
	//vtkSmartPointer<vtkImageData> input = NULL;
	//vtkSmartPointer<vtkImageData> extract = NULL;

	double generateValues[3] = { 0 };
	int VOI[6] = { 0 };

private:
	LumenSegmentationFilter(const LumenSegmentationFilter&);  // Not implemented.
	void operator=(const LumenSegmentationFilter&);  // Not implemented.

};

#endif
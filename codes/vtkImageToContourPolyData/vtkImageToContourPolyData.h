// .NAME vtkImageToContourPolyData
// .SECTION Description
// vtkImageToContourPolyData
#ifndef __VTK_IMAGE_TO_CONOUTR_POLY_DATA_H__
#include "vtkPolyDataAlgorithm.h"

class vtkImageToContourPolyData : public vtkPolyDataAlgorithm
{
public:
	vtkTypeMacro(vtkImageToContourPolyData, vtkPolyDataAlgorithm);
	static vtkImageToContourPolyData *New();

protected:
	vtkImageToContourPolyData() {}
	~vtkImageToContourPolyData() {}

	//int FillOutputPortInformation( int port, vtkInformation* info );
	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
	vtkImageToContourPolyData(const vtkImageToContourPolyData&);  // Not implemented.
	void operator=(const vtkImageToContourPolyData&);  // Not implemented.

};
#endif // !__VTK_IMAGE_TO_CONOUTR_POLY_DATA_H__






#ifndef __REORDER_POINT_ID_OF_CONTOUR_FILTER_H__
#define __REORDER_POINT_ID_OF_CONTOUR_FILTER_H__

#include <vtkPolyDataAlgorithm.h>

class ReorderPointIdOfContourFilter : public vtkPolyDataAlgorithm
{
public:
	vtkTypeMacro(ReorderPointIdOfContourFilter, vtkPolyDataAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent);

	static ReorderPointIdOfContourFilter *New();

protected:
	ReorderPointIdOfContourFilter();
	~ReorderPointIdOfContourFilter();

	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
	ReorderPointIdOfContourFilter(const ReorderPointIdOfContourFilter&);  // Not implemented.
	void operator=(const ReorderPointIdOfContourFilter&);  // Not implemented.

};

#endif // !__REORDER_POINT_ID_OF_CONTOUR_FILTER_H__

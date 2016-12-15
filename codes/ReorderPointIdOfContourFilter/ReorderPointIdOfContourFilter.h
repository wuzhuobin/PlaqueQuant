#ifndef __REORDER_POINT_ID_OF_CONTOUR_FILTER_H__
#define __REORDER_POINT_ID_OF_CONTOUR_FILTER_H__

#include <vtkPolyDataAlgorithm.h>

class ReorderPointIdOfContourFilter : public vtkPolyDataAlgorithm
{
public:
	vtkTypeMacro(ReorderPointIdOfContourFilter, vtkPolyDataAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent);

	static ReorderPointIdOfContourFilter *New();
	/**
	 * Get/Set m_outputCellType, but not just support VTK_POLY_LINE AND VTK_POLYGON
	 * @param	m_outputCellType
	 * @return	m_outputCellType
	 */
	vtkGetMacro(OutputCellType, int);
	vtkSetMacro(OutputCellType, int);

protected:
	ReorderPointIdOfContourFilter();
	~ReorderPointIdOfContourFilter();

	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
	ReorderPointIdOfContourFilter(const ReorderPointIdOfContourFilter&);  // Not implemented.
	void operator=(const ReorderPointIdOfContourFilter&);  // Not implemented.


	int OutputCellType = VTK_POLY_LINE;

};

#endif // !__REORDER_POINT_ID_OF_CONTOUR_FILTER_H__

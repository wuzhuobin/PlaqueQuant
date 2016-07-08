#ifndef EXTRACTCENTERLINE_H
#define EXTRACTCENTERLINE_H

#include <vtkPolyData.h>
#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkScalarBarActor.h>

#include <vtkvmtkCapPolyData.h>
#include <vtkvmtkPolyDataCenterlines.h>

using namespace std;

class ExtractCenterline
{
public:
	ExtractCenterline();
	~ExtractCenterline();

	void SetSurface(vtkPolyData* surface);
	void SetLookupTable(vtkLookupTable* LUT);
	void Update();
	void GetSurface(vtkPolyData* surface);
	void GetCappedSurface(vtkPolyData* cappedSurface);
	void GetCenterline(vtkPolyData* centerline);
	void GetLookupTable(vtkLookupTable* LUT);

private:
	vtkSmartPointer<vtkPolyData>			m_surface;
	vtkSmartPointer<vtkPolyData>			m_centerline;
	vtkSmartPointer<vtkPolyData>			m_cappedSurface;
	vtkSmartPointer<vtkLookupTable>			m_LUT;

	void CapSurface(vtkPolyData* inputSurface, vtkPolyData* cappedSurface, vtkIdList* CapCenterIds);
	void CalculateCenterline(vtkPolyData* cappedSurface, vtkIdList* CapCenterIds, vtkPolyData* centerline);
	void CreateLookupTable();
};

#endif

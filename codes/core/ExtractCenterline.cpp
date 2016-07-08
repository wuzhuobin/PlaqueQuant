#include "ExtractCenterline.h"

ExtractCenterline::ExtractCenterline()
{
	m_surface = NULL;
	m_centerline = vtkSmartPointer<vtkPolyData>::New();
	m_cappedSurface = vtkSmartPointer<vtkPolyData>::New();
	m_LUT = vtkSmartPointer<vtkLookupTable>::New();
}

ExtractCenterline::~ExtractCenterline()
{

}

void ExtractCenterline::SetSurface(vtkPolyData* surface)
{
	m_surface = surface;
}

void ExtractCenterline::Update()
{
	vtkSmartPointer<vtkIdList> capIds = vtkSmartPointer<vtkIdList>::New();
	this->CapSurface(m_surface, m_cappedSurface, capIds);
	this->CalculateCenterline(m_cappedSurface, capIds, m_centerline);
}

void ExtractCenterline::CapSurface(vtkPolyData* inputSurface, vtkPolyData* cappedSurface, vtkIdList* CapCenterIds)
{
	// calculate centerlines of lumen and vessl outer wall
	vtkSmartPointer<vtkvmtkCapPolyData> capper = vtkSmartPointer<vtkvmtkCapPolyData>::New();
	capper->SetInputData(inputSurface);
	capper->Update();
	cappedSurface->DeepCopy(capper->GetOutput());
	CapCenterIds->DeepCopy(capper->GetCapCenterIds());

	// create lookup table and scalar bar
	this->CreateLookupTable();
}

void ExtractCenterline::CalculateCenterline(vtkPolyData* cappedSurface, vtkIdList* CapCenterIds, vtkPolyData* centerline)
{
	vtkSmartPointer<vtkIdList> sourceIds = vtkSmartPointer<vtkIdList>::New();
	sourceIds->SetNumberOfIds(1);
	sourceIds->SetId(0, CapCenterIds->GetId(0));

	vtkSmartPointer<vtkIdList> targetIds = vtkSmartPointer<vtkIdList>::New();
	targetIds->SetNumberOfIds(CapCenterIds->GetNumberOfIds() - 1);
	for (int i = 1; i < CapCenterIds->GetNumberOfIds(); i++)
	{
		targetIds->SetId(i - 1, CapCenterIds->GetId(i));
	}

	vtkSmartPointer<vtkvmtkPolyDataCenterlines> centerlinesFilter = vtkSmartPointer<vtkvmtkPolyDataCenterlines>::New();
	centerlinesFilter->SetInputData(cappedSurface);
	centerlinesFilter->SetSourceSeedIds(sourceIds);
	centerlinesFilter->SetTargetSeedIds(targetIds);
	centerlinesFilter->SetAppendEndPointsToCenterlines(1);
	centerlinesFilter->SetRadiusArrayName("Radius");
	centerlinesFilter->Update();

	centerline->DeepCopy(centerlinesFilter->GetOutput());
	centerline->GetPointData()->SetScalars(centerline->GetPointData()->GetArray(0));
}

void ExtractCenterline::GetSurface(vtkPolyData* surface)
{
	surface = m_surface;
}

void ExtractCenterline::GetCappedSurface(vtkPolyData* cappedSurface)
{
	cappedSurface->DeepCopy(m_cappedSurface);
}

void ExtractCenterline::GetCenterline(vtkPolyData* centerline)
{
	centerline->DeepCopy(m_centerline);
}

void ExtractCenterline::CreateLookupTable()
{
	m_LUT->SetTableRange(m_centerline->GetScalarRange());
	m_LUT->Build();
	m_LUT->SetTableValue(0, 1.0, 0.0, 0.0, 1);  //Red
	m_LUT->SetTableValue(1, 0.0, 1.0, 0.0, 1); // Green
	m_LUT->SetTableValue(2, 0.0, 0.0, 1.0, 1); // Blue
}

void ExtractCenterline::SetLookupTable(vtkLookupTable* LUT)
{
	m_LUT = LUT;
}

void ExtractCenterline::GetLookupTable(vtkLookupTable* LUT)
{
	LUT->DeepCopy(m_LUT);
}
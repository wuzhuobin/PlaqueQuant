#include <vtkImageAccumulate.h>
#include "MeasurementFor2D.h"
#include "MainWindow.h"

vtkStandardNewMacro(MeasurementFor2D);

MeasurementFor2D::Measurements2D MeasurementFor2D::GetOutput(int i)
{
	if (this->m_outputList.size() - 1 < i || i < 0) {
		throw ERROR_OUTPUT_NOT_EXIST;
	}
	return this->m_outputList.at(i);
}

MeasurementFor2D::MeasurementFor2D()
{
	this->SliceImage = NULL;
}

MeasurementFor2D::~MeasurementFor2D()
{
}


void MeasurementFor2D::SetSliceImage(vtkImageData *im)
{
	if (this->SliceImage != NULL)
		this->SliceImage->Delete();

	this->SliceImage = vtkSmartPointer<vtkImageData>::New();
	this->SliceImage->DeepCopy(im);

	this->SliceImage->GetExtent(this->ImageExtent);
	if (this->ImageExtent[4] != this->ImageExtent[5]) {
		this->SliceImage->Delete();
		this->SliceImage = NULL;
		throw ERROR_INPUT_NOT_A_SLICE;
	}
}

void MeasurementFor2D::Update()
{
	this->CountLabels();
}


void MeasurementFor2D::CountLabels()
{
	vtkSmartPointer<vtkImageAccumulate> imageAccumulate = vtkSmartPointer<vtkImageAccumulate>::New();
	imageAccumulate->SetInputData(this->SliceImage);
	imageAccumulate->SetComponentExtent(0, 6.,0,0,0,0); // #LookupTable
	imageAccumulate->SetComponentOrigin(0, 0, 0);
	imageAccumulate->SetComponentSpacing(1, 0, 0);
	imageAccumulate->Update();

	double spacing[3], voxelVolume;
	this->SliceImage->GetSpacing(spacing);
	voxelVolume = spacing[0] * spacing[1] ;
	Measurements2D output;


	int *val = static_cast<int*>(imageAccumulate->GetOutput()->GetScalarPointer(MainWindow::LABEL_LUMEN, 0, 0));
	output.LumenArea = (*val * voxelVolume);

	val = static_cast<int*>(imageAccumulate->GetOutput()->GetScalarPointer(MainWindow::LABEL_VESSEL_WALL, 0, 0));
	output.VesselWallArea = (*val * voxelVolume);

	this->m_outputList.push_back(output);
}

int MeasurementFor2D::CountCluster(int x, int y)
{
	int count = 0;
	double* val = static_cast<double*>(this->SliceImage->GetScalarPointer(x, y, this->SliceNumber));
	if (val == nullptr)
		return count;

	if (*val == 0)
		return count;

	for (int i = -1; i < 2;i += 2)
	{
		for (int j = -1; j < 2; j += 2)
		{
			std::pair<int, int> index(x + i, y + j);

			if (x + i < this->ImageExtent[0] || x + i > this->ImageExtent[1] ||
				y + j < this->ImageExtent[2] || y + j > this->ImageExtent[3]) 
			{
				continue;
			}
			if (this->CheckCoordIndexInList(index)) {
				return count;
			}
			else {
				if (*val == this->SliceImage->GetScalarComponentAsDouble(x, y, this->SliceNumber, 0)) {
					this->m_countedList.push_back(index);
					count += this->CountCluster(x + i, y + j);
				};
			}
		}
	}
	return count;
}

/* Return true if the coordinate was visited */
bool MeasurementFor2D::CheckCoordIndexInList(CoordIndex index)
{
	for each (CoordIndex var in this->m_countedList)
	{
		if (var.first == index.first && var.second == index.second)
			return true;
	}

	return false;
}

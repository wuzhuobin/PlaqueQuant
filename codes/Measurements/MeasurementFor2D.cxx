#include <vtkSmartPointer.h>
#include "MeasurementFor2D.h"

void MeasurementFor2D::SetSliceImage(vtkImageData *im)
{
	if (this->SliceImage != NULL)
		this->SliceImage->Delete();

	this->SliceImage = vtkSmartPointer<vtkImageData>::New();
	this->SliceImage->DeepCopy(im);

	this->SliceImage->GetExtent(this->ImageExtent);
}

MeasurementFor2D::MeasurementFor2D()
{

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
				return;
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

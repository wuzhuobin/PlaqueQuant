/*
Author:		Wong, Matthew Lun
Date:		12, Nov, 2017
			Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

FileName: 	ContourInterpolateFilter.cpp

Descriptions:
	This class interpolates a set of contour at the specified locations. This
	class assume all the contours nodes of each slice has the same z-axis coordinate.
	Also, each contour should correspond to one vtk cell only.


Wong Matthew Lun
Copyright (C) 2017
*/

#include "ContourInterpolateFilter.h"
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkSplineFilter.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkLine.h>
#include <vtkKdTree.h>
#include <vtkIntArray.h>
#include <vtkIdList.h>
#include <vtkCenterOfMass.h>
#include <vtkCellData.h>
#include <vtkCell.h>
#include <vtkCardinalSpline.h>
#include <vtkAppendPolyData.h>
#include <QSet>
#include <QDebug>


ContourInterpolateFilter::ContourInterpolateFilter()
{
	this->m_output = vtkPolyData::New();
	this->m_modified = false;
}

ContourInterpolateFilter::~ContourInterpolateFilter()
{
	while (!this->m_outputContours.isEmpty())
	{
		this->m_outputContours.take(this->m_outputContours.lastKey())->Delete();
	}

	this->m_output->Delete();
}

int ContourInterpolateFilter::Update()
{
	try {
		/* Clean output list if exist */
		while (!this->m_outputContours.isEmpty())
		{
			this->m_outputContours.take(this->m_outputContours.lastKey())->Delete();
		}


		if (this->m_modified)
		{
			/* Obtain list of coordinate that needs interpolation, if not provide,
			   use min slice index and max slice indes as interpolation range */
			if (this->m_interpolateCoordinates.isEmpty())
			{
				std::list<unsigned int> keys = this->m_inputContours.keys().toStdList();
				const unsigned int min = *std::min_element(keys.cbegin(), keys.cend());
				const unsigned int max = *std::max_element(keys.cbegin(), keys.cend());
				for (int i = min; i <= max; i++)
				{
					this->m_interpolateCoordinates.push_back(i);
				}
			}

			/* Remove duplication slices */
			this->m_interpolateCoordinates = QList<unsigned int>::fromSet(QSet<unsigned int>::fromList(this->m_interpolateCoordinates));
			std::sort(this->m_interpolateCoordinates.begin(), this->m_interpolateCoordinates.end());

			for (QList<unsigned int>::const_iterator cit = this->m_interpolateCoordinates.cbegin();
				cit != this->m_interpolateCoordinates.cend(); cit++)
			{
				/* Copy from input if it exist */
				if ((this->m_inputContours.keys().contains(*cit)))
				{
					vtkPolyData* temp = vtkPolyData::New();
					temp->DeepCopy(this->m_inputContours[*cit]);

					/* cell properties */
					vtkSmartPointer<vtkIntArray> slicearray = vtkSmartPointer<vtkIntArray>::New();
					slicearray->SetName("SliceIndex");
					slicearray->InsertNextValue(*cit);

					vtkSmartPointer<vtkIntArray> interpolatedarray = vtkSmartPointer<vtkIntArray>::New();
					interpolatedarray->SetName("Interpolated");
					interpolatedarray->InsertNextValue(0);

					temp->GetCellData()->AddArray(slicearray);
					temp->GetCellData()->AddArray(interpolatedarray);
					this->m_outputContours[*cit] = temp;
				}
				else
				{
					/* Locate closest two input slices */
					unsigned int a = VTK_UNSIGNED_INT_MIN;
					unsigned int b = VTK_UNSIGNED_INT_MAX;
					QList<unsigned int> keys = this->m_inputContours.keys();
					for (QList<unsigned int>::const_iterator kit = keys.cbegin();
						kit != keys.cend(); kit++)
					{
						if (*kit < b && *kit > *cit)
						{
							b = *kit;
						}

						if (*kit > a && *kit < *cit)
						{
							a = *kit;
						}

					}
					this->m_outputContours[*cit] = this->InterpolateSlice(a, b, *cit);
				}
			}
			this->GetOutput();
			this->m_modified = false;
		}
	}
	catch (...)
	{
		return 1;
	}
	return 0;
}

void ContourInterpolateFilter::AddSliceContour(vtkPolyData* pd, unsigned int i)
{
	this->m_inputContours[i] = pd;
	this->Modified();
}

void ContourInterpolateFilter::SetInput(QMap<unsigned int, vtkPolyData*> in)
{
	this->m_inputContours = in;
	this->Modified();
}

void ContourInterpolateFilter::SetInterpolateSlices(QList<unsigned int> inlist)
{
	this->m_interpolateCoordinates = inlist;
}

vtkPolyData* ContourInterpolateFilter::GetSliceOutput(unsigned int i)
{
	if (this->m_outputContours.keys().indexOf(i) == -1)
	{
		qCritical() << "Requesting slice is not interpolated!";
		return nullptr;
	}
	else
	{
		return this->m_outputContours[i];
	}
}

vtkPolyData* ContourInterpolateFilter::GetOutput()
{
	/// Error check
	if (this->m_outputContours.isEmpty())
	{
		qCritical() << "No available output. Did you call Update()?";
		return nullptr;
	}


	if (this->m_modified)
	{
		/* Build output array */
		vtkSmartPointer<vtkAppendPolyData> addpdfilter = vtkSmartPointer<vtkAppendPolyData>::New();
		for (QMap<unsigned int, vtkPolyData*>::const_iterator cit = this->m_outputContours.cbegin();
			cit != this->m_outputContours.cend(); cit++)
		{
			/* The slice index array specify the number of slice of the cell */
			if ((*cit)->GetCellData()->HasArray("SliceIndex"))
			{
				vtkIntArray* slicearray = vtkIntArray::SafeDownCast((*cit)->GetCellData()->GetArray("SliceIndex"));
				slicearray->SetValue(0, this->m_outputContours.key(*cit));
			}
			else
			{
				vtkSmartPointer<vtkIntArray> slicearray = vtkSmartPointer<vtkIntArray>::New();
				slicearray->SetName("SliceIndex");
				slicearray->InsertNextValue(this->m_outputContours.key(*cit));
				(*cit)->GetCellData()->AddArray(slicearray);
			}

			/* The Interpolated array specified if this layer is interpolated */
			if ((*cit)->GetCellData()->HasArray("Interpolated"))
			{
				vtkIntArray* interpolatedarray = vtkIntArray::SafeDownCast((*cit)->GetCellData()->GetArray("Interpolated"));
				if ((this->m_inputContours.keys().contains(this->m_outputContours.key(*cit))))
					interpolatedarray->SetValue(0, 0);
				else {
					interpolatedarray->SetValue(0, 1);
				}
			}
			else
			{
				vtkSmartPointer<vtkIntArray> interpolatedarray = vtkSmartPointer<vtkIntArray>::New();
				interpolatedarray->SetName("Interpolated");
				if ((this->m_inputContours.keys().contains(this->m_outputContours.key(*cit))))
					interpolatedarray->InsertNextValue(0);
				else
					interpolatedarray->InsertNextValue(1);
				(*cit)->GetCellData()->AddArray(interpolatedarray);
			}

			addpdfilter->AddInputData(*cit);
		}
		addpdfilter->Update();
		this->m_output->DeepCopy(addpdfilter->GetOutput());
		return this->m_output;
	}
	else
	{
		return this->m_output;
	}
}

void ContourInterpolateFilter::Modified()
{
	this->m_modified = true;
}

double ContourInterpolateFilter::MapIndexToZCoordinate(unsigned int index)
{
	/* Check input polydata has at least tswo slice */
	if (this->m_inputContours.size() < 2) {
		qCritical() << "One slice is not enough for interpolation";
		return VTK_DOUBLE_MIN;
	};


	/* Calculate slice spacing*/
	int key0 = this->m_inputContours.keys()[0];
	int key1 = this->m_inputContours.keys()[1];
	double z0 = this->m_inputContours[key0]->GetPoint(0)[2];
	double z1 = this->m_inputContours[key1]->GetPoint(1)[2];
	double spacing = abs(z1 - z0) / float(abs(key1 - key0));

	/* Map index to coordinate */
	return z0 + (index - key0) * spacing;
}

vtkPolyData* ContourInterpolateFilter::InterpolateSlice(unsigned int s0, unsigned int s1, unsigned int o)
{
	/// Error check
	/* Assume slice 0 has a smaller slice index */
	if (!(s0 < s1))
	{
		qCritical() << "Input s0 must be smaller than s1.";
		return nullptr;
	};


	/* o must be between the two specified slices. */
	if (!(o > s0 && o < s1))
	{
		qCritical() << "Required interpolation slice o must be between s0 and s1!";
		return nullptr;
	};

	if (!this->m_inputContours.contains(s0)) {
		qCritical() << "m_inputContours does not contain " << s0;
		return nullptr;
	}

	if (!this->m_inputContours.contains(s1)) {
		qCritical() << "m_inputContours does not contain " << s1;
		return nullptr;
	}

	vtkPolyData* source0 = this->m_inputContours[s0];
	vtkPolyData* source1 = this->m_inputContours[s1];

	this->RectifyLoopDirection(source0);
	this->RectifyLoopDirection(source1);


	/* Calculate interpolation factor */
	double ss0 = this->MapIndexToZCoordinate(s0);
	double ss1 = this->MapIndexToZCoordinate(s1);
	double oo = this->MapIndexToZCoordinate(o);
	double factor = (oo - ss0) / (ss1 - ss0);


	/* Resample the source with same number of points */
	int b = 0;
	if (source0->GetNumberOfPoints() < source1->GetNumberOfPoints())
	{
		b = 1;
	}
	qDebug() << o << ": b=" << b << " " << source0->GetNumberOfPoints() << " " << source1->GetNumberOfPoints();
	vtkSmartPointer<vtkPolyData> temp = vtkSmartPointer<vtkPolyData>::New();
	//this->ResamplePolyLine(source0, temp, 50);
	//this->ResamplePolyLine(source1, source1, 50);

	if (b)
	{
		/* temp is now new source0 */
		this->ResamplePolyLine(source0, temp, source1->GetNumberOfPoints() - 1);
	}
	else
	{
		/* temp is now new source1 */
		this->ResamplePolyLine(source1, temp, source0->GetNumberOfPoints() - 1);
	}



	/* Pair up points, assume inputs are ordered, need to align center of mass before anything */
	typedef QMap<unsigned int, unsigned int> PointPairType;
	QVector<PointPairType> pointpairs;
	vtkSmartPointer<vtkCenterOfMass> com = vtkSmartPointer<vtkCenterOfMass>::New();
	double com1[3], com2[3], diff[3];
	com->SetInputData(temp);
	com->SetUseScalarsAsWeights(false);
	com->Update();
	com->GetCenter(com1);
	com->RemoveAllInputs();
	if (b)
	{
		com->SetInputData(source1);
		com->Update();
		com->GetCenter(com2);
	}
	else
	{
		com->SetInputData(source0);
		com->Update();
		com->GetCenter(com2);
	}
	vtkMath::Subtract(com2, com1, diff);

	if (b)
	{
		/* temp is source0 */
		double dist;
		for (int i = 0; i < source1->GetNumberOfPoints();i++)
		{
			PointPairType p;
			double v[3];
			vtkMath::Add(diff, v, v);
			vtkMath::Subtract(source1->GetPoint(i), v, v);


			p[1] = i;
			if (i == 0)
			{
				p[0] = this->ClosestPointAlongLine(temp, source1->GetPoint(i), v);

			}
			else
			{
				p[0] = (pointpairs.last()[0] + 1) % (temp->GetNumberOfPoints());

			}
			pointpairs.push_back(p);
		}
	}
	else
	{
		/* temp is source1 */
		double dist;
		for (int i = 0; i < source0->GetNumberOfPoints(); i++)
		{
			PointPairType p;
			double v[3];
			vtkMath::Subtract(source0->GetPoint(i), diff, v);
			p[0] = i;
			if (i == 0)
			{
				p[1] = this->ClosestPointAlongLine(temp, source0->GetPoint(i), v);

			}
			else
			{
				p[1] = (pointpairs.last()[1] + 1) % (temp->GetNumberOfPoints());
			}

			pointpairs.push_back(p);
		}
	}

	/* Linear interpolation */
	vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
	if (b)
	{
		/* The vector should be sorted along second element */
		for (QVector<PointPairType>::const_iterator cit = pointpairs.cbegin();
			cit != pointpairs.cend(); cit++)
		{
			double newpoint[3];
			vtkMath::Subtract(source1->GetPoint((*cit)[1]), temp->GetPoint((*cit)[0]), newpoint);
			vtkMath::MultiplyScalar(newpoint, factor);
			vtkMath::Add(temp->GetPoint((*cit)[0]), newpoint, newpoint);
			pts->InsertNextPoint(newpoint); /* Need to check if this will auto delete! */
		}
	}
	else
	{
		/* The vector should be sorted along first element */
		for (QVector<PointPairType>::const_iterator cit = pointpairs.cbegin();
			cit != pointpairs.cend(); cit++)
		{
			double newpoint[3];
			vtkMath::Subtract(temp->GetPoint((*cit)[1]), source0->GetPoint((*cit)[0]), newpoint);
			vtkMath::MultiplyScalar(newpoint, factor);
			vtkMath::Add(source0->GetPoint((*cit)[0]), newpoint, newpoint);
			pts->InsertNextPoint(newpoint); /* Need to check if this will auto delete! */
		}
	}

	/* cell properties */
	vtkSmartPointer<vtkIntArray> slicearray = vtkSmartPointer<vtkIntArray>::New();
	slicearray->SetName("SliceIndex");
	slicearray->InsertNextValue(o);

	vtkSmartPointer<vtkIntArray> interpolatedarray= vtkSmartPointer<vtkIntArray>::New();
	interpolatedarray->SetName("Interpolated");
	interpolatedarray->InsertNextValue(1);

	/* Build Cells */
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
	for (int i = 0; i < pts->GetNumberOfPoints(); i++)
	{
		line->GetPointIds()->InsertNextId(i);
	}
	line->GetPointIds()->InsertNextId(0);
	lines->InsertNextCell(line);

	vtkPolyData* out = vtkPolyData::New();
	out->SetPoints(pts);
	out->SetLines(lines);
	out->GetCellData()->AddArray(slicearray);
	out->GetCellData()->AddArray(interpolatedarray);
	return out;
}

void ContourInterpolateFilter::ResamplePolyLine(vtkPolyData* in, vtkPolyData* out, int subdivision)
{
	vtkSmartPointer<vtkCardinalSpline> spline =
		vtkSmartPointer<vtkCardinalSpline>::New();
	spline->SetLeftConstraint(2);
	spline->SetLeftValue(0.0);
	spline->SetRightConstraint(2);
	spline->SetRightValue(0.0);

	vtkSmartPointer<vtkSplineFilter> splineFilter =
		vtkSmartPointer<vtkSplineFilter>::New();
	splineFilter->SetInputData(in);
	splineFilter->SetNumberOfSubdivisions(subdivision);
	splineFilter->SetSpline(spline);
	splineFilter->Update();
	out->DeepCopy(splineFilter->GetOutput());
}

bool ContourInterpolateFilter::CheckLoopDirection(vtkPolyData* loop)
{
	vtkSmartPointer<vtkCenterOfMass> comfilter = vtkSmartPointer<vtkCenterOfMass>::New();
	comfilter->SetInputData(loop);
	comfilter->Update();

	double* com = comfilter->GetCenter();

	double reference[3] = { 0, 0, 1 };
	double cp[3] = { 0, 0, 0 };
	for (int i = 0; i < loop->GetNumberOfPoints(); i++)
	{
		double v1[3], v2[3], l_cp[3];
		vtkMath::Subtract(loop->GetPoint(i), com, v1);
		vtkMath::Subtract(loop->GetPoint((i + 1) % loop->GetNumberOfPoints()), com, v2);
		vtkMath::Cross(v2, v1, l_cp);
		vtkMath::Add(l_cp, cp, cp);
	}

	return (vtkMath::Dot(reference, cp) > 0);
}

void ContourInterpolateFilter::RectifyLoopDirection(vtkPolyData* loop)
{
	if (this->CheckLoopDirection(loop))
	{
		return;
	}
	else
	{
		vtkSmartPointer<vtkPoints> newpts = vtkSmartPointer<vtkPoints>::New();
		vtkPoints* oldpts = loop->GetPoints();

		for (int i = oldpts->GetNumberOfPoints() - 1; i >= 0; i--)
		{
			newpts->InsertNextPoint(oldpts->GetPoint(i));
		}

		loop->SetPoints(newpts);
	}
}

vtkIdType ContourInterpolateFilter::ClosestPointAlongLine(vtkPolyData* target, double* pt0, double* pt1)
{
	double dist = VTK_DOUBLE_MAX;
	vtkIdType out = -1;
	for (int i = 0; i < target->GetNumberOfPoints(); i++)
	{
		double d = vtkLine::DistanceToLine(target->GetPoint(i), pt0, pt1);
		if (d < dist)
		{
			out = i;
			dist = d;
		}
	}
	return out;
}

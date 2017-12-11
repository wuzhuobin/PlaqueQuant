/*
Author:		Wong, Matthew Lun
Date:		12, Nov, 2017
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

FileName: 	ContourInterpolateFilter.h

Descriptions:
	This class interpolates a set of contour at the specified locations. This
	class assume all the contours nodes of each slice has the same z-axis coordinate.
	Also, each contour should correspond to one vtk cell only.


Wong Matthew Lun
Copyright (C) 2017
*/

#include <QMap>
#include <vtkPolyData.h>

class ContourInterpolateFilter
{
public:
	ContourInterpolateFilter();
	~ContourInterpolateFilter();

	/**
	 * int Update
	 *
	 * @brief
	 *
	 * Update the output.
	 *
	 * @return int Return 0 for success
	 */
	int Update();

	/**
	 * void AddSliceContour
	 *
	 * @brief
	 *
	 * Add a contour of a specify slice number. Note that adding duplicated
	 * slice index will overwrite the old one.
	 *
	 * @param vtkPolyData *
	 * @param unsigned int
	 * @return void
	 */
	void AddSliceContour(vtkPolyData*, unsigned int);


	/**
	 * void SetInput
	 *
	 * @brief
	 *
	 * Directly set all input at once. Bewarned that this will not check for duplicated slice index.
	 *
	 * @param QMap<unsigned int, vtkPolyData * >
	 * @return void
	 */
	void SetInput(QMap<unsigned int, vtkPolyData*>);


	/**
	 * void SetInterpolateSlices
	 *
	 * @brief
	 *
	 * Specify which layer to interpolate. If not set, will automatically interpolate all integer
	 * layers from min supplied index to max supplied index.
	 *
	 * @param QList<unsigned int>  A list of integers to interpolate. Should include original slices.
	 * @return void
	 */
	void SetInterpolateSlices(QList<unsigned int>);

	/**
	 * vtkPolyData* GetSliceOutput
	 *
	 * @brief
	 *
	 * Get one slice output. Call after update.
	 *
	 * @param unsigned int   Number of slice.
	 * @return vtkPolyData*
	 */
	vtkPolyData* GetSliceOutput(unsigned int);


	/**
	 * vtkPolyData* GetOutput
	 *
	 * @brief
	 *
	 * Get the output as a whole. The output vtkCellData will have two extra array "SliceIndex" and
	 * "Interpolated" indicated which slice the cell corespond to and whether it is generated from
	 * interpolation or not.
	 *
	 * @return vtkPolyData*  Output polydata. Do not delete it outside of this class or crash.
	 */
	vtkPolyData* GetOutput();

	void Modified();

protected:
	QMap<unsigned int, vtkPolyData*> m_inputContours;
	QMap<unsigned int, vtkPolyData*> m_outputContours;
	QList<unsigned int> m_interpolateCoordinates;
	vtkPolyData* m_output;
	bool m_modified;

private:

	/**
	 * double MapIndexToZCoordinate
	 *
	 * @brief
	 *
	 * Based on input contours, map the slice index to Z-coordinate
	 *
	 * @param unsigned int  Slice index
	 * @return double		Z-coordinate
	 */
	double MapIndexToZCoordinate(unsigned int);

	/**
	 * vtkPolyData* InterpolateSlice
	 *
	 * @brief
	 *
	 * Returns an interpolated slice. Remember to deleted it after it is used!
	 *
	 * @param unsigned int s0 Input slice index 1
	 * @param unsigned int s1 Input slice index 2
	 * @param unsigned int o  Output slice index
	 * @return vtkPolyData*
	 */
	vtkPolyData* InterpolateSlice(unsigned int s0, unsigned int s1, unsigned int o);


	/**
	 * void ResamplePolyLine
	 *
	 * @brief
	 *
	 * Resample the polyline to the given subdivision using cardinal spline
	 *
	 * @param vtkPolyData * in  Input polydata
	 * @param vtkPolyData * out Stores output polydata
	 * @param int subdivision   Number of divisions
	 * @return void
	 */
	void ResamplePolyLine(vtkPolyData* in, vtkPolyData* out, int subdivision);



	/**
	 * bool CheckLoopDirection
	 *
	 * @brief
	 *
	 * Check if loop is clockwise with respect to {0, 0, 1}. If so, return true.
	 *
	 * @param vtkPolyData *
	 * @return bool
	 */
	bool CheckLoopDirection(vtkPolyData*);


	/**
	 * bool CheckLoopDirection
	 *
	 * @brief
	 *
	 * Change ID ordering so that the loop is clockwise with repect to {0, 0, 1}
	 *
	 * @param vtkPolyData *
	 * @return bool
	 */
	void RectifyLoopDirection(vtkPolyData*);

	/**
	 * vtkIdType ClosestPointAlongLine
	 *
	 * @brief
	 *
	 * Iterate through all points in polydata target and search the point with the least
	 * distance to the given line
	 *
	 * @param vtkPolyData * target
	 * @param double * pt0
	 * @param double * pt1
	 * @return vtkIdType
	 */
	vtkIdType ClosestPointAlongLine(vtkPolyData* target, double* pt0, double* pt1);

};
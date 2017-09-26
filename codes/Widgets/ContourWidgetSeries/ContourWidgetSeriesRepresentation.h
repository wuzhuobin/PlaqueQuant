#ifndef __CONTOUR_WIDGET_SERIES_REPRESENTATION_H__
#define __CONTOUR_WIDGET_SERIES_REPRESENTATION_H__

#include <vtkWidgetRepresentation.h>


class vtkContourRepList;
class vtkContourRepresentation;
class vtkPolyData;
class vtkContourList;
class vtkIncrementalOctreePointLocator;


class ContourWidgetSeriesRepresentation: public vtkWidgetRepresentation
{
public:
	// Description:
	// Instantiate class.
	static ContourWidgetSeriesRepresentation *New();

	// Description:
	// Standard VTK methods.
	vtkTypeMacro(ContourWidgetSeriesRepresentation, vtkWidgetRepresentation);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Methods to Set/Get the coordinates of seed points defining
	// this representation. Note that methods are available for both
	// display and world coordinates. The seeds are accessed by a seed
	// number.

	//virtual void InitializeContours(unsigned int contourNum, vtkPolyData* polyData);
	//virtual void GetSeedWorldPosition(unsigned int seedNum, double pos[3]);
	//virtual void SetSeedDisplayPosition(unsigned int seedNum, double pos[3]);
	//virtual void GetSeedDisplayPosition(unsigned int seedNum, double pos[3]);

	// Description:
	// Return the number of seeds (or Contours) that have been created.
	int GetNumberOfContours();

	// Description:
	// This method is used to specify the type of Contour representation to use
	// for the internal vtkContourWidgets within vtkSeedWidget.  To use this
	// method, create a dummy vtkContourWidget (or subclass), and then invoke
	// this method with this dummy. Then the vtkSeedRepresentation uses this
	// dummy to clone vtkContourWidgets of the same type. Make sure you set the
	// Contour representation before the widget is enabled.
	void SetContourRepresentation(vtkContourRepresentation *Contour);

	// Description:
	// Get the Contour representations used for a particular seed. A side effect of
	// this method is that it will create a Contour representation in the list of
	// representations if one has not yet been created.
	virtual vtkContourRepresentation *GetContourRepresentation(unsigned int num);

	// Description:
	// Returns the model ContourRepresentation.
	vtkContourRepresentation *GetContourRepresentation()
	{
		return this->ContourRepresentation;
	};

	// Description:
	// The tolerance representing the distance to the widget (in pixels) in
	// which the cursor is considered near enough to the seed points of
	// the widget to be active.
	vtkSetClampMacro(Tolerance, int, 1, 100);
	vtkGetMacro(Tolerance, int);

	//BTX -- used to communicate about the state of the representation
	enum
	{
		OUTSIDE = 0,
		NEAR_NODE,
		NEAR_CONTOUR
	};
	//ETX

	// Description:
	// These are methods specific to vtkSeedRepresentation and which are
	// invoked from vtkSeedWidget.
	virtual int GetActiveContour();
	// Returns the id of the seed created, -1 on failure. e is the display position.
	virtual int CreateContour(/*double e[2]*/);
	// Delete last Contour created
	virtual void RemoveLastContour();
	// Delete the currently active Contour
	virtual void RemoveActiveContour();

	// Description:
	// Remove the nth Contour.
	virtual void RemoveContour(int n);

	// Description:
	// These are methods that satisfy vtkWidgetRepresentation's API.
	virtual void BuildRepresentation();
	virtual int ComputeInteractionState(int X, int Y, int modify = 0);

	static bool PointOnContour(vtkContourRepresentation* contourRep ,int X, int Y);

protected:
	ContourWidgetSeriesRepresentation();
	virtual ~ContourWidgetSeriesRepresentation();

	// The Contour and the rep used to close the Contours
	vtkContourRepresentation  *ContourRepresentation;
	vtkContourList            *Contours;

	// Selection tolerance for the Contours
	int Tolerance;

	// The active seed (Contour) based on the last ComputeInteractionState()
	int ActiveContour;


	//Description:
	// Adding a point locator to the representation to speed
	// up lookup of the active node when dealing with large datasets (100k+)
	vtkIncrementalOctreePointLocator *Locator;

private:
	ContourWidgetSeriesRepresentation(const ContourWidgetSeriesRepresentation&);  //Not implemented
	void operator=(const ContourWidgetSeriesRepresentation&);  //Not implemented
};



#endif // !__CONTOUR_WIDGET_SERIES_REPRESENTATION_H__

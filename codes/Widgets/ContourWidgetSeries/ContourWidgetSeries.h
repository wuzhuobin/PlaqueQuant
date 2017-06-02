#ifndef __CONTOUR_WIDGET_SERIES_H__
#define __CONTOUR_WIDGET_SERIES_H__

#include <vtkAbstractWidget.h>

class vtkContourRepresentation;
class vtkContourWidget;
class vtkContourList;
class ContourWidgetSeriesRepresentation;


class ContourWidgetSeries : public vtkAbstractWidget
{
public:
	// Description:
	// Instantiate this class.
	static ContourWidgetSeries *New();

	// Description:
	// Standard methods for a VTK class.
	vtkTypeMacro(ContourWidgetSeries, vtkAbstractWidget);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// The method for activating and deactivating this widget. This method
	// must be overridden because it is a composite widget and does more than
	// its superclasses' vtkAbstractWidget::SetEnabled() method.
	virtual void SetEnabled(int enabling);

	// Description:
	// Set the current renderer. This method also propagates to all the child
	// contour widgets, if any exist
	virtual void SetCurrentRenderer(vtkRenderer *);

	// Description:
	// Set the interactor. This method also propagates to all the child
	// contour widgets, if any exist
	virtual void SetInteractor(vtkRenderWindowInteractor * rwi);

	// Description:
	// Specify an instance of vtkWidgetRepresentation used to represent this
	// widget in the scene. Note that the representation is a subclass of vtkProp
	// so it can be added to the renderer independent of the widget.
	void SetRepresentation(ContourWidgetSeriesRepresentation *rep)
	{
		this->Superclass::SetWidgetRepresentation(
			reinterpret_cast<vtkWidgetRepresentation*>(rep));
	}

	// Description:
	// Return the representation as a vtkSeedRepresentation.
	ContourWidgetSeriesRepresentation *GetContourWidgetSeriesRepresentation()
	{
		return reinterpret_cast<ContourWidgetSeriesRepresentation*>(this->WidgetRep);
	}

	// Description:
	// Create the default widget representation if one is not set.
	void CreateDefaultRepresentation();

	// Description:
	// Convenient method to close the contour loop.
	void CloseAllLoops();

	// Description:
	// Methods to change the whether the widget responds to interaction.
	// Overridden to pass the state to component widgets.
	virtual void SetProcessEvents(int);

	//// Description:
	//// Method to be called when the seed widget should stop responding to
	//// the place point interaction. The seed widget, when defined allows you
	//// place seeds by clicking on the render window. Use this method to
	//// indicate that you would like to stop placing seeds interactively. If
	//// you'd like the widget to stop responding to *any* user interaction
	//// simply disable event processing by the widget by calling
	////   widget->ProcessEventsOff()
	//virtual void CompleteInteraction();

	//// Description:
	//// Method to be called when the seed widget should start responding
	//// to the interaction.
	//virtual void RestartInteraction();

	// Description:
	// Use this method to programmatically create a new contour. In interactive
	// mode, (when the widget is in the DEFINE state) this method is
	// automatically invoked. The method returns the contour created.
	// A valid seed representation must exist for the widget to create a new
	// contour.
	virtual vtkContourWidget * CreateNewContourWidget();

	// Description:
	// Delete the nth seed.
	void DeleteContour(int i);

	// Description:
	// Get the nth seed
	vtkContourWidget * GetContour(int i);

	// Description:
	// Get the widget state.
	vtkSetClampMacro(WidgetState, int, 0, 2);
	vtkGetMacro(WidgetState, int);

	// The state of the widget
	//BTX
	enum
	{
		///< the beginnning before doing any interaction
		START = 0,
		///< defining the current contour
		DEFINE = 1,
		///< after adding a final node of a contour, 
		MANIPULATE = 2
	};
	//ETX
	// Description:
	// Set / Get the AllowNodePicking value. This ivar indicates whether the nodes
	// and points between nodes can be picked/un-picked by Ctrl+Click on the node.
	vtkSetMacro(AllowNodePicking, bool);
	vtkGetMacro(AllowNodePicking, bool);
	vtkBooleanMacro(AllowNodePicking, bool);

	// Description:
	// Follow the cursor ? If this is ON, during definition, the last node of the
	// contour will automatically follow the cursor, without waiting for the
	// point to be dropped. This may be useful for some interpolators, such as the
	// live-wire interpolator to see the shape of the contour that will be placed
	// as you move the mouse cursor.
	vtkSetMacro(FollowCursor, bool);
	vtkGetMacro(FollowCursor, bool);
	vtkBooleanMacro(FollowCursor, bool);

	// Description:
	// Define a contour by continuously drawing with the mouse cursor.
	// Press and hold the left mouse button down to continuously draw.
	// Releasing the left mouse button switches into a snap drawing mode.
	// Terminate the contour by pressing the right mouse button.  If you
	// do not want to see the nodes as they are added to the contour, set the
	// opacity to 0 of the representation's property.  If you do not want to
	// see the last active node as it is being added, set the opacity to 0
	// of the representation's active property.
	vtkSetMacro(ContinuousDraw, bool);
	vtkGetMacro(ContinuousDraw, bool);
	vtkBooleanMacro(ContinuousDraw, bool);

	virtual void ResetContours();

protected:
	ContourWidgetSeries();
	~ContourWidgetSeries();


	int WidgetState;

	bool AllowNodePicking = false;
	bool FollowCursor = false;
	bool ContinuousDraw = false;
	// Callback interface to capture events when
	// placing the widget.
	static void AddContourAction(vtkAbstractWidget*);
	static void CompletedAction(vtkAbstractWidget*);
	static void MoveAction(vtkAbstractWidget*);
	static void EndSelectAction(vtkAbstractWidget*);
	static void DeleteAction(vtkAbstractWidget*);
	static void ResetAction(vtkAbstractWidget*);

	// The positioning contour widgets
	vtkContourList *Contours;

	// Manipulating or defining ?
	//int Defining;

private:
	ContourWidgetSeries(const ContourWidgetSeries&);  //Not implemented
	void operator=(const ContourWidgetSeries&);  //Not implemented

};





#endif // !__CONTOUR_WIDGET_SERIES_H__

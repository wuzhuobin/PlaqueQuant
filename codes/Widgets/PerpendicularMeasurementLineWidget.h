#ifndef __PERPENDICULAR_MEASUREMENT_LINE_WIDGET_H__
#define __PERPENDICULAR_MEASUREMENT_LINE_WIDGET_H__

#include <vtkLineWidget2.h>

#include <vtkSmartPointer.h>

class vtkDistanceWidget;
class vtkPolyData;
class vtkKdTreePointLocator;

class PerpendicularMeasurementLineWidget : public vtkLineWidget2
{
public:
	static PerpendicularMeasurementLineWidget* New();
	vtkTypeMacro(PerpendicularMeasurementLineWidget, vtkLineWidget2);
	void PrintSelf(ostream& os, vtkIndent indent);
	// Description:
	// Override superclasses' SetEnabled() method because the line
	// widget must enable its internal handle widgets.
	virtual void SetEnabled(int enabling);
	// Description:
	// Create the default widget representation if one is not set.
	void CreateDefaultRepresentation() override;

	virtual void SetLine(vtkPolyData* line);


	vtkDistanceWidget* GetDistanceWidget();

	// Description:
	// Methods to change the whether the widget responds to interaction.
	// Overridden to pass the state to component widgets.
	virtual void SetProcessEvents(int flag) override;
protected:
	PerpendicularMeasurementLineWidget();
	~PerpendicularMeasurementLineWidget();

	/**
	 * @static 
	 * when finish EndSelectAction, update the #DistanceWidget 
	 */
	static void EndSelectAction(vtkAbstractWidget *w);
	/**
	 * @static
	 * remove the vtkLineRepresentation#OnLine situation, others are the same as 
	 * vtkLineWidget2#MoveAction
	 */
	static void MoveAction(vtkAbstractWidget* w);

	vtkDistanceWidget* DistanceWidget = nullptr;
	vtkSmartPointer<vtkPolyData> TriangulatedLine = nullptr;
	vtkSmartPointer<vtkKdTreePointLocator> PointLocater = nullptr;


};

#endif // !__PERPENDICULAR_MEASUREMENT_LINE_WIDGET_H__
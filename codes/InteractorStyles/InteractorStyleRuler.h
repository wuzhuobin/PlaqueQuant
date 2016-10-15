#ifndef __INTERACTORSTYLERULER_H
#define __INTERACTORSTYLERULER_H

#include "InteractorStyleNavigation.h"

#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation3D.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkStringArray.h>
#include <vtkLabelPlacementMapper.h>



class InteractorStyleRuler :
	public InteractorStyleNavigation
{
public:
	vtkTypeMacro(InteractorStyleRuler, InteractorStyleNavigation);
	static InteractorStyleRuler* New();
	void SetDistanceWidgetEnabled(bool flag);
	void EnableMaximumWallThickneesLabel(bool flag);
	void SetCurrentSlice(int slice);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

protected:
	InteractorStyleRuler();
	~InteractorStyleRuler();

	void UpdateMaximumWallThicknessLabel();

private:
	bool m_distanceWidgetEnabledFlag = false;
	bool m_MaximumWallThickneesLabelFlag = false;
	//vtkPointHandleRepresentation2D* m_pointHandleRepresentation2D = NULL;
	vtkDistanceRepresentation3D* m_distanceRepresentation3D = NULL;
	vtkDistanceWidget* m_distanceWidget = NULL;


	// Maximun wall thickness stuff
	vtkSmartPointer<vtkPolyData>					m_displayPD;
	vtkSmartPointer<vtkActor>						m_lineActor;
	vtkSmartPointer<vtkActor2D>						m_labelActor;
	vtkSmartPointer<vtkStringArray>					m_labelArray;
	vtkSmartPointer<vtkIntArray>					m_sizeArray;
	vtkSmartPointer<vtkPointSetToLabelHierarchy>	m_p2labelfilter;

	vtkSmartPointer<vtkLabelPlacementMapper> m_labelMapper;
	vtkSmartPointer<vtkPolyDataMapper> m_mapper;
};

#endif // !__INTERACTORSTYLERULER_H




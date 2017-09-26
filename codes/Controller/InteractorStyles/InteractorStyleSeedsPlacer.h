/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor updatedOverlay from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef __INTERACTOR_STYLE_SEEDS_PLACER_H__
#define __INTERACTOR_STYLE_SEEDS_PLACER_H__

#include "AbstractNavigation.h"

#include <vtkSmartPointer.h>
#include <vector>

class vtkSeedWidget;

class InteractorStyleSeedsPlacer : public AbstractNavigation
{
public:                                                                                                                                                                             
	vtkTypeMacro(InteractorStyleSeedsPlacer, AbstractNavigation);
	static InteractorStyleSeedsPlacer* New();

	virtual void SetCustomEnabled(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
	virtual void SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k);
	virtual void SetFocalSeed(int i);
	virtual void ClearAllSeedWidget();
	virtual void GenerateWidgetFromSeeds();
	virtual void SaveWidgetToSeeds();
	virtual void DropSeed();
	virtual void UpdateWidgetToSeeds(
		int* newImagePos,
		int* oldImagePos = nullptr);

	virtual void ClearAllSeeds();

protected:
	InteractorStyleSeedsPlacer();
	~InteractorStyleSeedsPlacer();

	void SetFocalSeed(std::vector<int*>& seeds, int i);
	void GenerateWidgetFromSeeds(const std::vector<int*>& seeds);
	void SaveWidgetToSeeds(std::vector<int*>& seeds);
	void DropSeed(std::vector<int*>& seeds);
	void UpdateWidgetToSeeds(
		std::vector<int*>& seeds,
		int* newImagePos,
		int* oldImagePos = nullptr);
	void ClearAllSeeds(std::vector<int*>& seed);


	virtual void OnLeftButtonUp();
	virtual void OnKeyPress();

	/**
	* Using a static vector to save all seeds and it will be shared by all other
	* InteractorStyleSeedsPlacer instances
	*/
	static std::vector<int*> m_seeds;

	friend class SeedsPlacerWidget;
	vtkSmartPointer<vtkSeedWidget> m_seedWidget = nullptr;

};

#endif // !__INTERACTOR_STYLE_SEEDS_PLACER_H__

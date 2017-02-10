/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor modified from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/
#include "InteractorStyleSeedsPlacer.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>


#include <vtkHandleWidget.h>

using namespace std;
vtkStandardNewMacro(InteractorStyleSeedsPlacer);

std::list<int*> InteractorStyleSeedsPlacer::m_seeds[2];
int InteractorStyleSeedsPlacer::m_oldSeedsSize = 0;

void InteractorStyleSeedsPlacer::SetCurrentSeedsType(SeedsType seedsType)
{
	m_currentSeedsType = seedsType;
}

InteractorStyleSeedsPlacer::SeedsType InteractorStyleSeedsPlacer::GetCurrentSeedsType()
{
	return m_currentSeedsType;
}

void InteractorStyleSeedsPlacer::SetSeedsPlacerEnable(bool flag)
{
	if (flag) {
		m_seedRep->GetImagePointPlacer()->SetImageActor(m_imageViewer->GetImageActor());
		// SetInteractor CANNOT place in the constructor
		m_seedWidget->SetInteractor(this->Interactor);
		m_seedWidget->EnabledOn();
		m_seedWidget->On();
		GenerateWidgetFromSeeds();
	}
	else {
		m_seedWidget->EnabledOff();
		m_seedWidget->Off();
		ClearAllSeedWidget();
	}
}

void InteractorStyleSeedsPlacer::SetFocalSeed(int i)
{
	//if (i >= m_seedRep->GetNumberOfSeeds() || i == m_focalSeed) {
	//	return;
	//}
	//m_focalSeed = i;

}

void InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	STYLE_DOWN_CAST_CONSTITERATOR(InteractorStyleSeedsPlacer, GenerateWidgetFromSeeds());
	MY_VIEWER_CONSTITERATOR(Render());
}

InteractorStyleSeedsPlacer::InteractorStyleSeedsPlacer()
	:AbstractNavigation()
{
	m_seedWidget = vtkSmartPointer<SeedsPlacerWidget>::New();
	m_seedWidget->seedsPlacer = this;
	m_seedRep = vtkSmartPointer<SeedsPlacerRepresentation>::New();
	m_seedWidget->SetRepresentation(m_seedRep);
}

InteractorStyleSeedsPlacer::~InteractorStyleSeedsPlacer()
{
	this->m_seedWidget->EnabledOff();
	this->m_seedWidget->SetInteractor(NULL);
	ClearAllSeeds();
}

void InteractorStyleSeedsPlacer::OnMouseMove()
{
	//if (m_oldSeedsSize != m_seedRep->GetNumberOfSeeds() ||
	//	m_oldSeedsSize != m_seeds.size()) {
	//	m_oldSeedsSize = m_seeds.size();
	//	SaveWidgetToSeeds();
	//}

	//if (!m_renderingFlag) {
	//	m_renderingFlag = true;
	//	GenerateWidgetFromSeeds();
	//}

	AbstractNavigation::OnMouseMove();
}

void InteractorStyleSeedsPlacer::OnLeftButtonDown()
{
	//CalculateIndex();
	//cout << m_world[0] << ' ' << m_world[1] << ' ' << m_world[2] << endl;
	//double pos[3];
	//m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(0, pos);
	//cout << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
	//m_imageViewer->Render();

	////Check if valid pick
	//if (m_world[0] == 0 &&
	//	m_world[1] == 0 &&
	//	m_world[2] == 0) {
	//	return;
	//}
	AbstractNavigation::OnLeftButtonDown();

	
}

void InteractorStyleSeedsPlacer::OnLeftButtonUp()
{
	AbstractNavigation::OnLeftButtonUp();
	
	//SaveWidgetToSeeds();
	// update other viewers' seed widgets
	//for (list<AbstractInteractorStyle*>::const_iterator cit =
	//	m_abstractInteractorStyles.cbegin(); cit != m_abstractInteractorStyles.cend(); ++cit) {
	//	InteractorStyleSeedsPlacer* _style = dynamic_cast<InteractorStyleSeedsPlacer*>(*cit);
	//	if (_style != nullptr && _style != this) {
	//		_style->GenerateWidgetFromSeeds();
	//	}
	//}
	this->GenerateWidgetFromSeeds();
}

void InteractorStyleSeedsPlacer::OnRightButtonDown()
{
	AbstractNavigation::OnRightButtonDown();
}


void InteractorStyleSeedsPlacer::OnKeyPress()
{
	string key = this->Interactor->GetKeySym();
	cout << __func__ << ' ' << key << endl;
	if (key == "D") {

	}
	else {
		AbstractNavigation::OnKeyPress();
	}
}


void InteractorStyleSeedsPlacer::OnLeave()
{
	AbstractNavigation::OnLeave();
}

void InteractorStyleSeedsPlacer::CalculateIndex()
{
	//Pick
	this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		m_imageViewer->GetRenderer());

	this->GetInteractor()->GetPicker()->GetPickPosition(m_world);

	if (m_imageViewer->GetInput() != NULL) {
		m_world[GetSliceOrientation()] = GetOrigin()[GetSliceOrientation()] +
			GetSlice() * GetSpacing()[GetSliceOrientation()];
	}

}

void InteractorStyleSeedsPlacer::GenerateWidgetFromSeeds()
{
	if (m_currentSeedsType == ObliqueViewSeeds || m_currentSeedsType == SegmentationSeeds)
	{
		ClearAllSeedWidget();

		for (list<int*>::const_iterator cit = m_seeds[m_currentSeedsType].cbegin();
		cit != m_seeds[m_currentSeedsType].cend(); ++cit) {
			int* imagePos = (*cit);
			double worldPos[3];
			for (int pos = 0; pos < 3; ++pos) {
				worldPos[pos] = (imagePos[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
			}
			if (imagePos[GetSliceOrientation()] == m_imageViewer->GetSlice()) {
				vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
				newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
				newSeed->EnabledOn();
			}

		}
	}
	m_imageViewer->Render();
}

void InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos)
{
	// only used in modify seed widget, replace the old position with the 
	// ImagePos

	list<int*>::iterator it = find_if(InteractorStyleSeedsPlacer::m_seeds[m_currentSeedsType].begin(),
		InteractorStyleSeedsPlacer::m_seeds[m_currentSeedsType].end(), [oldImagePos](int* index)->bool {
		return
			oldImagePos[0] == index[0] &&
			oldImagePos[1] == index[1] &&
			oldImagePos[2] == index[2];
	});
	if (it != InteractorStyleSeedsPlacer::m_seeds[m_currentSeedsType].end()) {
		delete *it;
		it = InteractorStyleSeedsPlacer::m_seeds[m_currentSeedsType].erase(it);
		if (it == m_seeds[m_currentSeedsType].end()) {
			InteractorStyleSeedsPlacer::m_seeds[m_currentSeedsType].push_back(new int[3]);
			memcpy(InteractorStyleSeedsPlacer::m_seeds[m_currentSeedsType].back(), newImagePos, sizeof(int) * 3);
		}
		else {
			InteractorStyleSeedsPlacer::m_seeds[m_currentSeedsType].insert(it, new int[3]);
			memcpy(*it, newImagePos, sizeof(int) * 3);

		}
	}
}

void InteractorStyleSeedsPlacer::SaveWidgetToSeeds()
{
	for (int i = m_seedRep->GetNumberOfSeeds() - 1; i >= 0; --i) {
		//double* worldPos = new double[3]; // #MemLeakHere
		double worldPos[3];
		m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(i, worldPos);
		int* imagePos = new int[3]; // #MemLeakHere
		for (int pos = 0; pos < 3; ++pos) {
			imagePos[pos] = (worldPos[pos] - GetOrigin()[pos]) / GetSpacing()[pos] + 0.5;
		}

		// Check if seeds already exist
		if (find_if(m_seeds[m_currentSeedsType].cbegin(), m_seeds[m_currentSeedsType].cend(), [&imagePos](int* index) -> bool {
			return (index[0] == imagePos[0] && index[1] == imagePos[1] && index[2] == imagePos[2]);
		}) == m_seeds[m_currentSeedsType].cend()) {
			m_seeds[m_currentSeedsType].push_back(imagePos);
			//ModuleWidget::SeedCoordinatesList.push_back(worldPos);
		}
	}

	//MainWindow::GetMainWindow()->GetModuleWidget()->UpdateSeedListView();
}

void InteractorStyleSeedsPlacer::DropSeed()
{
	double* worldPos = m_imageViewer->GetFocalPointWithWorldCoordinate();
	vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
	newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
	newSeed->EnabledOn();
	m_imageViewer->Render();

	SaveWidgetToSeeds();
	OnLeftButtonUp();
}

void InteractorStyleSeedsPlacer::ClearAllSeedWidget()
{
	for (int i = m_seedRep->GetNumberOfSeeds() - 1; i >= 0; --i) {
		m_seedWidget->DeleteSeed(i);
	}
}

void InteractorStyleSeedsPlacer::ClearAllSeeds()
{
	if (m_currentSeedsType != ObliqueViewSeeds && m_currentSeedsType != SegmentationSeeds)
		return;

	if (m_seeds[m_currentSeedsType].size() != 0) {
		while (!m_seeds[m_currentSeedsType].empty())
		{
			delete m_seeds[m_currentSeedsType].back();
			m_seeds[m_currentSeedsType].pop_back();
		}
	}
}

void InteractorStyleSeedsPlacer::OnChar()
{
	char key = this->Interactor->GetKeyCode();
	cout << __func__ << ' ' << key << endl;
	AbstractNavigation::OnChar();
	switch (key)
	{
	case 'P':
		for (int i = 0; i < m_seedRep->GetNumberOfSeeds(); ++i) {
			double pos[3];
			m_seedRep->GetSeedWorldPosition(i, pos);
			cout << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
		}
		break;
	case'D':
		ClearAllSeedWidget();
		break;
	default:
		AbstractNavigation::OnChar();
		break;
	}
}
	
SeedsPlacerWidget::SeedsPlacerWidget()
{
	//this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent,
	//	vtkWidgetEvent::Move,
	//	this, SeedsPlacerWidget::MoveAction);
	//this->CallbackMapper->SetCallbackMethod(vtkCommand::KeyPressEvent,
	//	vtkEvent::NoModifier, 127, 1, "Delete",
	//	vtkWidgetEvent::Delete,
	//	this, SeedsPlacerWidget::DeleteAction);
	//this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent,
	//	vtkWidgetEvent::Completed,
	//	this, SeedsPlacerWidget::CompletedAction);
	vtkSmartPointer<vtkCallbackCommand> callback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	callback->SetCallback(SeedsPlacerWidget::CallbackSave);
	this->AddObserver(vtkCommand::StartInteractionEvent, callback);
	this->AddObserver(vtkCommand::EndInteractionEvent, callback);
	this->AddObserver(vtkCommand::PlacePointEvent, callback);
}

int SeedsPlacerWidget::oldImagePos[3] = {-1};

void SeedsPlacerWidget::CallbackSave(vtkObject * caller, unsigned long eid, void * clientdata, void * calldata)
{
	SeedsPlacerWidget* _widget = static_cast<SeedsPlacerWidget*>(caller);
	vtkSeedRepresentation* seedRep = _widget->GetSeedRepresentation();
	double worldPos[3];
	int seedNum = seedRep->GetActiveHandle();
	int newImagePos[3];
	seedRep->GetSeedWorldPosition(seedNum, worldPos);
	if (eid == vtkCommand::EndInteractionEvent ) {

		for (int pos = 0; pos < 3; ++pos) {
			newImagePos[pos] = (worldPos[pos] - _widget->seedsPlacer->GetOrigin()[pos]) /
				_widget->seedsPlacer->GetSpacing()[pos] + 0.5;
		}
		_widget->seedsPlacer->UpdateWidgetToSeeds(oldImagePos, newImagePos);
		_widget->seedsPlacer->OnLeftButtonUp();
	}
	else if (eid == vtkCommand::PlacePointEvent) {
		_widget->seedsPlacer->SaveWidgetToSeeds();
	}
	else if (eid == vtkCommand::StartInteractionEvent) {
		for (int pos = 0; pos < 3; ++pos) {
			oldImagePos[pos] = (worldPos[pos] - _widget->seedsPlacer->GetOrigin()[pos]) /
				_widget->seedsPlacer->GetSpacing()[pos] + 0.5;
		}
	}
}

///*
//Author:		Wong, Matthew Lun
//Date:		16th, June 2016
//Occupation:	Chinese University of Hong Kong,
//Department of Imaging and Inteventional Radiology,
//Junior Research Assistant
//
//Author:		Lok, Ka Hei Jason
//Date:		16th, June 2016
//Occupation:	Chinese University of Hong Kong,
//Department of Imaging and Inteventional Radiology,
//M.Phil Student
//
//This class allows interactive segmentation on images with contour widget.
//
//Wong Matthew Lun, Lok Ka Hei
//Copyright (C) 2016
//*/
//
//#include <vtkContourWidget.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkPolygon.h>
//#include <vtkMath.h>
//#include <vtkLinearContourLineInterpolator.h>
//#include <vtkBezierContourLineInterpolator.h>
//#include <vtkProperty.h>
//#include <vtkMarchingSquares.h>
//#include <vtkPolyDataConnectivityFilter.h>
//#include <vtkCleanPolyData.h>
//#include <vtkTransform.h>
//#include <vtkTransformPolyDataFilter.h>
//#include <vtkCenterOfMass.h>
//#include <vtkImageThreshold.h>
//#include <vtkCallbackCommand.h>
//#include <vtkCellArray.h>
//#include <vtkParametricFunctionSource.h>
//#include <vtkParametricSpline.h>
//#include <vtkObjectFactory.h>	
//
//#include "ReorderPointIdOfContourFilter.h"
//#include "InteractorStylePolygonDraw.h"
//#include "ImageViewer.h"
//#include "InteractorStyleVesselSegmentation.h"
//#include "LumenSegmentationFilter2.h"
//
//using namespace std;
//vtkStandardNewMacro(InteractorStyleVesselSegmentation);
//
//InteractorStyleVesselSegmentation::InteractorStyleVesselSegmentation()
//	:InteractorStylePolygonDraw()
//{
//}
//
//InteractorStyleVesselSegmentation::~InteractorStyleVesselSegmentation()
//{
//}
//
//void InteractorStyleVesselSegmentation::OnLeftButtonUp()
//{
//	InteractorStylePolygonDraw::OnLeftButtonUp();
//}
//
//void InteractorStyleVesselSegmentation::OnMouseMove()
//{
//	InteractorStylePolygonDraw::OnMouseMove();
//	// save the polydata all the time
//	WriteToPolydata();
//}
//
//void InteractorStyleVesselSegmentation::OnKeyPress()
//{
//	string key = this->Interactor->GetKeySym();
//	if (key == "b") {
//		ReadFromPolydata();
//	}
//	else {
//		InteractorStylePolygonDraw::OnKeyPress();
//	}
//}
//
//void InteractorStyleVesselSegmentation::SetCustomEnabled(bool b)
//{
//	InteractorStylePolygonDraw::SetCustomEnabled(b);
//	if (b) {
//		ReadFromPolydata();
//	}
//}
//
//void InteractorStyleVesselSegmentation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
//{
//	// in case for double re-reading (generate the same contour twice)
//	// which made the contour blink and performance down
//	int ijk[3];
//	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk);
//	if (i == ijk[0] && j == ijk[1] && k == ijk[2]) {
//		return;
//	}
//	AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
//	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStyleVesselSegmentation, ReadFromPolydata());
//	//ReadFromPolydata();
//}
//
//void InteractorStyleVesselSegmentation::NewContour()
//{
//	// if user is manipulate a contour widget, the interactorstyle will not 
//	// generate new contour widget
//	if (m_currentContour != nullptr &&
//		m_currentContour->GetWidgetState() != vtkContourWidget::Manipulate) {
//		return;
//	}
//	switch (m_contourType)
//	{
//	case CONTOUR:
//		InteractorStylePolygonDraw::NewContour();
//		break;
//	case VESSEL_WALL:
//		// close loop the last contour
//		if (m_vesselWallContourWidgets.size() != 0) {
//			m_vesselWallContourWidgets.back()->CloseAllLoops();
//		}
//		m_vesselWallContourWidgets.push_back(vtkSmartPointer<vtkContourWidget>::New());
//		m_currentContour = m_vesselWallContourWidgets.back();
//		m_currentContour->SetRepresentation(
//			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New());
//
//		m_currentContourRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(
//			m_currentContour->GetContourRepresentation());
//		m_currentContourRep->GetLinesProperty()->SetColor(0, 0, 255);
//
//		m_currentContour->SetInteractor(this->Interactor);
//		//m_currentContour->SetCurrentRenderer(m_imageViewer->GetAnnotationRenderer());
//		m_currentContour->ContinuousDrawOn();
//		m_currentContour->FollowCursorOn();
//
//		m_currentContourRep->SetLineInterpolator(m_interpolator);
//		m_currentContourRep->AlwaysOnTopOn();
//
//		break;
//	case LUMEN:
//		// close loop the last contour
//		if (m_lumenWallContourWidgets.size() != 0) {
//			m_lumenWallContourWidgets.back()->CloseAllLoops();
//		}
//		m_lumenWallContourWidgets.push_back(vtkSmartPointer<vtkContourWidget>::New());
//		m_currentContour = m_lumenWallContourWidgets.back();
//		m_currentContour->SetRepresentation(
//			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New());
//
//		m_currentContourRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(
//			m_currentContour->GetContourRepresentation());
//		m_currentContourRep->GetLinesProperty()->SetColor(255, 0, 0);
//
//		m_currentContour->SetInteractor(this->Interactor);
//		//m_currentContour->SetCurrentRenderer(m_imageViewer->GetAnnotationRenderer());
//		m_currentContour->ContinuousDrawOn();
//		m_currentContour->FollowCursorOn();
//
//		m_currentContourRep->SetLineInterpolator(m_interpolator);
//		m_currentContourRep->AlwaysOnTopOn();
//		break;
//	default:
//		break;
//	}
//}
//
//void InteractorStyleVesselSegmentation::NewContour(int type, QList<vtkSmartPointer<vtkPolyData>>* list)
//{
//
//	for (QList<vtkSmartPointer<vtkPolyData>>::const_iterator cit = list->cbegin();
//		cit != list->cend(); ++cit) {
//		// skip strange polydate
//		if ((*cit) != nullptr && (*cit)->GetNumberOfPoints() < 1) {
//			continue;
//		}
//		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> _contourRep =
//			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
//		_contourRep->SetLineInterpolator(m_interpolator);
//		_contourRep->AlwaysOnTopOn();
//
//		vtkSmartPointer<vtkContourWidget> _contour =
//			vtkSmartPointer<vtkContourWidget>::New();
//		switch (type)
//		{
//		case CONTOUR:
//			_contourRep->GetLinesProperty()->SetColor(255, 0, 0);
//			m_contours.push_back(_contour);
//			break;
//		case VESSEL_WALL:
//			_contourRep->GetLinesProperty()->SetColor(0, 0, 255);
//			m_vesselWallContourWidgets.push_back(_contour);
//			break;
//		case LUMEN:
//			_contourRep->GetLinesProperty()->SetColor(255, 0, 0);
//			m_lumenWallContourWidgets.push_back(_contour);
//			break;
//		default:
//			break;
//		}
//
//
//		_contour->SetRepresentation(_contourRep);
//		_contour->SetInteractor(this->Interactor);
//		// Must be set before initialize();
//		_contour->GetContourRepresentation()->
//			SetRenderer(GetImageViewer()->GetRenderer());
//		_contour->ContinuousDrawOn();
//		_contour->FollowCursorOn();
//		_contour->EnabledOn();
//		// EnableOn() first
//		_contour->Initialize((*cit));
//		_contour->CloseAllLoops();
//
//
//	}
//
//}
//
//void InteractorStyleVesselSegmentation::ReadFromPolydata(int type, int slice)
//{
//	if (m_contourType == CONTOUR) {
//		return;
//	}
//	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY)
//		return;
//	Overlay* overlay = m_imageViewer->GetOverlay();
//	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* maps[2] = {
//		overlay->GetVesselWallPolyData(),
//		overlay->GetLumenPolyData()
//	};
//
//	if (maps[type - 1]->contains(slice)) {
//		QList<vtkSmartPointer<vtkPolyData>>* list = maps[type - 1]->value(slice);
//		NewContour(type, list);
//	}
//}
//
//void InteractorStyleVesselSegmentation::ReadFromPolydata()
//{
//	CleanContours(1);
//	CleanContours(2);
//	ReadFromPolydata(1);
//	ReadFromPolydata(2);
//	SetAllContoursEnabled(true);
//	m_currentContour = nullptr;
//	m_imageViewer->Render();
//}
//
//void InteractorStyleVesselSegmentation::ReadFromPolydata(int type)
//{
//	ReadFromPolydata(type, GetSlice());
//}
//
//void InteractorStyleVesselSegmentation::WriteToPolydata()
//{
//	if (m_contourType == CONTOUR) {
//		return;
//	}
//	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY)
//		return;
//
//	Overlay* overlay = m_imageViewer->GetOverlay();
//	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* maps[2] = {
//		overlay->GetVesselWallPolyData(),
//		overlay->GetLumenPolyData()
//	};
//	list<vtkSmartPointer<vtkContourWidget>>* lists[2] = {
//		&m_vesselWallContourWidgets,
//		&m_lumenWallContourWidgets 
//	};
//
//	for (int i = 0; i < 2; ++i) {
//		if (maps[i]->contains(GetSlice())) {
//			delete maps[i]->value(GetSlice());
//		}
//		QList<vtkSmartPointer<vtkPolyData>>* _qlist = 
//			new QList<vtkSmartPointer<vtkPolyData>>;
//		(*maps[i])[GetSlice()] = _qlist;
//		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit = lists[i]->cbegin();
//			cit != lists[i]->cend(); ++cit) {
//			vtkContourRepresentation* _contourRep = (*cit)->GetContourRepresentation();
//
//			// skip the null contour
//			if (_contourRep == nullptr || _contourRep->GetNumberOfNodes() == 0) {
//				continue;
//			}
//
//			vtkSmartPointer<vtkPoints> _points =
//				vtkSmartPointer<vtkPoints>::New();
//
//			vtkSmartPointer<vtkPolygon> _polygon =
//				vtkSmartPointer<vtkPolygon>::New();
//			_polygon->GetPointIds()->SetNumberOfIds(_contourRep->GetNumberOfNodes());
//			for (int j = 0; j < _contourRep->GetNumberOfNodes(); ++j) {
//				double worldCoordinate[3];
//				if (_contourRep->GetNthNodeWorldPosition(j, worldCoordinate)) {
//					_points->InsertNextPoint(worldCoordinate);
//					_polygon->GetPointIds()->SetId(j, j);
//
//				}
//			}
//			vtkSmartPointer<vtkCellArray> _polygons =
//				vtkSmartPointer<vtkCellArray>::New();
//			_polygons->InsertNextCell(_polygon);
//			
//			vtkSmartPointer<vtkPolyData> _nodePolyData =
//				vtkSmartPointer<vtkPolyData>::New();
//			_nodePolyData->SetPoints(_points);
//			_nodePolyData->SetPolys(_polygons);
//			//_contourRep->GetNodePolyData(_nodePolyData);
//
//			*_qlist += _nodePolyData;
//		}
//
//	}
//}
//
//void InteractorStyleVesselSegmentation::SetVesselWallLabel(unsigned char vesselWallLabel)
//{
//	this->m_vesselWallLabel = vesselWallLabel;
//}
//
//void InteractorStyleVesselSegmentation::SetLumenWallLabel(unsigned char lumenWallLabel)
//{
//	this->m_lumenWallLabel = lumenWallLabel;
//}
//
//void InteractorStyleVesselSegmentation::SetGenerateValue(int value)
//{
//	this->m_generateValue = value;
//}
//
//void InteractorStyleVesselSegmentation::SetSegmentationMode(int i)
//{
//	if (i < 0 || i > 2) {
//		return;
//	}
//	m_contourType = i;
//	CleanAllContours();
//
//
//	m_currentContour = nullptr;
//	ReadFromPolydata();
//}
//
//void InteractorStyleVesselSegmentation::EnableNoSegmentation()
//{
//	SetSegmentationMode(0);
//}
//
//void InteractorStyleVesselSegmentation::EnableLumenSegmentation()
//{
//	SetSegmentationMode(1);
//}
//
//void InteractorStyleVesselSegmentation::EnableVesselWallSegmentation()
//{
//	SetSegmentationMode(2);
//}
//
//void InteractorStyleVesselSegmentation::SetContourFilterGenerateValues(int generateValues)
//{
//	this->m_generateValue = generateValues;
//}
//
//void InteractorStyleVesselSegmentation::SetDilateValue(double value)
//{
//	m_dilateValue = value;
//}
//
//void InteractorStyleVesselSegmentation::CleanCurrentContour()
//{
//	list<vtkSmartPointer<vtkContourWidget>>* _contourss[] =
//		{&m_contours, &m_vesselWallContourWidgets, &m_lumenWallContourWidgets};
//
//	if (_contourss[m_contourType]->size() > 0) {
//		_contourss[m_contourType]->back()->EnabledOff();
//		_contourss[m_contourType]->pop_back();
//	}
//
//	// none contours left, just set m_currentContour = nullptr
//	if (m_contours.size() == 0) {
//		m_currentContour = nullptr;
//	}
//	else {
//		m_currentContour = _contourss[m_contourType]->back();
//	}
//	m_imageViewer->Render();
//}
//
//void InteractorStyleVesselSegmentation::CleanAllContours()
//{
//	m_currentContour = nullptr;
//	CleanContours(0);
//	CleanContours(1);
//	CleanContours(2);
//	m_imageViewer->Render();
//
//}
//
//void InteractorStyleVesselSegmentation::CleanContours(int type)
//{
//	switch (type)
//	{
//	case CONTOUR:
//		InteractorStylePolygonDraw::CleanAllContours();
//		break;
//	case VESSEL_WALL:
//		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
//			= m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend();
//			++cit) {
//			(*cit)->EnabledOff();
//		}
//		m_vesselWallContourWidgets.clear();
//		break;
//	case LUMEN:
//		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
//			= m_lumenWallContourWidgets.cbegin(); cit != m_lumenWallContourWidgets.cend();
//			++cit) {
//			(*cit)->EnabledOff();
//
//		}
//		m_lumenWallContourWidgets.clear();
//		break;
//	}
//
//}
//
//void InteractorStyleVesselSegmentation::SetAllContoursEnabled(int flag)
//{
//	SetAllContoursEnabled(CONTOUR, flag);
//	SetAllContoursEnabled(VESSEL_WALL, flag);
//	SetAllContoursEnabled(LUMEN, flag);
//}
//
//void InteractorStyleVesselSegmentation::SetAllContoursEnabled(int type, int flag)
//{
//	switch (type)
//	{
//	case CONTOUR:
//		InteractorStylePolygonDraw::SetAllContoursEnabled(flag);
//		break;
//	case VESSEL_WALL:
//		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
//			= m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend();
//			++cit) {
//			(*cit)->SetEnabled(flag);
//		}
//		break;
//	case LUMEN:
//		for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit
//			= m_lumenWallContourWidgets.cbegin(); cit != m_lumenWallContourWidgets.cend();
//			++cit) {
//			(*cit)->SetEnabled(flag);
//		}
//		break;
//	}
//}
//
//void InteractorStyleVesselSegmentation::GenerateLumenPolydata()
//{
//	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY) {
//		return;
//	}
//	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* lumenMap = 
//		m_imageViewer->GetOverlay()->GetLumenPolyData();
//	if (lumenMap->contains(GetSlice())) {
//		delete  lumenMap->value(GetSlice());
//	}
//	(*lumenMap)[GetSlice()] = new QList<vtkSmartPointer<vtkPolyData>>;
//	for (list<vtkSmartPointer<vtkContourWidget>>::const_iterator cit =
//		m_vesselWallContourWidgets.cbegin(); cit != m_vesselWallContourWidgets.cend(); ++cit) {
//		if ((*cit)->GetContourRepresentation()->GetContourRepresentationAsPolyData()->GetNumberOfPoints() < 3) {
//			continue;
//		}
//		vtkSmartPointer<LumenSegmentationFilter2> lsFilter =
//			vtkSmartPointer<LumenSegmentationFilter2>::New();
//		lsFilter->SetInputData(m_imageViewer->GetInput());
//		lsFilter->SetGenerateValues(1,m_generateValue, m_generateValue);
//		lsFilter->SetVesselWallContourRepresentation(
//			vtkOrientedGlyphContourRepresentation::SafeDownCast((*cit)->GetContourRepresentation())
//		);
//		lsFilter->SetSlice(GetSlice());
//		lsFilter->Update();
//
//		vtkSmartPointer<vtkPolyData> _lumenPolyData =
//			vtkSmartPointer<vtkPolyData>::New();
//		_lumenPolyData->ShallowCopy(lsFilter->GetOutput());
//
//		(*(*lumenMap)[GetSlice()]) += _lumenPolyData;
//	}
//	// Since the lumen polydata was wirten to the ovelay
//	// Reload the polydata from the overlay and genenrate new contour widgets
//	CleanContours(LUMEN);
//	ReadFromPolydata(LUMEN);
//	m_imageViewer->Render();
//}
//
//
//void InteractorStyleVesselSegmentation::GenerateVesselWallPolyData()
//{
//	if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY) {
//		return;
//	}
//	const int* extent = m_imageViewer->GetOverlay()->GetOutput()->GetExtent();
//	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* vesselWallMap =
//		m_imageViewer->GetOverlay()->GetVesselWallPolyData();
//	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* lumenMap =
//		m_imageViewer->GetOverlay()->GetLumenPolyData();
//
//	// since the upper range of the vtkMarchingSquare don't work at all
//	// here it do manually thresholding for remove 
//	vtkSmartPointer<vtkImageThreshold> imageThreshold =
//		vtkSmartPointer<vtkImageThreshold>::New();
//	imageThreshold->ThresholdByLower(1);
//	imageThreshold->SetInputData(m_imageViewer->GetOverlay()->GetOutput());
//	imageThreshold->ReplaceOutOn();
//	imageThreshold->SetOutValue(0);
//	imageThreshold->Update();
//
//	for (int i = extent[4]; i <= extent[5]; ++i) {
//
//		if (lumenMap->contains(i)) {
//			delete  lumenMap->value(i);
//		}
//		if (vesselWallMap->contains(i)) {
//			delete  vesselWallMap->value(i);
//		}
//		(*lumenMap)[i] = new QList<vtkSmartPointer<vtkPolyData>>;
//		(*vesselWallMap)[i] = new QList<vtkSmartPointer<vtkPolyData>>;
//
//
//		vtkSmartPointer<vtkMarchingSquares> marchingSquares =
//			vtkSmartPointer<vtkMarchingSquares>::New();
//		marchingSquares->SetInputConnection(imageThreshold->GetOutputPort());
//		marchingSquares->CreateDefaultLocator();
//		marchingSquares->SetImageRange(extent[0], extent[1], extent[2], extent[3], i, i);
//		marchingSquares->SetValue(1, 1.00);
//		marchingSquares->Update();
//
//		vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
//			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
//		connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
//		connectivityFilter->SetExtractionModeToAllRegions();
//		connectivityFilter->Update();
//
//		for (int j = 0; j < connectivityFilter->GetNumberOfExtractedRegions(); ++j) {
//			// extract lumen contour polydata
//			vtkSmartPointer<vtkPolyDataConnectivityFilter> _connectivityFilter =
//				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
//			_connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
//			_connectivityFilter->AddSpecifiedRegion(j);
//			_connectivityFilter->SetExtractionModeToSpecifiedRegions();
//			_connectivityFilter->Update();
//
//			vtkSmartPointer<vtkCleanPolyData> clean =
//				vtkSmartPointer<vtkCleanPolyData>::New();
//			clean->SetInputConnection(_connectivityFilter->GetOutputPort());
//			clean->PointMergingOn();
//			clean->Update();
//
//			// vtkError handling
//			vtkSmartPointer<vtkCallbackCommand> errorCatch =
//				vtkSmartPointer<vtkCallbackCommand>::New();
//			// lambda function for handling error
//			errorCatch->SetCallback([](vtkObject *caller, unsigned long eid,
//				void *clientdata, void *calldata)->void {
//				// error catch and error display
//				char* ErrorMessage = static_cast<char *>(calldata);
//				vtkOutputWindowDisplayErrorText(ErrorMessage);
//				ReorderPointIdOfContourFilter* reorder =
//					ReorderPointIdOfContourFilter::SafeDownCast(caller);
//				// if error happened, skip this filer
//				reorder->SetOutput(reorder->GetInput());
//			});
//			vtkSmartPointer<ReorderPointIdOfContourFilter> reorder =
//				vtkSmartPointer<ReorderPointIdOfContourFilter>::New();
//			reorder->SetInputConnection(clean->GetOutputPort());
//			reorder->SetOutputCellType(VTK_POLY_LINE);
//			reorder->AddObserver(vtkCommand::ErrorEvent, errorCatch);
//			reorder->Update();
//			if(reorder->GetInput() == reorder->GetOutput()) {
//				string ErrorMessage = "Error in slice " + to_string(i);
//				vtkErrorMacro(<< ErrorMessage);
//			}
//
//			double toleranceInitial = 1;
//			int loopBreaker = 0;
//
//			vtkSmartPointer<vtkCleanPolyData> clearPolyData =
//				vtkSmartPointer<vtkCleanPolyData>::New();
//			clearPolyData->SetInputConnection(reorder->GetOutputPort());
//			clearPolyData->ToleranceIsAbsoluteOn();
//			clearPolyData->SetAbsoluteTolerance(toleranceInitial);
//			clearPolyData->PointMergingOn();
//			clearPolyData->Update();
//			while (clearPolyData->GetOutput()->GetNumberOfPoints() < 3 && loopBreaker < 10) {
//				toleranceInitial *= 0.75;
//				clearPolyData->SetAbsoluteTolerance(toleranceInitial);
//				clearPolyData->Update();
//				loopBreaker += 1;
//			}
//
//			vtkSmartPointer<vtkPolyData> _lumenPolyData =
//				vtkSmartPointer<vtkPolyData>::New();
//			_lumenPolyData->ShallowCopy(clearPolyData->GetOutput());
//			(*(*lumenMap)[i]) += _lumenPolyData;
//
//
//			// Dilate part using scaling instead of image dilate
//			// generate polydata 
//			double center[3];
//			vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
//				vtkSmartPointer<vtkCenterOfMass>::New();
//			centerOfMassFilter->SetInputData(_lumenPolyData);
//			centerOfMassFilter->SetUseScalarsAsWeights(false);
//			centerOfMassFilter->Update();
//			centerOfMassFilter->GetCenter(center);
//
//			//vtkSmartPointer<vtkTransform> trans1 =
//			//	vtkSmartPointer<vtkTransform>::New();
//			//trans1->Translate(-center[0], -center[1], -center[2]);
//			//vtkSmartPointer<vtkTransform> trans2 =
//			//	vtkSmartPointer<vtkTransform>::New();
//			//trans2->Scale(1 + m_dilateValue, 1 + m_dilateValue, 1);
//			//trans2->Concatenate(trans1);
//			//vtkSmartPointer<vtkTransform> trans3 =
//			//	vtkSmartPointer<vtkTransform>::New();
//			//trans3->Translate(center);
//			//trans3->Concatenate(trans2);
//			//trans3->Update();
//
//			vtkSmartPointer<vtkTransform> trans3 =
//				vtkSmartPointer<vtkTransform>::New();
//			trans3->Translate(center);
//			trans3->Scale(1 + m_dilateValue, 1 + m_dilateValue, 1);
//			trans3->Translate(-center[0], -center[1], -center[2]);
//			trans3->Update();
//
//			vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyDataFilter =
//				vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//			transformPolyDataFilter->SetTransform(trans3);
//			transformPolyDataFilter->SetInputData(_lumenPolyData);
//			transformPolyDataFilter->Update();
//
//			vtkSmartPointer<vtkPolyData> _vesselWalll =
//				vtkSmartPointer<vtkPolyData>::New();
//			_vesselWalll->ShallowCopy(transformPolyDataFilter->GetOutput());
//			*(*vesselWallMap)[i] += _vesselWalll;
//		}
//
//	}
//	ReadFromPolydata();
//}
//
//void InteractorStyleVesselSegmentation::FillPolygon()
//{
//	InteractorStylePolygonDraw::FillPolygon(
//		&m_vesselWallContourWidgets, m_vesselWallLabel);
//	InteractorStylePolygonDraw::FillPolygon(
//		&m_lumenWallContourWidgets, m_lumenWallLabel);
//	InteractorStylePolygonDraw::FillPolygon();
//
//}
//
//void InteractorStyleVesselSegmentation::FillPolygonThroughSlice(int slice1, int slice2)
//{
//	if (m_imageViewer->GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY) {
//		return;
//	}
//	// using the bigger on as the end slice
//	if (slice1 > slice2) {
//		swap(slice1, slice2);
//	}
//	// camping
//	slice1 = max(slice1, GetExtent()[4]);
//	slice2 = min(slice2, GetExtent()[5]);
//
//
//	Overlay* overlay = m_imageViewer->GetOverlay();
//	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>*  _contours[2] =
//	{ overlay->GetVesselWallPolyData(), overlay->GetLumenPolyData() };
//	unsigned char _labels[2] = { m_vesselWallLabel, m_lumenWallLabel };
//
//	for (int index = 0; index < 2; ++index) {
//
//		list<vtkSmartPointer<vtkPolygon>> contourPolygons;
//
//		for (QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>::const_iterator cit1 =
//			_contours[index]->cbegin(); cit1 != _contours[index]->cend(); ++cit1) {
//			// only filling the extent between slice1 and slice2
//			if (cit1.key() < slice1 || cit1.key() > slice2) {
//				continue;
//			}
//
//			QList<vtkSmartPointer<vtkPolyData>>* _list = cit1.value();
//			for (QList<vtkSmartPointer<vtkPolyData>>::const_iterator cit2 = _list->cbegin();
//				cit2 != _list->cend(); ++cit2) {
//				
//				contourPolygons.push_back(vtkSmartPointer<vtkPolygon>::New());
//				
//				vtkSmartPointer<vtkParametricSpline> spline =
//					vtkSmartPointer<vtkParametricSpline>::New();
//				spline->SetPoints((*cit2)->GetPoints());
//				//spline->ClosedOn();
//				vtkSmartPointer<vtkParametricFunctionSource> functionSource =
//					vtkSmartPointer<vtkParametricFunctionSource>::New();
//				functionSource->SetUResolution((*cit2)->GetPoints()->GetNumberOfPoints() * 10);
//
//				functionSource->SetParametricFunction(spline);
//				functionSource->Update();
//
//
//				double lastPoint[3] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MAX, VTK_DOUBLE_MAX };
//				for (vtkIdType id = 0; id < functionSource->GetOutput()->GetNumberOfPoints(); ++id) {
//
//					double displayCoordinate[3];
//					const double* worldCoordinate = functionSource->GetOutput()->GetPoint(id);
//
//					//Take one image data 1 to be reference
//					displayCoordinate[0] = ((worldCoordinate[0] - GetOrigin()[0]) / GetSpacing()[0]);
//					displayCoordinate[1] = ((worldCoordinate[1] - GetOrigin()[1]) / GetSpacing()[1]);
//					displayCoordinate[2] = cit1.key();
//					double d = vtkMath::Distance2BetweenPoints(lastPoint, displayCoordinate);
//					if (d < 1E-5)
//						continue;
//					memcpy(lastPoint, displayCoordinate, sizeof(displayCoordinate));
//					contourPolygons.back()->GetPoints()->InsertNextPoint(displayCoordinate);
//
//				}
//			}
//		}
//
//
//		InteractorStylePolygonDraw::FillPolygon(&contourPolygons, _labels[index]);
//
//	}
//
//
//}
//

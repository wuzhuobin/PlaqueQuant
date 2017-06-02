#include "QInteractorStyleVesselSegmentation2.h"
#include "ui_QInteractorStyleVesselSegmentation2.h"
#include "ui_QInteractorStylePolygonDrawSeries.h"

#include <vtkObjectFactory.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkImageData.h>
#include <vtkContourWidget.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkPolyData.h>
#include <vtkCellData.h>
#include <vtkLinearContourLineInterpolator.h>
#include <vtkBezierContourLineInterpolator.h>
#include <vtkImageThreshold.h>
#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindowInteractor.h>

#include "MagneticContourLineInterpolator.h"
#include "ContourWidgetSeries.h"
#include "ContourWidgetSeriesOrientedGlyphRepresentation.h"
#include "VesselWallHoughTransformCircles.h"
#include "ReorderPointIdOfContourFilter.h"
#include "LumenSegmentationFilter2.h"
#include "ImageViewer.h"

vtkStandardNewMacro(QInteractorStyleVesselSegmentation2);
QSETUP_UI_SRC(QInteractorStyleVesselSegmentation2);

QInteractorStyleVesselSegmentation2::ContourMap QInteractorStyleVesselSegmentation2::m_normalXZ;
QInteractorStyleVesselSegmentation2::ContourMap QInteractorStyleVesselSegmentation2::m_normalYZ;
QInteractorStyleVesselSegmentation2::ContourMap QInteractorStyleVesselSegmentation2::m_normalXY;
QInteractorStyleVesselSegmentation2::ContourMap QInteractorStyleVesselSegmentation2::m_lumen;
QInteractorStyleVesselSegmentation2::ContourMap QInteractorStyleVesselSegmentation2::m_vesselWall;


class QInteractorStyleVesselSegmentation2Callback : public vtkCommand
{
public:
	vtkTypeMacro(QInteractorStyleVesselSegmentation2Callback, vtkCommand);
	static QInteractorStyleVesselSegmentation2Callback* New() {
		return new QInteractorStyleVesselSegmentation2Callback;
	}
	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData) override {
		//ContourWidgetSeries* contourSeries = ContourWidgetSeries::SafeDownCast(caller);
		if (self->m_currentMode != QInteractorStyleVesselSegmentation2::NORMAL)
		{
			switch (eventId)
			{
			case InteractionEvent:
			case EndInteractionEvent:
				self->SaveContoursToContourMap();
				self->m_lumenContours->SetWidgetState(ContourWidgetSeries::DEFINE);
				self->m_vesselWallContours->SetWidgetState(ContourWidgetSeries::DEFINE);
				break;
			case MouseMoveEvent:
				// conducting event to other widgets
			{
				//self->m_lumenContours->InvokeEvent(MouseMoveEvent, NULL);
				//self->m_vesselWallContours->InvokeEvent(MouseMoveEvent, NULL);

				int X = self->Interactor->GetEventPosition()[0];
				int Y = self->Interactor->GetEventPosition()[1];
				int state1, state2;
				state1 = self->m_lumenContoursRep->ComputeInteractionState(X, Y);
				state2 = self->m_vesselWallContoursRep->ComputeInteractionState(X, Y);
				if (state1 == ContourWidgetSeriesRepresentation::OUTSIDE &&
					state2 == ContourWidgetSeriesRepresentation::OUTSIDE) {
					self->m_contourSeries->ProcessEventsOn();
					self->m_lumenContours->ProcessEventsOff();
					self->m_vesselWallContours->ProcessEventsOff();
				}
				else if (state1 != ContourWidgetSeriesRepresentation::OUTSIDE) {
					self->m_contourSeries->ProcessEventsOff();
					self->m_lumenContours->ProcessEventsOn();
					self->m_vesselWallContours->ProcessEventsOff();
				}
				else if (state2 != ContourWidgetSeriesRepresentation::OUTSIDE) {
					self->m_contourSeries->ProcessEventsOff();
					self->m_lumenContours->ProcessEventsOff();
					self->m_vesselWallContours->ProcessEventsOn();
				}
			}
				break;
			case LeftButtonPressEvent:
			{
				int X = self->Interactor->GetEventPosition()[0];
				int Y = self->Interactor->GetEventPosition()[1];
				int state;

				state = self->m_lumenContoursRep->ComputeInteractionState(X, Y);
				if (state != ContourWidgetSeriesRepresentation::OUTSIDE) {
					self->m_lumenContours->InvokeEvent(LeftButtonPressEvent, NULL);
					return;
				}

				state = self->m_vesselWallContoursRep->ComputeInteractionState(X, Y);
				if (state != ContourWidgetSeriesRepresentation::OUTSIDE) {
					self->m_vesselWallContours->InvokeEvent(LeftButtonPressEvent, NULL);
					return;
				}

			}
				break;
			case LeftButtonReleaseEvent:
			{
				int X = self->Interactor->GetEventPosition()[0];
				int Y = self->Interactor->GetEventPosition()[1];
				int state;

				state = self->m_lumenContoursRep->ComputeInteractionState(X, Y);
				if (state != ContourWidgetSeriesRepresentation::OUTSIDE) {
					self->m_lumenContours->InvokeEvent(LeftButtonReleaseEvent, NULL);
					SetAbortFlag(1);
					return;
				}

				state = self->m_vesselWallContoursRep->ComputeInteractionState(X, Y);
				if (state != ContourWidgetSeriesRepresentation::OUTSIDE) {
					self->m_vesselWallContours->InvokeEvent(LeftButtonReleaseEvent, NULL);
					SetAbortFlag(1);
					return;
				}

			}
				break;
			default:
				break;
			}
		}
	}
	QInteractorStyleVesselSegmentation2* self = nullptr;

};


QInteractorStyleVesselSegmentation2::QInteractorStyleVesselSegmentation2(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleVesselSegmentation2::~QInteractorStyleVesselSegmentation2()
{
	QDELETE_UI();
}

void QInteractorStyleVesselSegmentation2::uniqueInitialization()
{

}

void QInteractorStyleVesselSegmentation2::initialization()
{
	
	
	
	m_callback =
		vtkSmartPointer<QInteractorStyleVesselSegmentation2Callback>::New();
	m_callback->self = this;

	m_contourSeries->AddObserver(vtkCommand::MouseMoveEvent, m_callback);
	//m_contourSeries->AddObserver(vtkCommand::LeftButtonPressEvent, m_callback);
	//m_contourSeries->AddObserver(vtkCommand::LeftButtonReleaseEvent, m_callback);

	m_lumenContoursRep = vtkSmartPointer<ContourWidgetSeriesOrientedGlyphRepresentation>::New();
	m_lumenContours = vtkSmartPointer<ContourWidgetSeries>::New();
	m_lumenContours->SetRepresentation(m_lumenContoursRep);
	m_lumenContours->AddObserver(vtkCommand::MouseMoveEvent, m_callback);
	m_lumenContours->AddObserver(vtkCommand::InteractionEvent, m_callback);
	m_lumenContours->AddObserver(vtkCommand::EndInteractionEvent, m_callback);


	m_vesselWallContoursRep = vtkSmartPointer<ContourWidgetSeriesOrientedGlyphRepresentation>::New();
	m_vesselWallContours = vtkSmartPointer<ContourWidgetSeries>::New();
	m_vesselWallContours->SetRepresentation(m_vesselWallContoursRep);
	m_vesselWallContours->AddObserver(vtkCommand::MouseMoveEvent, m_callback);
	m_vesselWallContours->AddObserver(vtkCommand::InteractionEvent, m_callback);
	m_vesselWallContours->AddObserver(vtkCommand::EndInteractionEvent, m_callback);
	
	// disconnect the old one
	disconnect(QInteractorStylePolygonDrawSeries::getUi()->cleanAllPushButton, SIGNAL(clicked()),
		this, SLOT(CleanAll()));
	disconnect(QInteractorStylePolygonDrawSeries::getUi()->cleanOnePushButton, SIGNAL(clicked()),
		this, SLOT(CleanOne()));
	disconnect(QInteractorStylePolygonDrawSeries::getUi()->fillContourPushButton, SIGNAL(clicked()),
		this, SLOT(FillContours()));

	disconnect(QInteractorStylePolygonDrawSeries::getUi()->polygonRadionButton, SIGNAL(clicked()),
		this, SLOT(SetLinearInterpolator()));
	disconnect(QInteractorStylePolygonDrawSeries::getUi()->smoothCurveRadioButton, SIGNAL(clicked()),
		this, SLOT(SetBezierInterpolator()));
	disconnect(QInteractorStylePolygonDrawSeries::getUi()->radioButtonMagnetic, SIGNAL(clicked()),
		this, SLOT(SetMagneticInterpolator()));
	disconnect(QInteractorStylePolygonDrawSeries::getUi()->comboBoxLabel, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetContourLabel(int)));
	
	// connect the new one
	connect(ui->cleanAllPushButton, SIGNAL(clicked()),
		this, SLOT(CleanAll()));
	connect(ui->cleanOnePushButton, SIGNAL(clicked()),
		this, SLOT(CleanOne()));
	connect(ui->fillContourPushButton, SIGNAL(clicked()),
		this, SLOT(FillContours()));

	connect(ui->polygonRadionButton, SIGNAL(clicked()),
		this, SLOT(SetLinearInterpolator()));
	connect(ui->smoothCurveRadioButton, SIGNAL(clicked()),
		this, SLOT(SetBezierInterpolator()));
	connect(ui->radioButtonMagnetic, SIGNAL(clicked()),
		this, SLOT(SetMagneticInterpolator()));
	connect(ui->comboBoxLabel, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetContourLabel(int)));
	// connect the mode change button
	connect(ui->radioButtonNormal, SIGNAL(clicked()),
		this, SLOT(SetSegmentationModeToNormal()));
	connect(ui->radioButtonLumenSegmentation, SIGNAL(clicked()),
		this, SLOT(SetSegmentationModeToLumenSegmentation()));
	connect(ui->radioButtonVesselWallSegmentation, SIGNAL(clicked()),
		this, SLOT(SetSegmentationModeToVesselSegmentation()));
	// autoLumenSegmentation
	connect(ui->autoLumenSegmentationSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(SetLumenSegmentationValue(int)));
	connect(ui->pushButtonAutoLumenSegmentation, SIGNAL(clicked()),
		this, SLOT(AutoLumenSegmentation()));
	// autoVesselWallSegmentation
	connect(ui->pushButtonAutoVesselWallSegmentation, SIGNAL(clicked()),
		this, SLOT(AutoVesselWallSegmentation()));
	connect(ui->spinBoxVesselWallThickness, SIGNAL(valueChanged(int)),
		this, SLOT(setVesselWallSegmentationRadius(int)));

	// fill slices
	connect(ui->spinBoxFillSlicesBegin, SIGNAL(valueChanged(int)),
		this, SLOT(SetFillSliceBegin(int)));
	connect(ui->spinBoxFillSliceEnd, SIGNAL(valueChanged(int)),
		this, SLOT(SetFillSliceEnd(int)));
	connect(ui->pushButtonFillSlices, SIGNAL(clicked()),
		this, SLOT(FillSlices()));
	

}

void QInteractorStyleVesselSegmentation2::uniqueEnable()
{
	QInteractorStylePolygonDrawSeries::uniqueEnable();
	int* extent = GetImageViewer()->GetDisplayExtent();
	ui->spinBoxFillSlicesBegin->setRange(extent[4], extent[5]);
	ui->spinBoxFillSliceEnd->setRange(extent[4], extent[5]);
}

void QInteractorStyleVesselSegmentation2::OnEnter()
{
	InteractorStylePolygonDrawSeries::OnEnter();
	QList<QInteractorStyleVesselSegmentation2*> _disable;
	QList<QInteractorStyleVesselSegmentation2*> _enable;

	for (std::list<AbstractInteractorStyleImage*>::const_iterator cit =
		m_imageStyles.cbegin(); cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleVesselSegmentation2* _style = QInteractorStyleVesselSegmentation2::SafeDownCast(*cit);
		if (_style != nullptr && _style->GetCustomEnabled()) {
			// find out all styles which needs to be disable
			if (_style->m_contourSeries->GetEnabled() &&
				_style->GetSliceOrientation() == this->GetSliceOrientation() &&
				_style != this) {
				_disable << _style;
			}
			// find out all styles which needs to be enable
			else if (!_style->m_contourSeries->GetEnabled() &&
				(_style->GetSliceOrientation() != this->GetSliceOrientation() || _style == this)) {
				_enable << _style;
			}
		}
	}
	foreach(QInteractorStyleVesselSegmentation2* style, _disable) {
		style->m_contourSeries->ResetContours();
		style->m_contourSeries->EnabledOff();
		if (m_currentMode != NORMAL &&
			style->GetSliceOrientation() == ImageViewer::SLICE_ORIENTATION_XY) {
			style->m_vesselWallContours->ResetContours();
			style->m_lumenContours->ResetContours();
			style->m_vesselWallContours->EnabledOff();
			style->m_lumenContours->EnabledOff();
		}
	}
	foreach(QInteractorStyleVesselSegmentation2* style, _enable) {
		style->m_contourSeries->EnabledOn();
		style->LoadContoursFromContourMap();
		if (m_currentMode != NORMAL &&
			style->GetSliceOrientation() == ImageViewer::SLICE_ORIENTATION_XY) {
			style->m_vesselWallContours->EnabledOn();
			style->m_lumenContours->EnabledOn();
		}
	}
}

void QInteractorStyleVesselSegmentation2::OnMouseMove()
{
	QInteractorStylePolygonDrawSeries::OnMouseMove();
	if (m_currentMode != NORMAL) {
		//int X = this->Interactor->GetEventPosition()[0];
		//int Y = this->Interactor->GetEventPosition()[1];
		//int state1, state2;

		//state1 = this->m_lumenContoursRep->ComputeInteractionState(X, Y);
		//state2 = this->m_vesselWallContoursRep->ComputeInteractionState(X, Y);
		//if (state1 == ContourWidgetSeriesRepresentation::OUTSIDE &&
		//	state2 == ContourWidgetSeriesRepresentation::OUTSIDE) {
		//	m_contourSeries->ProcessEventsOn();
		//}
		//else if (state1 != ContourWidgetSeriesRepresentation::OUTSIDE) {
		//	m_lumenContours->ProcessEventsOn();
		//}
		//else if (state2 != ContourWidgetSeriesRepresentation::OUTSIDE) {
		//	m_vesselWallContours->ProcessEventsOn();
		//}
	}
}

void QInteractorStyleVesselSegmentation2::OnLeftButtonDown()
{
	QInteractorStylePolygonDrawSeries::OnLeftButtonDown();
	if (m_currentMode != NORMAL) {
		//m_vesselWallContours->InvokeEvent(vtkCommand::LeftButtonPressEvent);
		//m_lumenContours->InvokeEvent(vtkCommand::LeftButtonPressEvent);
	}
}

void QInteractorStyleVesselSegmentation2::OnLeftButtonUp()
{
	QInteractorStylePolygonDrawSeries::OnLeftButtonUp();
	if (m_currentMode != NORMAL) {
		//m_vesselWallContours->InvokeEvent(vtkCommand::LeftButtonReleaseEvent);
		//m_lumenContours->InvokeEvent(vtkCommand::LeftButtonReleaseEvent);
	}
}

void QInteractorStyleVesselSegmentation2::SetCustomEnabled(bool flag)
{
	QInteractorStylePolygonDrawSeries::SetCustomEnabled(flag);
	if (flag) {
		vtkSmartPointer<vtkImageActorPointPlacer> imagePointPlacer =
			vtkSmartPointer<vtkImageActorPointPlacer>::New();
		imagePointPlacer->SetImageActor(GetImageViewer()->GetImageActor());

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> lumenRep =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
		lumenRep->SetPointPlacer(imagePointPlacer);
		lumenRep->SetLineColor(1, 0, 0);
		
		m_lumenContoursRep->SetContourRepresentation(lumenRep);
		
		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> vesselWallRep =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
		vesselWallRep->SetPointPlacer(imagePointPlacer);
		vesselWallRep->SetLineColor(0, 0, 1);
		
		m_vesselWallContoursRep->SetContourRepresentation(vesselWallRep);

		m_lumenContours->SetInteractor(this->Interactor);
		m_vesselWallContours->SetInteractor(this->Interactor);

		SetSegmentationMode(m_currentMode);

	}
	else
	{
		m_lumenContours->ResetContours();
		m_lumenContours->EnabledOff();
		m_vesselWallContours->ResetContours();
		m_vesselWallContours->EnabledOff();
	}
}

void QInteractorStyleVesselSegmentation2::SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k)
{
	int ijk[3];
	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk);
	InteractorStylePolygonDrawSeries::SetCurrentFocalPointWithImageCoordinateByViewer(i, j, k);
	if (GetSlice() != ijk[GetSliceOrientation()]) {
		m_contourSeries->ResetContours();
		m_vesselWallContours->ResetContours();
		m_lumenContours->ResetContours();
		LoadContoursFromContourMap();
	}
	
}

void QInteractorStyleVesselSegmentation2::SetSegmentationMode(int mode)
{
	m_currentMode = mode;
	switch (mode)
	{
	case NORMAL:
		ui->stackedWidgetMode->setCurrentWidget(ui->pageNormal);
		m_lumenContours->EnabledOff();
		m_vesselWallContours->EnabledOff();
		return;
	case LUMEN_SEGMENTATION:
		ui->stackedWidgetMode->setCurrentWidget(ui->pageLumenSegmentation);
		m_lumenContours->EnabledOn();
		//m_lumenContours->SetWidgetState(ContourWidgetSeries::DEFINE);

		m_vesselWallContours->EnabledOn();
		//m_vesselWallContours->SetWidgetState(ContourWidgetSeries::DEFINE);
		break;
	case VESSEL_WALL_SEGMENTATION:
		ui->stackedWidgetMode->setCurrentWidget(ui->pageVesselWallSegmentation);
		m_lumenContours->EnabledOn();
		//m_lumenContours->SetWidgetState(ContourWidgetSeries::DEFINE);

		m_vesselWallContours->EnabledOn();
		//m_vesselWallContours->SetWidgetState(ContourWidgetSeries::DEFINE);
		break;
	default:
		break;
	}



}

void QInteractorStyleVesselSegmentation2::SaveContoursToContourMap()
{
	if (m_contourSeries->GetEnabled()) {
		switch (this->GetSliceOrientation())
		{
		case ImageViewer::SLICE_ORIENTATION_YZ:
			QInteractorStylePolygonDrawSeries::SaveContoursToContourMap(this->GetSlice(), this->m_contourSeries, this->m_normalYZ);
			break;
		case ImageViewer::SLICE_ORIENTATION_XZ:
			QInteractorStylePolygonDrawSeries::SaveContoursToContourMap(this->GetSlice(), this->m_contourSeries, this->m_normalXZ);
			break;
		case ImageViewer::SLICE_ORIENTATION_XY:
			QInteractorStylePolygonDrawSeries::SaveContoursToContourMap(this->GetSlice(), this->m_contourSeries, this->m_normalXY);
			if (m_currentMode != NORMAL) {
				QInteractorStylePolygonDrawSeries::SaveContoursToContourMap(this->GetSlice(), this->m_lumenContours, this->m_lumen);
				QInteractorStylePolygonDrawSeries::SaveContoursToContourMap(this->GetSlice(), this->m_vesselWallContours, this->m_vesselWall);
			}
			break;
		default:
			break;
		}
	}
}

void QInteractorStyleVesselSegmentation2::LoadContoursFromContourMap()
{
	if (this->m_contourSeries->GetEnabled()) {
		int ijk[3];
		GetImageViewer()->GetFocalPointWithImageCoordinate(ijk);
		switch (this->GetSliceOrientation())
		{
		case ImageViewer::SLICE_ORIENTATION_YZ:
			QInteractorStylePolygonDrawSeries::LoadContoursFromContourMap(ijk[0], m_contourSeries, m_normalYZ);
			m_contourSeries->Render();
			break;
		case ImageViewer::SLICE_ORIENTATION_XZ:
			QInteractorStylePolygonDrawSeries::LoadContoursFromContourMap(ijk[1], m_contourSeries, m_normalXZ);
			m_contourSeries->Render();
			break;
		case ImageViewer::SLICE_ORIENTATION_XY:
			QInteractorStylePolygonDrawSeries::LoadContoursFromContourMap(ijk[2], m_contourSeries, m_normalXY);
			m_contourSeries->Render();
			if (m_currentMode != NORMAL) {
				QInteractorStylePolygonDrawSeries::LoadContoursFromContourMap(ijk[2], m_lumenContours, m_lumen);
				m_lumenContours->Render();
				QInteractorStylePolygonDrawSeries::LoadContoursFromContourMap(ijk[2], m_vesselWallContours, m_vesselWall);
				m_vesselWallContours->Render();
			}
		default:
			break;
		}
	}

}

void QInteractorStyleVesselSegmentation2::CleanAll()
{
	m_normalXY.clear();
	m_normalXZ.clear();
	m_normalYZ.clear();
	m_vesselWall.clear();
	m_lumen.clear();
	m_contourSeries->ResetContours();
	m_lumenContours->ResetContours();
	m_vesselWallContours->ResetContours();
	m_contourSeries->Render();
	m_lumenContours->Render();
	m_vesselWallContours->Render();
}

void QInteractorStyleVesselSegmentation2::FillContours()
{
	if (m_contourSeries->GetEnabled()) {
		InteractorStylePolygonDrawSeries::FillContours(m_contourSeriesRep);
		if (m_currentMode != NORMAL && 
			GetSliceOrientation() == ImageViewer::SLICE_ORIENTATION_XY) {
			InteractorStylePolygonDrawSeries::FillContours(m_vesselWallContoursRep);
			InteractorStylePolygonDrawSeries::FillContours(m_lumenContoursRep);
		}
	}
	GetImageViewer()->GetOverlay()->Modified();
	GetImageViewer()->GetOverlay()->InvokeEvent(vtkCommand::UpdateDataEvent);
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStylePolygonDrawSeries, GetImageViewer()->Render());

}

//void QInteractorStyleVesselSegmentation2::FillSlices()
//{
//}

void QInteractorStyleVesselSegmentation2::AutoVesselWallSegmentation()
{
	if (!m_lumenContours->GetEnabled() ||
		GetSliceOrientation() != ImageViewer::SLICE_ORIENTATION_XY) {
		return;
	}
	int extent[6];
	std::copy_n(GetImageViewer()->GetDisplayExtent(), 6, extent);

	vtkSmartPointer<vtkImageThreshold> imageThreshold =
		vtkSmartPointer<vtkImageThreshold>::New();
	imageThreshold->ThresholdBetween(m_lumenWallLabel, m_lumenWallLabel);
	imageThreshold->SetInputData(GetImageViewer()->GetOverlay());
	imageThreshold->ReplaceOutOn();
	imageThreshold->SetOutValue(0);
	imageThreshold->Update();

	for (int i = extent[4]; i <= extent[5]; ++i) {
		int _extent[6] = {
			extent[0], extent[1],
			extent[2], extent[3],
			i, i };

		ContourMapElementPtr elementLumen = ContourMapElementPtr(new PolyDataList);


		vtkSmartPointer<vtkMarchingSquares> marchingSquares =
			vtkSmartPointer<vtkMarchingSquares>::New();
		marchingSquares->SetInputConnection(imageThreshold->GetOutputPort());
		marchingSquares->CreateDefaultLocator();
		marchingSquares->SetImageRange(_extent);
		marchingSquares->SetValue(1, m_lumenWallLabel);
		marchingSquares->Update();

		vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
		connectivityFilter->SetExtractionModeToAllRegions();
		connectivityFilter->Update();

		for (int j = 0; j < connectivityFilter->GetNumberOfExtractedRegions(); ++j) {
			// extract lumen contour polydata
			vtkSmartPointer<vtkPolyData> nodePolyData =
				vtkSmartPointer<vtkPolyData>::New();

			vtkSmartPointer<vtkPolyDataConnectivityFilter> _connectivityFilter =
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
			_connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
			_connectivityFilter->AddSpecifiedRegion(j);
			_connectivityFilter->SetExtractionModeToSpecifiedRegions();
			_connectivityFilter->Update();

			vtkSmartPointer<vtkCleanPolyData> clean1 =
				vtkSmartPointer<vtkCleanPolyData>::New();
			clean1->SetInputConnection(_connectivityFilter->GetOutputPort());
			clean1->PointMergingOn();
			clean1->Update();

			// vtkError handling
			vtkSmartPointer<vtkCallbackCommand> errorCatch =
				vtkSmartPointer<vtkCallbackCommand>::New();
			// lambda function for handling error
			errorCatch->SetCallback([](vtkObject *caller, unsigned long eid,
				void *clientdata, void *calldata)->void {
				// error catch and error display
				char* ErrorMessage = static_cast<char *>(calldata);
				vtkOutputWindowDisplayErrorText(ErrorMessage);
				ReorderPointIdOfContourFilter* reorder =
					ReorderPointIdOfContourFilter::SafeDownCast(caller);
				// if error happened, skip this filer
				reorder->SetOutput(reorder->GetInput());
			});
			vtkSmartPointer<ReorderPointIdOfContourFilter> reorder =
				vtkSmartPointer<ReorderPointIdOfContourFilter>::New();
			reorder->SetInputConnection(clean1->GetOutputPort());
			reorder->SetOutputCellType(VTK_POLY_LINE);
			reorder->AddObserver(vtkCommand::ErrorEvent, errorCatch);
			reorder->Update();
			if (reorder->GetInput() == reorder->GetOutput()) {
				std::string ErrorMessage = "Error in slice " + std::to_string(i);
				vtkErrorMacro(<< ErrorMessage);
			}

			double toleranceInitial = 1;
			int loopBreaker = 0;

			vtkSmartPointer<vtkCleanPolyData> clean2 =
				vtkSmartPointer<vtkCleanPolyData>::New();
			clean2->SetInputConnection(reorder->GetOutputPort());
			clean2->ToleranceIsAbsoluteOn();
			clean2->SetAbsoluteTolerance(toleranceInitial);
			clean2->PointMergingOn();
			clean2->Update();
			while (clean2->GetOutput()->GetNumberOfPoints() < 3 && loopBreaker < 10) {
				toleranceInitial *= 0.75;
				clean2->SetAbsoluteTolerance(toleranceInitial);
				clean2->Update();
				loopBreaker += 1;
			}

			nodePolyData->ShallowCopy(clean2->GetOutput());
			(*elementLumen) << nodePolyData;
		}
		m_lumen[i] = elementLumen;

		// extract vessel wall contour polydata
		ContourMapElementPtr elementVesselWall = ContourMapElementPtr(new PolyDataList);


		vtkSmartPointer<VesselWallHoughTransformCircles> hough =
			vtkSmartPointer<VesselWallHoughTransformCircles>::New();
		hough->SetOuterRadius(m_vesselWallSegmentationRadius);

		hough->SetInputData(0, GetImageViewer()->GetInput());
		hough->SetInputData(1, imageThreshold->GetOutput());
		hough->SetImageRange(_extent);
		hough->Update();

		for (VesselWallHoughTransformCircles::CircleVector::const_iterator cit =
			hough->GetCircles()->cbegin(); cit != hough->GetCircles()->cend(); ++cit) {
			vtkSmartPointer<vtkPolyData> nodePolyData =
				vtkSmartPointer<vtkPolyData>::New();
			nodePolyData->ShallowCopy(*cit);
			(*elementVesselWall) << nodePolyData;
		}
		m_vesselWall[i] = elementVesselWall;
	}

}

void QInteractorStyleVesselSegmentation2::AutoLumenSegmentation()
{
	if (!m_lumenContours->GetEnabled() ||
		GetSliceOrientation() != ImageViewer::SLICE_ORIENTATION_XY) {
		return;
	}
	int ijk[3];
	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk); 
	
	ContourMapElementPtr element = ContourMapElementPtr(new PolyDataList);

	vtkLookupTable* lookupTable = GetImageViewer()->GetLookupTable();
	double vesselWallColour[3];
	lookupTable->GetColor(m_vesselWallLabel, vesselWallColour);

	for (int i = 0; i < m_contourSeriesRep->GetNumberOfContours(); ++i) {

		vtkOrientedGlyphContourRepresentation* _rep =
			vtkOrientedGlyphContourRepresentation::SafeDownCast(m_contourSeriesRep->GetContourRepresentation(i));
		double* colour = _rep->GetLinesProperty()->GetColor();
		

		if (std::equal(vesselWallColour, vesselWallColour + 3, colour)) {
			vtkSmartPointer<vtkPolyData> nodePolyData =
				vtkSmartPointer<vtkPolyData>::New();

			vtkSmartPointer<LumenSegmentationFilter2> lsFilter =
				vtkSmartPointer<LumenSegmentationFilter2>::New();
			lsFilter->SetInputData(GetImageViewer()->GetInput());
			lsFilter->SetGenerateValues(1, m_lumenSegmentationValue, m_lumenSegmentationValue);
			lsFilter->SetVesselWallContourRepresentation(_rep);
			lsFilter->SetSlice(ijk[2]);
			lsFilter->Update();

			nodePolyData->ShallowCopy(lsFilter->GetOutput());
			
			(*element) << nodePolyData;
		}
	}
	for (int i = 0; i < m_vesselWallContoursRep->GetNumberOfContours(); ++i) {
		vtkOrientedGlyphContourRepresentation* _rep =
			vtkOrientedGlyphContourRepresentation::SafeDownCast(m_vesselWallContoursRep->GetContourRepresentation(i));
		double* colour = _rep->GetLinesProperty()->GetColor();


		if (std::equal(vesselWallColour, vesselWallColour + 3, colour)) {
			vtkSmartPointer<vtkPolyData> nodePolyData =
				vtkSmartPointer<vtkPolyData>::New();

			vtkSmartPointer<LumenSegmentationFilter2> lsFilter =
				vtkSmartPointer<LumenSegmentationFilter2>::New();
			lsFilter->SetInputData(GetImageViewer()->GetInput());
			lsFilter->SetGenerateValues(1, m_lumenSegmentationValue, m_lumenSegmentationValue);
			lsFilter->SetVesselWallContourRepresentation(_rep);
			lsFilter->SetSlice(ijk[2]);
			lsFilter->Update();

			nodePolyData->ShallowCopy(lsFilter->GetOutput());

			(*element) << nodePolyData;
		}
	}

	m_lumen[ijk[2]] = element;
	m_lumenContours->ResetContours();
	QInteractorStylePolygonDrawSeries::LoadContoursFromContourMap(ijk[2], m_lumenContours, m_lumen);
	m_lumenContours->Render();
}

void QInteractorStyleVesselSegmentation2::SetLumenSegmentationValue(int value)
{
	this->m_lumenSegmentationValue = value;
		AutoLumenSegmentation();
}

void QInteractorStyleVesselSegmentation2::setVesselWallSegmentationRadius(int value)
{
	this->m_vesselWallSegmentationRadius = value;
}


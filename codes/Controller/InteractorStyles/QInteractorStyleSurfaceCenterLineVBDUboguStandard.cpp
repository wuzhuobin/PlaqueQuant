#include "QInteractorStyleSurfaceCenterLineVBDUboguStandard.h"
#include "ui_QInteractorStyleSurfaceCenterLineVBDUboguStandard.h"

#include "PerpendicularMeasurementLineWidget.h"
#include "CenterlineSurfaceViewer.h"

#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkDistanceRepresentation.h>
#include <vtkDistanceWidget.h>
#include <vtkLineRepresentation.h>
#include <vtkRenderer.h>
#include <vtkTextActor.h>
#include <vtkHandleWidget.h>
#include <vtkCallbackCommand.h>
#include <vtkKdTreePointLocator.h>

vtkStandardNewMacro(QInteractorStyleSurfaceCenterLineVBDUboguStandard);
QSETUP_UI_SRC(QInteractorStyleSurfaceCenterLineVBDUboguStandard);

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	setCurrentMode(m_currentMode);
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::setCurrentMode(int mode)
{
	if (mode < 0 || mode > 8) {
		return;
	}
	m_currentMode = mode;
	switch (m_currentMode)
	{
	case LENGTH_OF_BASILAR_ARTERY:
	case LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY:
	case LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY:
	case MAXIMUM_DIAMETER_OF_BASILAR_ARTERY:
	case MAXIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY:
	case MAXIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY:
		if (m_lineWidget) {
			m_lineWidget->SetInteractor(nullptr);
			m_lineWidget->EnabledOff();
			m_lineWidget = nullptr;
		}
		InitializeHandleWidgets();
		GetSurfaceViewer()->GetRenderer()->AddActor(m_measurementText);
		break;
	case DISTANCE_BETWEEN_CENTERLINE_AND_THE_LEAD:
	case DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT:
	case DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT:
		for (int i = 0; i < NUM_OF_HANDLES; ++i) {
			// removing widgets
			if (m_handleWidgets[i]) {
				m_handleWidgets[i]->SetInteractor(nullptr);
				m_handleWidgets[i]->EnabledOff();
				m_handleWidgets[i]->RemoveAllObservers();
				m_handleWidgets[i] = nullptr;
			}

		}
		m_pointLocator = nullptr;
		GetSurfaceViewer()->GetRenderer()->RemoveActor(m_measurementText);
		InitializeLinePerpendicularMeasurementWidget();
	default:
		break;
	}
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::FindMaximumRadius()
{
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::FindMaximumRadius();
	switch (m_currentMode)
	{
	case LENGTH_OF_BASILAR_ARTERY:
		ui->doubleSpinBoxLengthOfBasilaarArtery->setValue(m_distance);
		break;
	case LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY:
		ui->doubleSpinBoxLengthOfIntracranialSegmentOfLeftVetrabralArtery->setValue(m_distance);
		break;
	case LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY:
		ui->doubleSpinBoxLengthOfIntracranialSegmentOfRightVetrabralArtery->setValue(m_distance);
		break;
	case MAXIMUM_DIAMETER_OF_BASILAR_ARTERY:
		ui->doubleSpinBoxMaximumDiameterOfBasilarArtery->setValue(m_maxRadius * 2);
		break;
	case MAXIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY:
		ui->doubleSpinBoxMaximumDiameterOfLeftVetebralArtery->setValue(m_maxRadius * 2);
		break;
	case MAXIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY:
		ui->doubleSpinBoxMaximumDiameterOfRightVetebralArtery->setValue(m_maxRadius * 2);
		break;
	default:
		break;
	}
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::FindMaxiMumPerpendicularDistance()
{
	double distance = m_lineWidget->GetDistanceWidget()->GetDistanceRepresentation()->GetDistance();

	switch (m_currentMode)
	{
	case DISTANCE_BETWEEN_CENTERLINE_AND_THE_LEAD:
		ui->doubleSpinBoxDistanceBetweenCenterlineAndTheLeadOfStartPointAndFockOfBasilarArtery->setValue(distance);
		break;
	case DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT:
		ui->doubleSpinBoxDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeftIntracranialSegmentOfVertebralArteryAndBasilarArtery->setValue(distance);
		break;
	case DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT:
		ui->doubleSpinBoxDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRightIntracranialSegmentOfVertebralArteryAndBasilarArtery->setValue(distance);
		break;
	default:
		break;
	}

}

QInteractorStyleSurfaceCenterLineVBDUboguStandard::QInteractorStyleSurfaceCenterLineVBDUboguStandard(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleSurfaceCenterLineVBDUboguStandard::~QInteractorStyleSurfaceCenterLineVBDUboguStandard()
{
	QDELETE_UI();
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::InitializeLinePerpendicularMeasurementWidget()
{
	if (!GetCenterlineSurfaceViewer()->GetCenterline() ||
		GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
		vtkErrorMacro(<< "no centerline ");
		return;
	}
	vtkSmartPointer<vtkCallbackCommand> _callback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	_callback->SetClientData(this);
	_callback->SetCallback( [](vtkObject *caller, unsigned long eid,
		void *clientdata, void *calldata) {
		QInteractorStyleSurfaceCenterLineVBDUboguStandard* _self =
			reinterpret_cast<QInteractorStyleSurfaceCenterLineVBDUboguStandard*>(clientdata);
		_self->FindMaxiMumPerpendicularDistance();
	});
	

	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
		vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	pointPlacer->AddProp(GetCenterlineSurfaceViewer()->GetCenterlineActor());


	m_lineWidget = vtkSmartPointer<PerpendicularMeasurementLineWidget>::New();
	m_lineWidget->AddObserver(vtkCommand::InteractionEvent, _callback);
	m_lineWidget->SetInteractor(this->Interactor);
	m_lineWidget->CreateDefaultRepresentation();
	m_lineWidget->GetLineRepresentation()->GetPoint1Representation()->SetPointPlacer(pointPlacer);
	m_lineWidget->GetLineRepresentation()->GetPoint2Representation()->SetPointPlacer(pointPlacer);
	m_lineWidget->GetLineRepresentation()->SetPoint1WorldPosition(
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(0));
	m_lineWidget->GetLineRepresentation()->SetPoint2WorldPosition(
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(1));
	m_lineWidget->GetLineRepresentation()->SetDistanceAnnotationFormat("%-#11.2f mm");
	m_lineWidget->GetDistanceWidget()->GetDistanceRepresentation()->SetLabelFormat("%-#11.2f mm");
	m_lineWidget->SetLine(GetCenterlineSurfaceViewer()->GetCenterline());
	m_lineWidget->EnabledOn();
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::initialization()
{
	connect(ui->pushButtonLengthOfBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToLengthOfBasilarArtery()));
	connect(ui->pushButtonLengthOfIntracranialSegmentOfLeftVetrabralArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToLengthOfIntracranialSegmentOfLeftVertebralArtery()));
	connect(ui->pushButtonLengthOfIntracranialSegmentOfRightVetrabralArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToLengthOfIntracranialSegmentOfRightVertebralArtery()));
	connect(ui->pushButtonDistanceBetweenCenterlineAndTheLeadOfStartPointAndFockOfBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToDistanceBetweenCenterlineAndTheLead()));
	connect(ui->pushButtonDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeftIntracranialSegmentOfVertebralArteryAndBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeft()));
	connect(ui->pushButtonDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRightIntracranialSegmentOfVertebralArteryAndBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRight()));
	connect(ui->pushButtonMaximumDiameterOfBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToMaximumDiameterOfBasilarArtery()));
	connect(ui->pushButtonMaximumDiameterOfLeftVetebralArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToMaximumDiameterOfLeftVertebralArtery()));
	connect(ui->pushButtonMaximumDiameterOfRightVetebralArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToMaximumDiameterOfRightVertebralArtery()));


	connect(ui->doubleSpinBoxLengthOfBasilaarArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedLengthOfBasilarArtery(double)));
	connect(ui->doubleSpinBoxLengthOfIntracranialSegmentOfLeftVetrabralArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedLengthOfIntracranialSegmentOfLeftVertebralArtery(double)));
	connect(ui->doubleSpinBoxLengthOfIntracranialSegmentOfRightVetrabralArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedLengthOfIntracranialSegmentOfRightVertebralArtery(double)));
	connect(ui->doubleSpinBoxDistanceBetweenCenterlineAndTheLeadOfStartPointAndFockOfBasilarArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedDistanceBetweenCenterlineAndTheLead(double)));
	connect(ui->doubleSpinBoxDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeftIntracranialSegmentOfVertebralArteryAndBasilarArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeft(double)));
	connect(ui->doubleSpinBoxDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRightIntracranialSegmentOfVertebralArteryAndBasilarArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRight(double)));
	connect(ui->doubleSpinBoxMaximumDiameterOfBasilarArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedMaximumDiameterOfBasilarArtery(double)));
	connect(ui->doubleSpinBoxMaximumDiameterOfLeftVetebralArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedMaximumDiameterOfLeftVertebralArtery(double)));
	connect(ui->doubleSpinBoxMaximumDiameterOfRightVetebralArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(valueChangedMaximumDiameterOfRightVertebralArtery(double)));

}

#include "QInteractorStyleVOI.h"
#include "ui_QInteractorStyleVOI.h"
#include "vtkROIWidget.h"

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>
#include <ImageViewer.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>

#include <vtkImageCroppingRegionsWidget.h>

vtkStandardNewMacro(QInteractorStyleVOI);
QSETUP_UI_SRC(QInteractorStyleVOI);
vtkSmartPointer<vtkROIWidget> QInteractorStyleVOI::m_roi = nullptr;
vtkSmartPointer<vtkRenderWindow> QInteractorStyleVOI::m_renderWindow = nullptr;


//vtkROIWidget* QInteractorStyleVOI::GetROIWidget()
//{
//	return m_roi;
//}

void QInteractorStyleVOI::SetCustomEnabled(bool flag)
{
	QInteractorStyleNavigation::SetCustomEnabled(flag);
	if (flag) {
		// Change orientation of border widgets too
		m_roi->SetBorderWidgetOrientation(m_uniqueROIId, GetSliceOrientation());
		m_roi->SetBorderWidgetsInteractor(m_uniqueROIId, this->Interactor);
		m_roi->GetRepresentation()->PlaceWidget(
			GetImageViewer()->GetInput()->GetBounds());
		m_roi->SetPositionPointer(
			GetImageViewer()->GetFocalPointWithWorldCoordinate());
		m_roi->EnabledOn();
	}
	else {
		m_roi->EnabledOff();
	}
	GetImageViewer()->Render();
	// suppose it should able to disappear by the following
	//m_roi->Render();
	uniqueInvoke(flag);
}
void QInteractorStyleVOI::slotUpdateVOISpinBoxes(double * values)
{
	int extent[6];
	double realSize[3];

	extent[0] = ((values[0] - GetOrigin()[0]) / GetSpacing()[0]) + 0.5;
	extent[1] = ((values[1] - GetOrigin()[0]) / GetSpacing()[0]) + 0.5;
	extent[2] = ((values[2] - GetOrigin()[1]) / GetSpacing()[1]) + 0.5;
	extent[3] = ((values[3] - GetOrigin()[1]) / GetSpacing()[1]) + 0.5;
	extent[4] = ((values[4] - GetOrigin()[2]) / GetSpacing()[2]) + 0.5;
	extent[5] = ((values[5] - GetOrigin()[2]) / GetSpacing()[2]) + 0.5;

	for (int i = 0; i < 3; i++)
	{
		extent[2 * i] = { extent[2 * i] < GetExtent()[2 * i] ? GetExtent()[2 * i] : extent[2 * i] };
		extent[2 * i + 1] = { extent[2 * i + 1] > GetExtent()[2 * i + 1] ? GetExtent()[2 * i + 1] : extent[2 * i + 1] };
	}

	int size[] = {
		extent[1] - extent[0] + 1,
		extent[3] - extent[2] + 1,
		extent[5] - extent[4] + 1	};

	this->ui->sizeSpinBoxX->setValue(size[0]);
	this->ui->sizeSpinBoxY->setValue(size[1]);
	this->ui->sizeSpinBoxZ->setValue(size[2]);

	for (int i = 0; i < 3; i++)
	{
		realSize[i] = GetSpacing()[i] * size[i];
	}
	this->ui->sizeDoubleSpinBoxX_mm->setValue(realSize[0]);
	this->ui->sizeDoubleSpinBoxY_mm->setValue(realSize[1]);
	this->ui->sizeDoubleSpinBoxZ_mm->setValue(realSize[2]);

	this->ui->volumeDoubleSpinBox->setValue(realSize[0] * realSize[1] * realSize[2] / 1000.);
}

void QInteractorStyleVOI::ExtractVOI()
{
	int extent[6];
	const double* bounds = m_roi->GetRepresentation()->GetBounds();
	for (int i = 0; i < 3; ++i) {
		extent[i*2] = (bounds[i*2] - GetOrigin()[i]) / GetSpacing()[i];
		extent[i*2 + 1] = (bounds[i*2 + 1] - GetOrigin()[i]) / GetSpacing()[i];
		// not voi clamping, the extraction can be more flexible
		//extent[i*2] = extent[i*2] > GetExtent()[i*2] ? extent[i*2] : GetExtent()[i*2];
		//extent[i*2 + 1] = extent[i*2 + 1] < GetExtent()[i*2 + 1] ? extent[i*2 + 1] : GetExtent()[i*2 +1];
	}
	GetImageViewer()->SetDisplayExtent(extent);
	SetExtentRange(GetImageViewer()->GetDisplayExtent());
	GetImageViewer()->Render();
}

void QInteractorStyleVOI::ResetVOI()
{
	GetImageViewer()->ResetDisplayExtent();
	SetExtentRange(GetImageViewer()->GetDisplayExtent());
	GetImageViewer()->Render();
}

QInteractorStyleVOI::QInteractorStyleVOI(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleVOI::~QInteractorStyleVOI()
{
	QDELETE_UI();
	m_renderWindow = nullptr;
	m_roi = nullptr;
}

void QInteractorStyleVOI::uniqueInitialization()
{
	if (m_roi == nullptr) {
		m_roi = vtkSmartPointer<vtkROIWidget>::New();
		m_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		m_renderWindow->OffScreenRenderingOn();
		m_renderWindow->AddRenderer(vtkSmartPointer<vtkRenderer>::New());
		vtkSmartPointer<vtkRenderWindowInteractor> interactor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		m_renderWindow->SetInteractor(interactor);
		//interactor->Start();
		m_roi->SetInteractor(interactor);
		m_roi->GetRepresentation()->SetPlaceFactor(1);
	}
	/// ROI
	connect(m_roi, SIGNAL(signalROIBounds(double*)),
		this, SLOT(slotUpdateVOISpinBoxes(double*)));
}

void QInteractorStyleVOI::initialization()
{
	m_uniqueROIId = numOfMyself - 1;

	connect(ui->pushButtonResetVOI, SIGNAL(clicked()),
		this, SLOT(ResetVOI()));
	connect(ui->pushButtonExtractVOI, SIGNAL(clicked()),
		this, SLOT(ExtractVOI()));
}

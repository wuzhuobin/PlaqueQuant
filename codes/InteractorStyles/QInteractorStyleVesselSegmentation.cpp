#include "QInteractorStyleVesselSegmentation.h"
#include "ui_qinteractorstylevesselsegmentation.h"
#include "ui_QAbstractNavigation.h"

vtkStandardNewMacro(QInteractorStyleVesselSegmentation);
QSETUP_UI_SRC(QInteractorStyleVesselSegmentation);
void QInteractorStyleVesselSegmentation::UniqueEnable(bool flag)
{
	QAbstractNavigation::UniqueEnable(flag);
	if (flag && flag != initializationFlag) {
		// turn on codes

		connect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
	}
	// turn off
	if (!flag && flag != initializationFlag) {
		// turn off codes
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));

	}
	if (flag != initializationFlag) {
		ui->fillSlicesBeginSpinBox->setValue(GetExtent()[4]);
		ui->fillSliceEndSpinBox->setValue(GetExtent()[5]);
		ui->fillSlicesBeginSpinBox->setRange(GetExtent()[4], GetExtent()[5]);
		ui->fillSliceEndSpinBox->setRange(GetExtent()[4], GetExtent()[5]);
	}
	initializationFlag = flag;



}
void QInteractorStyleVesselSegmentation::SetPolygonModeEnabled(bool flag)
{
	InteractorStyleVesselSegmentation::SetPolygonModeEnabled(flag);
	UniqueEnable(flag);
}
void QInteractorStyleVesselSegmentation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleVesselSegmentation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleVesselSegmentation::FillPolygon()
{
	InteractorStyleVesselSegmentation::FillPolygon();
}

void QInteractorStyleVesselSegmentation::SetSmoothCurveEnable(bool flag)
{
	if (flag) {
		InteractorStyleVesselSegmentation::SetSmoothCurveEnable();
	}
}

void QInteractorStyleVesselSegmentation::SetPolygonEnable(bool flag)
{
	if (flag) {
		InteractorStyleVesselSegmentation::SetPolygonEnable();
	}
}

void QInteractorStyleVesselSegmentation::EnableNoSegmentation(bool flag)
{
	if (flag)
		InteractorStyleVesselSegmentation::EnableNoSegmentation();
}

void QInteractorStyleVesselSegmentation::EnableLumenSegmentation(bool flag)
{
	if(flag)
		InteractorStyleVesselSegmentation::EnableLumenSegmentation();
}

void QInteractorStyleVesselSegmentation::EnableVesselWallSegmentation(bool flag)
{
	if (flag)
		InteractorStyleVesselSegmentation::EnableVesselWallSegmentation();
}

void QInteractorStyleVesselSegmentation::SetContourLabel(int label)
{
	InteractorStyleVesselSegmentation::SetContourLabel(label + 1);
}

void QInteractorStyleVesselSegmentation::SetGenerateValue(int value)
{
	InteractorStyleVesselSegmentation::SetGenerateValue(value);
	InteractorStyleVesselSegmentation::GenerateLumenPolydata();

}

void QInteractorStyleVesselSegmentation::SetDilateValue(double value)
{
	InteractorStyleVesselSegmentation::SetDilateValue(value);
}

void QInteractorStyleVesselSegmentation::GenerateLumenWallContourWidget()
{
	//InteractorStyleVesselSegmentation::SetGenerateValue(ui->autoLumenSegmentationSpinBox->value());
	switch (m_contourType)
	{
	case VESSEL_WALL:
		GenerateLumenPolydata();
		break;
	case LUMEN:
		GenerateVesselWallPolyData();
		break;
	default:
		break;
	}
	
}

void QInteractorStyleVesselSegmentation::FillPolygonThroughSlice()
{
	InteractorStyleVesselSegmentation::FillPolygonThroughSlice(
		ui->fillSlicesBeginSpinBox->value(),
		ui->fillSliceEndSpinBox->value());

}

void QInteractorStyleVesselSegmentation::CleanOne()
{
	CleanCurrentContour();
	WriteToPolydata();
}

void QInteractorStyleVesselSegmentation::CleanAll()
{
	CleanAllContours();
	WriteToPolydata();
}

QInteractorStyleVesselSegmentation::QInteractorStyleVesselSegmentation(int uiType, QWidget * parent)
{
	QNEW_UI();
	if (numOfMyself == 1) {
		ui->vesselFrame->setHidden(true);

	}
	connect(ui->polygonRadionButton, SIGNAL(toggled(bool)), this, SLOT(SetPolygonEnable(bool)));
	connect(ui->smoothCurveRadioButton, SIGNAL(toggled(bool)), this, SLOT(SetSmoothCurveEnable(bool)));
	connect(ui->fillContourPushButton, SIGNAL(clicked()), this, SLOT(FillPolygon()));
	connect(ui->fillSlicesPushButton, SIGNAL(clicked()), this, SLOT(FillPolygonThroughSlice()));
	connect(ui->lumenSegmentationRadioButton, SIGNAL(toggled(bool)),
		this, SLOT(EnableLumenSegmentation(bool)));
	connect(ui->vesselWallSegmentationRadioButton, SIGNAL(toggled(bool)),
		this, SLOT(EnableVesselWallSegmentation(bool)));
	connect(ui->noSegmentationRadioButton, SIGNAL(toggled(bool)),
		this, SLOT(EnableNoSegmentation(bool)));
	connect(ui->labelComboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetContourLabel(int)));
	connect(ui->generatePushButton, SIGNAL(clicked()),
		this, SLOT(GenerateLumenWallContourWidget()));
	connect(ui->autoLumenSegmentationSpinBox, SIGNAL(valueChanged(int)), 
		this, SLOT(SetGenerateValue(int)), Qt::QueuedConnection);
	connect(ui->doubleSpinBoxVesselWallThickness, SIGNAL(valueChanged(double)),
		this, SLOT(SetDilateValue(double)));
	connect(ui->cleanOnePushButton, SIGNAL(clicked()),
		this, SLOT(CleanOne()));
	connect(ui->cleanAllPushButton, SIGNAL(clicked()),
		this, SLOT(CleanAll()));
	//connect(ui->autoLumenSegmentationHorizontalSlider, SIGNAL(valueChanged(int)),
	//	this, SLOT(GenerateLumenWallContourWidget()), Qt::QueuedConnection);
}

QInteractorStyleVesselSegmentation::~QInteractorStyleVesselSegmentation() 
{
	QDELETE_UI();
}

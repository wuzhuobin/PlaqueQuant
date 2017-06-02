#include "QInteractorStyleThreshold.h"
#include "ui_QInteractorStyleThreshold.h"
#include "ui_QAbstractNavigation.h"
#include "ImageViewer.h"

#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkImageThreshold.h>
#include <vtkImageCast.h>

#include <qpushbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlabel.h>

vtkStandardNewMacro(QInteractorStyleThreshold);
QSETUP_UI_SRC(QInteractorStyleThreshold);
using namespace std;
QInteractorStyleThreshold::QInteractorStyleThreshold(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleThreshold::~QInteractorStyleThreshold()
{
	QDELETE_UI();
}

void QInteractorStyleThreshold::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
	UpdateTargetViewer();
}

void QInteractorStyleThreshold::SetThresholdByViewer(double lower, double upper)
{
	InteractorStyleThreshold::SetThresholdByViewer(lower, upper);

	m_spinBoxLowerThreshold->setValue(qRound(lower));
	m_spinBoxUpperThreshold->setValue(qRound(upper));
}

void QInteractorStyleThreshold::UpdateTargetViewer()
{
	QStringList listOfModalityNames;
	//QStringList listOfModalityName;
	ui->comboBoxTargeImage->clear();
	listOfModalityNames.clear();
	for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleThreshold* _style = QInteractorStyleThreshold::SafeDownCast(*cit);
		// because not all have been CustomEnabled this time
		if (_style /*&& _style->GetCustomEnabled()*/) {
			listOfModalityNames.append(QString::fromStdString(_style->GetImageViewer()->GetWindowName()));
		}
	}
	listOfModalityNames.removeDuplicates();
	//if (listOfModalityNames != listOfModalityName)
	//{
	for (int i = 0; i < listOfModalityNames.size(); ++i) {
		ui->comboBoxTargeImage->addItem(listOfModalityNames[i]);
	}
	//}
}

void QInteractorStyleThreshold::initialization()
{
	m_label = new QLabel(this);
	m_spinBoxUpperThreshold = new QSpinBox(this);
	m_spinBoxLowerThreshold = new QSpinBox(this);
	m_sliderUpperThreshold = new QSlider(Qt::Horizontal, this);
	m_sliderLowerThreshold = new QSlider(Qt::Horizontal, this);
	m_pushButtonReset = new QPushButton("Reset", this);


	ui->gridLayout->addWidget(m_label, numOfMyself, 0);
	ui->gridLayout->addWidget(m_spinBoxLowerThreshold, numOfMyself, 1);
	ui->gridLayout->addWidget(m_sliderLowerThreshold, numOfMyself, 2);
	ui->gridLayout->addWidget(m_spinBoxUpperThreshold, numOfMyself, 3);
	ui->gridLayout->addWidget(m_sliderUpperThreshold, numOfMyself, 4);
	ui->gridLayout->addWidget(m_pushButtonReset, numOfMyself, 5);

	connect(m_pushButtonReset, SIGNAL(clicked()),
		this, SLOT(ResetWindowLevel()), Qt::UniqueConnection);
	connect(m_spinBoxUpperThreshold, SIGNAL(valueChanged(int)),
		this, SLOT(SetUpperThreshold(int)), Qt::UniqueConnection);
	connect(m_spinBoxLowerThreshold, SIGNAL(valueChanged(int)),
		this, SLOT(SetLowerThreshold(int)), Qt::UniqueConnection);
	connect(m_spinBoxUpperThreshold, SIGNAL(valueChanged(int)),
		m_sliderUpperThreshold, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_spinBoxLowerThreshold, SIGNAL(valueChanged(int)),
		m_sliderLowerThreshold, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_sliderUpperThreshold, SIGNAL(valueChanged(int)),
		m_spinBoxUpperThreshold, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_sliderLowerThreshold, SIGNAL(valueChanged(int)),
		m_spinBoxLowerThreshold, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui->checkBoxPreview, SIGNAL(toggled(bool)),
		this, SLOT(SetPreview(bool)), Qt::UniqueConnection);

	// set brushShape
	connect(ui->comboBoxLabel, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetOutputLabel(int)));
}

void QInteractorStyleThreshold::uniqueInitialization()
{
	connect(ui->pushButtonThreshold, SIGNAL(clicked()),
		this, SLOT(ThresholdTargetViewerToOverlay()));
}

void QInteractorStyleThreshold::SetCustomEnabled(bool flag)
{
	InteractorStyleThreshold::SetCustomEnabled(flag);
	uniqueInvoke(flag);
	if (flag) {
		double* range = GetImageViewer()->GetInput()->GetScalarRange();
		m_label->setText(GetImageViewer()->GetWindowName());
		m_spinBoxUpperThreshold->setRange(range[0], range[1]);
		m_sliderUpperThreshold->setRange(range[0], range[1]);
		m_spinBoxLowerThreshold->setRange(range[0], range[1]);
		m_sliderLowerThreshold->setRange(range[0], range[1]);
		m_spinBoxLowerThreshold->setValue(m_threshold->GetLowerThreshold());
		m_spinBoxUpperThreshold->setValue(m_threshold->GetUpperThreshold());
	}
}

void QInteractorStyleThreshold::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleThreshold::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleThreshold::SetLowerThreshold(int lower)
{
	if (qRound(GetLevel() - GetWindow()*0.5 ) != lower) {
		//SetLowerThreshold(lower);
		if (lower > m_spinBoxUpperThreshold->value()) {
			SetThreshold(lower, lower);
		}
		else {
			SetThreshold(lower, m_spinBoxUpperThreshold->value());
		}
	}
}

void QInteractorStyleThreshold::SetUpperThreshold(int upper)
{
	if (qRound(m_threshold->GetUpperThreshold() ) != upper) {
		if (upper < m_spinBoxLowerThreshold->value()) {
			SetThreshold(upper, upper);
		}
		else {
			SetThreshold(m_spinBoxLowerThreshold->value(), upper);
		}
		//SetUpperThreshold(upper);
	}
}

void QInteractorStyleThreshold::ThresholdTargetViewerToOverlay()
{

	for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleThreshold* _style = QInteractorStyleThreshold::SafeDownCast(*cit);
		if (_style && _style->GetCustomEnabled() && 
			QString::fromStdString(_style->GetImageViewer()->GetWindowName()) ==
			ui->comboBoxTargeImage->currentText()) {
			_style->ThresholdToOverlay();
			break;
		}
	}
}

void QInteractorStyleThreshold::SetOutputLabel(int label)
{
	InteractorStyleThreshold::SetOutputLabel(label);
}

void QInteractorStyleThreshold::ResetWindowLevel()
{
	InteractorStyleThreshold::ResetWindowLevel();
}

void QInteractorStyleThreshold::SetPreview(bool flag)
{
	InteractorStyleThreshold::SetPreview(flag);

}

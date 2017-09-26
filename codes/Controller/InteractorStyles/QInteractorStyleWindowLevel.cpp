#include "QInteractorStyleWindowLevel.h"
#include "ui_qinteractorstylewindowlevel.h"
#include "ui_QAbstractNavigation.h"
#include "ImageViewer.h"

#include <vtkImageData.h>
#include <vtkObjectFactory.h>

#include <qpushbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlabel.h>

vtkStandardNewMacro(QInteractorStyleWindowLevel);
QSETUP_UI_SRC(QInteractorStyleWindowLevel);

QInteractorStyleWindowLevel::QInteractorStyleWindowLevel(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleWindowLevel::~QInteractorStyleWindowLevel()
{
	QDELETE_UI();
}

void QInteractorStyleWindowLevel::SetWindowByViewer(double window)
{
	InteractorStyleWindowLevel::SetWindowByViewer(window);
	m_spinBoxWindowWidth->setValue(
		static_cast<int>(qRound(window)));
}

void QInteractorStyleWindowLevel::SetLevelByViewer(double level)
{
	InteractorStyleWindowLevel::SetLevelByViewer(level);
	m_spinBoxWindowLevel->setValue(
		static_cast<int>(qRound(level)));
}

void QInteractorStyleWindowLevel::initialization()
{
	m_label = new QLabel(this);
	QLabel* width = new QLabel("Width", this);
	QLabel* level = new QLabel("Level", this);
	m_spinBoxWindowLevel = new QSpinBox(this);
	m_spinBoxWindowWidth = new QSpinBox(this);
	m_sliderWindowLevel = new QSlider(Qt::Horizontal, this);
	m_sliderWindowWidth = new QSlider(Qt::Horizontal, this);
	m_pushButtonReset = new QPushButton("Reset", this);

	ui->gridLayout->addWidget(m_label, numOfMyself * 3, 0);
	ui->gridLayout->addWidget(m_pushButtonReset, numOfMyself * 3, 2);
	ui->gridLayout->addWidget(width, numOfMyself * 3 + 1, 0);
	ui->gridLayout->addWidget(m_spinBoxWindowWidth, numOfMyself * 3 + 1, 1);
	ui->gridLayout->addWidget(m_sliderWindowWidth, numOfMyself * 3 + 1, 2);
	ui->gridLayout->addWidget(level, numOfMyself * 3 + 2, 0);
	ui->gridLayout->addWidget(m_spinBoxWindowLevel, numOfMyself * 3 + 2, 1);
	ui->gridLayout->addWidget(m_sliderWindowLevel, numOfMyself * 3 + 2, 2);


	connect(m_pushButtonReset, SIGNAL(clicked()),
		this, SLOT(ResetWindowLevel()), Qt::UniqueConnection);
	connect(m_spinBoxWindowLevel, SIGNAL(valueChanged(int)),
		this, SLOT(SetLevel(int)), Qt::UniqueConnection);
	connect(m_spinBoxWindowWidth, SIGNAL(valueChanged(int)),
		this, SLOT(SetWindow(int)), Qt::UniqueConnection);
	connect(m_spinBoxWindowLevel, SIGNAL(valueChanged(int)),
		m_sliderWindowLevel, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_spinBoxWindowWidth, SIGNAL(valueChanged(int)),
		m_sliderWindowWidth, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_sliderWindowLevel, SIGNAL(valueChanged(int)),
		m_spinBoxWindowLevel, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_sliderWindowWidth, SIGNAL(valueChanged(int)),
		m_spinBoxWindowWidth, SLOT(setValue(int)), Qt::UniqueConnection);
}

void QInteractorStyleWindowLevel::SetCustomEnabled(bool flag)
{
	InteractorStyleWindowLevel::SetCustomEnabled(flag);
	uniqueInvoke(flag);
	if (flag) {
		double* range = GetImageViewer()->GetInput()->GetScalarRange();
		m_label->setText(GetImageViewer()->GetWindowName());
		m_spinBoxWindowLevel->setRange(range[0], range[1]);
		m_sliderWindowLevel->setRange(range[0], range[1]);
		m_spinBoxWindowWidth->setRange(range[0], range[1]);
		m_sliderWindowWidth->setRange(range[0], range[1]);
		m_spinBoxWindowWidth->setValue(GetWindow());
		m_spinBoxWindowLevel->setValue(GetLevel());
	}
}

void QInteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleWindowLevel::SetWindow(int window)
{
	if (static_cast<int>(GetWindow()) != window) {
		InteractorStyleWindowLevel::SetWindow(window);
	}
}

void QInteractorStyleWindowLevel::SetLevel(int level)
{
	if (static_cast<int>(GetLevel()) != level) {
		InteractorStyleWindowLevel::SetLevel(level);
	}
}

void QInteractorStyleWindowLevel::ResetWindowLevel()
{
	InteractorStyleWindowLevel::ResetWindowLevel();
}
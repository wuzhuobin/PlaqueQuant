#include "QInteractorStyleSurfaceCenterLineICDAStandard.h"
#include "ui_QInteractorStyleSurfaceCenterLineICDAStandard.h"

#include <vtkObjectFactory.h>

vtkStandardNewMacro(QInteractorStyleSurfaceCenterLineICDAStandard);
QSETUP_UI_SRC(QInteractorStyleSurfaceCenterLineICDAStandard);

void QInteractorStyleSurfaceCenterLineICDAStandard::setCurrentMode(int mode)
{
	if (mode < 0 || mode > 5) {
		return;
	}
	m_currentMode = mode;
}

void QInteractorStyleSurfaceCenterLineICDAStandard::FindMaximumRadius()
{
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::FindMaximumRadius();
	switch (m_currentMode)
	{
	case LEFT_CAVERNOUS_SEGMENT_OF_INTERNAL_CAROTID_ARTERY:
		ui->doubleSpinBoxCavernousSegmentOfLeftInternalCarotidArtery->setValue(m_maxRadius * 2);
		break;
	case RIGHT_CAVERNOUS_SEGMENT_OF_INTERNAL_CAROTID_ARTERY:
		ui->doubleSpinBoxCavernousSegmentOfRightInternalCarotidArtery->setValue(m_maxRadius * 2);
		break;
	case LEFT_MIDDLE_CEREBRAL_ARTERY:
		ui->doubleSpinBoxLeftMiddleCerebralArteryM1->setValue(m_maxRadius * 2);
		break;
	case RIGHT_MIDDLE_CEREBRAL_ARTERY:
		ui->doubleSpinBoxRightMiddleCerebralArteryM1->setValue(m_maxRadius * 2);
		break;
	case LEFT_ANTERIOR_CEREBRAL_ARTERY:
		ui->doubleSpinBoxLeftAnteriorCerebralArtery->setValue(m_maxRadius * 2);
		break;
	case RIGHT_ANTERIOR_CEREBRAL_ARTERY:
		ui->doubleSpinBoxRightAnteriorCerebralArtery->setValue(m_maxRadius * 2);
		break;
	default:
		break;
	}
}

QInteractorStyleSurfaceCenterLineICDAStandard::QInteractorStyleSurfaceCenterLineICDAStandard(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleSurfaceCenterLineICDAStandard::~QInteractorStyleSurfaceCenterLineICDAStandard()
{
	QDELETE_UI();
}

void QInteractorStyleSurfaceCenterLineICDAStandard::initialization()
{
	connect(ui->pushButtonCavernousSegmentOfLeftInternalCarotidArtery, SIGNAL(clicked()),
		this, SLOT(SetCurrentModeToLeftCavernousSegmentOfInternalCarotidArtery()));
	connect(ui->pushButtonCavernousSegmentOfRightInternalCarotidArtery, SIGNAL(clicked()),
		this, SLOT(SetCurrentModeToRightCavernousSegmentOfInternalCarotidArtery()));
	connect(ui->pushButtonLeftMiddleCerebralArteryM1, SIGNAL(clicked()),
		this, SLOT(SetCurrentModeToLeftMiddleCerebralArtery()));
	connect(ui->pushButtonRightMiddleCerebralArteryM1, SIGNAL(clicked()),
		this, SLOT(SetCurrentModeToRightMiddleCerebralArtery()));
	connect(ui->pushButtonLeftAnteriorCerebralArtery, SIGNAL(clicked()),
		this, SLOT(SetCurrentModeToLeftAnteriorCerebralArtery()));
	connect(ui->pushButtonRightAnteriorCerebralArtery, SIGNAL(clicked()),
		this, SLOT(SetCurrentModeToRightAnteriorCerebralArtery()));

	connect(ui->doubleSpinBoxCavernousSegmentOfLeftInternalCarotidArtery, SIGNAL(valueChanged(double)), 
		this, SIGNAL(diameterChangedLeftCavernousSegmentOfInternalCarotidArtery(double)));
	connect(ui->doubleSpinBoxCavernousSegmentOfRightInternalCarotidArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(diameterChangedRightCavernousSegmentOfInternalCarotidArtery(double)));
	connect(ui->doubleSpinBoxLeftMiddleCerebralArteryM1, SIGNAL(valueChanged(double)),
		this, SIGNAL(diameterChangedLeftMiddleCerebralArtery(double)));
	connect(ui->doubleSpinBoxRightMiddleCerebralArteryM1, SIGNAL(valueChanged(double)),
		this, SIGNAL(diameterChangedRightMiddleCerebralArtery(double)));
	connect(ui->doubleSpinBoxLeftAnteriorCerebralArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(diameterChangedLeftAnteriorCerebralArtery(double)));
	connect(ui->doubleSpinBoxRightAnteriorCerebralArtery, SIGNAL(valueChanged(double)),
		this, SIGNAL(diameterChangedRightAnteriorCerebralArtery(double)));


}

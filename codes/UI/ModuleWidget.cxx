#include "ui_ModuleWidget.h"

#include <QTextBrowser>
#include <qtablewidget.h>

#include "ModuleWidget.h"


ModuleWidget::ModuleWidget(QWidget *parent) :
	QWidget(parent),
	m_contourRadioButtonGroup(this)
{

	ui = new Ui::ModuleWidget;
	m_mainWnd = static_cast<MainWindow*>(parent);
	Core* core = m_mainWnd->m_core;
    ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
    
	// setting of the contourWidget of auto lumen segmentation
	this->ui->autoLumenSegmentationHorizontalSlider->setEnabled(false);
	this->ui->autoLumenSegmentationSpinBox->setValue(
		this->ui->autoLumenSegmentationHorizontalSlider->value());
	this->ui->autoLumenSegmentationSpinBox->setMaximum(
	this->ui->autoLumenSegmentationHorizontalSlider->maximum());
	this->ui->autoLumenSegmentationSpinBox->setMinimum(
		this->ui->autoLumenSegmentationHorizontalSlider->minimum());
	this->ui->autoLumenSegmentationSpinBox->setEnabled(false);
	this->ui->autoLumenSegmentationCheckBox->setChecked(false);
	this->ui->lumenComboBox->setHidden(true);
	this->ui->lumenLabel->setHidden(true);
	this->m_contourRadioButtonGroup.addButton(this->ui->polygonRadionButton);
	this->m_contourRadioButtonGroup.addButton(this->ui->smoothCurveRadioButton);
	this->m_contourRadioButtonGroup.setExclusive(true);
	/// Polygon segmentation
	connect(ui->fillContourPushButton, SIGNAL(clicked()), 
		core, SLOT(slotFillContour()));
	connect(ui->clearContourPushButton, SIGNAL(clicked()),
		core, SLOT(slotClearContour()));
	connect(ui->autoLumenSegmentationSpinBox, SIGNAL(valueChanged(int)), 
		ui->autoLumenSegmentationHorizontalSlider, SLOT(setValue(int)));
	connect(ui->autoLumenSegmentationHorizontalSlider, SIGNAL(valueChanged(int)),
		ui->autoLumenSegmentationSpinBox, SLOT(setValue(int)));
	connect(ui->autoLumenSegmentationCheckBox, SIGNAL(toggled(bool)), this,
		SLOT(slotEnableAutoLumenSegmentation(bool)));
	connect(ui->autoLumenSegmentationCheckBox, SIGNAL(toggled(bool)), core, 
		SLOT(slotEnableAutoLumenSegmentation(bool)));
	connect(ui->autoLumenSegmentationHorizontalSlider, SIGNAL(valueChanged(int)), core,
		SLOT(slotSetContourFilterGenerateValues(int)));
	connect(ui->smoothCurveRadioButton, SIGNAL(toggled(bool)), core,
		SLOT(slotSetLineInterpolatorToSmoothCurve(bool)));
	connect(ui->polygonRadionButton, SIGNAL(toggled(bool)), core,
		SLOT(slotSetLineInterpolatorToPolygon(bool)));
	connect(ui->labelComboBox, SIGNAL(currentIndexChanged(int)),
		core,SLOT(slotSetImageLayerColor(int)));

	/// Paint brush segmentation
	// set brushSize
	connect(ui->BrushSizeSlider, SIGNAL(valueChanged(int)), 
		ui->BrushSizeSpinBox, SLOT(setValue(int)),Qt::UniqueConnection);
	connect(ui->BrushSizeSpinBox, SIGNAL(valueChanged(int)),
		ui->BrushSizeSlider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui->BrushSizeSpinBox, SIGNAL(valueChanged(int)),
		core, SLOT(slotSetBrushSize(int)));
	// set brushShape
	connect(ui->BrushComBox, SIGNAL(currentIndexChanged(int)),
		core, SLOT(slotSetBrushShape(int)));
	// set brushEraserMode
	connect(ui->eraserCheckBox, SIGNAL(toggled(bool)),
		core, SLOT(slotSetPaintBrushToEraser(bool)));
	// set overlay opacity
	connect(ui->opacitySpinBox,					SIGNAL(valueChanged(int)),			this,		SLOT(slotChangeOpacity(int)));
	connect(ui->opacitySlider,					SIGNAL(valueChanged(int)),			ui->opacitySpinBox, SLOT(setValue(int)));
	connect(ui->opacitySpinBox,					SIGNAL(valueChanged(int)),			ui->opacitySlider, SLOT(setValue(int)));

	/// set page
	connect(m_mainWnd->ui->actionNavigation, SIGNAL(triggered()),
		this, SLOT(slotSetPage()));
	connect(m_mainWnd->ui->actionWindowLevel, SIGNAL(triggered()),
		this, SLOT(slotSetPage()));
	connect(m_mainWnd->ui->actionContour, SIGNAL(triggered()),
		this, SLOT(slotSetPage()));
	connect(m_mainWnd->ui->actionBrush, SIGNAL(triggered()),
		this, SLOT(slotSetPage()));
	connect(m_mainWnd->ui->actionRuler, SIGNAL(triggered()),
		this, SLOT(slotSetPage()));
	connect(m_mainWnd->ui->actionROI, SIGNAL(triggered()),
		this, SLOT(slotSetPage()));
	connect(m_mainWnd->ui->actionSmartContour, SIGNAL(triggered()),
		this, SLOT(slotSetPage()));

	/// ROI
	connect(ui->segmentationPushButton,			SIGNAL(clicked()),					core,		SLOT(slotSelectROI()));
	connect(ui->resetROIPushButton,				SIGNAL(clicked()),					core,		SLOT(slotResetROI()));
	connect(ui->maximumWallThicknessChkBox,		SIGNAL(toggled(bool)),				core,		SLOT(slotEnableMaximumWallThickneesLabel(bool)));
}

ModuleWidget::~ModuleWidget()
{
    delete ui;
}

void ModuleWidget::slotSetPage()
{
	QAction* actionSender = dynamic_cast<QAction*>(sender());
	if (actionSender == m_mainWnd->ui->actionNavigation) {
	}
	else if (actionSender == m_mainWnd->ui->actionRuler) {
		ui->stackedWidget->setCurrentIndex(4);
	}
	else if (actionSender == m_mainWnd->ui->actionContour) {
		ui->stackedWidget->setCurrentIndex(2);
	}
	else if (actionSender == m_mainWnd->ui->actionBrush) {
		ui->stackedWidget->setCurrentIndex(3);
	}
	else if (actionSender == m_mainWnd->ui->actionROI) {
		ui->stackedWidget->setCurrentIndex(0);
	}
	else if (actionSender == m_mainWnd->ui->actionSmartContour) {
		ui->stackedWidget->setCurrentIndex(1);

	}

}

void ModuleWidget::slotChangeOpacity(int opacity)
{
	int layer = ui->labelComboBox->currentIndex() + 1;
	m_mainWnd->m_core->slotChangeOpacity(layer, opacity);
}
void ModuleWidget::slotChangeROI(int * bound)
{
	ui->centerSpinBox1->setValue((bound[0] + bound[1])*0.5);
	ui->centerSpinBox2->setValue((bound[2] + bound[3])*0.5);
	ui->centerSpinBox3->setValue((bound[4] + bound[5])*0.5);
	ui->sizeSpinBox1->setValue(bound[1] - bound[0]);
	ui->sizeSpinBox2->setValue(bound[3] - bound[2]);
	ui->sizeSpinBox3->setValue(bound[5] - bound[4]);

}

void ModuleWidget::slotEnableAutoLumenSegmentation(bool flag)
{
	if (flag) {
		this->ui->lumenLabel->setHidden(false);
		this->ui->lumenComboBox->setHidden(false);
		this->ui->autoLumenSegmentationHorizontalSlider->setEnabled(true);
		this->ui->autoLumenSegmentationHorizontalSlider->setValue(
			this->ui->autoLumenSegmentationHorizontalSlider->value());
		this->ui->autoLumenSegmentationSpinBox->setEnabled(true);
		this->ui->labelComboBox->setCurrentIndex(1);
		this->ui->activeLabelLabel->setText("Vessel Wall Label");
	}
	else {
		this->ui->lumenLabel->setHidden(true);
		this->ui->lumenComboBox->setHidden(true);
		this->ui->autoLumenSegmentationHorizontalSlider->setEnabled(false);
		this->ui->autoLumenSegmentationSpinBox->setEnabled(false);
		this->ui->activeLabelLabel->setText("Active Label");

	}
}

	



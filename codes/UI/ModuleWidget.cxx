#include "ui_ModuleWidget.h"

#include <QSpinBox>
#include <QTextBrowser>
#include <qtablewidget.h>

#include "ModuleWidget.h"

//#define setValueConnection(qtObj, filterName, valueName, dataType) \
//connect(ui->##qtObj, SIGNAL(valueChanged(##dataType), ##filterName, SIGNAL(slotSet##filterName##valueName(##dataType))

std::vector<int*>		ModuleWidget::SeedIJKList;
std::vector<double*>	ModuleWidget::SeedCoordinatesList;

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
	
	/// UI Linkage
	connect(ui->sliderInitNeighborhoodRadius,		SIGNAL(valueChanged(int)), 
		this, SLOT(slotChangeSliderNeighborhoodRadius()));
	connect(ui->doubleSpinBoxNeighborRadius,		SIGNAL(valueChanged(double)), 
		this, SLOT(slotChangeSpinBoxNeighborhoodRadius()));
	connect(ui->sliderVesselWallThickness,			SIGNAL(valueChanged(int)), 
		this, SLOT(slotChangeSliderVesselWallThickness()));
	connect(ui->doubleSpinBoxVesselWallThickness,	SIGNAL(valueChanged(double)), 
		this, SLOT(slotChangeSpinBoxVesselWallThickness()));
	
	/// Seed operations
	connect(ui->listWidgetSeedList, SIGNAL(currentRowChanged(int)),
		this, SLOT(slotUpdateCoordinateLabel()));
	connect(ui->pushBtnDeleteSeed,	SIGNAL(clicked()), this, SLOT(slotDeleteCurrentSeed()));

	/// Polygon segmentation
	connect(ui->fillContourPushButton,	SIGNAL(clicked()), 
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
	connect(this->m_mainWnd->GetCore()->GetMyWidgetManager()->GetROIWidget(), SIGNAL(signalROIBounds(double*)), 
		this, SLOT(slotUpdateROISpinBoxes(double*)));
	connect(ui->segmentationPushButton,			SIGNAL(clicked()),		core,		SLOT(slotSelectROI()));
	connect(ui->resetROIPushButton,				SIGNAL(clicked()),		core,		SLOT(slotResetROI()));
	connect(ui->maximumWallThicknessChkBox,		SIGNAL(toggled(bool)),	core,		SLOT(slotEnableMaximumWallThickneesLabel(bool)));

}

ModuleWidget::~ModuleWidget()
{
    delete ui;
}

void ModuleWidget::UdateTargetImageComboBox()
{
	MyImageManager* imageManager = this->m_mainWnd->GetCore()->GetMyImageManager();
	if (imageManager->getNumberOfImages() == 0)
		return;

	this->ui->comboBoxTargeImage->clear();
	for (int i = 0; i < imageManager->getNumberOfImages();i++)
	{
		this->ui->comboBoxTargeImage->addItem(imageManager->getListOfModalityNames()[i]);
	}
}

void ModuleWidget::UpdateSeedListView()
{
	if (SeedIJKList.size() == 0)
		return;
	
	QListWidget* listWidget = this->ui->listWidgetSeedList;
	listWidget->clear();

	for each (int* ijk in SeedIJKList)
	{
		char item[100];
		sprintf_s(item, "Seed Index: [%i, %i, %i]", ijk[0], ijk[1], ijk[2]);
		listWidget->addItem(QString(item));
	}
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

	
void ModuleWidget::slotUpdateROISpinBoxes(double* bounds)
{
	Core* core = m_mainWnd->GetCore();
	int* extent = core->ConvertBoundsToExtent(bounds, false);

	int size[] = { 
		extent[1] - extent[0] + 1,
		extent[3] - extent[2] + 1,
		extent[5] - extent[4] + 1
	};

	this->ui->sizeSpinBoxX->setValue(size[0]);
	this->ui->sizeSpinBoxY->setValue(size[1]);
	this->ui->sizeSpinBoxZ->setValue(size[2]);

	if (this->m_mainWnd->GetCore()->GetMyImageManager()->getNumberOfImages() == 0)
		return;

	double spacing[3];
	double realSize[3];
	this->m_mainWnd->GetCore()->GetMyImageManager()->getListOfViewerInputImages().at(0)->GetSpacing(spacing);
	for (int i = 0; i < 3;i++)
	{
		realSize[i] = spacing[i] * size[i];
	}
	this->ui->sizeDoubleSpinBoxX_mm->setValue(realSize[0]);
	this->ui->sizeDoubleSpinBoxY_mm->setValue(realSize[1]);
	this->ui->sizeDoubleSpinBoxZ_mm->setValue(realSize[2]);

	this->ui->volumeDoubleSpinBox->setValue(realSize[0] * realSize[1] * realSize[2] / 1000.);
}

void ModuleWidget::slotChangeSliderNeighborhoodRadius()
{
	int sliderValue = this->ui->sliderInitNeighborhoodRadius->value();
	this->ui->doubleSpinBoxNeighborRadius->setValue(sliderValue / 10.);
}

void ModuleWidget::slotChangeSpinBoxNeighborhoodRadius()
{
	double spindboxValue = this->ui->doubleSpinBoxNeighborRadius->value();
	this->ui->sliderInitNeighborhoodRadius->setValue(floor(spindboxValue * 10));
}

void ModuleWidget::slotChangeSliderVesselWallThickness()
{
	int sliderValue = this->ui->sliderVesselWallThickness->value();
	this->ui->doubleSpinBoxVesselWallThickness->setValue(sliderValue / 10.);
}

void ModuleWidget::slotChangeSpinBoxVesselWallThickness()
{
	double spindboxValue = this->ui->doubleSpinBoxVesselWallThickness->value();
	this->ui->sliderVesselWallThickness->setValue(floor(spindboxValue * 10));
}


void ModuleWidget::slotUpdateSeedListView()
{
	this->UpdateSeedListView();
}

void ModuleWidget::slotDeleteCurrentSeed()
{
	int curIndex = this->ui->listWidgetSeedList->currentRow();
	if (curIndex < 0 || curIndex > SeedIJKList.size() - 1)
		return;

	SeedIJKList.erase(SeedIJKList.begin() + curIndex);
	SeedCoordinatesList.erase(SeedCoordinatesList.begin() + curIndex);

	this->slotUpdateSeedListView();
	for (int i = 0; i < 3;i++)
	{
		this->m_mainWnd->m_core->Get2DInteractorStyle(i)->GetSmartContour()->ReloadSeedFromList();
	}

	this->ui->listWidgetSeedList->setCurrentRow({ curIndex > SeedIJKList.size() - 1 ? SeedIJKList.size() - 1 : curIndex });
}

void ModuleWidget::slotUpdateCoordinateLabel()
{
	/// Get current selected seed
	int curIndex = this->ui->listWidgetSeedList->currentRow();
	if (curIndex < 0 || curIndex > SeedIJKList.size() - 1)
		return;
	double* currentSeedIJK = SeedCoordinatesList[curIndex];

	char label[60];
	sprintf_s(label, "(%.3f, %.3f, %.3f)", currentSeedIJK[0], currentSeedIJK[1], currentSeedIJK[2]);

	/// change label text
	this->ui->labelSeedCoordinate->setText(QString(label));
}


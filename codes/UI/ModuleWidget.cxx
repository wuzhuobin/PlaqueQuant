#include "ui_ModuleWidget.h"
#include <QSpinBox>
#include <QTextBrowser>
#include <qtablewidget.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ModuleWidget.h"

#include "ui_LabelWidget.h"

#define setValueConnection(qtObj, filterName, valueName, dataType) \
connect(ui->##qtObj , SIGNAL(valueChanged(##dataType)), core , SLOT(slotSet##filterName##valueName(##dataType)))

//std::vector<int*>		ModuleWidget::SeedIJKList;
//std::vector<double*>	ModuleWidget::SeedCoordinatesList;

ModuleWidget::ModuleWidget(QWidget *parent) :
	QWidget(parent)
{

	ui = new Ui::ModuleWidget;
	m_mainWnd = static_cast<MainWindow*>(parent);
	Core* core = m_mainWnd->m_core;
    ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
	ui->stackedWidget->setHidden(true);
    
	connect(ui->opacitySpinBox, SIGNAL(valueChanged(int)), 
		this, SLOT(slotChangeOpacity(int)));
	connect(ui->labelComboBox, SIGNAL(currentIndexChanged(int)), 
		this, SLOT(slotUpdateOpacity()));
	
	/// Seed operations
	//connect(ui->pushBtnDrawSegmentation, SIGNAL(clicked()),
	//	core, SLOT(slotDrawSegmentation()));
	//connect(ui->listWidgetSeedList,  SIGNAL(currentRowChanged(int)),
	//	this, SLOT(slotSnapToSeed(int)));
	//connect(ui->pushBtnExtractLumen, SIGNAL(clicked()), 
	//	core, SLOT(slotExtractLumen()));
	//connect(ui->listWidgetSeedList, SIGNAL(currentRowChanged(int)),
	//	this, SLOT(slotUpdateCoordinateLabel()));
	//connect(ui->pushBtnDeleteSeed,	SIGNAL(clicked()), 
	//	this, SLOT(slotDeleteCurrentSeed()));
	//connect(ui->comboBoxTargeImage, SIGNAL(currentTextChanged(QString)),
	//	this, SLOT(slotSetExtractLumenTargetImage(QString)));
	//setValueConnection(spinBoxNeighborRadius, ExtractLumen, InitialNeighborhoodRadius, int);
	//setValueConnection(doubleSpinBoxMultiplier, ExtractLumen, Multiplier, double);
	//setValueConnection(spinBoxVesselWallThickness, ExtractLumen, DilationValue, int);


	/// Paint brush segmentation

	/// set page
	//connect(m_mainWnd->ui->actionNavigation, SIGNAL(triggered()),
	//	this, SLOT(slotSetPage()));
	//connect(m_mainWnd->ui->actionWindowLevel, SIGNAL(triggered()),
	//	this, SLOT(slotSetPage()));
	//connect(m_mainWnd->ui->actionContour, SIGNAL(triggered()),
	//	this, SLOT(slotSetPage()));
	//connect(m_mainWnd->ui->actionBrush, SIGNAL(triggered()),
	//	this, SLOT(slotSetPage()));
	//connect(m_mainWnd->ui->actionRuler, SIGNAL(triggered()),
	//	this, SLOT(slotSetPage()));
	//connect(m_mainWnd->ui->actionROI, SIGNAL(triggered()),
	//	this, SLOT(slotSetPage()));
	//connect(m_mainWnd->ui->actionSeedsPlacer, SIGNAL(triggered()),
	//	this, SLOT(slotSetPage()));

	/// ROI
	connect(this->m_mainWnd->GetCore()->GetMyWidgetManager()->GetROIWidget(), SIGNAL(signalROIBounds(double*)), 
		this, SLOT(slotUpdateROISpinBoxes(double*)));
	connect(ui->segmentationPushButton,			SIGNAL(clicked()),		core,		SLOT(slotSelectROI()));
	connect(ui->resetROIPushButton,				SIGNAL(clicked()),		core,		SLOT(slotResetROI()));


}

ModuleWidget::~ModuleWidget()
{
    delete ui;
}

void ModuleWidget::ClearWidget()
{
}

void ModuleWidget::addWidget(QWidget * style)
{
	ui->styleStackedWidget->addWidget(style);
}

void ModuleWidget::setWidget(QWidget * style)
{
	int index = ui->styleStackedWidget->indexOf(style);
	if (index < 0) {
		ui->styleStackedWidget->setDisabled(true);
	}
	else {
		ui->styleStackedWidget->setEnabled(true);
		ui->styleStackedWidget->setCurrentIndex(index);
	}
}

//void ModuleWidget::UdateTargetImageComboBox()
//{
//	MyImageManager* imageManager = this->m_mainWnd->GetCore()->GetMyImageManager();
//	if (imageManager->getNumberOfImages() == 0)
//		return;
//
//	this->ui->comboBoxTargeImage->clear();
//	for (int i = 0; i < imageManager->getListOfModalityNames().size();i++)
//	{
//		if (imageManager->getListOfVtkImages()[i] != NULL)
//		{
//			this->ui->comboBoxTargeImage->addItem(imageManager->getListOfModalityNames()[i]);
//		}
//	}
//}
//
//void ModuleWidget::UpdateSeedListView()
//{
//	if (SeedIJKList.size() == 0) {
//		this->ui->listWidgetSeedList->clear();
//	}
//	
//	QListWidget* listWidget = this->ui->listWidgetSeedList;
//	listWidget->clear();
//
//	for each (int* ijk in SeedIJKList)
//	{
//		char item[100];
//		sprintf_s(item, "Seed Index: [%i, %i, %i]", ijk[0], ijk[1], ijk[2]);
//		listWidget->addItem(QString(item));
//	}
//
//	this->m_mainWnd->m_core->slotSetExtractLumenSeedList(SeedIJKList);
//}

//void ModuleWidget::slotSetPage()
//{
//	QAction* actionSender = dynamic_cast<QAction*>(sender());
//	if (actionSender == m_mainWnd->ui->actionNavigation) {
//	}
//	else if (actionSender == m_mainWnd->ui->actionRuler) {
//		ui->stackedWidget->setCurrentIndex(4);
//	}
//	else if (actionSender == m_mainWnd->ui->actionContour) {
//		ui->stackedWidget->setCurrentIndex(2);
//	}
//	else if (actionSender == m_mainWnd->ui->actionBrush) {
//		ui->stackedWidget->setCurrentIndex(3);
//	}
//	else if (actionSender == m_mainWnd->ui->actionROI) {
//		ui->stackedWidget->setCurrentIndex(0);
//	}
//	else if (actionSender == m_mainWnd->ui->actionSeedsPlacer) {
//		ui->stackedWidget->setCurrentIndex(1);
//
//	}
//
//}

void ModuleWidget::slotChangeOpacity(int opacity)
{
	int layer = ui->labelComboBox->currentIndex() + 1;
	m_mainWnd->m_core->slotChangeOpacity(layer, opacity);
}

void ModuleWidget::slotUpdateOpacity()
{
	int layer = ui->labelComboBox->currentIndex() + 1;
	double rgba[4];
	m_mainWnd->m_core->GetLookupTable()->GetIndexedColor(layer, rgba);
	ui->opacitySpinBox->setValue(rgba[3] * 100+0.5);

}

void ModuleWidget::slotChangeROI(int * bound)
{
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
	this->m_mainWnd->GetCore()->GetMyImageManager()->getListOfVtkImages().at(0)->GetSpacing(spacing);
	for (int i = 0; i < 3;i++)
	{
		realSize[i] = spacing[i] * size[i];
	}
	this->ui->sizeDoubleSpinBoxX_mm->setValue(realSize[0]);
	this->ui->sizeDoubleSpinBoxY_mm->setValue(realSize[1]);
	this->ui->sizeDoubleSpinBoxZ_mm->setValue(realSize[2]);

	this->ui->volumeDoubleSpinBox->setValue(realSize[0] * realSize[1] * realSize[2] / 1000.);
}

//void ModuleWidget::slotChangeSliderNeighborhoodRadius()
//{
//	int sliderValue = this->ui->sliderInitNeighborhoodRadius->value();
//	this->ui->spinBoxNeighborRadius->setValue(sliderValue);
//}
//
//void ModuleWidget::slotChangeSpinBoxNeighborhoodRadius()
//{
//	int spindboxValue = this->ui->spinBoxNeighborRadius->value();
//	this->ui->sliderInitNeighborhoodRadius->setValue(spindboxValue);
//}
//
//void ModuleWidget::slotChangeSliderVesselWallThickness()
//{
//	int sliderValue = this->ui->sliderVesselWallThickness->value();
//	this->ui->spinBoxVesselWallThickness->setValue(sliderValue);
//}
//
//void ModuleWidget::slotChangeSpinBoxVesselWallThickness()
//{
//	int spindboxValue = this->ui->spinBoxVesselWallThickness->value();
//	this->ui->sliderVesselWallThickness->setValue(spindboxValue);
//}
//
////void ModuleWidget::slotUpdateSeedListView()
////{
////	this->UpdateSeedListView();
////}
//
//void ModuleWidget::slotDeleteCurrentSeed()
//{
//	int curIndex = this->ui->listWidgetSeedList->currentRow();
//	if (!this->ui->listWidgetSeedList->isItemSelected(this->ui->listWidgetSeedList->item(curIndex)))
//		return;
//
//	if (curIndex < 0 || curIndex > SeedIJKList.size() - 1)
//	{
//		SeedIJKList.clear();
//		SeedCoordinatesList.clear();
//		return;
//	}
//
//	SeedIJKList.erase(SeedIJKList.begin() + curIndex);
//	SeedCoordinatesList.erase(SeedCoordinatesList.begin() + curIndex);
//	//this->slotUpdateSeedListView();
//
//	for (int i = 0; i < 3;i++)
//	{
//		this->m_mainWnd->m_core->Get2DInteractorStyle(i)->GetSmartContour()->ReloadSeedFromList();
//	}
//
//
//	int newRow = { curIndex > int(SeedIJKList.size() - 1) ? int(SeedIJKList.size() - 1) : curIndex };
//	this->ui->listWidgetSeedList->setCurrentRow(newRow);
//}
//
//void ModuleWidget::slotSetExtractLumenTargetImage(QString modName)
//{
//	MyImageManager* imManager = this->m_mainWnd->m_core->GetMyImageManager();
//	int index = imManager->GetModalityIndex(modName);
//	if (index == -1)
//	{
//		qDebug() << modName << " " << index;
//		return;
//	}
//	this->m_mainWnd->m_core->slotSetExtractLumenInputImage(imManager->getListOfVtkImages()[index]);
//}
//
//void ModuleWidget::slotGenerateContour()
//{
//	// Dilate the modified lumen label to get a label with 
//	this->m_mainWnd->m_core->slotExtractLumenDilateLabel(this->m_mainWnd->m_core->GetMyImageManager()->getOverlay()->GetOutput());
//	// Generate contour operations
//	/* Modfy here */
//	m_mainWnd->m_core->slotSmartContour2Mode();
//}
//
//void ModuleWidget::slotSnapToSeed(int rowIndex)
//{
//	if (SeedIJKList.size() == 0 || rowIndex == -1)
//		return;
//
//	int* seedIJK = ModuleWidget::SeedIJKList[rowIndex];
//
//	//this->m_mainWnd->m_core->slotChangeSlices(seedIJK);
//	for (int i = 0; i < 3;i++)
//	{
//		this->m_mainWnd->m_core->Get2DInteractorStyle(i)->GetSmartContour()->ReloadSeedFromList();
//	}
//}
//
//void ModuleWidget::slotUpdateCoordinateLabel()
//{
//	if (SeedIJKList.size() == 0)
//		return;
//
//	/// Get current selected seed
//	int curIndex = this->ui->listWidgetSeedList->currentRow();
//	if (curIndex < 0 || curIndex > SeedIJKList.size() - 1 || curIndex == -1)
//		return;
//	double* currentSeedIJK = SeedCoordinatesList[curIndex];
//
//	char label[60];
//	sprintf_s(label, "(%.3f, %.3f, %.3f)", currentSeedIJK[0], currentSeedIJK[1], currentSeedIJK[2]);
//
//	/// change label text
//	this->ui->labelSeedCoordinate->setText(QString(label));
//}
//

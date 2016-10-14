#include "ui_ModuleWidget.h"

#include <QTextBrowser>
#include <qtablewidget.h>


#include <vtkLine.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkCellArray.h>

#include "MaximumWallThickness.h"
#include "ModuleWidget.h"


ModuleWidget::ModuleWidget(QWidget *parent) :
	QWidget(parent),
	m_contourRadioButtonGroup(this)
{

	ui = new Ui::ModuleWidget;
	m_mainWnd = static_cast<MainWindow*>(parent);
	Core* core = m_mainWnd->GetCore();
    ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
    
    // Initialize widget
	m_maxParameters = 10;
	this->m_labelActor = vtkSmartPointer<vtkActor2D>::New();
	this->m_lineActor = vtkSmartPointer<vtkActor>::New();
	this->m_displayPD = vtkSmartPointer<vtkPolyData>::New();
	this->m_labelArray = vtkSmartPointer<vtkStringArray>::New();
	this->m_labelArray->SetName("Distance");
	this->m_sizeArray = vtkSmartPointer<vtkIntArray>::New();
	this->m_sizeArray->SetName("Sizes");
	this->m_p2labelfilter = vtkSmartPointer<vtkPointSetToLabelHierarchy>::New();
	this->m_p2labelfilter->SetLabelArrayName("Distance");
	this->m_p2labelfilter->SetPriorityArrayName("Sizes");

	this->m_lineActor->GetProperty()->SetColor(0, 1, 0);
	this->m_lineActor->GetProperty()->SetLineWidth(2);

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

	/// ROI
	connect(ui->segmentationPushButton,			SIGNAL(clicked()),					this,		SLOT(slotSelectROI()));
	connect(ui->resetROIPushButton,				SIGNAL(clicked()),					this,		SLOT(slotResetROI()));
	connect(ui->maximumWallThicknessChkBox,		SIGNAL(stateChanged(int)),			this,		SLOT(slotEnableMWTCalculation(int)));
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
		ui->stackedWidget->setCurrentIndex(3);
	}
	else if (actionSender == m_mainWnd->ui->actionContour) {
		ui->stackedWidget->setCurrentIndex(1);
	}
	else if (actionSender == m_mainWnd->ui->actionBrush) {
		ui->stackedWidget->setCurrentIndex(2);
	}
	else if (actionSender == m_mainWnd->ui->actionROI) {
		ui->stackedWidget->setCurrentIndex(0);
	}

}
void ModuleWidget::UpdateStenosisValue(double val)
{
	//ui->stenosisSpinBox->setValue(val * 100);
}
void ModuleWidget::SetBrushSize()
{
	ui->BrushSizeSpinBox->setValue((ui->BrushSizeSlider->value()));
}

void ModuleWidget::slotSegmentationView()
{
	m_mainWnd->GetCore()->slotSegmentationView();
}

void ModuleWidget::slotChangeOpacity(int opacity)
{
	 
	int layer = ui->labelComboBox->currentIndex() + 1;
	double* value = m_mainWnd->GetLookupTable()->GetTableValue(layer);
	value[3] = opacity * 0.01;
	m_mainWnd->GetLookupTable()->SetTableValue(layer, value);
	m_mainWnd->GetLookupTable()->Build();
	m_mainWnd->RenderAllViewer();
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

void ModuleWidget::slotEnableMWTCalculation(int checked)
{
	 
	Ui_MainWindow* main_ui = m_mainWnd->GetUI();

	switch (checked)
	{
	case Qt::Unchecked:
		// disconnect zSpinBox with MWT calculation
		disconnect(main_ui->zSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotCalculateMaximumWallThickness()));
		// remove line and label actors
		for (int i = 0; i < 3; i++)
		{
			if (m_mainWnd->GetMyImageViewer(i)->GetSliceOrientation() == 2) {
				m_mainWnd->GetMyImageViewer(2)->GetAnnotationRenderer()->RemoveActor(this->m_lineActor);
				m_mainWnd->GetMyImageViewer(2)->GetAnnotationRenderer()->RemoveActor2D(this->m_labelActor);
			}
		}
		m_mainWnd->RenderAll2DViewers();
		break;
	case Qt::Checked:
		connect(main_ui->zSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotCalculateMaximumWallThickness()), Qt::QueuedConnection);
		// connect zSpinBox 
		this->slotCalculateMaximumWallThickness();
		break;
	default:
		break;
	}
}

void ModuleWidget::slotSelectROI()
{
	 
	m_mainWnd->GetCore()->slotSelectROI();
}

void ModuleWidget::slotResetROI()
{
	 
	m_mainWnd->GetCore()->slotResetROI();
}

void ModuleWidget::slotCalculateMaximumWallThickness()
{
	 
	int currentSlice = m_mainWnd->GetUI()->zSpinBox->value();

	vtkImageData* overlayImage = m_mainWnd->imageManager->getOverlay()->GetOutput();

	vtkSmartPointer<MaximumWallThickness> calculator = vtkSmartPointer<MaximumWallThickness>::New();
	calculator->SetImage(overlayImage);
	calculator->SetSliceNumber(currentSlice);
	try {
		calculator->Update();
	} 
	catch (MaximumWallThickness::ERROR_CODE e) {
		// #DisplayErrorMsgHere
		switch (e) {
		case MaximumWallThickness::ERROR_EXTRACT_LOOP:
			this->ui->MWTTextBrowser->setText("Extract loop failed!");
			break;
		case MaximumWallThickness::ERROR_EXTRACT_SLICE:
			this->ui->MWTTextBrowser->setText("Extract slice failed!");
			break;
		case MaximumWallThickness::ERROR_INPUT_NOT_A_SLICE:
			this->ui->MWTTextBrowser->setText("Input is not a slice!");
			break;
		case MaximumWallThickness::ERROR_THICKNESS_CAL:
			this->ui->MWTTextBrowser->setText("");
			break;
		case MaximumWallThickness::ERROR_UNDEFINED_BRANCH:
			this->ui->MWTTextBrowser->setText("Cannot pair up vessel wall and lumen!");
			break;
		case MaximumWallThickness::ERROR_VALUE_TRANSFORM:
			this->ui->MWTTextBrowser->setText("Type casting failed!");
			break;
		default:
			break;
		}

		for (int i = 0; i < 3; i++)
		{
			if (m_mainWnd->GetMyImageViewer(i)->GetSliceOrientation() == 2) {
				m_mainWnd->GetMyImageViewer(i)->GetAnnotationRenderer()->RemoveActor(this->m_lineActor);
				m_mainWnd->GetMyImageViewer(i)->GetAnnotationRenderer()->RemoveActor2D(this->m_labelActor);
			}
		}
		m_mainWnd->RenderAll2DViewers();
		return;
	}
	
	// clean variables
	this->m_labelArray->Reset();
	this->m_sizeArray->Reset();

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	std::vector<MaximumWallThickness::DistanceLoopPair> looppairs = calculator->GetDistanceLoopPairVect();
	for (int i = 0; i < looppairs.size(); i++)
	{
		MaximumWallThickness::DistanceLoopPair l_lp = looppairs.at(i);
		double p1[3], p2[3];
		if (l_lp.LoopPair.first->GetNumberOfPoints() == 0) {
			// #DisplayErrorMsgHere
			this->ui->MWTTextBrowser->setText("Loop pair extraction incorrect!");
			for (int j = 0; j < 3; i++)
			{
				if (m_mainWnd->GetMyImageViewer(j)->GetSliceOrientation() == 2) {
					m_mainWnd->GetMyImageViewer(j)->GetAnnotationRenderer()->RemoveActor(this->m_lineActor);
					m_mainWnd->GetMyImageViewer(j)->GetAnnotationRenderer()->RemoveActor2D(this->m_labelActor);
				}
			}
			m_mainWnd->RenderAll2DViewers();
			return;
		}

		l_lp.LoopPair.first->GetPoint(l_lp.PIDs.first, p1);
		l_lp.LoopPair.second->GetPoint(l_lp.PIDs.second, p2);

		points->InsertNextPoint(p1);
		points->InsertNextPoint(p2);
		
		cells->InsertNextCell(2);
		cells->InsertCellPoint(points->GetNumberOfPoints() - 2);
		cells->InsertCellPoint(points->GetNumberOfPoints() - 1);

		// Update UI
		this->m_sizeArray->InsertNextValue(3);
		this->m_sizeArray->InsertNextValue(3);

		char distanceString[25];
		sprintf_s(distanceString, "%.3f mm", l_lp.Distance);
		this->m_labelArray->InsertNextValue(distanceString);
		this->m_labelArray->InsertNextValue(" ");

	}

	this->m_displayPD->SetPoints(points);
	this->m_displayPD->SetLines(cells);
	this->m_displayPD->GetPointData()->AddArray(this->m_labelArray);
	this->m_displayPD->GetPointData()->AddArray(this->m_sizeArray);

	this->m_p2labelfilter->SetInputData(this->m_displayPD);
	this->m_p2labelfilter->Update();

	vtkSmartPointer<vtkLabelPlacementMapper> labelMapper = vtkSmartPointer<vtkLabelPlacementMapper>::New();
	labelMapper->SetInputConnection(this->m_p2labelfilter->GetOutputPort());
	this->m_labelActor->SetMapper(labelMapper);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(this->m_displayPD);
	mapper->Update();

	this->m_lineActor->SetMapper(mapper);

	// #MyViewerHardCode
	for (int i = 0; i < 3;i++)
	{
		if (m_mainWnd->GetMyImageViewer(i)->GetSliceOrientation() == 2) {
			m_mainWnd->GetMyImageViewer(i)->GetAnnotationRenderer()->AddActor(this->m_lineActor);
			m_mainWnd->GetMyImageViewer(i)->GetAnnotationRenderer()->AddActor2D(this->m_labelActor);
		}
	}

	// reset error message
	this->ui->MWTTextBrowser->setText("Normal.");
	m_mainWnd->RenderAll2DViewers();
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

void ModuleWidget::slotUpdateTableWidget()
{
	//QTableWidget* table = ui->measurement2DTableWidget->
}
	
void ModuleWidget::setMainWindow(MainWindow* mainWnd)
{
	m_mainWnd = mainWnd;
}


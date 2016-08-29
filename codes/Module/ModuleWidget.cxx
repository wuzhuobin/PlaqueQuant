#include <vtkLine.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkCellArray.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkImageResize.h>

#include "MaximumWallThickness.h"
#include "MeasurementFor2D.h"
#include "ModuleWidget.h"
#include "ui_ModuleWidget.h"

#include <qtablewidget.h>


ModuleWidget::ModuleWidget(QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::ModuleWidget)
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();

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
	


    /// Initialize the widget
	this->ui->BrushSizeSlider->setMaximum(40);
	this->ui->BrushSizeSlider->setMinimum(1);
	this->ui->BrushSizeSpinBox->setMaximum(40);
	this->ui->BrushSizeSpinBox->setMinimum(1);
	// Set table widget properties
	this->ui->measurement3DTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->ui->measurement3DTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->ui->measurement2DTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->ui->measurement2DTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->ui->measurement2DTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->ui->measurement2DTableWidget->setColumnHidden(1, true);

	// setting of the contourWidget of auto lumen segmentation
	this->ui->autoLumenSegmentationHorizontalSlider->setEnabled(false);
	this->ui->autoLumenSegmentationHorizontalSlider->setMaximum(500);
	this->ui->autoLumenSegmentationHorizontalSlider->setMinimum(1);
	this->ui->autoLumenSegmentationHorizontalSlider->setValue(60);
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
	connect(ui->autoLumenSegmentationSpinBox, SIGNAL(valueChanged(int)), 
		ui->autoLumenSegmentationHorizontalSlider, SLOT(setValue(int)));
	connect(ui->autoLumenSegmentationHorizontalSlider, SIGNAL(valueChanged(int)),
		ui->autoLumenSegmentationSpinBox, SLOT(setValue(int)));
	connect(ui->autoLumenSegmentationCheckBox, SIGNAL(toggled(bool)), this,
		SLOT(slotEnableAutoLumenSegmentation(bool)));
	connect(ui->autoLumenSegmentationCheckBox, SIGNAL(toggled(bool)), mainWnd, 
		SLOT(slotEnableAutoLumenSegmentation(bool)));
	connect(ui->autoLumenSegmentationHorizontalSlider, SIGNAL(valueChanged(int)), mainWnd, 
		SLOT(slotSetContourFilterGenerateValues(int)));



	//connect
	connect(ui->autoLumenSegmentationPushButton, SIGNAL(clicked()),					mainWnd,	SLOT(slotCenterline()));
	connect(ui->generateReportPushButton,		SIGNAL(clicked()), 					this,		SLOT(slotReportGetInput()));
	connect(ui->labelComboBox,					SIGNAL(currentIndexChanged(int)),	this,		SLOT(slotChangeLayerNo()), Qt::UniqueConnection);
	connect(ui->NextBtn,						SIGNAL(clicked()),					this,		SLOT(NextPage()));
	connect(ui->BackBtn,						SIGNAL(clicked()),					this,		SLOT(BackPage()));
	connect(ui->BrushSizeSlider,				SIGNAL(valueChanged(int)),			this,		SLOT(SetBrushSize()),Qt::UniqueConnection);
	connect(ui->segmentationPushButton,			SIGNAL(clicked()),					this,		SLOT(slotSelectROI()));
	connect(ui->resetROIPushButton,				SIGNAL(clicked()),					this,		SLOT(slotResetROI()));
	//connect(ui->opacitySlider,					SIGNAL(valueChanged(int)),			this,		SLOT(slotChangeOpacity()));
	connect(ui->opacitySpinBox,					SIGNAL(valueChanged(int)),			this,		SLOT(slotChangeOpacity(int)));
	connect(ui->maximumWallThicknessBtn,		SIGNAL(clicked()),					this,		SLOT(slotCalculateMaximumWallThickness()));
	connect(ui->opacitySlider,					SIGNAL(valueChanged(int)),			ui->opacitySpinBox, SLOT(setValue(int)));
	connect(ui->opacitySpinBox,					SIGNAL(valueChanged(int)),			ui->opacitySlider, SLOT(setValue(int)));
	connect(ui->measureCurrentVolumeOfEveryLabelPushButton, SIGNAL(clicked()), 
		mainWnd, SLOT(slotMeasureCurrentVolumeOfEveryLabel()));


	this->GenerateReportPushButtonVisible();
}

ModuleWidget::~ModuleWidget()
{
    delete ui;
}

void ModuleWidget::GenerateReportPushButtonVisible()
{
	int index = ui->stackedWidget->currentIndex();
	if (index == 4)	{
		ui->NextBtn->setVisible(false);
		ui->generateReportPushButton->setVisible(true);
	}
	else {
		ui->NextBtn->setVisible(true);
		ui->generateReportPushButton->setVisible(false);
	}
	
}
void ModuleWidget::NextPage()
{
	int index = ui->stackedWidget->currentIndex();
	ui->stackedWidget->setCurrentIndex(index + 1);
	this->GenerateReportPushButtonVisible();

	this->InternalUpdate();
}

void ModuleWidget::BackPage()
{
	int index = ui->stackedWidget->currentIndex();
	ui->stackedWidget->setCurrentIndex(index - 1);
	this->GenerateReportPushButtonVisible();

	this->InternalUpdate();
}

void ModuleWidget::SetPage(int index)
{
	ui->stackedWidget->setCurrentIndex(index);
	GenerateReportPushButtonVisible();
}
void ModuleWidget::UpdateStenosisValue(double val)
{
	ui->stenosisSpinBox->setValue(val * 100);
}
void ModuleWidget::SetBrushSize()
{
	ui->BrushSizeSpinBox->setValue((ui->BrushSizeSlider->value()));
}
QSpinBox* ModuleWidget::GetBrushSizeSpinBox()
{
	return this->ui->BrushSizeSpinBox;
}

QComboBox* ModuleWidget::GetBrushShapeComBox()
{
	return this->ui->BrushComBox;
}
void ModuleWidget::slotSegmentationView()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	mainWnd->slotSegmentationView();
}

void ModuleWidget::slotChangeLayerNo()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	int layer = ui->labelComboBox->currentIndex() + 1;
	const double* value = mainWnd->GetLookupTable()->GetTableValue(layer);

	ui->opacitySpinBox->setValue(value[3] * 100);
	mainWnd->SetImageLayerNo(layer);

}

void ModuleWidget::slotChangeOpacity(int opacity)
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	int layer = ui->labelComboBox->currentIndex() + 1;
	double* value = mainWnd->GetLookupTable()->GetTableValue(layer);
	value[3] = opacity * 0.01;
	mainWnd->GetLookupTable()->SetTableValue(layer, value);
	mainWnd->GetLookupTable()->Build();
	mainWnd->RenderAllViewer();
}
void ModuleWidget::slotChangeROI(double * bound)
{
	ui->centerSpinBox1->setValue((bound[0] + bound[1])*0.5);
	ui->centerSpinBox2->setValue((bound[2] + bound[3])*0.5);
	ui->centerSpinBox3->setValue((bound[4] + bound[5])*0.5);
	ui->sizeSpinBox1->setValue(bound[1] - bound[0]);
	ui->sizeSpinBox2->setValue(bound[3] - bound[2]);
	ui->sizeSpinBox3->setValue(bound[5] - bound[4]);

}
void ModuleWidget::slotSelectROI()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	mainWnd->slotSelectROI();
}

void ModuleWidget::slotResetROI()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	mainWnd->slotResetROI();
}



void ModuleWidget::slotReportGetInput()
{
	this->GenerateReport();
}

void ModuleWidget::slotMeasureCurrentVolumeOfEveryLabel(double* volumes, int numOfVolumes)
{
	cout << "volumes\n";
	ui->measurement3DTableWidget->clearContents();
	
	for (int i = 0; i < numOfVolumes; ++i) {
		ui->measurement3DTableWidget->setItem(i, 0,
			new QTableWidgetItem(QString::number(volumes[i])));
		cout << volumes[i] << endl;
	}
	
}

void ModuleWidget::slotUpdate2DMeasurements()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();
	int currentSlice = mainwnd->GetUI()->zSpinBox->value();

	vtkImageData* overlayImage = mainwnd->GetOverlay()->GetVTKImageData();
	int extent[6];
	mainwnd->GetOverlay()->GetDisplayExtent(extent);
	extent[4] = currentSlice;
	extent[5] = currentSlice;

	// Extract slice image
	vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	voi->SetInputData(overlayImage);
	voi->SetVOI(extent);
	voi->Update();

	// calculate wall thickness
	vtkSmartPointer<MaximumWallThickness> mwt = vtkSmartPointer<MaximumWallThickness>::New();
	mwt->SetSliceImage(voi->GetOutput());
	try {
		mwt->Update();
	}
	catch (MaximumWallThickness::ERROR_CODE e) {
		// #DisplayErrorMsgHere
		cerr << "MaximumWallThickness error: " << e << endl;
		//return;
	}

	// Calculate areas
	vtkSmartPointer<MeasurementFor2D> m2d = vtkSmartPointer<MeasurementFor2D>::New();
	m2d->SetSliceImage(voi->GetOutput());
	try {
		m2d->Update();
	}
	catch (MeasurementFor2D::ERROR_CODE e) {
		// #DisplayErrorMsgHere
		cerr << "MeasurementFor2D error: " << e << endl;
		//return;
	}

	/// Write measurements to table
	// Error checking
	double lumenArea, vesselArea, NMI, distance; // #todo: Allow multiple branch
	try {
		lumenArea = m2d->GetOutput(0).LumenArea;
		vesselArea = m2d->GetOutput(0).VesselWallArea;
		NMI = vesselArea / (lumenArea + vesselArea);
	}
	catch (...) {
		ui->measurement2DTableWidget->setItem(0, 0, new QTableWidgetItem("Undefined"));
		ui->measurement2DTableWidget->setItem(1, 0, new QTableWidgetItem("Undefined"));
		ui->measurement2DTableWidget->setItem(3, 0, new QTableWidgetItem("Undefined"));
	}
	try {
		distance = mwt->GetDistanceLoopPairVect().at(0).Distance;
	}
	catch (...) {
		ui->measurement2DTableWidget->setItem(2, 0, new QTableWidgetItem("Undefined"));
	}

	// Write to table
	try {
		ui->measurement2DTableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(m2d->GetOutput(0).LumenArea)));
		ui->measurement2DTableWidget->setItem(1, 0, new QTableWidgetItem(QString::number(m2d->GetOutput(0).VesselWallArea)));
		ui->measurement2DTableWidget->setItem(2, 0, new QTableWidgetItem(QString::number(mwt->GetDistanceLoopPairVect().at(0).Distance)));
		ui->measurement2DTableWidget->setItem(3, 0, new QTableWidgetItem(QString::number(NMI)));
	}
	catch (...) {
		// #DisplayErrorMsgHere
		cerr << "Error assigning items to table!\n";
	}
}

void ModuleWidget::slotCalculateMaximumWallThickness()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();
	int currentSlice = mainwnd->GetUI()->zSpinBox->value();

	vtkImageData* overlayImage = mainwnd->GetOverlay()->GetVTKImageData();

	vtkSmartPointer<MaximumWallThickness> calculator = vtkSmartPointer<MaximumWallThickness>::New();
	calculator->SetImage(overlayImage);
	calculator->SetSliceNumber(currentSlice);
	try {
		calculator->Update();
	} 
	catch (MaximumWallThickness::ERROR_CODE e) {
		// #DisplayErrorMsgHere
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
	mainwnd->GetMyImageViewer(2)->GetAnnotationRenderer()->AddActor(this->m_lineActor);
	mainwnd->GetMyImageViewer(2)->GetAnnotationRenderer()->AddActor2D(this->m_labelActor);
	mainwnd->RenderAll2DViewers();
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
	

void ModuleWidget::InternalUpdate()
{
	MainWindow* mainwnd = MainWindow::GetMainWindow();
	Ui_MainWindow* main_ui = mainwnd->GetUI();
	// if page is 5

	int currentIndex = this->ui->stackedWidget->currentIndex();
	if (currentIndex == 4) {
		slotUpdate2DMeasurements();
		mainwnd->slotMeasureCurrentVolumeOfEveryLabel();
		connect(main_ui->zSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotUpdate2DMeasurements()), Qt::QueuedConnection);
		connect(main_ui->zSpinBox, SIGNAL(valueChanged(int)), mainwnd, SLOT(slotMeasureCurrentVolumeOfEveryLabel()), Qt::QueuedConnection);
	}
	else {
		disconnect(main_ui->zSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotUpdate2DMeasurements()));
		disconnect(main_ui->zSpinBox, SIGNAL(valueChanged(int)), mainwnd, SLOT(slotMeasureCurrentVolumeOfEveryLabel()));
	}
}

void ModuleWidget::GenerateReport()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	//General 
	//Basic Information to fill
	QFileInfo fileInfo("./report.pdf");
	QString ReportName = "Plaque Quantification Report";
	QString PatientName = "Chan Tai Man";
	QString PatientID = "11223344";
	QString PatientDOB = "01/01/1956";
	QString PatientGender = "M";
	QString ScanDate = "2016/5/27";
	QString ReportDate = "2016/5/30";
	QString MRISide = "R";
	QString DoctorName = "Dr. Lau";
	//Stenosis Measurement
	QString StenosisPercent = this->ui->stenosisSpinBox->text();
	//2D Measurement
	QString LumenArea = this->ui->measurement2DTableWidget->item(0, 0)->text();
	QString VesselWallArea = this->ui->measurement2DTableWidget->item(1, 0)->text();
	QString WallThickness = this->ui->measurement2DTableWidget->item(2, 0)->text();
	QString NWI = this->ui->measurement2DTableWidget->item(3, 0)->text();
	//3D Measurement
	QString LumenVolume = this->ui->measurement3DTableWidget->item(1, 0)->text();
	QString WallVolume = this->ui->measurement3DTableWidget->item(2, 0)->text();
	QString PlaqueVolume = this->ui->measurement3DTableWidget->item(0, 0)->text();
	//Plaque Composition
	double plaqueVolumeNum = this->ui->measurement3DTableWidget->item(0, 0)->data(Qt::DisplayRole).toDouble();
	double calcificationNum = this->ui->measurement3DTableWidget->item(3, 0)->data(Qt::DisplayRole).toDouble();
	double hemorrhageNum = this->ui->measurement3DTableWidget->item(4, 0)->data(Qt::DisplayRole).toDouble();
	double LRNCNum = this->ui->measurement3DTableWidget->item(5, 0)->data(Qt::DisplayRole).toDouble();
	QString Calcification = this->ui->measurement3DTableWidget->item(3, 0)->text();
	QString CalcificationPercent = QString::number(calcificationNum/plaqueVolumeNum) + "%";
	QString Hemorrhage = this->ui->measurement3DTableWidget->item(4, 0)->text();
	QString HemorrhagePercent = QString::number(hemorrhageNum/plaqueVolumeNum) + "%";
	QString LRNC = this->ui->measurement3DTableWidget->item(5, 0)->text();
	QString LRNCPercent = QString::number(LRNCNum/ plaqueVolumeNum) + "%";
	// Screen Shot
	QString _2dResult = "./2dResult.png";
	QString _3dResult = "./3dResult.png";
	// Filter connecting
	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
		vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
	vtkSmartPointer<vtkImageResize> imageResizeFilter =
		vtkSmartPointer<vtkImageResize>::New();
	imageResizeFilter->SetInputConnection(windowToImageFilter->GetOutputPort());
	imageResizeFilter->SetOutputDimensions(350, 350, 1);
	vtkSmartPointer<vtkPNGWriter> writer =
		vtkSmartPointer<vtkPNGWriter>::New();
	writer->SetInputConnection(imageResizeFilter->GetOutputPort());
	
	// 2d
	windowToImageFilter->SetInput(mainWnd->GetRenderWindow(3));
	writer->SetFileName(_2dResult.toStdString().c_str());
	writer->Update();
	writer->Write();
	//3d
	windowToImageFilter->SetInput(mainWnd->GetRenderWindow(4));
	writer->SetFileName(_3dResult.toStdString().c_str());
	writer->Update();
	writer->Write();

	ReportGenerator* reportGenerator = new ReportGenerator;
	reportGenerator->SetDirectory(fileInfo.absoluteFilePath());
	reportGenerator->SetReportName(ReportName);

	//Table
	int headerColor[4] = { 155, 155, 155, 255 };
	reportGenerator->AddTable(0, "Information", QStringList(), QStringList(), 5, 4);
	reportGenerator->SetTableSize(0, 680, 185);
	reportGenerator->SetTableItem(0, 0, 0, "Patient Information");
	reportGenerator->SetTableItemBackgroundColor(0, 0, 0, headerColor);
	reportGenerator->SetTableItemColumnSpan(0, 0, 0, 2);
	reportGenerator->SetTableItemBold(0, 0, 0, true);
	reportGenerator->SetTableItem(0, 0, 2, "MRI Information");
	reportGenerator->SetTableItemBackgroundColor(0, 0, 2, headerColor);
	reportGenerator->SetTableItemColumnSpan(0, 0, 2, 2);
	reportGenerator->SetTableItemBold(0, 0, 2, true);
	reportGenerator->SetTableItem(0, 1, 0, "Name");
	reportGenerator->SetTableItem(0, 1, 1, PatientName);
	reportGenerator->SetTableItem(0, 1, 2, "Scan Date");
	reportGenerator->SetTableItem(0, 1, 3, ScanDate);
	reportGenerator->SetTableItem(0, 2, 0, "ID");
	reportGenerator->SetTableItem(0, 2, 1, PatientID);
	reportGenerator->SetTableItem(0, 2, 2, "Report Date");
	reportGenerator->SetTableItem(0, 2, 3, ReportDate);
	reportGenerator->SetTableItem(0, 3, 0, "DOB");
	reportGenerator->SetTableItem(0, 3, 1, PatientDOB);
	reportGenerator->SetTableItem(0, 3, 2, "L/R Side");
	reportGenerator->SetTableItem(0, 3, 3, MRISide);
	reportGenerator->SetTableItem(0, 4, 0, "Gender");
	reportGenerator->SetTableItem(0, 4, 1, PatientGender);
	reportGenerator->SetTableItem(0, 4, 2, "Doctor's Name");
	reportGenerator->SetTableItem(0, 4, 3, DoctorName);


	reportGenerator->AddTable(1, "Stenosis Measurement", QStringList(), QStringList(), 2, 4);
	reportGenerator->SetTableSize(1, 680, 74);
	reportGenerator->SetTableItem(1, 0, 0, "Stenosis Measurement");
	reportGenerator->SetTableItemBackgroundColor(1, 0, 0, headerColor);
	reportGenerator->SetTableItemColumnSpan(1, 0, 0, 4);
	reportGenerator->SetTableItemBold(1, 0, 0, true);
	reportGenerator->SetTableItem(1, 1, 0, "% stenosis");
	reportGenerator->SetTableItemColumnSpan(1, 1, 0, 2);
	reportGenerator->SetTableItem(1, 1, 2, StenosisPercent);
	reportGenerator->SetTableItemColumnSpan(1, 1, 2, 2);



	reportGenerator->AddTable(2, "2D Measurement", QStringList(), QStringList(), 4, 4);
	reportGenerator->SetTableSize(2, 680, 148);
	reportGenerator->SetTableItem(2, 0, 0, "2D Measurement");
	reportGenerator->SetTableItemBackgroundColor(2, 0, 0, headerColor);
	reportGenerator->SetTableItemColumnSpan(2, 0, 0, 4);
	reportGenerator->SetTableItemBold(2, 0, 0, true);
	reportGenerator->SetTableItem(2, 1, 0, "Lumen Area\n(mm^2)");
	reportGenerator->SetTableItemRowSpan(2, 1, 0, 2);
	reportGenerator->SetTableItem(2, 3, 0, LumenArea);
	reportGenerator->SetTableItemRowSpan(2, 2, 0, 2);
	reportGenerator->SetTableItem(2, 1, 1, "Vessel Wall Area\n(mm^2)");
	reportGenerator->SetTableItemRowSpan(2, 1, 1, 2);
	reportGenerator->SetTableItem(2, 3, 1, VesselWallArea);
	reportGenerator->SetTableItemRowSpan(2, 2, 1, 2);
	reportGenerator->SetTableItem(2, 1, 2, "Wall Thickness\n(mm)");
	reportGenerator->SetTableItemRowSpan(2, 1, 2, 2);
	reportGenerator->SetTableItem(2, 3, 2, WallThickness);
	reportGenerator->SetTableItemRowSpan(2, 2, 2, 2);
	reportGenerator->SetTableItem(2, 1, 3, "Normalized Wall Index\n(NWI)");
	reportGenerator->SetTableItemRowSpan(2, 1, 3, 2);
	reportGenerator->SetTableItem(2, 3, 3, NWI);
	reportGenerator->SetTableItemRowSpan(2, 2, 3, 2);

	reportGenerator->AddTable(3, "3D Measurement", QStringList(), QStringList(), 3, 4);
	reportGenerator->SetTableSize(3, 680, 111);
	reportGenerator->SetTableItem(3, 0, 0, "3D Measurement");
	reportGenerator->SetTableItemBackgroundColor(3, 0, 0, headerColor);
	reportGenerator->SetTableItemColumnSpan(3, 0, 0, 4);
	reportGenerator->SetTableItemBold(3, 0, 0, true);
	reportGenerator->SetTableItem(3, 2, 0, "volume");
	reportGenerator->SetTableItem(3, 1, 1, "Lumen(mm^3)");
	reportGenerator->SetTableItem(3, 2, 1, LumenVolume);

	reportGenerator->SetTableItem(3, 1, 2, "Wall(mm^3)");
	reportGenerator->SetTableItem(3, 2, 2, WallVolume);
	reportGenerator->SetTableItem(3, 1, 3, "Plaque(mm^3)");
	reportGenerator->SetTableItem(3, 2, 3, PlaqueVolume);

	reportGenerator->AddTable(4, "Plaque Composition", QStringList(), QStringList(), 4, 4);
	reportGenerator->SetTableSize(4, 680, 148);
	reportGenerator->SetTableItem(4, 0, 0, "Plaque Composition");
	reportGenerator->SetTableItemBackgroundColor(4, 0, 0, headerColor);
	reportGenerator->SetTableItemColumnSpan(4, 0, 0, 4);
	reportGenerator->SetTableItemBold(4, 0, 0, true);
	reportGenerator->SetTableItem(4, 2, 0, "volume");
	reportGenerator->SetTableItem(4, 2, 1, Calcification);
	reportGenerator->SetTableItem(4, 2, 2, Hemorrhage);
	reportGenerator->SetTableItem(4, 2, 3, LRNC);
	reportGenerator->SetTableItem(4, 3, 1, CalcificationPercent);
	reportGenerator->SetTableItem(4, 3, 2, HemorrhagePercent);
	reportGenerator->SetTableItem(4, 3, 3, LRNCPercent);
	reportGenerator->SetTableItem(4, 3, 0, "% volume");
	reportGenerator->SetTableItem(4, 1, 1, "calcification");
	reportGenerator->SetTableItem(4, 1, 2, "hemorrhage");
	reportGenerator->SetTableItem(4, 1, 3, "lipid-rich necrotic core");

	//Figure

	reportGenerator->AddFigure(0, "2D result", _2dResult);
	reportGenerator->AddFigure(1, "3D result", _3dResult);
	reportGenerator->AddTable(5, "Images", QStringList(), QStringList(), 10, 4);
	reportGenerator->SetTableSize(5, 680, 350);
	reportGenerator->SetTableItemRowSpan(5, 1, 0, 9);
	reportGenerator->SetTableItemRowSpan(5, 1, 2, 9);
	reportGenerator->SetTableItemColumnSpan(5, 0, 0, 2);
	reportGenerator->SetTableItemColumnSpan(5, 0, 2, 2);
	reportGenerator->SetTableItemColumnSpan(5, 1, 0, 2);
	reportGenerator->SetTableItemColumnSpan(5, 1, 2, 2);
	reportGenerator->SetTableItem(5, 0, 0, "2D result");
	reportGenerator->SetTableItem(5, 0, 2, "3D result");

	reportGenerator->SetTableImageItem(5, 1, 0, 0);
	reportGenerator->SetTableImageItem(5, 1, 2, 1);

	//Update
	reportGenerator->Update();

	QDesktopServices::openUrl(fileInfo.absoluteFilePath());

	delete reportGenerator;

}


#include "MeasurementWidget.hpp"

#include <QDesktopServices>
#include <QFileInfo>

#include <vtkWindowToImageFilter.h>
#include <vtkRenderWindow.h>
#include <vtkImageResize.h>
#include <vtkPNGWriter.h>

#include "Overlay.h"
#include "ReportGenerator.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_QAbstractNavigation.h"

MeasurementWidget::MeasurementWidget(QWidget * parent) : QWidget(parent) 
{
	ui.setupUi(this);

	m_mainWnd = MainWindow::GetMainWindow();
	Core* core = m_mainWnd->GetCore();
	// Set table widget properties
	ui.measurement3DTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.measurement3DTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.measurement2DTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.measurement2DTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.measurement2DTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.measurement2DTableWidget->setColumnHidden(1, true);

	// measurement
	Overlay* overlay = core->GetMyImageManager()->getOverlay();
	connect(overlay, SIGNAL(signalOverlayUpdated()),
		this, SLOT(slotUpdate3DMeasurements()));
	connect(overlay, SIGNAL(signalOverlayUpdated()),
		this, SLOT(slotUpdate2DMeasurements()));
	connect(core->Get2DInteractorStyle(Core::DEFAULT_IMAGE)->GetNavigation()->
		QAbstractNavigation::getUi()->sliceHorizontalSliderZ, SIGNAL(valueChanged(int)),
		this, SLOT(slotUpdate2DMeasurements(int)));

	// generate report
	connect(ui.generateReportPushButton, SIGNAL(clicked()), this, SLOT(slotReportGetInput()));

}

MeasurementWidget::~MeasurementWidget() {
	
}

void MeasurementWidget::slotUpdate3DMeasurements()
{
	m_mainWnd->m_core->GetMyImageManager()->getOverlay()->Measure3D();
	ui.measurement3DTableWidget->clearContents();
	QStringList volumes = m_mainWnd->GetCore()->GetMyImageManager()->
		getOverlay()->Get3DMeasurementsStrings();
	int numOfVolumes = m_mainWnd->GetCore()->GetMyImageManager()->getOverlay()->GetLookupTable()->
		GetNumberOfColors();
	for (int i = 0; i < numOfVolumes; ++i) {
		ui.measurement3DTableWidget->setItem(i, 0,
			new QTableWidgetItem(volumes[i]));
	}

}

void MeasurementWidget::slotUpdate2DMeasurements()
{
	//slotUpdate2DMeasurements(m_mainWnd->ui->zSpinBox->value());
}

void MeasurementWidget::slotUpdate2DMeasurements(int slice)
{
	m_mainWnd->m_core->GetMyImageManager()->getOverlay()->Measure2D(slice);
	QStringList _2DMeasurements = m_mainWnd->GetCore()->GetMyImageManager()->getOverlay()->
		Get2DMeasurementsStrings(slice);
	for (int i = 0; i < 4; ++i) {
		ui.measurement2DTableWidget->setItem(i, 0, new QTableWidgetItem((_2DMeasurements)[i]));
	}

	ui.MWTTextBrowser->setText((_2DMeasurements)[4]);
}

void MeasurementWidget::slotUpdateImformation()
{
	QMap<QString, QString>* header = 
		m_mainWnd->GetCore()->GetMyImageManager()->getListOfDICOMHeader()[Core::DEFAULT_IMAGE];

	if (header == NULL) {
		return;
	}
	QString patientName = header->value("0010|0010");
	QString patientID = header->value("0010|0020");
	QString patientDOB = header->value("0010|0030");
	QString patientGender = header->value("0010|0040");
	QString scanDate = header->value("0008|0020");
	if (!patientName.isEmpty()) {
		ui.patientTableWidget->setItem(0, 0, new QTableWidgetItem(patientName));
	}
	if (!patientID.isEmpty()) {
		ui.patientTableWidget->setItem(1, 0, new QTableWidgetItem(patientID));
	}
	if (!patientDOB.isEmpty()) {
		ui.patientTableWidget->setItem(2, 0, new QTableWidgetItem(patientDOB));
	}
	if (!patientGender.isEmpty()) {
		ui.patientTableWidget->setItem(3, 0, new QTableWidgetItem(patientGender));
	}
	if (!scanDate.isEmpty()) {
		ui.MRITableWidget->setItem(0, 0, new QTableWidgetItem(scanDate));
	}

}


void MeasurementWidget::slotReportGetInput()
{
	this->GenerateReport();
}


void MeasurementWidget::GenerateReport()
{

	//General 
	//Basic Information to fill
	QFileInfo fileInfo("report.pdf");
	QString ReportName = "Plaque Quantification Report";
	QString PatientName = ui.patientTableWidget->item(0,0)->text();
	QString PatientID = ui.patientTableWidget->item(1, 0)->text();
	QString PatientDOB = ui.patientTableWidget->item(2, 0)->text();
	QString PatientGender = ui.patientTableWidget->item(3, 0)->text();
	QString ScanDate = ui.MRITableWidget->item(0, 0)->text();
	QString ReportDate = ui.MRITableWidget->item(1, 0)->text();
	QString MRISide = ui.MRITableWidget->item(2, 0)->text();
	QString DoctorName = ui.MRITableWidget->item(3, 0)->text();
	//Stenosis Measurement
	QString StenosisPercent = ui.stenosisSpinBox->text();
	//2D Measurement
	QString LumenArea = ui.measurement2DTableWidget->item(0, 0)->text();
	QString VesselWallArea = ui.measurement2DTableWidget->item(1, 0)->text();
	QString WallThickness = ui.measurement2DTableWidget->item(2, 0)->text();
	QString NWI = ui.measurement2DTableWidget->item(3, 0)->text();
	//3D Measurement
	QString LumenVolume = ui.measurement3DTableWidget->item(1, 0)->text();
	QString WallVolume = ui.measurement3DTableWidget->item(2, 0)->text();
	QString PlaqueVolume = ui.measurement3DTableWidget->item(0, 0)->text();
	//Plaque Composition
	double plaqueVolumeNum = ui.measurement3DTableWidget->item(0, 0)->data(Qt::DisplayRole).toDouble();
	double calcificationNum = ui.measurement3DTableWidget->item(3, 0)->data(Qt::DisplayRole).toDouble();
	double hemorrhageNum = ui.measurement3DTableWidget->item(4, 0)->data(Qt::DisplayRole).toDouble();
	double LRNCNum = ui.measurement3DTableWidget->item(5, 0)->data(Qt::DisplayRole).toDouble();
	QString Calcification = ui.measurement3DTableWidget->item(3, 0)->text();
	QString CalcificationPercent = QString::number(calcificationNum / plaqueVolumeNum) + "%";
	QString Hemorrhage = ui.measurement3DTableWidget->item(4, 0)->text();
	QString HemorrhagePercent = QString::number(hemorrhageNum / plaqueVolumeNum) + "%";
	QString LRNC = ui.measurement3DTableWidget->item(5, 0)->text();
	QString LRNCPercent = QString::number(LRNCNum / plaqueVolumeNum) + "%";
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
	windowToImageFilter->SetInput(m_mainWnd->GetRenderWindow(3));
	writer->SetFileName(_2dResult.toStdString().c_str());
	writer->Update();
	writer->Write();
	//3d

	windowToImageFilter->SetInput(m_mainWnd->GetRenderWindow(4));
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

	QDesktopServices::openUrl(QUrl("file:///" + fileInfo.absoluteFilePath(), QUrl::TolerantMode));

	delete reportGenerator;

}
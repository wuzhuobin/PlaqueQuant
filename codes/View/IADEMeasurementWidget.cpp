#include "IADEMeasurementWidget.h"
#include "ui_IADEMeasurementWidget.h"

#include "ReportGenerator.h"
#include "qtcsv/writer.h"
#include "qtcsv/variantdata.h"

#include <qfileinfo.h>
#include <qdesktopservices.h>

IADEMeasurementWidget::IADEMeasurementWidget(QWidget * parent)
	:QWidget(parent), 
	ui(QSharedPointer<Ui::IADEMeasurementWidget>(new Ui::IADEMeasurementWidget))
{
	ui->setupUi(this);

	//Ubogu
	connect(ui->doubleSpinBoxLengthBA, SIGNAL(valueChanged(double)), 
		this, SLOT(slotUpdateLengthBA()));
	connect(ui->doubleSpinBoxLengthLVA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateLengthVA()));
	connect(ui->doubleSpinBoxLengthRVA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateLengthVA()));
	connect(ui->doubleSpinBoxDeviationBA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateDeviationBA()));
	connect(ui->doubleSpinBoxDeviationLVA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateDeviationVA()));
	connect(ui->doubleSpinBoxDeviationRVA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateDeviationVA()));
	connect(ui->doubleSpinBoxMaxDiameterBA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateMaxDiameterBA()));
	connect(ui->doubleSpinBoxMaxDiameterLVA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateMaxDiameterVA()));
	connect(ui->doubleSpinBoxMaxDiameterRVA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateMaxDiameterVA()));

	connect(ui->comboBoxBAElongation, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUboguElongation()));
	connect(ui->comboBoxVAElongation, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUboguElongation()));
	connect(ui->comboBoxDeviationBA, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUboguDeviation()));
	connect(ui->comboBoxDeviationVA, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUboguDeviation()));
	connect(ui->comboBoxMaxDiameterBA, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUboguExpansion()));
	connect(ui->comboBoxMaxDiameterVA, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUboguExpansion()));

	connect(ui->comboBoxUboguElongation, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUbogu()));
	connect(ui->comboBoxUboguDeviation, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUbogu()));
	connect(ui->comboBoxUboguExpansion, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateUbogu()));

	//Smoker
	connect(ui->comboBoxElongationClass, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateSmokerElongation()));
	connect(ui->comboBoxTortuosityClass, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateSmokerTortuosity()));
	connect(ui->doubleSpinBoxSmokerMaxDiameterBA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateSmokerEctasia()));

	connect(ui->comboBoxElongation, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateSmoker()));
	connect(ui->comboBoxTortuosity, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateSmoker()));
	connect(ui->comboBoxEctasia, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateSmoker()));

	//ICDA
	connect(ui->doubleSpinBoxLICA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateICA()));
	connect(ui->doubleSpinBoxRICA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateICA()));
	connect(ui->doubleSpinBoxLMCA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateMCA()));
	connect(ui->doubleSpinBoxRMCA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateMCA()));
	connect(ui->doubleSpinBoxLACA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateMCA()));
	connect(ui->doubleSpinBoxRACA, SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateMCA()));

	connect(ui->comboBoxICA, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateICDA()));
	connect(ui->comboBoxMCAM1, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateICDA()));
	connect(ui->comboBoxACA, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateICDA()));

}

IADEMeasurementWidget::~IADEMeasurementWidget()
{
}

void IADEMeasurementWidget::slotUpdateLengthBA()
{
	if (ui->doubleSpinBoxLengthBA->value() > 29.5) {
		ui->comboBoxBAElongation->setCurrentIndex(1);
	}
	else {
		ui->comboBoxBAElongation->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateLengthVA()
{
	if (ui->doubleSpinBoxLengthLVA->value() > 23.5 ||
		ui->doubleSpinBoxLengthRVA->value() > 23.5) {
		ui->comboBoxVAElongation->setCurrentIndex(1);
	}
	else {
		ui->comboBoxVAElongation->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateDeviationBA()
{
	if (ui->doubleSpinBoxDeviationBA->value() > 10) {
		ui->comboBoxDeviationBA->setCurrentIndex(1);
	}
	else {
		ui->comboBoxDeviationBA->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateDeviationVA()
{
	if (ui->doubleSpinBoxDeviationLVA->value() > 10 || 
		ui->doubleSpinBoxDeviationRVA->value() > 10) {
		ui->comboBoxDeviationVA->setCurrentIndex(1);
	}
	else {
		ui->comboBoxDeviationVA->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateMaxDiameterBA()
{
	if (ui->doubleSpinBoxMaxDiameterBA->value() > 4.5) {
		ui->comboBoxMaxDiameterBA->setCurrentIndex(1);
	}
	else {
		ui->comboBoxMaxDiameterBA->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateMaxDiameterVA()
{
	if (ui->doubleSpinBoxMaxDiameterLVA->value() > 4.5 ||
		ui->doubleSpinBoxMaxDiameterRVA->value() > 4.5) {
		ui->comboBoxMaxDiameterVA->setCurrentIndex(1);
	}
	else {
		ui->comboBoxMaxDiameterVA->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateUboguElongation()
{
	if (ui->comboBoxBAElongation->currentIndex() ||
		ui->comboBoxVAElongation->currentIndex()) {
		ui->comboBoxUboguElongation->setCurrentIndex(1);
	}
	else {
		ui->comboBoxUboguElongation->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateUboguDeviation()
{
	if (ui->comboBoxDeviationBA->currentIndex() ||
		ui->comboBoxDeviationVA->currentIndex()) {
		ui->comboBoxUboguDeviation->setCurrentIndex(1);
	}
	else {
		ui->comboBoxUboguDeviation->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateUboguExpansion()
{
	if (ui->comboBoxMaxDiameterBA->currentIndex() ||
		ui->comboBoxMaxDiameterVA->currentIndex()) {
		ui->comboBoxUboguExpansion->setCurrentIndex(1);
	}
	else {
		ui->comboBoxUboguExpansion->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateUbogu()
{
	if (ui->comboBoxUboguElongation->currentIndex() ||
		ui->comboBoxUboguDeviation->currentIndex() ||
		ui->comboBoxUboguExpansion->currentIndex()) {
		ui->comboBoxVBDUbogu->setCurrentIndex(1);
	}
	else {
		ui->comboBoxVBDUbogu->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateSmokerElongation()
{
	if (ui->comboBoxElongationClass->currentIndex() > 1) {
		ui->comboBoxElongation->setCurrentIndex(1);
	}
	else {
		ui->comboBoxElongation->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateSmokerTortuosity()
{
	if (ui->comboBoxTortuosityClass->currentIndex() > 1) {
		ui->comboBoxTortuosity->setCurrentIndex(1);
	}
	else {
		ui->comboBoxTortuosity->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateSmokerEctasia()
{
	if (ui->doubleSpinBoxSmokerMaxDiameterBA->value() > 4.5) {
		ui->comboBoxEctasia->setCurrentIndex(1);
	}
	else {
		ui->comboBoxEctasia->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateSmoker()
{
	if (ui->comboBoxElongation->currentIndex() ||
		ui->comboBoxTortuosity->currentIndex() ||
		ui->comboBoxEctasia->currentIndex()) {
		ui->comboBoxVBDSmoker->setCurrentIndex(1);
	}
	else {
		ui->comboBoxVBDSmoker->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateICA()
{
	if (ui->doubleSpinBoxLICA->value() > 7 ||
		ui->doubleSpinBoxRICA->value() > 7) {
		ui->comboBoxICA->setCurrentIndex(1);
	}
	else {
		ui->comboBoxICA->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateMCA()
{
	if (ui->doubleSpinBoxLMCA->value() > 4 ||
		ui->doubleSpinBoxRMCA->value() > 4) {
		ui->comboBoxMCAM1->setCurrentIndex(1);
	}
	else {
		ui->comboBoxMCAM1->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::slotUpdateACA()
{
	if (ui->doubleSpinBoxLACA->value() > 4 ||
		ui->doubleSpinBoxRACA->value() > 4) {
		ui->comboBoxACA->setCurrentIndex(1);
	}
	else {
		ui->comboBoxACA->setCurrentIndex(0);
	}

}

void IADEMeasurementWidget::slotUpdateICDA()
{
	if (ui->comboBoxICA->currentIndex() ||
		ui->comboBoxMCAM1->currentIndex() ||
		ui->comboBoxACA->currentIndex()) {
		ui->comboBoxICDA->setCurrentIndex(1);
	}
	else {
		ui->comboBoxICDA->setCurrentIndex(0);
	}
}

void IADEMeasurementWidget::GenerateReport(QString path)
{
}

#define VARIANT_DATA_ADD_ROW(PARAM1, PARAM2) \
PARAM1.addRow(QStringList() << #PARAM2 << ##PARAM2);
void IADEMeasurementWidget::GenerateCSV(QString path)
{

	QString PatientName = ui->tableWidgetDicom->item(0, 0)->text();
	QString PatientID = ui->tableWidgetDicom->item(1, 0)->text();
	QString PatientDOB = ui->tableWidgetDicom->item(2, 0)->text();
	QString PatientGender = ui->tableWidgetDicom->item(3, 0)->text();
	QString ScanDate = ui->tableWidgetDicom->item(4, 0)->text();
	QString ReportDate = ui->tableWidgetReport->item(0, 0)->text();
	QString DoctorName = ui->tableWidgetReport->item(1, 0)->text();
	//ICDA 
	QString ICDA_LICALength = QString::number(ui->doubleSpinBoxLICA->value());
	QString ICDA_RICALength = QString::number(ui->doubleSpinBoxRICA->value());
	QString ICDA_LMCALength = QString::number(ui->doubleSpinBoxLMCA->value());
	QString ICDA_RMCALength = QString::number(ui->doubleSpinBoxRMCA->value());
	QString ICDA_LACALength = QString::number(ui->doubleSpinBoxLACA->value());
	QString ICDA_RACALength = QString::number(ui->doubleSpinBoxRACA->value());
	//VBD-Smoker
	QString Smoker_Elongation = ui->comboBoxElongationClass->currentText();
	QString Smoker_Tortuosity = ui->comboBoxTortuosityClass->currentText();
	QString Smoker_Ectasia = QString::number(ui->doubleSpinBoxSmokerMaxDiameterBA->value());
	//VBD-Ubogu
	QString Ubogu_BALength = QString::number(ui->doubleSpinBoxLengthBA->value());
	QString Ubogu_LVALength = QString::number(ui->doubleSpinBoxLengthLVA->value());
	QString Ubogu_RVALength = QString::number(ui->doubleSpinBoxLengthRVA->value());
	QString Ubogu_BADeviation = QString::number(ui->doubleSpinBoxDeviationBA->value());
	QString Ubogu_LVADeviation = QString::number(ui->doubleSpinBoxDeviationLVA->value());
	QString Ubogu_RVADeviation = QString::number(ui->doubleSpinBoxDeviationRVA->value());
	QString Ubogu_BADiameter = QString::number(ui->doubleSpinBoxMaxDiameterBA->value());
	QString Ubogu_LVADiameter = QString::number(ui->doubleSpinBoxMaxDiameterLVA->value());
	QString Ubogu_RVADiameter = QString::number(ui->doubleSpinBoxMaxDiameterRVA->value());

	QtCSV::VariantData varData;
	VARIANT_DATA_ADD_ROW(varData, PatientName);
	VARIANT_DATA_ADD_ROW(varData, PatientID);
	VARIANT_DATA_ADD_ROW(varData, PatientDOB);
	VARIANT_DATA_ADD_ROW(varData, PatientGender);
	VARIANT_DATA_ADD_ROW(varData, ScanDate);
	varData.addEmptyRow();

	VARIANT_DATA_ADD_ROW(varData, ReportDate);
	VARIANT_DATA_ADD_ROW(varData, DoctorName);
	varData.addEmptyRow();

	VARIANT_DATA_ADD_ROW(varData, ICDA_LICALength);
	VARIANT_DATA_ADD_ROW(varData, ICDA_RICALength);
	VARIANT_DATA_ADD_ROW(varData, ICDA_LMCALength);
	VARIANT_DATA_ADD_ROW(varData, ICDA_RMCALength);
	VARIANT_DATA_ADD_ROW(varData, ICDA_LACALength);
	VARIANT_DATA_ADD_ROW(varData, ICDA_RACALength);
	varData.addEmptyRow();

	VARIANT_DATA_ADD_ROW(varData, Smoker_Elongation);
	VARIANT_DATA_ADD_ROW(varData, Smoker_Tortuosity);
	VARIANT_DATA_ADD_ROW(varData, Smoker_Ectasia);
	varData.addEmptyRow();

	VARIANT_DATA_ADD_ROW(varData, Ubogu_BALength);
	VARIANT_DATA_ADD_ROW(varData, Ubogu_LVALength);
	VARIANT_DATA_ADD_ROW(varData, Ubogu_RVALength);
	VARIANT_DATA_ADD_ROW(varData, Ubogu_BADeviation);
	VARIANT_DATA_ADD_ROW(varData, Ubogu_LVADeviation);
	VARIANT_DATA_ADD_ROW(varData, Ubogu_RVADeviation);
	VARIANT_DATA_ADD_ROW(varData, Ubogu_BADiameter);
	VARIANT_DATA_ADD_ROW(varData, Ubogu_LVADiameter);
	VARIANT_DATA_ADD_ROW(varData, Ubogu_RVADiameter);

	QFileInfo fileInfo(path);
	// wirte
	if (true == QtCSV::Writer::write(fileInfo.absoluteFilePath(), varData))
	{
		QDesktopServices::openUrl(fileInfo.absoluteFilePath());
	}
}

void IADEMeasurementWidget::slotUpdateInformation()
{
	if (info.IsNull()) {
		return;
	}
	std::string patientID;
	std::string patientDOB;
	std::string patientGender;
	std::string scanDate;
	std::string patientName;
	info->GetValueFromTag("0010|0010", patientName);
	info->GetValueFromTag("0010|0020", patientID);
	info->GetValueFromTag("0010|0030", patientDOB);
	info->GetValueFromTag("0010|0040", patientGender);
	info->GetValueFromTag("0008|0020", scanDate);
	if (!patientName.empty()) {
		ui->tableWidgetDicom->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(patientName)));
	}
	if (!patientID.empty()) {
		ui->tableWidgetDicom->setItem(1, 0, new QTableWidgetItem(QString::fromStdString(patientID)));
	}
	if (!patientDOB.empty()) {
		ui->tableWidgetDicom->setItem(2, 0, new QTableWidgetItem(QString::fromStdString(patientDOB)));
	}
	if (!patientGender.empty()) {
		ui->tableWidgetDicom->setItem(3, 0, new QTableWidgetItem(QString::fromStdString(patientGender)));
	}
	if (!scanDate.empty()) {
		ui->tableWidgetDicom->setItem(4, 0, new QTableWidgetItem(QString::fromStdString(scanDate)));
	}

}

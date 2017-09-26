#include "QInteractorStyleLumenSeedsPlacer.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkObjectFactory.h>
#include <vtkImageData.h>
#include <vtkExtractVOI.h>
#include <vtkSeedWidget.h>

#include <vtkHandleWidget.h>
#include <vtkHandleRepresentation.h>
#include <vtkSeedRepresentation.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkImageBlend.h>


#include "LumenExtractionFilter.h"
#include "ImageViewer.h"
#include "ui_QInteractorStyleLumenSeedsPlacer.h"
#include "ui_QAbstractNavigation.h"

#include <qevent.h>

vtkStandardNewMacro(QInteractorStyleLumenSeedsPlacer);
QSETUP_UI_SRC(QInteractorStyleLumenSeedsPlacer);
QList<int*> QInteractorStyleLumenSeedsPlacer::m_lumenSeeds;

using namespace std;

void QInteractorStyleLumenSeedsPlacer::SetCustomEnabled(bool flag)
{
	InteractorStyleSeedsPlacer::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleLumenSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleLumenSeedsPlacer::UpdateTargetViewer()
{
	//QStringList listOfModalityName;
	QStringList listOfModalityNames;
	ui->comboBoxTargeImage->clear();
	listOfModalityNames.clear();
	for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleLumenSeedsPlacer* _style = QInteractorStyleLumenSeedsPlacer::SafeDownCast(*cit);
		// because not all have been CustomEnabled this timet 
		if (_style /*&& _style->GetCustomEnabled()*/) {
			listOfModalityNames.append(QString::fromStdString(_style->GetImageViewer()->GetWindowName()));
		}
	}
	listOfModalityNames.removeDuplicates();

	for (int i = 0; i < listOfModalityNames.size(); ++i) {
		ui->comboBoxTargeImage->addItem(listOfModalityNames[i]);
	}

}

void QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(int * newImagePos, int* oldImagePos)
{
	QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(m_lumenSeeds, newImagePos, oldImagePos);
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumenByTargetViewer()
{
	for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleLumenSeedsPlacer* _style = QInteractorStyleLumenSeedsPlacer::SafeDownCast(*cit);

		if (_style && _style->GetCustomEnabled() &&
			QString::fromStdString(_style->GetImageViewer()->GetWindowName()) ==
			ui->comboBoxTargeImage->currentText()) {
			_style->ExtractLumen();
			break;
		}
	}
}

void QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(QList<int*>& seeds, int * newImagePos, int * oldImagePos)
{
	// if oldImagePos is nullptr or oldImagePos is not a valid number, 
	// replace the existed newImagePos or push the new newImagePos
	if (oldImagePos == nullptr ||
		oldImagePos[0] < 0 ||
		oldImagePos[1] < 0 ||
		oldImagePos[2] < 0) {
		oldImagePos = newImagePos;
	}
	QList<int*>::iterator it = std::find_if(seeds.begin(),
		seeds.end(), [oldImagePos](int* index)->bool {
		return
			oldImagePos[0] == index[0] &&
			oldImagePos[1] == index[1] &&
			oldImagePos[2] == index[2];
	});
	if (it != seeds.end()) {
		memcpy(*it, newImagePos, sizeof(int[3]));
		QString listItem = "Seed Index: [" +
			QString::number(newImagePos[0]) + "," +
			QString::number(newImagePos[1]) + "," +
			QString::number(newImagePos[2]) + "]";
		GetListWidget()->item(seeds.indexOf(*it))->setText(listItem);
	}
	else {
		seeds.push_back(new int[3]);
		memcpy(seeds.back(), newImagePos, sizeof(int[3]));
		QString listItem = "Seed Index: [" +
			QString::number(seeds.back()[0]) + "," +
			QString::number(seeds.back()[1]) + "," +
			QString::number(seeds.back()[2]) + "]";
		GetListWidget()->addItem(listItem);
	}
}

void QInteractorStyleLumenSeedsPlacer::ClearAllSeeds(QList<int*>& seed)
{
	if (seed.size() != 0) {
		while (!seed.empty())
		{
			delete[] seed.back();
			seed.pop_back();
		}
	}
	GetListWidget()->clear();
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
}

void QInteractorStyleLumenSeedsPlacer::DeleteFocalSeed(QList<int*>& seeds)
{
	int i = GetListWidget()->currentRow();
	if (i >= 0 && i < seeds.size()) {
		GetListWidget()->removeItemWidget(
			GetListWidget()->takeItem(i));
		delete[] seeds[i];
		seeds.removeAt(i);
	}
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, GenerateWidgetFromSeeds());
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, m_seedWidget->Render());
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumen(QList<int*>& seeds)
{
	typedef itk::Index<3> IndexType;
	//ImageViewer* inputViewer;

	//for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
	//	cit != m_imageStyles.cend(); ++cit) {
	//	QInteractorStyleLumenSeedsPlacer* _style = QInteractorStyleLumenSeedsPlacer::SafeDownCast(*cit);
	//	
	//	if (_style && _style->GetCustomEnabled() && 
	//		QString::fromStdString(_style->GetImageViewer()->GetWindowName()) == 
	//		ui->comboBoxTargeImage->currentText()) {
	//		inputViewer = GetImageViewer();
	//	}
	//}


	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputData(GetImageViewer()->GetInput());
	extractVOI->SetVOI(GetImageViewer()->GetDisplayExtent());
	extractVOI->Update();

	vtkSmartPointer<LumenExtractionFilter> lumenExtractionFilter =
		vtkSmartPointer<LumenExtractionFilter>::New();
	lumenExtractionFilter->CoreFilter->SetNumberOfIterations(m_numberOfIteractions);
	lumenExtractionFilter->CoreFilter->SetMultiplier(m_multiplier);
	lumenExtractionFilter->CoreFilter->SetInitialNeighborhoodRadius(2);
	for (QList<int*>::const_iterator cit = seeds.cbegin(); cit != seeds.cend(); ++cit) {
		IndexType index = {
			(*cit)[0], (*cit)[1], (*cit)[2] };
		lumenExtractionFilter->CoreFilter->AddSeed(index);
	}
	//lumenExtractionFilter->SetInputData(m_listOfVtkImages[index]);
	lumenExtractionFilter->SetInputConnection(extractVOI->GetOutputPort());
	lumenExtractionFilter->Update();

	vtkSmartPointer<vtkImageBlend> blend =
		vtkSmartPointer<vtkImageBlend>::New();
	blend->AddInputData(GetImageViewer()->GetOverlay());
	blend->AddInputConnection(lumenExtractionFilter->GetOutputPort());
	blend->SetOpacity(0, 0);
	blend->SetOpacity(1, 1);
	blend->Update();
	GetImageViewer()->GetOverlay()->ShallowCopy(blend->GetOutput());

	//for (int i = GetImageViewer()->GetDisplayExtent()[0];
	//	i <= GetImageViewer()->GetDisplayExtent()[1]; ++i) {
	//	for (int j = GetImageViewer()->GetDisplayExtent()[2];
	//		j <= GetImageViewer()->GetDisplayExtent()[3]; ++j) {
	//		for (int k = GetImageViewer()->GetDisplayExtent()[4];
	//			k <= GetImageViewer()->GetDisplayExtent()[5]; ++k) {
	//			unsigned char* pixelLayer = static_cast<unsigned char*>
	//				(GetImageViewer()->GetOverlay()->GetScalarPointer(i, j, k));
	//			unsigned char* pixel = static_cast<unsigned char*>
	//				(lumenExtractionFilter->GetOutput()->GetScalarPointer(i, j, k));
	//			*pixelLayer = *pixel;
	//		}
	//	}
	//}

	//GetImageViewer()->GetOverlay()->Modified();
	//GetImageViewer()->GetOverlay()->InvokeEvent(vtkCommand::UpdateDataEvent);
	//MY_VIEWER_CONSTITERATOR(Render());
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, GetImageViewer()->Render());

	//ExtractLumenPolyData();
}

void QInteractorStyleLumenSeedsPlacer::ClearAllSeeds()
{
	ClearAllSeeds(m_lumenSeeds);
	//m_seedWidget->Render();
}

void QInteractorStyleLumenSeedsPlacer::SetFocalSeed(int i)
{
	QInteractorStyleLumenSeedsPlacer::SetFocalSeed(i, m_lumenSeeds);

	const double* worldCoordinate = GetImageViewer()->GetFocalPointWithWorldCoordinate();

	//char label[60];
	//sprintf_s(label, "(%.3f, %.3f, %.3f)", worldCoordinate[0], worldCoordinate[1], worldCoordinate[2]);
	QString label = "(" + QString::number(worldCoordinate[0], 'f', 3) + "," +
		QString::number(worldCoordinate[1], 'f', 3) + "," +
		QString::number(worldCoordinate[2], 'f', 3) + ")";
	/// change label text
	this->ui->labelSeedCoordinate->setText(label);
}

void QInteractorStyleLumenSeedsPlacer::SetFocalSeed(int i, QList<int*>& seeds)
{
	if (i < 0 || i > seeds.size()) {
		return;
	}
	int imageCoordinate[3];
	memcpy(imageCoordinate, seeds[i], sizeof(imageCoordinate));
	SetCurrentFocalPointWithImageCoordinate(imageCoordinate[0], imageCoordinate[1],
		imageCoordinate[2]);
}

void QInteractorStyleLumenSeedsPlacer::GenerateWidgetFromSeeds()
{
	GenerateWidgetFromSeeds(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::GenerateWidgetFromSeeds(const QList<int*>& seeds)
{
	ClearAllSeedWidget();

	for (QList<int*>::const_iterator cit = seeds.cbegin();
		cit != seeds.cend(); ++cit) {
		int* imagePos = (*cit);
		double worldPos[3];
		for (int pos = 0; pos < 3; ++pos) {
			worldPos[pos] = (imagePos[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
		}
		if (imagePos[GetSliceOrientation()] == GetImageViewer()->GetSlice()) {
			vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
			newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
			newSeed->EnabledOn();
		}
	}
	m_seedWidget->Render();
}

void QInteractorStyleLumenSeedsPlacer::SaveWidgetToSeeds()
{
	SaveWidgetToSeeds(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::DeleteFocalSeed()
{
	DeleteFocalSeed(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::SaveWidgetToSeeds(QList<int*>& seeds)
{
	for (int i = m_seedWidget->GetSeedRepresentation()->GetNumberOfSeeds() - 1; i >= 0; --i) {
		//double* worldPos = new double[3]; // #MemLeakHere
		double worldPos[3];
		m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(i, worldPos);

		int imagePos[3];
		for (int pos = 0; pos < 3; ++pos) {
			imagePos[pos] = (worldPos[pos] - GetOrigin()[pos]) / GetSpacing()[pos] + 0.5;
		}
		UpdateWidgetToSeeds(seeds, imagePos);
	}
}

void QInteractorStyleLumenSeedsPlacer::DropSeed(QList<int*>& seeds)
{
	double* worldPos = GetImageViewer()->GetFocalPointWithWorldCoordinate();
	vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
	newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
	newSeed->EnabledOn();
	m_seedWidget->Render();

	SaveWidgetToSeeds(seeds);
	OnLeftButtonUp();
}

void QInteractorStyleLumenSeedsPlacer::DropSeed()
{
	DropSeed(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumen()
{
	ExtractLumen(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::SetMultipier(double value)
{
	m_multiplier = value;
}

void QInteractorStyleLumenSeedsPlacer::SetNumberOfIteractions(int value)
{
	m_numberOfIteractions = value;
}

void QInteractorStyleLumenSeedsPlacer::SetInitialNeighborhoodRadius(int value)
{
	m_initialNeighborhoodRadius = value;
}

QInteractorStyleLumenSeedsPlacer::QInteractorStyleLumenSeedsPlacer(int uiType, QWidget* parent)
{
	QNEW_UI();
}

QInteractorStyleLumenSeedsPlacer::~QInteractorStyleLumenSeedsPlacer()
{
	QDELETE_UI();

}

QListWidget * QInteractorStyleLumenSeedsPlacer::GetListWidget()
{
	return ui->listWidgetSeedList;
}

void QInteractorStyleLumenSeedsPlacer::uniqueInitialization()
{
	connect(ui->pushBtnDeleteSeed, SIGNAL(clicked()),
		this, SLOT(DeleteFocalSeed()));

	connect(GetListWidget(), SIGNAL(currentRowChanged(int)),
		this, SLOT(SetFocalSeed(int)));

	connect(ui->dropSeedPushButton, SIGNAL(clicked()),
		this, SLOT(DropSeed()));
	// only create lumenExtraction and connect its parameter in the first 
	// InteractorStyleLumenSeedsPalcer, others' are nullptr and won't be 
	// connected
	connect(ui->pushBtnExtractLumen, SIGNAL(clicked()),
		this, SLOT(ExtractLumenByTargetViewer()));
	connect(ui->doubleSpinBoxMultiplier, SIGNAL(valueChanged(double)),
		this, SLOT(SetMultipier(double)));
	connect(ui->numberOfIterationsSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(SetNumberOfIteractions(int)));
	connect(ui->initialNeighbodhoodSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(SetInitialNeighborhoodRadius(int)));

}

void QInteractorStyleLumenSeedsPlacer::initialization()
{
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(ClearAllSeeds()));
}

void QInteractorStyleLumenSeedsPlacer::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
	UpdateTargetViewer();
}

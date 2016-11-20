#include "QInteractorStyleLumenSeedsPlacer.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>

#include "LumenSegmentationFilter2.h"
#include "ui_QInteractorStyleLumenSeedsPlacer.h"
#include "ui_QAbstractNavigation.h"


vtkStandardNewMacro(QInteractorStyleLumenSeedsPlacer);
QSETUP_UI_SRC(QInteractorStyleLumenSeedsPlacer);
using namespace std;

void QInteractorStyleLumenSeedsPlacer::SetSeedsPlacerEnable(bool flag)
{
	InteractorStyleSeedsPlacer::SetSeedsPlacerEnable(flag);
	UniqueEnable(flag);
}

void QInteractorStyleLumenSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(int * oldImagePos, int* newImagePos)
{
	InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(oldImagePos, newImagePos);
	ui->listWidgetSeedList->clear();
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	//for (QList<int*>::ConstIterator cit = _seeds.cbegin(); cit != _seeds.cend(); ++cit) {
	//	ui->listWidgetSeedList->insertItem()
	//}
	for (int i = 0; i < _seeds.size(); ++i) {
		QString listItem = "Seed Index: [" +
			QString::number(_seeds[i][0]) + "," +
			QString::number(_seeds[i][1]) + "," +
			QString::number(_seeds[i][2]) + "]";
		ui->listWidgetSeedList->insertItem(i, listItem);
	}
}

void QInteractorStyleLumenSeedsPlacer::SlotClearAllSeeds()
{
	ClearAllSeeds();
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
	ui->listWidgetSeedList->clear();
	m_imageViewer->Render();
}

void QInteractorStyleLumenSeedsPlacer::SetFocalSeed(int i)
{
	if (i < 0)
		return;
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	int imageCoordinate[3];
	double worldCoordinate[3];

	memcpy(imageCoordinate, _seeds[i], sizeof(imageCoordinate));
	SetCurrentFocalPointWithImageCoordinate(imageCoordinate[0], imageCoordinate[1], 
		imageCoordinate[2]);

	for (int pos = 0; pos < 3; ++pos) {
		worldCoordinate[pos] = imageCoordinate[pos] * GetSpacing()[pos] - GetOrigin()[i];
	}

	//char label[60];
	//sprintf_s(label, "(%.3f, %.3f, %.3f)", worldCoordinate[0], worldCoordinate[1], worldCoordinate[2]);
	QString label = "(" + QString::number(worldCoordinate[0], 'f', 3) + "," +
		QString::number(worldCoordinate[1], 'f', 3) + "," +
		QString::number(worldCoordinate[2], 'f', 3) + ")";
	/// change label text
	this->ui->labelSeedCoordinate->setText(label);
}

void QInteractorStyleLumenSeedsPlacer::DeleteFocalSeed()
{
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	int i = ui->listWidgetSeedList->currentRow();
	if (i >= 0 && i < _seeds.size()) {
		ui->listWidgetSeedList->removeItemWidget(
			ui->listWidgetSeedList->takeItem(i));
		m_seeds.remove(_seeds[i]);
		delete _seeds[i];
	}
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, GenerateWidgetFromSeeds());
	MY_VIEWER_CONSTITERATOR(Render());
}

void QInteractorStyleLumenSeedsPlacer::SaveWidgetToSeeds()
{
	InteractorStyleSeedsPlacer::SaveWidgetToSeeds();
	ui->listWidgetSeedList->clear();
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	//for (QList<int*>::ConstIterator cit = _seeds.cbegin(); cit != _seeds.cend(); ++cit) {
	//	ui->listWidgetSeedList->insertItem()
	//}
	for (int i = 0; i < _seeds.size(); ++i) {
		QString listItem = "Seed Index: [" +
			QString::number(_seeds[i][0]) + "," +
			QString::number(_seeds[i][1]) + "," +
			QString::number(_seeds[i][2]) + "]";
		ui->listWidgetSeedList->insertItem(i, listItem);
	}
}

void QInteractorStyleLumenSeedsPlacer::DropSeed()
{
	InteractorStyleSeedsPlacer::DropSeed();
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumen()
{
	typedef itk::Index<3> IndexType;


	vtkSmartPointer<LumenExtractionFilter> lumenExtractionFilter =
		vtkSmartPointer<LumenExtractionFilter>::New();
	lumenExtractionFilter->CoreFilter->SetNumberOfIterations(m_numberOfIteractions);
	lumenExtractionFilter->CoreFilter->SetMultiplier(m_multiplier);
	lumenExtractionFilter->CoreFilter->SetInitialNeighborhoodRadius(2);

	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	for (int i = 0; i < _seeds.size(); ++i) {
		IndexType index = {_seeds[i][0], _seeds[i][1], _seeds[i][2]};
		lumenExtractionFilter->CoreFilter->AddSeed(index);
	}
	lumenExtractionFilter->SetInputData(m_imageViewer->GetOriginalInput());
	lumenExtractionFilter->Update();

	m_imageViewer->GetOverlay()->vtkShallowCopyImage(
		lumenExtractionFilter->GetOutput());
	MY_VIEWER_CONSTITERATOR(Render());

	ExtractLumenPolyData();
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumenPolyData()
{

	const int* extent = m_imageViewer->GetOverlay()->GetOutput()->GetExtent(); 
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* lumenMap =
		m_imageViewer->GetOverlay()->GetLumenPolyData();


	for (int i = extent[4]; i <= extent[5]; ++i) {

		if (lumenMap->contains(i)) {
			delete  lumenMap->value(i);
		}
		(*lumenMap)[i] = new QList<vtkSmartPointer<vtkPolyData>>;

		vtkSmartPointer<vtkMarchingSquares> marchingSquares =
			vtkSmartPointer<vtkMarchingSquares>::New();
		marchingSquares->SetInputData(m_imageViewer->GetOverlay()->GetOutput());
		marchingSquares->CreateDefaultLocator();
		marchingSquares->SetImageRange(extent[0], extent[1], extent[2], extent[3], i, i);
		marchingSquares->GenerateValues(1, 1, 1);
		marchingSquares->Update();

		vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
		connectivityFilter->SetExtractionModeToAllRegions();
		connectivityFilter->Update();
		
		for (int j = 0; j < connectivityFilter->GetNumberOfExtractedRegions(); ++j) {
			vtkSmartPointer<vtkPolyDataConnectivityFilter> _connectivityFilter =
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
			_connectivityFilter->SetInputConnection(marchingSquares->GetOutputPort());
			_connectivityFilter->AddSpecifiedRegion(j);
			_connectivityFilter->SetExtractionModeToSpecifiedRegions();
			_connectivityFilter->Update();

			LumenSegmentationFilter2::ReorderPolyData(_connectivityFilter->GetOutput());

			double toleranceInitial = 1;
			int loopBreaker = 0;

			vtkSmartPointer<vtkCleanPolyData> clearPolyData =
				vtkSmartPointer<vtkCleanPolyData>::New();
			clearPolyData->SetInputConnection(_connectivityFilter->GetOutputPort());
			clearPolyData->ToleranceIsAbsoluteOn();
			clearPolyData->SetAbsoluteTolerance(toleranceInitial);
			clearPolyData->PointMergingOn();
			clearPolyData->Update();
			while (clearPolyData->GetOutput()->GetNumberOfPoints() < 3 && loopBreaker < 10) {
				toleranceInitial *= 0.75;
				clearPolyData->SetAbsoluteTolerance(toleranceInitial);
				clearPolyData->Update();
				loopBreaker += 1;
			}

			vtkSmartPointer<vtkPolyData> _lumenPolyData =
				vtkSmartPointer<vtkPolyData>::New();
			_lumenPolyData->ShallowCopy(clearPolyData->GetOutput());
			(*(*lumenMap)[i]) += _lumenPolyData;

		}
		

	}
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
	if (numOfMyself == 1) {
		connect(ui->pushBtnDeleteSeed, SIGNAL(clicked()),
			this, SLOT(DeleteFocalSeed()));

		connect(ui->listWidgetSeedList, SIGNAL(currentRowChanged(int)),
			this, SLOT(SetFocalSeed(int)));
		
		connect(ui->dropSeedPushButton, SIGNAL(clicked()),
			this, SLOT(DropSeed()));
		// only create lumenExtraction and connect its parameter in the first 
		// InteractorStyleLumenSeedsPalcer, others' are nullptr and won't be 
		// connected
		connect(ui->pushBtnExtractLumen, SIGNAL(clicked()),
			this, SLOT(ExtractLumen()));
		connect(ui->doubleSpinBoxMultiplier, SIGNAL(valueChanged(double)),
			this, SLOT(SetMultipier(double)));
		connect(ui->numberOfIterationsSpinBox, SIGNAL(valueChanged(int)),
			this, SLOT(SetNumberOfIteractions(int)));
		connect(ui->initialNeighbodhoodSpinBox, SIGNAL(valueChanged(int)),
			this, SLOT(SetInitialNeighborhoodRadius(int)));

	}
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(SlotClearAllSeeds()));
}

QInteractorStyleLumenSeedsPlacer::~QInteractorStyleLumenSeedsPlacer()
{
	QDELETE_UI();

}

void QInteractorStyleLumenSeedsPlacer::UniqueEnable(bool flag)
{
	QAbstractNavigation::UniqueEnable(flag);

	if (flag && flag != initializationFlag) {
		// turn on codes

		connect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()),
			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
	}
	// turn off
	if (!flag && flag != initializationFlag) {
		// turn off codes
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));
		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			this, SLOT(slotChangeSlice()));
	}
	if (flag != initializationFlag) {

	}
	initializationFlag = flag;
}

void QInteractorStyleLumenSeedsPlacer::OnKeyPress()
{
	string key = this->Interactor->GetKeySym();
	cout << __func__ << ' ' << key << endl;
	if (key == "D") {
		InteractorStyleSeedsPlacer::OnKeyPress();
	}
	else {
		InteractorStyleSeedsPlacer::OnKeyPress();
	}
}

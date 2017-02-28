#include "QInteractorStyleLumenSeedsPlacer.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>

#include <vtkvmtkPolyDataCenterlines.h>
#include <vtkIdList.h>

#include "vtkPolylineToTubularVolume.h"
#include "vtkImageMask.h"
#include "LumenSegmentationFilter2.h"
#include "ui_QInteractorStyleLumenSeedsPlacer.h"
#include "ui_QAbstractNavigation.h"
#include "vtkCellArray.h"
#include "vtkMath.h"
#include "vtkCardinalSpline.h"
#include "vtkSplineFilter.h"


vtkStandardNewMacro(QInteractorStyleLumenSeedsPlacer);
QSETUP_UI_SRC(QInteractorStyleLumenSeedsPlacer);
using namespace std;

void QInteractorStyleLumenSeedsPlacer::SetSeedsPlacerEnable(bool flag)
{
	InteractorStyleSeedsPlacer::SetSeedsPlacerEnable(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleLumenSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleLumenSeedsPlacer::SetTargetImages(
	QList<vtkSmartPointer<vtkImageData>> listOfVtkImages, 
	QList<QString> listOfModalityNames)
{
	m_listOfModalityNames = listOfModalityNames;
	m_listOfVtkImages = listOfVtkImages;

	ui->comboBoxTargeImage->clear();
	for (int i = 0; i < m_listOfModalityNames.size(); ++i) {
		if (m_listOfVtkImages[i] != nullptr) {
			ui->comboBoxTargeImage->addItem(m_listOfModalityNames[i]);
		}
	}
}

void QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(int * oldImagePos, int* newImagePos)
{
	InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(oldImagePos, newImagePos);
	ui->listWidgetSeedList->clear();
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds[m_currentSeedsType]);
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

void QInteractorStyleLumenSeedsPlacer::SlotUpdateSpinBoxExtractRadius()
{
	this->ui->doubleSpinBoxExtractRadius->setValue(this->ui->hSliderExtractRadius->value() / 10.);
}

void QInteractorStyleLumenSeedsPlacer::SlotUpdateHSliderExtractRadius()
{
	this->ui->hSliderExtractRadius->setValue(this->ui->doubleSpinBoxExtractRadius->value() * 10);
}

void QInteractorStyleLumenSeedsPlacer::SlotClearAllSeeds()
{
	ClearAllSeeds();
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
	ui->listWidgetSeedList->clear();
	m_imageViewer->Render();
}

void QInteractorStyleLumenSeedsPlacer::SlotCentreLine()
{
}

void QInteractorStyleLumenSeedsPlacer::SetFocalSeed(int i)
{
	if (i < 0)
		return;
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds[m_currentSeedsType]);
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
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds[m_currentSeedsType]);
	int i = ui->listWidgetSeedList->currentRow();
	if (i >= 0 && i < _seeds.size()) {
		ui->listWidgetSeedList->removeItemWidget(
			ui->listWidgetSeedList->takeItem(i));
		m_seeds[m_currentSeedsType].remove(_seeds[i]);
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
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds[m_currentSeedsType]);
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

void QInteractorStyleLumenSeedsPlacer::CenterLine()
{
	if (m_seeds[m_currentSeedsType].size() < 2) {
		return;
	}
	//QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);

	//vtkSmartPointer<vtkIdList> sourceIds = vtkSmartPointer<vtkIdList>::New();
	//sourceIds->SetNumberOfIds(1);
	//sourceIds->SetId(0, _seeds[0]);
	//sourceIds->getid

	//vtkSmartPointer<vtkIdList> targetIds = vtkSmartPointer<vtkIdList>::New();
	//targetIds->SetNumberOfIds(1);
	//targetIds->SetId(0, CapCenterIds->GetId(0));



	//else {
	//	vtkSmartPointer<vtkvmtkPolyDataCenterlines> centerlinesFilter = vtkSmartPointer<vtkvmtkPolyDataCenterlines>::New();
	//	centerlinesFilter->SetInputData(cappedSurface);
	//	centerlinesFilter->SetSourceSeedIds(targetIds);
	//	centerlinesFilter->SetTargetSeedIds(targetIds);
	//	centerlinesFilter->SetAppendEndPointsToCenterlines(1);
	//	centerlinesFilter->SetRadiusArrayName("Radius");
	//	centerlinesFilter->Update();

	//}
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumen()
{
	typedef itk::Index<3> IndexType;


	vtkSmartPointer<LumenExtractionFilter> lumenExtractionFilter =
		vtkSmartPointer<LumenExtractionFilter>::New();
	lumenExtractionFilter->CoreFilter->SetNumberOfIterations(m_numberOfIteractions);
	lumenExtractionFilter->CoreFilter->SetMultiplier(m_multiplier);
	lumenExtractionFilter->CoreFilter->SetInitialNeighborhoodRadius(2);

	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds[m_currentSeedsType]);
	for (int i = 0; i < _seeds.size(); ++i) {
		IndexType index = {_seeds[i][0], _seeds[i][1], _seeds[i][2]};
		lumenExtractionFilter->CoreFilter->AddSeed(index);
	}
	//lumenExtractionFilter->SetInputData(m_imageViewer->GetOriginalInput());

	int index = m_listOfModalityNames.indexOf(ui->comboBoxTargeImage->currentText());
	lumenExtractionFilter->SetInputData(m_listOfVtkImages[index]);
	lumenExtractionFilter->Update();

	/* If the ExtractRadius check box is checked */
	if (this->ui->checkBoxExtractWithRadius->isChecked()) 
	{
		/* Generate polyline from seeds */
		vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
		this->ConstructPolydataFromSeeds(pd);

		/* Use mask to process the segmentation */
		vtkSmartPointer<vtkImageData> im = vtkSmartPointer<vtkImageData>::New();
		this->ExtractSegment(lumenExtractionFilter->GetOutput(), im, pd);

		m_imageViewer->GetOverlay()->vtkShallowCopyImage(
			im);
	}
	else
	{
		m_imageViewer->GetOverlay()->vtkShallowCopyImage(lumenExtractionFilter->GetOutput());
	}
	MY_VIEWER_CONSTITERATOR(Render());

	//ExtractLumenPolyData();
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

			//LumenSegmentationFilter2::ReorderPolyData(_connectivityFilter->GetOutput());

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
		connect(ui->doubleSpinBoxExtractRadius, SIGNAL(valueChanged(double)),
			this, SLOT(SlotUpdateHSliderExtractRadius()));
		connect(ui->hSliderExtractRadius, SIGNAL(valueChanged(int)),
			this, SLOT(SlotUpdateSpinBoxExtractRadius()));

	}
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(SlotClearAllSeeds()));
}

QInteractorStyleLumenSeedsPlacer::~QInteractorStyleLumenSeedsPlacer()
{
	QDELETE_UI();

}
//
//void QInteractorStyleLumenSeedsPlacer::uniqueInvoke(bool flag)
//{
//	QAbstractNavigation::uniqueInvoke(flag);
//
//	if (flag && flag != initializationFlag) {
//		// turn on codes
//
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//	}
//	// turn off
//	if (!flag && flag != initializationFlag) {
//		// turn off codes
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//	}
//	if (flag != initializationFlag) {
//
//	}
//	initializationFlag = flag;
//}

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

void QInteractorStyleLumenSeedsPlacer::ConstructPolydataFromSeeds(vtkPolyData* outPD)
{
	if (!outPD)
	{
		vtkErrorMacro("Input polydata pointer must be allocated before inputing to this function!");
		return;
	}

	/// Initiate reconstruction view
	// Construct a polyline from the way point 
	vtkSmartPointer<vtkPolyData> polyline = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(m_seeds[SegmentationSeeds].size());
	int iteratorCursor = 0;
	for (std::list<int*>::const_iterator iter = m_seeds[SegmentationSeeds].cbegin();
	iter != m_seeds[SegmentationSeeds].cend(); iter++)
	{
		int* imagePos = *iter;
		double worldPos[3];
		for (int pos = 0; pos < 3; ++pos) {
			worldPos[pos] = (imagePos[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
		}
		pts->InsertNextPoint(worldPos);
		cells->InsertCellPoint(iteratorCursor);
		iteratorCursor += 1;
	}
	iteratorCursor = 0;
	polyline->SetPoints(pts);
	polyline->SetLines(cells);

	// Interpolate the polyline to form a uniformly seperated polyline 
	/* First calculate the total length of the line */
	double totalLength = 0;
	for (int i = 1; i < pts->GetNumberOfPoints(); i++)
	{
		double segmentVect[3], prevPoint[3];
		memcpy(prevPoint, pts->GetPoint(i - 1), sizeof(double) * 3);
		vtkMath::Subtract(pts->GetPoint(i), prevPoint, segmentVect);
		totalLength += vtkMath::Norm(segmentVect);
	}
	/* Calculate subdivision value, default spacing will be 1mm */
	double defaultSpacing_mm = 1;
	int subdivision = int(ceil(totalLength / defaultSpacing_mm)) + 1;

	/* Interpolate the polyline */
	vtkSmartPointer<vtkCardinalSpline> spline = vtkSmartPointer<vtkCardinalSpline>::New();
	spline->SetLeftConstraint(2);
	spline->SetLeftValue(0.0);
	spline->SetRightConstraint(2);
	spline->SetRightValue(0.0);
	vtkSmartPointer<vtkSplineFilter> splineFilter = vtkSmartPointer<vtkSplineFilter>::New();
	splineFilter->SetInputData(polyline);
	splineFilter->SetSpline(spline);
	splineFilter->SetNumberOfSubdivisions(subdivision);
	splineFilter->Update();
	outPD->DeepCopy(splineFilter->GetOutput());
}

void QInteractorStyleLumenSeedsPlacer::ExtractSegment(vtkImageData* inImage, 
	vtkImageData* outImage, vtkPolyData* inPolydata)
{
	vtkSmartPointer<vtkPolylineToTubularVolume> pd2vol 
		= vtkSmartPointer<vtkPolylineToTubularVolume>::New();
	pd2vol->SetInputData(inImage);
	pd2vol->SetPolyline(inPolydata);
	pd2vol->SetTubeRadius(this->ui->doubleSpinBoxExtractRadius->value()*10);
	pd2vol->Update();

	vtkSmartPointer<vtkImageMask> maskFilter = vtkSmartPointer<vtkImageMask>::New();
	maskFilter->SetInput1Data(inImage);
	maskFilter->SetMaskInputData(pd2vol->GetOutput());
	maskFilter->Update();

	outImage->DeepCopy(maskFilter->GetOutput());
}

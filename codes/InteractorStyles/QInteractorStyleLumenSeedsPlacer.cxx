#include "QInteractorStyleLumenSeedsPlacer.h"

#include <vtkRenderWindowInteractor.h>
//#include <itkImageToVTKImageFilter.h>

#include "ui_QInteractorStyleLumenSeedsPlacer.h"
#include "ui_QAbstractNavigation.h"
//#include "lumenExtraction.h"


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
	//m_lumenExtraction->SetInputData(m_imageViewer->GetOriginalInput());
	//QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	//for (int i = 0; i < _seeds.size(); ++i) {
	//	m_lumenExtraction->AddSeed(_seeds[i]);
	//}
	//m_lumenExtraction->Update();
	//vtkSmartPointer<vtkImageData> overlay = vtkSmartPointer<vtkImageData>::New();
	//m_lumenExtraction->GetLumenVtkImage(overlay);
	//m_imageViewer->GetOverlay()->ReplacePixels(overlay->GetExtent(), overlay);

	//typedef itk::ImageToVTKImageFilter<double> ToVTKImage;
	//ToVTKImage::
	

}

void QInteractorStyleLumenSeedsPlacer::SetMultipier(double value)
{
	//if (m_lumenExtraction != nullptr) {
	//	m_lumenExtraction->SetMultiplier(value);
	//}
}

void QInteractorStyleLumenSeedsPlacer::SetInitialNeighborhoodRadius(int value)
{
	//if (m_lumenExtraction != nullptr) {
	//	m_lumenExtraction->SetInitialNeighborhoodRadius(value);
	//}
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
		connect(ui->spinBoxNeighborRadius, SIGNAL(valueChanged(int)),
			this, SLOT(SetInitialNeighborhoodRadius(int)));

		//m_lumenExtraction = new LumenExtraction;
		//m_lumenExtraction->SetNumberOfIterations(30);
		//m_lumenExtraction->SetDilationValue(4);
		//m_lumenExtraction->SetMultiplier(2.3);
		//m_lumenExtraction->SetInitialNeighborhoodRadius(1);

		
		//m_lumenExtractionFilter = LumenExtractionFilter::New();
		//m_lumenExtractionFilter->SetNumberOfIterations(30);
		//m_lumenExtractionFilter->SetDilationValue(4);
		//m_lumenExtractionFilter->SetMultiplier(2.3);
		//m_lumenExtractionFilter->SetInitialNeighborhoodRadius(1);
	}
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(SlotClearAllSeeds()));
}

QInteractorStyleLumenSeedsPlacer::~QInteractorStyleLumenSeedsPlacer()
{
	QDELETE_UI();

	//if (m_lumenExtraction != nullptr) {
	//	delete m_lumenExtraction;
	//}
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

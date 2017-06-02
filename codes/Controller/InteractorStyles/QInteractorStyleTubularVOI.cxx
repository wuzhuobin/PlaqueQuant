#include "QInteractorStyleTubularVOI.h"
#include "ui_QInteractorStyleTubularVOI.h"
#include "ui_QInteractorStyleLumenSeedsPlacer.h"

#include "vtkPolylineToTubularVolume.h"
#include "ImageViewer.h"

#include <vtkObjectFactory.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkImageMask.h>
#include <vtkImageBlend.h>

vtkStandardNewMacro(QInteractorStyleTubularVOI);
QSETUP_UI_SRC(QInteractorStyleTubularVOI);
QList<int*> QInteractorStyleTubularVOI::m_tubularSeeds;

void QInteractorStyleTubularVOI::GenerateWidgetFromSeeds() 
{
	QInteractorStyleLumenSeedsPlacer::GenerateWidgetFromSeeds(m_tubularSeeds);
}

void QInteractorStyleTubularVOI::ExtractSegmentation(QList<int*>& seed)
{
	UpdateSpline(seed);

	vtkSmartPointer<vtkPolylineToTubularVolume> polylineToTubularVolume =
		vtkSmartPointer<vtkPolylineToTubularVolume>::New();
	polylineToTubularVolume->SetInputData(GetImageViewer()->GetOverlay());
	polylineToTubularVolume->SetPolyline(m_spline);
	polylineToTubularVolume->SetTubeRadius(m_extractRadius);
	polylineToTubularVolume->Update();

	vtkSmartPointer<vtkImageMask> maskFilter = vtkSmartPointer<vtkImageMask>::New();
	maskFilter->SetInput1Data(GetImageViewer()->GetOverlay());
	maskFilter->SetMaskInputData(polylineToTubularVolume->GetOutput());
	maskFilter->Update();

	vtkSmartPointer<vtkImageBlend> imageBlend =
		vtkSmartPointer<vtkImageBlend>::New();
	imageBlend->AddInputData(GetImageViewer()->GetOverlay());
	imageBlend->AddInputConnection(maskFilter->GetOutputPort());
	imageBlend->SetOpacity(0, 0);
	imageBlend->SetOpacity(1, 1);
	imageBlend->Update();

	GetImageViewer()->GetOverlay()->ShallowCopy(imageBlend->GetOutput());
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, GetImageViewer()->Render());

}

void QInteractorStyleTubularVOI::ExtractVOI(QList<int*>& seed)
{
	vtkSmartPointer<vtkPolyData> splinePoints =
		vtkSmartPointer<vtkPolyData>::New();
	splinePoints->SetPoints(vtkSmartPointer<vtkPoints>::New());
	for (QList<int*>::const_iterator cit = seed.cbegin(); cit != seed.cend(); ++cit) {
		splinePoints->GetPoints()->InsertNextPoint((*cit)[0], (*cit)[1], (*cit)[2]);
	}

	int extent[6] = {
		static_cast<int>(splinePoints->GetBounds()[0] - m_extractRadius / GetSpacing()[0] * 1),
		static_cast<int>(splinePoints->GetBounds()[1] + m_extractRadius / GetSpacing()[0] * 1),
		static_cast<int>(splinePoints->GetBounds()[2] - m_extractRadius / GetSpacing()[1] * 1),
		static_cast<int>(splinePoints->GetBounds()[3] + m_extractRadius / GetSpacing()[1] * 1),
		static_cast<int>(splinePoints->GetBounds()[4] - m_extractRadius / GetSpacing()[2] * 1),
		static_cast<int>(splinePoints->GetBounds()[5] + m_extractRadius / GetSpacing()[2] * 1)
	};

	GetImageViewer()->SetDisplayExtent(extent);
	SetExtentRange(GetImageViewer()->GetDisplayExtent());
	GetImageViewer()->Render();
}

void QInteractorStyleTubularVOI::UpdateWidgetToSeeds(QList<int*>& seeds, int * newImagePos, int * oldImagePos)
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
		QString listItem = "way point: [" +
			QString::number(newImagePos[0]) + "," +
			QString::number(newImagePos[1]) + "," +
			QString::number(newImagePos[2]) + "]";
		GetListWidget()->item(seeds.indexOf(*it))->setText(listItem);
	}
	else {
		seeds.push_back(new int[3]);
		memcpy(seeds.back(), newImagePos, sizeof(int[3]));
		QString listItem = "way point: [" +
			QString::number(seeds.back()[0]) + "," +
			QString::number(seeds.back()[1]) + "," +
			QString::number(seeds.back()[2]) + "]";
		GetListWidget()->addItem(listItem);
	}
}

vtkPolyData* QInteractorStyleTubularVOI::UpdateSpline(QList<int*>& seed)
{
	m_spline = vtkSmartPointer<vtkPolyData>::New();

	vtkSmartPointer<vtkPolyData> splinePoints =
		vtkSmartPointer<vtkPolyData>::New();
	splinePoints->SetPoints(vtkSmartPointer<vtkPoints>::New());
	for (QList<int*>::const_iterator cit = seed.cbegin(); cit != seed.cend(); ++cit) {
		splinePoints->GetPoints()->InsertNextPoint((*cit)[0], (*cit)[1], (*cit)[2]);
		//double worldPos[3];
		//for (int pos = 0; pos < 3; ++pos) {
		//	worldPos[pos] = ((*cit)[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
		//}
		//splinePoints->GetPoints()->InsertNextPoint(worldPos);
	}

	vtkSmartPointer<vtkTransform> translation =
		vtkSmartPointer<vtkTransform>::New();
	// because default is premultiply 
	// T* (S * Points)
	translation->Translate(GetOrigin());
	translation->Scale(GetSpacing());
	translation->Update();

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInputData(splinePoints);
	transformFilter->SetTransform(translation);
	transformFilter->Update();


	vtkSmartPointer<vtkParametricSpline> spline =
		vtkSmartPointer<vtkParametricSpline>::New();
	//spline->SetPoints(splinePoints->GetPoints());
	spline->SetPoints(transformFilter->GetOutput()->GetPoints());

	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(spline);
	functionSource->Update();
	m_spline->ShallowCopy(functionSource->GetOutput());
	return m_spline;
}

vtkPolyData* QInteractorStyleTubularVOI::UpdateSpline()
{
	return UpdateSpline(m_tubularSeeds);
	
}

void QInteractorStyleTubularVOI::SaveWidgetToSeeds()
{
	QInteractorStyleLumenSeedsPlacer::SaveWidgetToSeeds(m_tubularSeeds);
}

void QInteractorStyleTubularVOI::ClearAllSeeds()
{
	QInteractorStyleLumenSeedsPlacer::ClearAllSeeds(m_tubularSeeds);
}

void QInteractorStyleTubularVOI::SetFocalSeed(int i)
{
	QInteractorStyleLumenSeedsPlacer::SetFocalSeed(i, m_tubularSeeds);
	const double* worldCoordinate = GetImageViewer()->GetFocalPointWithWorldCoordinate();
	//char label[60];
	//sprintf_s(label, "(%.3f, %.3f, %.3f)", worldCoordinate[0], worldCoordinate[1], worldCoordinate[2]);
	QString label = "(" + QString::number(worldCoordinate[0], 'f', 3) + "," +
		QString::number(worldCoordinate[1], 'f', 3) + "," +
		QString::number(worldCoordinate[2], 'f', 3) + ")";
	/// change label text
	this->ui->labelSeedCoordinate->setText(label);
}

void QInteractorStyleTubularVOI::DeleteFocalSeed()
{
	QInteractorStyleLumenSeedsPlacer::DeleteFocalSeed(m_tubularSeeds);

}

void QInteractorStyleTubularVOI::DropSeed()
{
	QInteractorStyleLumenSeedsPlacer::DropSeed(m_tubularSeeds);
}

void QInteractorStyleTubularVOI::UpdateWidgetToSeeds(int * newImagePos, int * oldImagePos)
{
	QInteractorStyleTubularVOI::UpdateWidgetToSeeds(m_tubularSeeds, newImagePos, oldImagePos);
}

void QInteractorStyleTubularVOI::SetExtractRadius(int radius)
{
	this->m_extractRadius = radius;
}

void QInteractorStyleTubularVOI::ExtractSegmentation()
{
	ExtractSegmentation(m_tubularSeeds);
}

void QInteractorStyleTubularVOI::ExtractVOI()
{
	ExtractVOI(m_tubularSeeds);
}

void QInteractorStyleTubularVOI::ResetVOI()
{
	GetImageViewer()->ResetDisplayExtent();
	SetExtentRange(GetImageViewer()->GetDisplayExtent());
	GetImageViewer()->Render();
}

QInteractorStyleTubularVOI::QInteractorStyleTubularVOI(int uiType, QWidget * parent)
	:QInteractorStyleLumenSeedsPlacer(NO_UI, this)
{
	QNEW_UI();
}

QInteractorStyleTubularVOI::~QInteractorStyleTubularVOI()
{
	QDELETE_UI();
}

void QInteractorStyleTubularVOI::uniqueInitialization()
{
	connect(ui->pushBtnDeleteSeed, SIGNAL(clicked()),
		this, SLOT(DeleteFocalSeed()));
	connect(GetListWidget(), SIGNAL(currentRowChanged(int)),
		this, SLOT(SetFocalSeed(int)));
	connect(ui->dropSeedPushButton, SIGNAL(clicked()),
		this, SLOT(DropSeed()));
	connect(ui->spinBoxExtractRadius, SIGNAL(valueChanged(int)),
		this, SLOT(SetExtractRadius(int)));
	connect(ui->pushButtonExtractSegmentation, SIGNAL(clicked()),
		this, SLOT(ExtractSegmentation()));
}

void QInteractorStyleTubularVOI::initialization()
{
	//disconnect(QInteractorStyleLumenSeedsPlacer::getUi()->deleteAllSeedsPushButton, SIGNAL(clicked()),
	//	this, SLOT(ClearAllSeeds()));
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(ClearAllSeeds()));
	connect(ui->pushButtonExtractVOI, SIGNAL(clicked()),
		this, SLOT(ExtractVOI()));
	connect(ui->pushButtonResetVOI, SIGNAL(clicked()),
		this, SLOT(ResetVOI()));
}

void QInteractorStyleTubularVOI::uniqueEnable()
{
	// dont invoke 	QInteractorStyleLumenSeedsPlacer#UpdateTargetViewer()
	QAbstractNavigation::uniqueEnable();
}

QListWidget * QInteractorStyleTubularVOI::GetListWidget()
{
	return ui->listWidgetSeedList;
}


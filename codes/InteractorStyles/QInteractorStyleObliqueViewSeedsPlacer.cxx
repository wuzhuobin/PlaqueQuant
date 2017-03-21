#include "QInteractorStyleObliqueViewSeedsPlacer.h"

#include <vtkCardinalSpline.h>
#include <vtkSplineFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkCellArray.h>
#include <vtkvmtkPolyDataCenterlines.h>
#include <vtkvmtkCurvedMPRImageFilterNearestInterpolation.h>
#include <vtkvmtkCenterlineAttributesFilter.h>
#include <vtkvmtkCenterlineGeometry.h>
#include <vtkIdList.h>
#include <vtkMath.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkConnectivityFilter.h>

#include "LumenSegmentationFilter2.h"
#include "ui_QInteractorStyleObliqueViewSeedsPlacer.h"
#include "ui_QAbstractNavigation.h"
#include "vtkPlane.h"
#include "vtkCamera.h"
#include "vtkImageSlice.h"
#include "vtkImageProperty.h"
#include "MeasurementFor2D.h"
#include "vtkPolylineToTubularVolume.h"
#include "vtkImageMask.h"
#include "SurfaceCreator.h"
#include "InteractorStyleNavigation.h"
#include "MeasurementWidget.hpp"
#include "MainWindow.h"

// DEBUG
#include "vtkXMLPolyDataWriter.h"
#include "vtkNIFTIImageWriter.h"
#include "vtkCell.h"

vtkStandardNewMacro(QInteractorStyleObliqueViewSeedsPlacer);
QSETUP_UI_SRC(QInteractorStyleObliqueViewSeedsPlacer);
using namespace std;

QInteractorStyleObliqueViewSeedsPlacer::QInteractorStyleObliqueViewSeedsPlacer(int uiType, QWidget* parent)
{
	QNEW_UI();
	if (numOfMyself == 1) {
		connect(ui->pushBtnDeleteSeed, SIGNAL(clicked()),
			this, SLOT(DeleteFocalSeed()));
		connect(ui->listWidgetSeedList, SIGNAL(currentRowChanged(int)),
			this, SLOT(SetFocalSeed(int)));
		connect(ui->dropSeedPushButton, SIGNAL(clicked()),
			this, SLOT(DropSeed()));
		connect(ui->pushBtnAxialReconstruction, SIGNAL(clicked()),
			this, SLOT(slotPushBtnAxialReconstruction()));
		connect(ui->doubleSpinBoxExtractRadius, SIGNAL(valueChanged(double)),
			this, SLOT(slotUpdateHSliderExtractRadius()));
		connect(ui->hSliderExtractRadius, SIGNAL(valueChanged(int)),
			this, SLOT(slotUpdateSpinBoxExtractRadius()));
		connect(ui->pushBtnExtractSegmentation, SIGNAL(clicked()),
			this, SLOT(slotExtractSegmentFromOverlay()));
	}
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(slotClearAllSeeds()));
}

QInteractorStyleObliqueViewSeedsPlacer::~QInteractorStyleObliqueViewSeedsPlacer()
{
	if (s_curvedImage)
	{
		s_curvedImage->Delete();
		s_curvedImage = NULL;
	}

	if (s_curvedOverlay)
	{
		s_curvedOverlay->Delete();
		s_curvedOverlay = NULL;
	}

	while (s_vectImageStore.size())
	{
		vtkImageData* img = s_vectImageStore.back();
		s_vectImageStore.pop_back();
		if (img != NULL)
			img->Delete();
	}

	this->CurrentRenderer = NULL;

	QDELETE_UI();
}

void QInteractorStyleObliqueViewSeedsPlacer::slotUpdateSpinBoxExtractRadius()
{
	this->ui->doubleSpinBoxExtractRadius->setValue(this->ui->hSliderExtractRadius->value() / 10.);
}

void QInteractorStyleObliqueViewSeedsPlacer::slotUpdateHSliderExtractRadius()
{
	this->ui->hSliderExtractRadius->setValue(this->ui->doubleSpinBoxExtractRadius->value() * 10);
}



void QInteractorStyleObliqueViewSeedsPlacer::SetSeedsPlacerEnable(bool flag)
{
	InteractorStyleSeedsPlacer::SetSeedsPlacerEnable(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleObliqueViewSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleObliqueViewSeedsPlacer::EnableObliqueView(bool state)
{
	if (state && this->m_inObliqueView == false)
	{
		/// Replace image with curved MPR image
		STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, SetImageToViewer(s_curvedImage, s_curvedOverlay));
		STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, SetSeedsPlacerEnable(false));


		STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, CenterCursorPosition());
		MY_VIEWER_CONSTITERATOR(Render());
		MY_VIEWER_CONSTITERATOR(ResetCamera());

		MainWindow::GetMainWindow()->EnableImageSelection(false);
		this->m_inObliqueView = true;
	}
	else if (this->m_inObliqueView) {
		/// Replace image back
		STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, SetImageToViewer(s_vectImageStore.at(0), s_vectImageStore.at(1)));
		STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, SetSeedsPlacerEnable(true));

		STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, CenterCursorPosition());
		MY_VIEWER_CONSTITERATOR(Render());
		MY_VIEWER_CONSTITERATOR(ResetCamera());

		MainWindow::GetMainWindow()->EnableImageSelection(true);
		this->m_inObliqueView = false;
	}
}


void QInteractorStyleObliqueViewSeedsPlacer::SetInteractor(vtkRenderWindowInteractor *interactor)
{
	if (interactor)
	{
		this->CurrentRenderer = interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
	}

	Superclass::SetInteractor(interactor);
}

void QInteractorStyleObliqueViewSeedsPlacer::UpdateWidgetToSeeds(int * oldImagePos, int* newImagePos)
{
	InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(oldImagePos, newImagePos);
	UpdateSeedTable();

}

void QInteractorStyleObliqueViewSeedsPlacer::UpdateSeedTable()
{
	ui->listWidgetSeedList->clear();
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds[m_currentSeedsType]);
	//for (QList<int*>::ConstIterator cit = _seeds.cbegin(); cit != _seeds.cend(); ++cit) {
	//	ui->listWidgetSeedList->insertItem()
	//}
	for (int i = 0; i < _seeds.size(); ++i) {
		QString listItem = "Way Point " + QString::number(i) + ": [" +
			QString::number(_seeds[i][0]) + "," +
			QString::number(_seeds[i][1]) + "," +
			QString::number(_seeds[i][2]) + "]";
		ui->listWidgetSeedList->insertItem(i, listItem);
	}
}

void QInteractorStyleObliqueViewSeedsPlacer::slotClearAllSeeds()
{
	ClearAllSeeds();
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, ClearAllSeedWidget());
	ui->listWidgetSeedList->clear();
	m_imageViewer->Render();
}

void QInteractorStyleObliqueViewSeedsPlacer::slotPushBtnAxialReconstruction()
{
	this->InitializeObliqueView();
	//STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, InitializeObliqueView());
}


void QInteractorStyleObliqueViewSeedsPlacer::slotExtractSegmentFromOverlay()
{
	if (QList<int*>::fromStdList(m_seeds[this->m_currentSeedsType]).count() > 1)
	{
		/* Generate polyline from seeds */
		vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
		this->InterpolateWayPointsToPolyline(pd);

		/* Use mask to process the segmentation */
		vtkSmartPointer<vtkImageData> im = vtkSmartPointer<vtkImageData>::New();
		this->ExtractSegment(m_imageViewer->GetOverlay()->GetVTKOutput(), im, pd);

		m_imageViewer->GetOverlay()->vtkShallowCopyImage(
			im);
		MY_VIEWER_CONSTITERATOR(Render());
	}
}


void QInteractorStyleObliqueViewSeedsPlacer::InitializeObliqueView()
{
	/// Error check
	// If the way point has less than two point
	if (ui->listWidgetSeedList->count() < 2)
	{
		ui->labelReconstructionStatus->setText(QString("Require more then two way point to construct reslice path."));
		return;
	}
	else
	{
		ui->labelReconstructionStatus->setText("");
	}

	if (!this->m_inObliqueView)
	{
		/// Spline path
			// Construct a polyline from the way point 
		vtkSmartPointer<vtkPolyData> interpedPD = vtkSmartPointer<vtkPolyData>::New();
		this->InterpolateWayPointsToPolyline(interpedPD);

		/// Extract lumen 
		//// User should extract lumen themself
		//this->slotExtractSegmentFromOverlay();

		/// Marching cube and obtain surface
		// thresehold image first
		SurfaceCreator* sfc = new SurfaceCreator();
		sfc->SetInput(this->m_imageViewer->GetOverlay()->GetVTKOutput());
		sfc->SetDiscrete(1);
		sfc->SetValue(Core::LABEL_LUMEN); // #lumenLabel
		sfc->SetSmoothIteration(10);
		sfc->SetResamplingFactor(1);
		sfc->SetLargestConnected(true);
		sfc->Update();
		vtkSmartPointer<vtkPolyData> bloodVesselPD = vtkSmartPointer<vtkPolyData>::New();
		bloodVesselPD->DeepCopy(sfc->GetOutput());
		delete sfc;

		/// Find centerline
		// Check if there are actually a surface
		vtkSmartPointer<vtkPolyData> centerline = vtkSmartPointer<vtkPolyData>::New();
		if (bloodVesselPD->GetNumberOfPoints() > 20)
		{
			// push first and last point into the polydata
			bloodVesselPD->GetPoints()->InsertNextPoint(interpedPD->GetPoint(0));
			bloodVesselPD->GetPoints()->InsertNextPoint(interpedPD->GetPoint(interpedPD->GetNumberOfPoints() - 1));
			vtkIdList *sourceID = vtkIdList::New();
			vtkIdList *sinkID = vtkIdList::New();
			sourceID->InsertNextId(bloodVesselPD->GetNumberOfPoints() - 2);
			sinkID->InsertNextId(bloodVesselPD->GetNumberOfPoints() - 1);
			// Use centerline filter to find centerline
			vtkSmartPointer<vtkvmtkPolyDataCenterlines> centerlineFilter = vtkSmartPointer<vtkvmtkPolyDataCenterlines>::New();
			centerlineFilter->SetInputData(bloodVesselPD);
			centerlineFilter->SetSourceSeedIds(sourceID);
			centerlineFilter->SetTargetSeedIds(sinkID);
			centerlineFilter->SetRadiusArrayName("Radius");
			centerlineFilter->SetEdgeArrayName("Edge");
			centerlineFilter->SetEdgePCoordArrayName("PCoord");
			centerlineFilter->SetAppendEndPointsToCenterlines(true);
			centerlineFilter->Update();
			// Interpolate the centerline
			centerline->DeepCopy(centerlineFilter->GetOutput());
			this->InterpolatePolyline(centerline, centerline);
		}

		////DEBUG
		//vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
		//writer->SetFileName("C:/Users/lwong/Source/datacenter/Segment.vtp");
		//writer->SetInputData(bloodVesselPD);
		//writer->Update();
		//writer->Write();
		//writer->SetFileName("C:/Users/lwong/Source/datacenter/centerline.vtp");
		//writer->SetInputData(centerline);
		//writer->Update();
		//writer->Write();

		/// Use centerline to obtain curved MPR
		vtkSmartPointer<vtkvmtkCenterlineGeometry> centerlineGeometry =
			vtkSmartPointer<vtkvmtkCenterlineGeometry>::New();
		if (centerline->GetNumberOfPoints() > 25)
		{
			// Select centerline if centerline seems to be good
			centerlineGeometry->SetInputData(centerline);
			this->ui->labelReconstructionStatus->setText(QString(""));
		}
		else
		{
			// otherwise, use the line interpolated from input seeds
			centerlineGeometry->SetInputData(interpedPD);
			this->ui->labelReconstructionStatus->setText(QString("Centerline extraction failed, using users' waypoints as reference."));
		}
		centerlineGeometry->SetLengthArrayName("Length");
		centerlineGeometry->SetCurvatureArrayName("Curvature");
		centerlineGeometry->SetTorsionArrayName("Torsion");
		centerlineGeometry->SetTortuosityArrayName("Tortuosity");
		centerlineGeometry->SetFrenetTangentArrayName("FrenetTangent");
		centerlineGeometry->SetFrenetNormalArrayName("FrenetNormal");
		centerlineGeometry->SetFrenetBinormalArrayName("FrenetBinormal");
		centerlineGeometry->LineSmoothingOff();
		centerlineGeometry->Update();

		vtkSmartPointer<vtkvmtkCenterlineAttributesFilter> centerlineAttributes =
			vtkSmartPointer<vtkvmtkCenterlineAttributesFilter>::New();
		centerlineAttributes->SetInputConnection(centerlineGeometry->GetOutputPort());
		centerlineAttributes->SetAbscissasArrayName("Abscissas");
		centerlineAttributes->SetParallelTransportNormalsArrayName("Normals");
		centerlineAttributes->Update();

		double *oriSpacing = this->m_imageViewer->GetOverlay()->GetVTKOutput()->GetSpacing();
		double minSpacing = *std::min_element(oriSpacing, oriSpacing + 3);

		// Prepare multi planar reconstruction for baseImage
		double radius = this->ui->doubleSpinBoxExtractRadius->value() * 10; // mm
		int size_XY = ceil(radius / minSpacing)*2.5;
		vtkSmartPointer<vtkvmtkCurvedMPRImageFilter> curvedMPRImageFilter =
			vtkSmartPointer<vtkvmtkCurvedMPRImageFilter>::New();
		curvedMPRImageFilter->SetInputData(this->m_imageViewer->GetWindowLevel()->GetInput());
		curvedMPRImageFilter->SetCenterline(centerlineAttributes->GetOutput());
		curvedMPRImageFilter->SetParallelTransportNormalsArrayName("Normals");
		curvedMPRImageFilter->SetFrenetTangentArrayName("FrenetTangent");
		curvedMPRImageFilter->SetInplaneOutputSpacing(minSpacing, minSpacing);
		curvedMPRImageFilter->SetInplaneOutputSize(size_XY, size_XY);
		curvedMPRImageFilter->SetReslicingBackgroundLevel(0.0);
		curvedMPRImageFilter->Update();
		
		if (s_curvedImage)
		{
			s_curvedImage->Delete();
			s_curvedImage = NULL;
		}
		s_curvedImage = vtkImageData::New();
		s_curvedImage->DeepCopy(curvedMPRImageFilter->GetOutput());

		// Prepare MPR for overlay 
		vtkSmartPointer<vtkvmtkCurvedMPRImageFilterNearestInterpolation> curvedMPRImageFilter_overlay =
			vtkSmartPointer<vtkvmtkCurvedMPRImageFilterNearestInterpolation>::New();
		curvedMPRImageFilter_overlay->SetInputData(this->m_imageViewer->GetOverlay()->GetVTKOutput());
		curvedMPRImageFilter_overlay->SetCenterline(centerlineAttributes->GetOutput());
		curvedMPRImageFilter_overlay->SetParallelTransportNormalsArrayName("Normals");
		curvedMPRImageFilter_overlay->SetFrenetTangentArrayName("FrenetTangent");
		curvedMPRImageFilter_overlay->SetInplaneOutputSpacing(minSpacing, minSpacing);
		curvedMPRImageFilter_overlay->SetInplaneOutputSize(size_XY, size_XY);
		curvedMPRImageFilter_overlay->SetReslicingBackgroundLevel(0.0);
		curvedMPRImageFilter_overlay->Update();

		if (s_curvedOverlay)
		{
			s_curvedOverlay->Delete();
			s_curvedOverlay = NULL;
		}
		s_curvedOverlay = vtkImageData::New();
		s_curvedOverlay->DeepCopy(curvedMPRImageFilter_overlay->GetOutput());

		/// Extract center-area using the filter
		//vtkSmartPointer<vtkPolyData> straightline = vtkSmartPointer<vtkPolyData>::New();
		//vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
		//lines->InsertCellPoint(0);
		//lines->InsertCellPoint(1);
		//straightline->SetPoints(vtkSmartPointer<vtkPoints>::New());
		//straightline->GetPoints()->InsertNextPoint(0, 0, 0);
		//straightline->GetPoints()->InsertNextPoint(0, 0, s_curvedOverlay->GetSpacing()[2] * (s_curvedOverlay->GetExtent()[5] - 1));
		//straightline->SetLines(lines);
		//vtkSmartPointer<vtkPolylineToTubularVolume> pd2vol
		//	= vtkSmartPointer<vtkPolylineToTubularVolume>::New();
		//pd2vol->SetInputData(s_curvedOverlay);
		//pd2vol->SetPolyline(straightline);
		//pd2vol->SetTubeRadius(this->ui->doubleSpinBoxExtractRadius->value() * 10);
		//pd2vol->Update();
		//vtkSmartPointer<vtkImageMask> maskFilter = vtkSmartPointer<vtkImageMask>::New();
		//maskFilter->SetInput1Data(s_curvedOverlay);
		//maskFilter->SetMaskInputData(pd2vol->GetOutput());
		//maskFilter->Update();
		//s_curvedOverlay->DeepCopy(maskFilter->GetOutput());

		// Store original data first 
		while (s_vectImageStore.size())
		{
			vtkImageData* imag = s_vectImageStore.back();
			imag->Delete();
			s_vectImageStore.pop_back();
		}
		s_vectImageStore.push_back(vtkImageData::New());
		s_vectImageStore.push_back(vtkImageData::New());
		s_vectImageStore.at(0)->DeepCopy(this->m_imageViewer->GetInput());
		s_vectImageStore.at(1)->DeepCopy(this->m_imageViewer->GetOverlay()->GetVTKOutput());

		this->EnableObliqueView(true);
	}
	else {
		this->EnableObliqueView(false);
	}
}


void QInteractorStyleObliqueViewSeedsPlacer::SetImageToViewer(vtkImageData* image, vtkImageData* overlay)
{
	int extent1[6], extent2[6];
	memcpy(extent1, image->GetExtent(), sizeof(int) * 6);
	memcpy(extent2, overlay->GetExtent(), sizeof(int) * 6);

	//this->m_imageViewer->SetInputData(image);
	if (!std::equal(extent1, extent1 + 6, this->m_imageViewer->GetOriginalInput()->GetExtent()))
	{
		// Deep copy once if the extent is different
		this->m_imageViewer->GetOriginalInput()->DeepCopy(image);
	}
	this->m_imageViewer->SetInputData(this->m_imageViewer->GetOriginalInput());
	this->m_imageViewer->ResetImageVOI();
	this->m_imageViewer->GetOverlay()->vtkDeepCopy(overlay);
	this->m_imageViewer->GetOverlay()->GetOutput()->Modified();
	this->m_imageViewer->GetOverlayWindowLevel()->SetUpdateExtent(extent2);
	this->m_imageViewer->GetOverlayWindowLevel()->Update();
	this->m_imageViewer->ResetOverlayVOI();
	this->m_imageViewer->GetOverlayActor()->Update();
	//this->m_imageViewer->SetOverlayVOI(extent2);
	this->m_imageViewer->UpdateDisplayExtent();
	
	////DEBUG
	//vtkSmartPointer<vtkNIFTIImageWriter> writer = vtkSmartPointer<vtkNIFTIImageWriter>::New();
	//writer->SetInputData(overlay);
	//writer->SetFileName("C:/Users/lwong/Source/datacenter/curvedMPROverlay.nii.gz");
	//writer->Update();
	//writer->Write();

}

void QInteractorStyleObliqueViewSeedsPlacer::CenterCursorPosition()
{
	int* extent = this->m_imageViewer->GetInput()->GetExtent();
	int slice;
	switch (this->GetSliceOrientation())
	{
	case 0:
		slice = (extent[1] - extent[0]) / 2.;
		break;
	case 1:
		slice = (extent[3] - extent[2]) / 2.;
		break;
	case 2:
		slice = (extent[5] - extent[4]) / 2.;
		break;
	}

	this->SetCurrentSlice(slice);
}

void QInteractorStyleObliqueViewSeedsPlacer::InterpolateWayPointsToPolyline(vtkPolyData* outPD)
{
	vtkSmartPointer<vtkPolyData> polyline = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(m_seeds[ObliqueViewSeeds].size());
	int iteratorCursor = 0;
	for (std::list<int*>::const_iterator iter = m_seeds[ObliqueViewSeeds].cbegin();
	iter != m_seeds[ObliqueViewSeeds].cend(); iter++)
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

	InterpolatePolyline(polyline, outPD);
}

void QInteractorStyleObliqueViewSeedsPlacer::InterpolatePolyline(vtkPolyData* inPolyline, vtkPolyData* outPD)
{
	// Interpolate the polyline to form a uniformly seperated polyline 
	/* First calculate the total length of the line */
	vtkPoints* pts = inPolyline->GetPoints();
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
	splineFilter->SetInputData(inPolyline);
	splineFilter->SetSpline(spline);
	splineFilter->SetNumberOfSubdivisions(subdivision);
	splineFilter->Update();

	outPD->DeepCopy(splineFilter->GetOutput());
}

void QInteractorStyleObliqueViewSeedsPlacer::SetFocalSeed(int i)
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

void QInteractorStyleObliqueViewSeedsPlacer::DeleteFocalSeed()
{
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds[m_currentSeedsType]);
	int i = ui->listWidgetSeedList->currentRow();
	if (i >= 0 && i < _seeds.size()) {
		ui->listWidgetSeedList->removeItemWidget(
			ui->listWidgetSeedList->takeItem(i));
		m_seeds[m_currentSeedsType].remove(_seeds[i]);
		delete _seeds[i];
	}
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, ClearAllSeedWidget());
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, GenerateWidgetFromSeeds());
	MY_VIEWER_CONSTITERATOR(Render());
}

void QInteractorStyleObliqueViewSeedsPlacer::SaveWidgetToSeeds()
{
	InteractorStyleSeedsPlacer::SaveWidgetToSeeds();
	this->UpdateSeedTable();
}

void QInteractorStyleObliqueViewSeedsPlacer::DropSeed()
{
	InteractorStyleSeedsPlacer::DropSeed();
}



void QInteractorStyleObliqueViewSeedsPlacer::CleanAllLists()
{
	while (this->m_coordList.size())
	{
		int *element = this->m_coordList.last();
		this->m_coordList.pop_back();
		delete element;
	}

	this->m_savedActors.clear();
}

//
//void QInteractorStyleObliqueViewSeedsPlacer::uniqueInvoke(bool flag)
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

void QInteractorStyleObliqueViewSeedsPlacer::OnKeyPress()
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

void QInteractorStyleObliqueViewSeedsPlacer::MoveSliceForward()
{
	Superclass::MoveSliceForward();
}

void QInteractorStyleObliqueViewSeedsPlacer::MoveSliceBackward()
{
	Superclass::MoveSliceBackward();
}

void QInteractorStyleObliqueViewSeedsPlacer::SetObliqueSlice(int sliceIndex)
{
	// DEPREICATED
}

void QInteractorStyleObliqueViewSeedsPlacer::ExtractSegment(vtkImageData* inImage,
	vtkImageData* outImage, vtkPolyData* inPolydata)
{
	vtkSmartPointer<vtkPolylineToTubularVolume> pd2vol
		= vtkSmartPointer<vtkPolylineToTubularVolume>::New();
	pd2vol->SetInputData(inImage);
	pd2vol->SetPolyline(inPolydata);
	pd2vol->SetTubeRadius(this->ui->doubleSpinBoxExtractRadius->value() * 10);
	pd2vol->Update();

	vtkSmartPointer<vtkImageMask> maskFilter = vtkSmartPointer<vtkImageMask>::New();
	maskFilter->SetInput1Data(inImage);
	maskFilter->SetMaskInputData(pd2vol->GetOutput());
	maskFilter->Update();

	outImage->DeepCopy(maskFilter->GetOutput());
}

QList<int*> QInteractorStyleObliqueViewSeedsPlacer::m_coordList;
vtkImageData* QInteractorStyleObliqueViewSeedsPlacer::s_curvedImage;
vtkImageData* QInteractorStyleObliqueViewSeedsPlacer::s_curvedOverlay;
std::vector<vtkImageData*> QInteractorStyleObliqueViewSeedsPlacer::s_vectImageStore;
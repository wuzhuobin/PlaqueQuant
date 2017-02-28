#include "QInteractorStyleObliqueViewSeedsPlacer.h"

#include <vtkCardinalSpline.h>
#include <vtkSplineFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkCellArray.h>
#include <vtkvmtkPolyDataCenterlines.h>
#include <vtkIdList.h>
#include <vtkMath.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>


#include "LumenSegmentationFilter2.h"
#include "ui_QInteractorStyleObliqueViewSeedsPlacer.h"
#include "ui_QAbstractNavigation.h"
#include "vtkPlane.h"
#include "vtkCamera.h"
#include "vtkImageSlice.h"
#include "vtkImageProperty.h"
#include "InteractorStyleNavigation.h"

// DEBUG
#include "vtkXMLPolyDataWriter.h"
#include "vtkNIFTIImageWriter.h"

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
		connect(ui->pushBtnCopyFromSegSeeds, SIGNAL(clicked()),
			this, SLOT(slotPushBtnCopySeedsFromSegmentation()));

	}
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(slotClearAllSeeds()));
}

QInteractorStyleObliqueViewSeedsPlacer::~QInteractorStyleObliqueViewSeedsPlacer()
{
	QDELETE_UI();
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

//void QInteractorStyleObliqueViewSeedsPlacer::SetTargetImages(
//	QList<vtkSmartPointer<vtkImageData>> listOfVtkImages,
//	QList<QString> listOfModalityNames)
//{
//	m_listOfModalityNames = listOfModalityNames;
//	m_listOfVtkImages = listOfVtkImages;
//
//	ui->comboBoxTargeImage->clear();
//	for (int i = 0; i < m_listOfModalityNames.size(); ++i) {
//		if (m_listOfVtkImages[i] != nullptr) {
//			ui->comboBoxTargeImage->addItem(m_listOfModalityNames[i]);
//		}
//	}
//}

void QInteractorStyleObliqueViewSeedsPlacer::EnableObliqueView(bool axialView)
{
	if (this->GetSliceOrientation() == 2)
	{
		// Do only for Axial view
		if (axialView && !m_inObliqueView)
		{
			// Remember all actors
			this->Interactor->GetRenderWindow()->GetRenderers()->InitTraversal();
			while (vtkRenderer* ren = this->Interactor->GetRenderWindow()->GetRenderers()->GetNextItem())
			{
				vtkPropCollection* actors = ren->GetViewProps();
				actors->InitTraversal();
				while (vtkProp* actor = actors->GetNextProp())
				{
					this->m_savedActors.append(actor);
				}
			}

			// Hide original actors
			for (QList<vtkProp*>::iterator iter = this->m_savedActors.begin();
					 iter != this->m_savedActors.end(); iter++)
			{
				if ((*iter)->GetVisibility())
				{
					(*iter)->SetVisibility(false);
				}
				else
				{
					this->m_savedActors.erase(iter);
					iter--;
				}

			}
			MY_VIEWER_CONSTITERATOR(Render());


			// Setup reslicer
			if (this->m_reslicer[0] != NULL)
			{
				this->m_reslicer[0]->RemoveAllObservers();
				this->m_reslicer[0]->Delete();
			}
			if (this->m_resliceMapper[0] != NULL)
				this->m_resliceMapper[0]->Delete();

			// Read currently viewing image
			
			vtkImageData* im = this->m_imageViewer->GetInput();
			vtkImageData* overlay = this->m_imageViewer->GetInputLayer();

			// Declare new mapper and reslicers
			this->m_resliceMapper[0] = vtkSmartPointer<vtkImageResliceMapper>::New();
			this->m_reslicer[0] = vtkSmartPointer<vtkImageSlice>::New();

			//// DEBUG
			//vtkSmartPointer<vtkNIFTIImageWriter> writer = vtkSmartPointer<vtkNIFTIImageWriter>::New();
			//writer->SetInputData(overlay);
			//writer->SetFileName("C:/Users/lwong/Downloads/output.nii.gz");
			//writer->Update();
			//writer->Write();

			this->m_resliceMapper[0]->SetInputData(im);
			this->m_resliceMapper[0]->SetSlicePlane(vtkSmartPointer<vtkPlane>::New());
			this->m_reslicer[0]->SetMapper(this->m_resliceMapper[0]);
			this->m_reslicer[0]->GetProperty()->SetColorLevel(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorLevel());
			this->m_reslicer[0]->GetProperty()->SetColorWindow(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorWindow());
			this->m_reslicer[0]->Update();			
			this->CurrentRenderer->AddViewProp(this->m_reslicer[0]);
			this->CurrentRenderer->SetBackground(0.3, 0.3, 0.3);
			
			// Handle overlay if exist
			if (overlay)
			{
				this->m_imageViewer->GetOverlay()->GetLookupTable()->Print(cout);
				this->m_resliceMapper[1] = vtkSmartPointer<vtkImageResliceMapper>::New();
				this->m_reslicer[1] = vtkSmartPointer<vtkImageSlice>::New();
				this->m_resliceMapper[1]->SetInputData(overlay);
				this->m_resliceMapper[1]->SetSlicePlane(vtkSmartPointer<vtkPlane>::New());
				this->m_reslicer[1]->SetMapper(this->m_resliceMapper[1]);
				this->m_reslicer[1]->GetProperty()->SetLookupTable(this->m_imageViewer->GetOverlay()->GetLookupTable());
				this->m_reslicer[1]->GetProperty()->SetColorWindow(this->m_imageViewer->GetOverlay()->GetLookupTable()->GetNumberOfColors());
				this->m_reslicer[1]->GetProperty()->SetColorLevel(this->m_imageViewer->GetOverlay()->GetLookupTable()->GetNumberOfColors()/2.);
				this->m_reslicer[1]->GetProperty()->SetInterpolationTypeToNearest();
				this->m_reslicer[1]->Update();

				this->CurrentRenderer->AddViewProp(this->m_reslicer[1]);
			}


			this->SetObliqueSlice(0);
			this->Interactor->Render();
			this->m_inObliqueView = true;

			// Disable UI and seed widget
			this->ClearAllSeedWidget();
			InteractorStyleSeedsPlacer::SetSeedsPlacerEnable(false); // This advoid invoking uniqueDisable()
			this->ui->listWidgetSeedList->setDisabled(true);
			this->ui->pushBtnDeleteSeed->setDisabled(true);
			this->ui->dropSeedPushButton->setDisabled(true);
			this->ui->deleteAllSeedsPushButton->setDisabled(true);

			this->m_inObliqueView = true;
		}
		else if (this->m_inObliqueView) {
			// Remove & delete reslice actor
			this->m_reslicer[0]->SetMapper(NULL);
			this->CurrentRenderer->RemoveViewProp(this->m_reslicer[0]);
			if (this->m_reslicer[1])
			{
				this->m_reslicer[1]->SetMapper(NULL);
				this->CurrentRenderer->RemoveViewProp(this->m_reslicer[1]);
			}
			if (!this->Interactor)
				this->CurrentRenderer = NULL;             

			this->m_resliceMapper[0]->RemoveAllObservers();
			if (this->m_resliceMapper[1])
			{
				this->m_resliceMapper[1]->RemoveAllObservers();
			}

			this->m_resliceMapper[0] = NULL;
			this->m_resliceMapper[1] = NULL;
			this->m_reslicer[0] = NULL;
			this->m_reslicer[1] = NULL;

			// Re-enable all actors
			while (this->m_savedActors.size())
			{
				vtkProp* lastElement = this->m_savedActors.last();
				lastElement->SetVisibility(true);
				this->m_savedActors.pop_back();
			}

			this->m_inObliqueView = false;

			// Recalculate camera position
			double newCamPos[3], refVect[3] = { 0, 0, 1 };
			vtkCamera* cam = this->m_imageViewer->GetRenderer()->GetActiveCamera();
			this->m_imageViewer->SetFocalPointWithWorldCoordinate(cam->GetFocalPoint()[0], cam->GetFocalPoint()[1], cam->GetFocalPoint()[2]);
			cam->SetViewUp(0, -1, 0);
			vtkMath::MultiplyScalar(refVect, cam->GetDistance());
			vtkMath::Add(cam->GetFocalPoint(), refVect, newCamPos);
			cam->SetPosition(newCamPos);

			// Enable UI and seedwidget
			InteractorStyleSeedsPlacer::SetSeedsPlacerEnable(true); // This advoid invoking uiqueEnable()
			this->ui->listWidgetSeedList->setDisabled(false);
			this->ui->pushBtnDeleteSeed->setDisabled(false);
			this->ui->dropSeedPushButton->setDisabled(false);
			this->ui->deleteAllSeedsPushButton->setDisabled(false);
			this->m_inObliqueView = false;
		}
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
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, InitializeObliqueView());
}


void QInteractorStyleObliqueViewSeedsPlacer::slotPushBtnCopySeedsFromSegmentation()
{
	this->CopySegmentationSeeds();
}

void QInteractorStyleObliqueViewSeedsPlacer::InitializeObliqueView()
{
	/// Error check
	// Trigger this function only in the AXIAL view
	if (this->GetSliceOrientation() != 2)
		return;
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

	if (m_inObliqueView)
	{
		/// Stop reconstruction view
		this->EnableObliqueView(false);
	}
	else {
		/// Initiate reconstruction view
		// Construct a polyline from the way point 
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

		// Interpolate the polyline to form a uniformly seperated polyline 
		/* First calculate the total length of the line */
		double totalLength = 0;
		for (int i = 1; i < pts->GetNumberOfPoints();i++)
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
		vtkSmartPointer<vtkPolyData> interpolated = splineFilter->GetOutput();

		//// Debug
		//vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
		//writer->SetInputData(interpolated);
		//writer->SetFileName("C:\\Users\\lwong\\Source\\datacenter\\interpPolyline.vtp");
		//writer->Update();
		//writer->Write();
		

		// Construct the normal list and the origin list for reslicer;
		/* clean the list first */
		this->CleanAllLists();
		for (int i = 0; i < interpolated->GetNumberOfPoints(); i++)
		{
			double *normal = (double*)malloc(sizeof(double) * 3);
			double *origin = (double*)malloc(sizeof(double) * 3);
			int* ijk = (int*)malloc(sizeof(int) * 3);
			double p1[3], p2[3];

			/* last point */
			if (i == interpolated->GetNumberOfPoints() - 1)
			{
				memcpy(p1, interpolated->GetPoint(i - 1), sizeof(double) * 3);
				memcpy(p2, interpolated->GetPoint(i), sizeof(double) * 3);
				memcpy(origin, p2, sizeof(double) * 3);
			}
			else {

				memcpy(p1, interpolated->GetPoint(i), sizeof(double) * 3);
				memcpy(p2, interpolated->GetPoint(i + 1), sizeof(double) * 3);
				memcpy(origin, p1, sizeof(double) * 3);
			}
			
			/* calculate back the ijk*/
			ijk[0] = (origin[0] - GetOrigin()[0]) / GetSpacing()[0];
			ijk[1] = (origin[1] - GetOrigin()[1]) / GetSpacing()[1];
			ijk[2] = (origin[2] - GetOrigin()[2]) / GetSpacing()[2];

			vtkMath::Subtract(p2, p1, normal);
			vtkMath::Normalize(normal);

			this->m_normalList.append(normal);
			this->m_originList.append(origin);
			this->m_coordList.append(ijk);
		}

		// Initialize seed view
		this->EnableObliqueView(true);

	}
}

void QInteractorStyleObliqueViewSeedsPlacer::CopySegmentationSeeds()
{
	this->ClearAllSeeds();

	for each (int* seed in m_seeds[SegmentationSeeds])
	{
		int *copy = new int[3];
		memcpy(copy, seed, sizeof(int) * 3);
		m_seeds[ObliqueViewSeeds].push_back(copy);
	}
	this->GenerateWidgetFromSeeds();
	this->UpdateSeedTable();
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
	while (this->m_originList.size())
	{
		double *element = this->m_originList.last();
		this->m_originList.pop_back();
		delete element;
	}

	while (this->m_normalList.size())
	{
		double *element = this->m_normalList.last();
		this->m_normalList.pop_back();
		delete element;
	}

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
	if (this->m_inObliqueView && this->GetSliceOrientation() == 2)
	{
		SetObliqueSlice(this->m_currentObliqueSlice + 1);
	}
	else {
		Superclass::MoveSliceForward();
	}
}

void QInteractorStyleObliqueViewSeedsPlacer::MoveSliceBackward()
{
	if (this->m_inObliqueView && this->GetSliceOrientation() == 2)
	{
		SetObliqueSlice(this->m_currentObliqueSlice - 1);
		return;
	}
	else {
		Superclass::MoveSliceBackward();
	}
}

void QInteractorStyleObliqueViewSeedsPlacer::SetObliqueSlice(int sliceIndex)
{
	if (sliceIndex < 0)
	{
		sliceIndex = 0;
		return;
	}
	else if (sliceIndex > this->m_normalList.size() - 1)
	{
		sliceIndex = this->m_normalList.size() - 1;
		return;
	}
	else {
		this->m_currentObliqueSlice = sliceIndex;
		this->m_resliceMapper[0]->GetSlicePlane()->SetNormal(this->m_normalList[this->m_currentObliqueSlice]);
		this->m_resliceMapper[0]->GetSlicePlane()->SetOrigin(this->m_originList[this->m_currentObliqueSlice]);
		this->m_reslicer[0]->GetProperty()->SetColorLevel(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorLevel()*10);
		this->m_reslicer[0]->GetProperty()->SetColorWindow(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorWindow() * 12);
		this->m_reslicer[0]->Update();

		if (this->m_reslicer[1])
		{
			this->m_resliceMapper[1]->GetSlicePlane()->SetNormal(this->m_normalList[this->m_currentObliqueSlice]);
			this->m_resliceMapper[1]->GetSlicePlane()->SetOrigin(this->m_originList[this->m_currentObliqueSlice]);
			this->m_reslicer[1]->Update();
		}


		/* Change camera to view the slice*/
		vtkCamera* cam = this->CurrentRenderer->GetActiveCamera();
		double camPos[3], refVect[3];
		memcpy(refVect, this->m_normalList[this->m_currentObliqueSlice], sizeof(double) * 3);
		vtkMath::MultiplyScalar(refVect, cam->GetDistance());
		vtkMath::Add(this->m_originList[this->m_currentObliqueSlice], refVect, camPos);
		cam->SetFocalPoint(this->m_originList[this->m_currentObliqueSlice]);
		cam->SetViewUp(0, 0, 1);
		cam->SetPosition(camPos);


		int *curIndex = this->m_coordList[this->m_currentObliqueSlice];
		QList<MyImageViewer*> viewers = QList<MyImageViewer*>::fromStdList(this->m_synchronalViewers);
		viewers[0]->SetFocalPointWithImageCoordinate(curIndex[0], curIndex[1], curIndex[2]);
		viewers[1]->SetFocalPointWithImageCoordinate(curIndex[0], curIndex[1], curIndex[2]);
		MY_VIEWER_CONSTITERATOR(Render());
	}
}

QList<vtkSmartPointer<vtkImageData>> QInteractorStyleObliqueViewSeedsPlacer::m_listOfVtkImages;
QList<QString> QInteractorStyleObliqueViewSeedsPlacer::m_listOfModalityNames;
QList<double*> QInteractorStyleObliqueViewSeedsPlacer::m_normalList;
QList<double*> QInteractorStyleObliqueViewSeedsPlacer::m_originList;
QList<int*> QInteractorStyleObliqueViewSeedsPlacer::m_coordList;

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
//#include "vtkXMLPolyDataWriter.h"
#include "vtkCamera.h"
#include "vtkImageSlice.h"
#include "vtkImageProperty.h"


vtkStandardNewMacro(QInteractorStyleObliqueViewSeedsPlacer);
QSETUP_UI_SRC(QInteractorStyleObliqueViewSeedsPlacer);
using namespace std;

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

void QInteractorStyleObliqueViewSeedsPlacer::SetTargetImages(
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

void QInteractorStyleObliqueViewSeedsPlacer::EnableObliqueView(bool axialView)
{
	if (this->GetSliceOrientation() == 2)
	{
		// Do only for Axial view
		if (axialView && !m_inReconstructedView)
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
			if (this->m_reslicer != NULL)
			{
				this->m_reslicer->RemoveAllObservers();
				this->m_reslicer->Delete();
			}
			if (this->m_resliceMapper != NULL)
				this->m_resliceMapper->Delete();

			this->m_resliceMapper = vtkSmartPointer<vtkImageResliceMapper>::New();
			this->m_reslicer = vtkSmartPointer<vtkImageSlice>::New();
			int index = m_listOfModalityNames.indexOf(ui->comboBoxTargeImage->currentText());
			vtkImageData* im = m_listOfVtkImages[index];
			im->Print(cout);
			this->m_resliceMapper->SetInputData(im);
			this->m_resliceMapper->SetSlicePlane(vtkSmartPointer<vtkPlane>::New());
			this->m_reslicer->SetMapper(this->m_resliceMapper);
			this->m_reslicer->GetProperty()->SetColorLevel(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorLevel());
			this->m_reslicer->GetProperty()->SetColorWindow(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorWindow());
			this->m_reslicer->Update();
			

			// set cut plane to first point
			vtkPlane* plane = this->m_resliceMapper->GetSlicePlane();
			this->m_currentObliqueSlice = 0;
			plane->SetNormal(this->m_normalList[0]);
			plane->SetOrigin(this->m_originList[0]);
			this->m_reslicer->Update();

			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddViewProp(this->m_reslicer);
			this->Interactor->Render();

			this->m_inReconstructedView = true;

			// Disable UI and seed widget
			this->ClearAllSeedWidget();
			this->SetSeedsPlacerEnable(false);
			this->ui->listWidgetSeedList->setDisabled(true);
			this->ui->pushBtnDeleteSeed->setDisabled(true);
			this->ui->dropSeedPushButton->setDisabled(true);
			this->ui->deleteAllSeedsPushButton->setDisabled(true);
		}
		else {
			// Remove & delete reslice actor
			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveViewProp(this->m_reslicer);
			this->m_reslicer->Delete();
			this->m_resliceMapper->Delete();

			// Re-enable all actors
			while (this->m_savedActors.size())
			{
				vtkProp* lastElement = this->m_savedActors.last();
				lastElement->SetVisibility(true);
				this->m_savedActors.pop_back();
			}

			this->m_inReconstructedView = false;

			// Enable UI and seedwidget
			this->SetSeedsPlacerEnable(true);
			this->ui->listWidgetSeedList->setDisabled(false);
			this->ui->pushBtnDeleteSeed->setDisabled(false);
			this->ui->dropSeedPushButton->setDisabled(false);
			this->ui->deleteAllSeedsPushButton->setDisabled(false);
		}
	}
}

void QInteractorStyleObliqueViewSeedsPlacer::UpdateWidgetToSeeds(int * oldImagePos, int* newImagePos)
{
	InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(oldImagePos, newImagePos);
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

void QInteractorStyleObliqueViewSeedsPlacer::SlotClearAllSeeds()
{
	ClearAllSeeds();
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, ClearAllSeedWidget());
	ui->listWidgetSeedList->clear();
	m_imageViewer->Render();
}

void QInteractorStyleObliqueViewSeedsPlacer::slotPushBtnAxialReconstruction()
{
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleObliqueViewSeedsPlacer, InitializeObliqueView())
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

	if (m_inReconstructedView)
	{
		/// Stop reconstruction view
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
		double totalLength;
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
			

			vtkMath::Subtract(p2, p1, normal);
			vtkMath::Normalize(normal);

			this->m_normalList.append(normal);
			this->m_originList.append(origin);
		}

		// Initialize seed view
		this->EnableObliqueView(true);

	}
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

void QInteractorStyleObliqueViewSeedsPlacer::DropSeed()
{
	InteractorStyleSeedsPlacer::DropSeed();
}

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

	}
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(SlotClearAllSeeds()));
}

QInteractorStyleObliqueViewSeedsPlacer::~QInteractorStyleObliqueViewSeedsPlacer()
{
	QDELETE_UI();

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
	if (this->m_inReconstructedView && this->GetSliceOrientation() == 2)
	{
		if (this->m_currentObliqueSlice >= this->m_normalList.size() - 1)
		{
			return;
		}
		else {
			cout << "Current slice :" << this->m_currentObliqueSlice << endl;
			this->m_currentObliqueSlice += 1;
			this->m_resliceMapper->GetSlicePlane()->SetNormal(this->m_normalList[this->m_currentObliqueSlice]);
			this->m_resliceMapper->GetSlicePlane()->SetOrigin(this->m_originList[this->m_currentObliqueSlice]);
			this->m_reslicer->GetProperty()->SetColorLevel(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorLevel());
			this->m_reslicer->GetProperty()->SetColorWindow(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorWindow()*15);
			this->m_reslicer->Update();

			/* Change camera to view the slice*/
			vtkCamera* cam = this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
			double camPos[3], refVect[3];
			memcpy(refVect, this->m_normalList[this->m_currentObliqueSlice], sizeof(double) * 3);
			vtkMath::MultiplyScalar(refVect, cam->GetDistance());
			vtkMath::Add(this->m_originList[this->m_currentObliqueSlice], refVect, camPos);
			cam->SetFocalPoint(this->m_originList[this->m_currentObliqueSlice]);
			cam->SetPosition(camPos);

			MY_VIEWER_CONSTITERATOR(Render());
		}
	}
	else {
		Superclass::MoveSliceForward();
	}
}

void QInteractorStyleObliqueViewSeedsPlacer::MoveSliceBackward()
{
	if (this->m_inReconstructedView && this->GetSliceOrientation() == 2)
	{
		if (this->m_currentObliqueSlice == 0)
		{
			return;
		}
		else {
			this->m_currentObliqueSlice -= 1;
			this->m_resliceMapper->GetSlicePlane()->SetNormal(this->m_normalList[this->m_currentObliqueSlice]);
			this->m_resliceMapper->GetSlicePlane()->SetOrigin(this->m_originList[this->m_currentObliqueSlice]);
			this->m_reslicer->GetProperty()->SetColorLevel(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorLevel());
			this->m_reslicer->GetProperty()->SetColorWindow(this->m_imageViewer->GetImageActor()->GetProperty()->GetColorWindow()*15);
			this->m_reslicer->Update();

			/* Change camera to view the slice*/
			vtkCamera* cam = this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
			double camPos[3], refVect[3];
			memcpy(refVect, this->m_normalList[this->m_currentObliqueSlice], sizeof(double) * 3);
			vtkMath::MultiplyScalar(refVect, cam->GetDistance());
			vtkMath::Add(this->m_originList[this->m_currentObliqueSlice], refVect, camPos);
			cam->SetFocalPoint(this->m_originList[this->m_currentObliqueSlice]);
			cam->SetPosition(camPos);
			
			MY_VIEWER_CONSTITERATOR(Render());
		}
	}
	else {
		Superclass::MoveSliceBackward();
	}
}

QList<vtkSmartPointer<vtkImageData>> QInteractorStyleObliqueViewSeedsPlacer::m_listOfVtkImages;
QList<QString> QInteractorStyleObliqueViewSeedsPlacer::m_listOfModalityNames;
QList<double*> QInteractorStyleObliqueViewSeedsPlacer::m_normalList;
QList<double*> QInteractorStyleObliqueViewSeedsPlacer::m_originList;

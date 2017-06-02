#include "QInteractorStyleVBDSmokerSeeds.h"
#include "ui_QInteractorStyleVBDSmokerSeeds.h"
#include "ImageViewer.h"

#include <vtkObjectFactory.h>
#include <vtkHandleWidget.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>
#include <vtkMarchingSquares.h>
#include <vtkCenterOfMass.h>
#include <vtkImageData.h>
//#include <qpushbutton.h>

using namespace std;
vtkStandardNewMacro(QInteractorStyleVBDSmokerSeeds);
QSETUP_UI_SRC(QInteractorStyleVBDSmokerSeeds);


// using a initial value
vector<int*> QInteractorStyleVBDSmokerSeeds::m_VBDSmokerSeeds;

QPushButton* QInteractorStyleVBDSmokerSeeds::pushButtons[];

void QInteractorStyleVBDSmokerSeeds::SetCustomEnabled(bool flag)
{
	InteractorStyleSeedsPlacer::SetCustomEnabled(flag);
	uniqueInvoke(flag);

	if (flag) {
		m_seedWidget->CompleteInteraction();
	}
}

void QInteractorStyleVBDSmokerSeeds::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleVBDSmokerSeeds::GenerateWidgetFromSeeds()
{
	ClearAllSeedWidget();
	int i = 0;
	for (vector<int*>::const_iterator cit = m_VBDSmokerSeeds.cbegin();
		cit != m_VBDSmokerSeeds.cend(); ++cit, ++i) {
		int imagePos[3];
		vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
		if ((*cit)[0] >= 0 && pushButtons[i]->isChecked()) {
			// for not twinkle, make it always on top
			//const int* displayExtent = m_imageViewer->GetDisplayExtent();
			double worldPos[3]; 
			//worldPos[0] = displayExtent[1] * GetSpacing()[0] + GetOrigin()[0];
			//worldPos[1] = displayExtent[3] * GetSpacing()[1] + GetOrigin()[1];
			//worldPos[0] = displayExtent[1] * GetSpacing()[2] + GetOrigin()[2];
			GetImageViewer()->GetFocalPointWithWorldCoordinate(worldPos);
			GetImageViewer()->GetFocalPointWithImageCoordinate(imagePos);

			if (i < NUM_OF_ELONGATION) {
				if (GetSliceOrientation() == ImageViewer::SLICE_ORIENTATION_XY && 
					imagePos[ImageViewer::SLICE_ORIENTATION_XY] != 
					(*cit)[ImageViewer::SLICE_ORIENTATION_XY]) {
					continue;
				}
				worldPos[ImageViewer::SLICE_ORIENTATION_XY] =
					((*cit)[ImageViewer::SLICE_ORIENTATION_XY] *
						GetSpacing()[ImageViewer::SLICE_ORIENTATION_XY]) +
					GetOrigin()[ImageViewer::SLICE_ORIENTATION_XY];

				//vtkPointHandleRepresentation3D::SafeDownCast(newSeed->GetHandleRepresentation())->
				//	GetProperty()->SetColor(1,0,0); // red
			}
			else {
				if (GetSliceOrientation() != ImageViewer::SLICE_ORIENTATION_XY &&
					(imagePos[ImageViewer::SLICE_ORIENTATION_YZ] != 
					(*cit)[ImageViewer::SLICE_ORIENTATION_YZ] || 
						imagePos[ImageViewer::SLICE_ORIENTATION_XZ] != 
						(*cit)[ImageViewer::SLICE_ORIENTATION_XZ])) {
					continue;
				}
				worldPos[ImageViewer::SLICE_ORIENTATION_YZ] =
					((*cit)[ImageViewer::SLICE_ORIENTATION_YZ] *
						GetSpacing()[ImageViewer::SLICE_ORIENTATION_YZ]) +
					GetOrigin()[ImageViewer::SLICE_ORIENTATION_YZ];
				worldPos[ImageViewer::SLICE_ORIENTATION_XZ] = ((*cit)[ImageViewer::SLICE_ORIENTATION_XZ]
					* GetSpacing()[ImageViewer::SLICE_ORIENTATION_XZ])
					+ GetOrigin()[ImageViewer::SLICE_ORIENTATION_XZ];
				//vtkPointHandleRepresentation3D::SafeDownCast(newSeed->GetHandleRepresentation())->
				//	GetProperty()->SetColor(0, 1, 0); // blue
			}
			newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
			m_seedWidget->GetSeed(i)->SetEnabled(pushButtons[i]->isChecked());
		}
		else {
			m_seedWidget->GetSeed(i)->EnabledOff();
		}
	}
	m_seedWidget->Render();
	//for (vector<int*>::const_iterator cit = m_VBDSmokerDetour.cbegin();
	//	cit != m_VBDSmokerDetour.cend(); ++cit) {
	//	int* imagePos = (*cit);
	//	vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
	//	if (imagePos[0] >= 0 ) {
	//		double worldPos[3];
	//		m_imageViewer->GetFocalPointWithWorldCoordinate(worldPos);

	//		newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
	//		newSeed->EnabledOn();
	//	}
	//}
}

//void QInteractorStyleVBDSmokerSeeds::GenerateWidgetFromSeeds(const std::vector<int*>& seeds)
//{
//
//}

void QInteractorStyleVBDSmokerSeeds::SaveWidgetToSeeds()
{
	//vector<int*>::const_iterator cit1 = m_VBDSmokerSeeds.cbegin();
	//vector<int*>::const_iterator cit2 = m_VBDSmokerDetour.cbegin();

	for (int i = 0; i < m_seedWidget->GetSeedRepresentation()->GetNumberOfSeeds(); ++i/*, ++cit1*/) {
		if (m_seedWidget->GetSeed(i)->GetEnabled() && pushButtons[i]->isChecked()) {
			double worldPos[3];
			m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(i, worldPos);
			int imagePos[3];
			for (int pos = 0; pos < 3; ++pos) {
				imagePos[pos] = (worldPos[pos] - GetOrigin()[pos]) / GetSpacing()[pos] + 0.5;
			}
			memcpy(m_VBDSmokerSeeds[i], imagePos, sizeof(imagePos));
			//if (i < NUM_OF_ELONGATION) {

			//}
			//else {
			//	memcpy(*cit2, imagePos, sizeof(imagePos));
			//	++cit2;
			//}
		}
	}
	cout << __func__ << endl;
	cout << __FUNCTION__ << endl;
	cout << __LINE__ << endl;
	print();
}

void QInteractorStyleVBDSmokerSeeds::UpdateWidgetToSeeds(int * newImagePos, int * oldImagePos)
{
	SaveWidgetToSeeds();
	GenerateWidgetFromSeeds();
}

void QInteractorStyleVBDSmokerSeeds::UpdateWidgetToSeeds(std::vector<int*>& seeds, int * newImagePos, int * oldImagePos)
{
	// do nothing
}

void QInteractorStyleVBDSmokerSeeds::ClearAllSeeds()
{
	InteractorStyleSeedsPlacer::ClearAllSeeds(m_VBDSmokerSeeds);
}

void QInteractorStyleVBDSmokerSeeds::slotUpdateBasilarArteryBifurcationLocation()
{
	int i = 0;
	//vector<int*>::const_iterator cit = m_VBDSmokerSeeds.cbegin();
	//int class0 = (*cit++)[2];
	//int class1 = (*cit++)[2];
	//int class2 = (*cit++)[2];
	int class0 = m_VBDSmokerSeeds[0][2];
	int class1 = m_VBDSmokerSeeds[1][2];
	int class2 = m_VBDSmokerSeeds[2][2];
	int value = ui->spinBoxBasilarArteryBifurcationLocation->value();
	if (value <= class0) {
		ui->lineEditElongation->setText("class0");
		emit elongationChanged(0);
	}
	else if (class0 < value && value <= class1) {
		ui->lineEditElongation->setText("class1");
		emit elongationChanged(1);

	}
	else if (class1 < value && value <= class2) {
		ui->lineEditElongation->setText("class2");
		emit elongationChanged(2);

	}
	else {
		ui->lineEditElongation->setText("class3");
		emit elongationChanged(3);

	}

}

void QInteractorStyleVBDSmokerSeeds::slotUpdatePonsCentralSectionLocation()
{
	int range[6];
	memcpy(range, GetImageViewer()->GetDisplayExtent(), sizeof(range));
	range[4] = ui->spinBoxPonsCentralSectionLocation->value();
	range[5] = ui->spinBoxPonsCentralSectionLocation->value();
	vtkSmartPointer<vtkMarchingSquares> marchingSquare =
		vtkSmartPointer<vtkMarchingSquares>::New();
	marchingSquare->SetInputData(GetImageViewer()->GetOverlay());
	marchingSquare->GenerateValues(1, 1, 1);
	marchingSquare->SetImageRange(range);
	marchingSquare->Update();

	vtkSmartPointer<vtkCenterOfMass> centerOfMass =
		vtkSmartPointer<vtkCenterOfMass>::New();
	centerOfMass->SetInputConnection(marchingSquare->GetOutputPort());
	centerOfMass->SetUseScalarsAsWeights(false);
	centerOfMass->Update();

	const double* center = centerOfMass->GetCenter();
	int centerX = (center[0] - GetOrigin()[0]) / GetSpacing()[0] + 0.5;
	int centerY = (center[1] - GetOrigin()[1]) / GetSpacing()[1] + 0.5;
	int centerZ = (center[2] - GetOrigin()[2]) / GetSpacing()[2] + 0.5;

	//vector<int*>::const_iterator cit = m_VBDSmokerSeeds.cbegin();
	//int class0 = (*cit++)[2];
	//int class1 = (*cit++)[2];
	//int class2 = (*cit++)[2];
	//int class0M = (*cit++)[0];
	//int class1R = (*cit++)[0];
	//int class1L = (*cit++)[0];
	//int class2R = (*cit++)[0];
	//int class2L = (*cit++)[0];
	//int class3R = (*cit++)[0];
	//int class3L = (*cit++)[0];


	int class0M = m_VBDSmokerSeeds[3][0];
	int class1R = m_VBDSmokerSeeds[4][0];
	int class1L = m_VBDSmokerSeeds[5][0];
	int class2R = m_VBDSmokerSeeds[6][0];
	int class2L = m_VBDSmokerSeeds[7][0];
	int class3R = m_VBDSmokerSeeds[8][0];
	int class3L = m_VBDSmokerSeeds[9][0];

	if ((class1R < centerX && centerX <= class0M) ||
		(class0M < centerX && centerX <= class1L)) {
		ui->lineEditDetour->setText("class0");
		emit detourChanged(0);
	}
	else if((class2R < centerX && centerX <= class1R) ||
		(class1L < centerX && centerX <= class2L)) {
		ui->lineEditDetour->setText("class1");
		emit detourChanged(1);
	}
	else if ((class3R < centerX && centerX <= class2R) ||
		(class2L < centerX && centerX <= class3L)) {
		ui->lineEditDetour->setText("class2");
		emit detourChanged(2);
	}
	else {
		ui->lineEditDetour->setText("class3");
		emit detourChanged(3);

	}

}

void QInteractorStyleVBDSmokerSeeds::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
	int* extent = GetExtent();
	ui->spinBoxBasilarArteryBifurcationLocation->setRange(extent[4], extent[5]);
	ui->spinBoxPonsCentralSectionLocation->setRange(extent[4], extent[5]);



	//connect(ui->pushButtonBasilarArteryBifurcationLocation0, SIGNAL(toggled(bool)),
	//	this, SLOT(slotEnableTest(bool)));
}

void QInteractorStyleVBDSmokerSeeds::uniqueDisable()
{
}

void QInteractorStyleVBDSmokerSeeds::slotBasilarArteryBifurcationLocationCurrentSlice() 
{
	int ijk[3];
	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk);
	ui->spinBoxBasilarArteryBifurcationLocation->setValue(ijk[2]);
}

void QInteractorStyleVBDSmokerSeeds::slotPonsCentralSectionLocationCurrentSlice()
{
	int ijk[3];
	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk);
	ui->spinBoxPonsCentralSectionLocation->setValue(ijk[2]);
}


QInteractorStyleVBDSmokerSeeds::QInteractorStyleVBDSmokerSeeds(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleVBDSmokerSeeds::~QInteractorStyleVBDSmokerSeeds()
{
	QDELETE_UI();
	ClearAllSeeds();
}

void QInteractorStyleVBDSmokerSeeds::OnLeftButtonDown()
{
	InteractorStyleSeedsPlacer::OnLeftButtonDown();

	//vector<int*>::const_iterator cit1 = m_VBDSmokerSeeds.cbegin();
	for (int i = 0; i < NUM_OF_DETOUR + NUM_OF_ELONGATION; ++i/*, ++cit1*/) {
		if (!m_seedWidget->GetSeed(i)->GetEnabled() &&
			pushButtons[i]->isChecked() && 
			m_VBDSmokerSeeds[i][0] == -1) {

			this->GetInteractor()->GetPicker()->Pick(
				this->GetInteractor()->GetEventPosition()[0],
				this->GetInteractor()->GetEventPosition()[1],
				0,  // always zero.
				GetImageViewer()->GetRenderer());

			double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

			//Check if valid pick
			if (picked[0] == 0.0&&picked[1] == 0.0)
				break;
			int imagePos[3];
			GetImageViewer()->GetFocalPointWithImageCoordinate(imagePos);
			picked[GetSliceOrientation()] = imagePos[GetSliceOrientation()] * 
				GetSpacing()[GetSliceOrientation()] + GetOrigin()[GetSliceOrientation()];
			m_seedWidget->GetSeed(i)->SetEnabled(pushButtons[i]->isChecked());
			m_seedWidget->GetSeed(i)->GetHandleRepresentation()->SetWorldPosition(picked);
			
			SaveWidgetToSeeds();
			GenerateWidgetFromSeeds();

			break;

		}
	}

	cout << __func__ << endl;
	cout << __FUNCTION__ << endl;
	cout << __LINE__ << endl;
	print();

}

void QInteractorStyleVBDSmokerSeeds::OnKeyPress()
{
	string key = this->Interactor->GetKeySym();
	cout << __func__ << ' ' << key << endl;
	if (key == "D") {
		cout << __func__ << endl;
		cout << __FUNCTION__ << endl;
		cout << __LINE__ << endl;
		print();
	}
	else {
		InteractorStyleSeedsPlacer::OnKeyPress();
	}
}


void QInteractorStyleVBDSmokerSeeds::uniqueInitialization()
{
	QAbstractNavigation::uniqueInitialization();

	QPushButton* _pushButtons[] = {
		ui->pushButtonBasilarArteryBifurcationLocation0,
		ui->pushButtonBasilarArteryBifurcationLocation1,
		ui->pushButtonBasilarArteryBifurcationLocation2,
		ui->pushButtonPonsCentralSectionLocation0,
		ui->pushButtonPonsCentralSectionLocation1Right,
		ui->pushButtonPonsCentralSectionLocation1Left,
		ui->pushButtonPonsCentralSectionLocation2Right,
		ui->pushButtonPonsCentralSectionLocation2Left,
		ui->pushButtonPonsCentralSectionLocation3Right,
		ui->pushButtonPonsCentralSectionLocation3Left };
	memcpy(pushButtons, _pushButtons, sizeof(pushButtons));

	for (int i = 0; i < NUM_OF_ELONGATION + NUM_OF_DETOUR; ++i) {
		m_VBDSmokerSeeds.push_back(new int[3]{ -1,-1,-1 });
	}

	connect(ui->pushButtonBasilarArteryBifurcationLocation, SIGNAL(clicked()),
		this, SLOT(slotBasilarArteryBifurcationLocationCurrentSlice()));

	connect(ui->pushButtonPonsCentralSectionLocation, SIGNAL(clicked()),
		this, SLOT(slotPonsCentralSectionLocationCurrentSlice()));

	connect(ui->spinBoxBasilarArteryBifurcationLocation, SIGNAL(valueChanged(int)),
		this, SLOT(slotUpdateBasilarArteryBifurcationLocation()));
	connect(ui->spinBoxPonsCentralSectionLocation, SIGNAL(valueChanged(int)),
		this, SLOT(slotUpdatePonsCentralSectionLocation()));
}

void QInteractorStyleVBDSmokerSeeds::initialization()
{
	QAbstractNavigation::initialization();
	for (int i = 0; i < NUM_OF_DETOUR + NUM_OF_ELONGATION; ++i) {
		connect(pushButtons[i], SIGNAL(toggled(bool)), this, SLOT(GenerateWidgetFromSeeds()));
	}
}

void QInteractorStyleVBDSmokerSeeds::print()
{
	for (vector<int*>::const_iterator cit = m_VBDSmokerSeeds.cbegin();
		cit != m_VBDSmokerSeeds.cend(); ++cit) {
		int* imagePos = (*cit);
		cout << "Elongation" << endl;
		cout << imagePos[0] << ' ' << imagePos[1] << ' '
			<< imagePos[2] << endl;
	}

	//for (vector<int*>::const_iterator cit = m_VBDSmokerDetour.cbegin();
	//	cit != m_VBDSmokerDetour.cend(); ++cit) {
	//	int* imagePos = (*cit);
	//	cout << "Detour" << endl;
	//	cout << imagePos[0] << ' ' << imagePos[1] << ' '
	//		<< imagePos[2] << endl;
	//}

}

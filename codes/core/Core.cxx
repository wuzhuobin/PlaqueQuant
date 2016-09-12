#include "Core.h"

#include <vtkExtractVOI.h>

#include <QMessageBox>

Core::Core(QWidget* parent)
	:m_imageManager(this), m_ioManager(this), QWidget(parent)
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_2DimageViewer[i] = vtkSmartPointer<MyImageViewer>::New();
		m_interactor[i] = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		m_style[i] = vtkSmartPointer<InteractorStyleSwitch>::New();
	}

	/// Set up 3D renderer
	this->m_style3D = InteractorStyleSwitch3D::New();
	this->m_3Dinteractor = vtkRenderWindowInteractor::New();
	this->m_3DimageViewer = vtkRenderWindow::New();
	this->m_3DimageViewer->SetNumberOfLayers(2);
	this->m_3DimageViewer->SetInteractor(this->m_3Dinteractor);
	this->m_3Dinteractor->SetInteractorStyle(this->m_style3D);
	this->m_3DAnnotationRenderer = vtkRenderer::New();
	this->m_3DAnnotationRenderer->SetLayer(1);
	this->m_3DDataRenderer = vtkRenderer::New();
	this->m_3DDataRenderer->SetLayer(0);
	this->m_3DimageViewer->AddRenderer(this->m_3DDataRenderer);
	this->m_3Dinteractor->Initialize();


	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_lookupTable->SetNumberOfTableValues(7);
	m_lookupTable->SetTableRange(0.0, 6);
	m_lookupTable->SetTableValue(0, 0, 0, 0, 0.0);
	m_lookupTable->SetTableValue(1, 1, 0, 0, 0.2);
	m_lookupTable->SetTableValue(2, 0, 0, 1, 0.05);
	m_lookupTable->SetTableValue(3, 0, 1, 0, 0.4);
	m_lookupTable->SetTableValue(4, 1, 1, 0, 0.5);
	m_lookupTable->SetTableValue(5, 0, 1, 1, 0.5);
	m_lookupTable->SetTableValue(6, 1, 0, 1, 0.5);
	m_lookupTable->Build();

	m_ioManager.setMyImageManager(&m_imageManager);

	connect(&m_ioManager, SIGNAL(finishOpenMultiImages()), this, SLOT(slotVisualizeViewer()));
}

Core::~Core() 
{

}

vtkLookupTable * Core::GetLookupTable()
{
	return m_lookupTable;
}

vtkRenderWindow * Core::GetRenderWindow(int num)
{
	if (num < 0 || num > 3) {
		return NULL;
	}
	else if (num == 3) {
		return m_3DimageViewer;
	}
	else {
		return m_2DimageViewer[num]->GetRenderWindow();
	}
}

void Core::DisplayErrorMessage(std::string str)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setText(QString::fromStdString(str));
	//msgBox.setStyleSheet("QMessageBox{background-color:rgb(22,22,22);}\n QLabel {color:rgb(255,255,255);}\n QPushButton {background-color: rgb(22,22,22); color:white; border-color:rgb(63,63,70); border-style: outset; border-width: 1px; min-width: 8em; padding: 3px;} QPushButton::hover{background-color: rgb(0,102,204);}");

	msgBox.addButton(QMessageBox::Ok);
	msgBox.exec();
}

IOManager * Core::GetIOManager()
{
	return &m_ioManager;
}

MyImageManager * Core::GetMyImageManager()
{
	return &m_imageManager;
}

void Core::slotAddOverlayToImageViewer() {
	int *extent1 = this->m_imageManager.getOverlay().GetOutput()->GetExtent();
	int *extent2 = this->m_imageManager.getListOfViewerInputImages()[0]->GetExtent();
	if (!std::equal(extent1, extent1 + 6, extent2)) {
		this->DisplayErrorMessage("Selected segmentation has wrong spacing!");
		return;
	}
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		// The tableRange of LookupTable Change when the vtkImageViewer2::Render was call
		// Very strange
		//this->LookupTable->SetTableRange(0, 6);
		m_2DimageViewer[i]->SetLookupTable(m_lookupTable);
		m_2DimageViewer[i]->SetInputDataLayer(
			this->m_imageManager.getOverlay().GetOutput());
		m_2DimageViewer[i]->GetdrawActor()->SetVisibility(true);
	}
	this->m_imageManager.getOverlay().GetOutput()->Modified();
}

void Core::slotVisualizeViewer()
{

	slotChangeView(m_viewMode);
	for (int i = 0; i < VIEWER_NUM; ++i) {
		m_style[i]->AddSynchronalViewer(m_2DimageViewer[0]);
		m_style[i]->AddSynchronalViewer(m_2DimageViewer[1]);
		m_style[i]->AddSynchronalViewer(m_2DimageViewer[2]);
	}
	slotAddOverlayToImageViewer();
	//Update UI stuff
	//Assume the four images have equal number of slices
	//QSpinBox* spinBoxes[3] = { ui->xSpinBox, ui->ySpinBox, ui->zSpinBox };
	//const int* extent = m_imageManager.getListOfViewerInputImages()[0]->GetExtent();
	//for (int i = 0; i < 3; ++i) {
	//	spinBoxes[i]->setMinimum(extent[i * 2]);
	//	spinBoxes[i]->setMaximum(extent[i * 2 + 1]);
	//	spinBoxes[i]->setValue((extent[i * 2 + 1] + extent[i * 2])*0.5);
	//}
	//ui->windowDoubleSpinBoxUL->setValue(m_2DimageViewer[0]->GetColorWindow());
	//ui->levelDoubleSpinBoxUL->setValue(m_2DimageViewer[0]->GetColorLevel());

	//connect(ui->windowDoubleSpinBoxUL, SIGNAL(valueChanged(double)), this, SLOT(slotChangeWindowLevel()), Qt::QueuedConnection);
	//connect(ui->levelDoubleSpinBoxUL, SIGNAL(valueChanged(double)), this, SLOT(slotChangeWindowLevel()), Qt::QueuedConnection);

	//connect(this->ui->xSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotChangeSlice()), Qt::QueuedConnection);
	//connect(this->ui->ySpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotChangeSlice()), Qt::QueuedConnection);
	//connect(this->ui->zSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotChangeSlice()), Qt::QueuedConnection);
	emit signalVisualizeViewer();
}

void Core::slotChangeView(Core::VIEW_MODE viewMode)
{
	if (m_viewMode == viewMode) {
		return;
	}
	m_viewMode = viewMode;
	// SEGMENTATION_VIEW
	if (viewMode) {
		// i1 for looping all 5 vtkImage, while i2 for looping all 3 m_2DimageViewer
		for (int i1 = 0, i2 = 0; i2 < VIEWER_NUM; ++i2)
		{
			for (; i1 < this->m_imageManager.getListOfViewerInputImages().size() && i2 < VIEWER_NUM;
				++i1) {
				if (this->m_imageManager.getListOfViewerInputImages()[i1] != NULL) {
					this->m_2DimageViewer[i2]->SetInputData(
						this->m_imageManager.getListOfViewerInputImages()[i1]);
					this->m_2DimageViewer[i2]->SetSliceOrientation(2);
					m_2DimageViewer[i2]->SetupInteractor(m_interactor[i2]);
					m_style[i2]->SetImageViewer(m_2DimageViewer[i2]);
					for (int j = 0; j < VIEWER_NUM; ++j) {
						m_style[i2]->AddSynchronalViewer(m_2DimageViewer[j]);
					}
					m_interactor[i2]->SetInteractorStyle(m_style[i2]);

					//this->m_2DimageViewer[i2]->GetRenderer()->GetActiveCamera()->SetViewUp(0, -1, 0);
					//this->m_style[i2]->SetOrientation(2);
					++i2;
				}
			}
			if (i1 >= 5 && i2 <3) {
				// disable view props
				vtkPropCollection* props = this->m_2DimageViewer[i2]->GetRenderer()->GetViewProps();
				props->InitTraversal();
				for (int j = 0; j < props->GetNumberOfItems(); j++)
				{
					props->GetNextProp()->SetVisibility(false);
				}

				vtkPropCollection* annProps = m_2DimageViewer[i2]->GetAnnotationRenderer()->GetViewProps();
				annProps->InitTraversal();
				for (int j = 0; j < annProps->GetNumberOfItems(); j++)
				{
					annProps->GetNextProp()->SetVisibility(false);
				}
				this->m_2DimageViewer[i2]->GetRenderWindow()->Modified();
				this->m_2DimageViewer[i2]->GetRenderWindow()->Render();
				this->m_2DimageViewer[i2]->GetRenderWindow()->GetInteractor()->Disable();

			}

		}
		emit signalSegmentationView();
		//this->slotChangeSlice();

		//QAction* action = widgetGroup.checkedAction();
		//if (action != NULL) {
		//	action->trigger();
		//}
	}
	// MULTIPLANAR_VIEW
	else {
		for (int i = 0; i < VIEWER_NUM; ++i) {
			// Change input to same image, default 0
			m_2DimageViewer[i]->SetInputData(
				this->m_imageManager.getListOfViewerInputImages()[DEFAULT_IMAGE]);
			m_2DimageViewer[i]->SetSliceOrientation(i);
			//m_2DimageViewer[i]->Render();
			// initialize stuff
			m_2DimageViewer[i]->SetupInteractor(m_interactor[i]);
			m_2DimageViewer[i]->InitializeHeader(m_imageManager.getModalityName(DEFAULT_IMAGE));

			// setup interactorStyle
			m_style[i]->SetImageViewer(m_2DimageViewer[i]);
			for (int j = 0; j < VIEWER_NUM; ++j) {
				m_style[i]->AddSynchronalViewer(m_2DimageViewer[j]);
			}
			//m_style[i]->initializeQWidget(ui->xSpinBox, ui->ySpinBox, ui->zSpinBox,
			//	ui->windowDoubleSpinBoxUL, ui->levelDoubleSpinBoxUL,
			//	m_moduleWidget->GetBrushSizeSpinBox(),
			//	m_moduleWidget->GetBrushShapeComBox(),
			//	NULL, NULL);
			//m_style[i]->SetOrientation(i);

			m_interactor[i]->SetInteractorStyle(m_style[i]);


			//if (m_firstInitializedFlag) {
				// else only change input and viewer m_orientation
				this->m_2DimageViewer[i]->GetRenderWindow()->GetInteractor()->Enable();
				// Show view props for overlay
				vtkPropCollection* props = this->m_2DimageViewer[i]->GetRenderer()->GetViewProps();
				for (int j = 0; j < props->GetNumberOfItems(); j++)
				{
					//reinterpret_cast<vtkProp*>(props->GetItemAsObject(j))->SetVisibility(true);
					props->GetNextProp()->SetVisibility(true);
				}
				vtkPropCollection* annProps = m_2DimageViewer[i]->GetAnnotationRenderer()->GetViewProps();
				for (int j = 0; j < annProps->GetNumberOfItems(); j++)
				{
					//reinterpret_cast<vtkProp*>(annProps->GetItemAsObject(j))->SetVisibility(true);
					props->GetNextProp()->SetVisibility(true);
				}
			//}
		}
		m_firstInitializedFlag = true;
		//this->slotChangeSlice();
		emit signalMultiPlanarView();
		//QAction* action = widgetGroup.checkedAction();
		//if (action != NULL) {
		//	action->trigger();
		//}
	}
}

void Core::slotNavigationMode()
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_style[i]->SetInteractorStyleToNavigation();
	}

	this->slotRuler(false);
}

void Core::slotWindowLevelMode()
{

	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_style[i]->SetInteractorStyleToWindowLevel();
	}
	this->slotRuler(false);
}

void Core::slotBrushMode()
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_style[i]->SetInteractorStyleToPaintBrush();
	}
	//Update ui
	//m_moduleWidget->SetPage(2);

	this->slotRuler(false);
}

void Core::slotContourMode()
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_style[i]->SetInteractorStyleToPolygonDraw();
		//m_style[i]->SetMode(4);
	}
	//m_moduleWidget->SetPage(1);

	this->slotRuler(false);
}
void Core::slotRuler(bool b)
{
	for (int i = 0; i< VIEWER_NUM; i++)
	{
		m_2DimageViewer[i]->SetRulerEnabled(b);
	}

}

void Core::slotROIMode()
{
	if (SEGMENTATION_VIEW) {
		//connected to slotMultiPlanarView()
		emit signalMultiPlanarView();
		//ui->actionMultiPlanarView->trigger();
	}
	//m_moduleWidget->SetPage(0);
	for (int i = 0; i < VIEWER_NUM; ++i) {
		m_style[i]->SetInteractorStyleToROI();
	}

	this->slotRuler(false);
}

void Core::slotChangeROI()
{
	int extent[6];
	m_style[0]->GetROI()->SelectROI(extent);
	//m_moduleWidget->slotChangeROI(extent);
}

void Core::slotSelectROI()
{

	int newExtent[6];
	m_style[0]->GetROI()->SelectROI(newExtent);
	//m_style[0]->GetROI()->SelectROI(this->m_boundingExtent);
	// Extract VOI of the overlay Image data

	// Extract VOI of the vtkImage data
	for (int i = 0; i < this->m_imageManager.getListOfViewerInputImages().size(); ++i) {
		if (m_imageManager.getListOfViewerInputImages()[i] != NULL) {
			vtkSmartPointer<vtkExtractVOI> extractVOIFilter =
				vtkSmartPointer<vtkExtractVOI>::New();
			extractVOIFilter->SetInputData(m_imageManager.getListOfViewerInputImages()[i]);
			extractVOIFilter->SetVOI(newExtent);
			extractVOIFilter->Update();
			this->m_imageManager.getListOfViewerInputImages()[i]->DeepCopy(
				extractVOIFilter->GetOutput());
		}
	}

	//this->m_imageManager.getOverlay().SetDisplayExtent(newExtent);

	for (int i = 0; i < VIEWER_NUM; i++)
	{
		this->m_2DimageViewer[i]->SetBound(m_style[i]->GetROI()->GetPlaneWidget()->GetCurrentBound());
		this->m_2DimageViewer[i]->GetRenderer()->ResetCamera();
	}

	//ui->actionMultiPlanarView->trigger();
	//ui->actionNavigation->trigger();

	//this->slotChangeSlice(0, 0, 0);
}
void Core::slotResetROI()
{
	for (int i = 0; i < m_imageManager.getListOfViewerInputImages().size(); ++i)
	{
		if (m_imageManager.getListOfViewerInputImages()[i] != NULL) {
			m_imageManager.getListOfViewerInputImages()[i]->DeepCopy(
				m_imageManager.getListOfVtkImages()[i]);
		}
	}

	//this->imageManager.getOverlay().DisplayExtentOff();

	emit signalMultiPlanarView();
	//ui->actionMultiPlanarView->trigger();
	//ui->actionNavigation->trigger();

	//this->slotChangeSlice(0, 0, 0);
}
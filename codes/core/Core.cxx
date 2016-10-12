#include "Core.h"
#include "SurfaceCreator.h"
#include "Centerline.h"
#include <GPUVolumeRenderingFilter.h>
#include <vtkClipPolyData.h>
#include <vtkExtractVOI.h>
#include <vtkRendererCollection.h>
#include <vtkImageThreshold.h>
#include <QMessageBox>
#include <QAction>

Core::Core(QWidget* parent)
	:m_imageManager(new  MyImageManager(this)), m_ioManager(new IOManager(this)), QWidget(parent)
{
	// enable registration
	m_ioManager->enableRegistration(true);
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_2DimageViewer[i] = vtkSmartPointer<MyImageViewer>::New();
		m_interactor[i] = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		m_style[i] = vtkSmartPointer<InteractorStyleSwitch>::New();
	}
	this->m_3DDataRenderer = vtkRenderer::New();
	this->m_3DAnnotationRenderer = vtkRenderer::New();
	this->m_style3D = InteractorStyleSwitch3D::New();
	this->m_3Dinteractor = vtkRenderWindowInteractor::New();
	this->m_3DimageViewer = vtkRenderWindow::New();

	this->m_centerlinePD = NULL;
}

void Core::Initialization()
{
	/// Set up 3D renderer
	this->m_3DimageViewer->SetNumberOfLayers(2);
	this->m_3DimageViewer->SetInteractor(this->m_3Dinteractor);
	this->m_3Dinteractor->SetInteractorStyle(this->m_style3D);
	this->m_3DAnnotationRenderer->SetLayer(1);
	this->m_3DDataRenderer->SetLayer(0);
	this->m_3DimageViewer->AddRenderer(this->m_3DDataRenderer);
	this->m_3Dinteractor->Initialize();



	m_ioManager->setMyImageManager(m_imageManager);

	connect(m_ioManager, SIGNAL(finishOpenMultiImages()),
		this, SLOT(slotVisualizeAll2DViewers()));
	connect(m_ioManager, SIGNAL(finishOpenSegmentation()),
		this, SLOT(slotAddOverlayToImageViewer()));

	connect(m_2DimageViewer[DEFAULT_IMAGE], SIGNAL(FocalPointWithImageCoordinateChanged(int, int, int)),
		this, SLOT(slotChangeFocalPointWithImageCoordinate(int, int, int)));
}

vtkSmartPointer<vtkPolyData> Core::GetCenterlinePD()
{
	return this->m_centerlinePD;
}

Core::~Core() 
{

}

vtkLookupTable * Core::GetLookupTable()
{
	return m_imageManager->getOverlay()->GetLookupTable();
}

vtkRenderWindow * Core::GetRenderWindow(int num)
{
	if (num < 0 || num > VIEWER_NUM) {
		return NULL;
	}
	else if (num == VIEWER_NUM) {
		return m_3DimageViewer;
	}
	else {
		return m_2DimageViewer[num]->GetRenderWindow();
	}
}

void Core::RenderAllViewer()
{
	for (int i = 0; i < VIEWER_NUM; ++i) {
		if(m_2DimageViewer[i]->GetInput() != NULL)
			m_2DimageViewer[i]->Render();
	}
	m_3DimageViewer->Render();
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
	return m_ioManager;
}

MyImageManager * Core::GetMyImageManager()
{
	return m_imageManager;
}

void Core::slotAddOverlayToImageViewer() {
	int *extent1 = this->m_imageManager->getOverlay()->GetOutput()->GetExtent();
	int *extent2 = this->m_imageManager->getListOfViewerInputImages()[0]->GetExtent();
	if (!std::equal(extent1, extent1 + 6, extent2)) {
		this->DisplayErrorMessage("Selected segmentation has wrong spacing!");
		return;
	}
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		// The tableRange of LookupTable Change when the vtkImageViewer2::Render was call
		// Very strange
		//this->LookupTable->SetTableRange(0, 6);
		m_2DimageViewer[i]->SetOverlay(m_imageManager->getOverlay());
		m_2DimageViewer[i]->GetOverlayActor()->SetVisibility(true);
	}
	this->m_imageManager->getOverlay()->GetOutput()->Modified();
}

void Core::slotVisualizeAll2DViewers()
{
	slotChangeView(MULTIPLANAR_VIEW);
	if (m_firstInitialize) {
		for (int i = 0; i < VIEWER_NUM; ++i) {
			m_2DimageViewer[i]->SetupInteractor(m_interactor[i]);
			m_style[i]->SetImageViewer(m_2DimageViewer[i]);
			for (int j = 0; j < VIEWER_NUM; ++j) {
				m_style[i]->AddSynchronalViewer(m_2DimageViewer[j]);
			}
			m_interactor[i]->SetInteractorStyle(m_style[i]);
		}
		slotAddOverlayToImageViewer();
		m_firstInitialize = false;
		emit signalMultiPlanarView();
	}

	// reset slice to all viewer
	const int* extent = m_2DimageViewer[DEFAULT_IMAGE]->GetInput()->GetExtent();
	slotChangeFocalPointWithImageCoordinate(
		(extent[1] - extent[0]) / 2,
		(extent[3] - extent[2]) / 2,
		(extent[5] - extent[4]) / 2);

	emit signalVisualizeAllViewers();
}

void Core::slotChangeModality(QAction * action)
{
	if (action == NULL)
		return;
	slotChangeModality(action->text(), action->data().toInt());
}

void Core::slotChangeModality(QString modalityName, int viewerNum)
{
	if (modalityName.isEmpty())
		return;
	for (int i = 0; i < m_imageManager->getListOfModalityNames().size(); ++i) {
		if (modalityName == m_imageManager->getListOfModalityNames()[i]) {
			if (m_viewMode == MULTIPLANAR_VIEW) {
				for (int j = 0; j < VIEWER_NUM; ++j) {
					m_2DimageViewer[j]->SetInputData(
						this->m_imageManager->getListOfViewerInputImages()[i]);
					m_2DimageViewer[j]->Render();
					m_2DimageViewer[j]->InitializeHeader(m_imageManager->getModalityName(i));
				}
				break;
			}
			else {
				m_2DimageViewer[viewerNum]->SetInputData(
					this->m_imageManager->getListOfViewerInputImages()[i]);
				m_2DimageViewer[viewerNum]->Render();
				m_2DimageViewer[viewerNum]->InitializeHeader(m_imageManager->getModalityName(i));
				break;
			}
		}

	}
	RenderAllViewer();
}

void Core::slotSegmentationView() {
	if (m_viewMode == SEGMENTATION_VIEW) {
		return;
	}
	slotChangeView(SEGMENTATION_VIEW);
	emit signalSegmentationView();
}

void Core::slotMultiPlanarView() {
	if (m_viewMode == MULTIPLANAR_VIEW) {
		return;
	}
	slotChangeView(MULTIPLANAR_VIEW);
	emit signalMultiPlanarView();

}

void Core::slotChangeSlice(int slice) {
	MyImageViewer* viewer = dynamic_cast<MyImageViewer*>(sender());
	if (viewer != NULL) {
		switch (viewer->GetSliceOrientation())
		{
		case MyImageViewer::SLICE_ORIENTATION_YZ:
			emit signalChangeSliceX(slice);
			break;
		case MyImageViewer::SLICE_ORIENTATION_XZ:
			emit signalChangeSliceY(slice);
			break;
		case MyImageViewer::SLICE_ORIENTATION_XY:
			emit signalChangeSliceZ(slice);
		default:
			break;
		}
	}

}

void Core::slotChangeView(int viewMode)
{

	m_viewMode = viewMode;
	// SEGMENTATION_VIEW
	if (viewMode) {
		// i1 for looping all 5 vtkImage, while i2 for looping all 3 m_2DimageViewer
		for (int i1 = 0, i2 = 0; i2 < VIEWER_NUM; ++i2)
		{
			for (; i1 < this->m_imageManager->getListOfViewerInputImages().size() && i2 < VIEWER_NUM;
				++i1) {
				// skip the NULL image
				if (this->m_imageManager->getListOfViewerInputImages()[i1] != NULL) {
					// SetupInteractor should be ahead of InitializeHeader
					this->m_2DimageViewer[i2]->SetInputData(
						this->m_imageManager->getListOfViewerInputImages()[i1]);
					this->m_2DimageViewer[i2]->SetSliceOrientation(MyImageViewer::SLICE_ORIENTATION_XY);
					this->m_2DimageViewer[i2]->Render();
					this->m_2DimageViewer[i2]->InitializeHeader(m_imageManager->getModalityName(i2));

					++i2;
				}
			}
			if (i1 >= m_imageManager->getListOfVtkImages().size() && i2 < VIEWER_NUM ) {
				this->m_2DimageViewer[i2]->GetRenderWindow()->GetInteractor()->Disable();
				// disable view props
				m_2DimageViewer[i2]->SetVisibility(false);

			}

		}

	}
	// MULTIPLANAR_VIEW
	else {
		for (int i = 0; i < VIEWER_NUM; ++i) {
			// Change input to same image, default 0
			// SetupInteractor should be ahead of InitializeHeader
			m_2DimageViewer[i]->SetInputData(
				this->m_imageManager->getListOfViewerInputImages()[DEFAULT_IMAGE]);
			m_2DimageViewer[i]->SetSliceOrientation(i);
			m_2DimageViewer[i]->Render();
			m_2DimageViewer[i]->InitializeHeader(m_imageManager->getModalityName(DEFAULT_IMAGE));

			// else only change input and viewer m_orientation
			this->m_2DimageViewer[i]->GetRenderWindow()->GetInteractor()->Enable();
			// Show view props for overlay
			m_2DimageViewer[i]->SetVisibility(true);

		}

	}
	// reset slice to all viewer
	int ijk[3];
	m_2DimageViewer[DEFAULT_IMAGE]->GetFocalPointWithImageCoordinate(ijk);
	slotChangeFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);

	RenderAllViewer();
}

void Core::slotChangeSliceX(int x) {

	int ijk[3];
	m_2DimageViewer[DEFAULT_IMAGE]->GetFocalPointWithImageCoordinate(ijk);
	if (x == ijk[0]) {
		return;
	}
	ijk[0] = x;
	m_style[DEFAULT_IMAGE]->SetCurrentFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);

	emit signalChangeSliceX(x);

}

void Core::slotChangeSliceY(int y) {


	int ijk[3];
	m_2DimageViewer[DEFAULT_IMAGE]->GetFocalPointWithImageCoordinate(ijk);
	if (y == ijk[1]) {
		return;
	}
	ijk[1] = y;
	m_style[DEFAULT_IMAGE]->SetCurrentFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);


	emit signalChangeSliceY(y);

}

void Core::slotChangeSliceZ(int z) {


	int ijk[3];
	m_2DimageViewer[DEFAULT_IMAGE]->GetFocalPointWithImageCoordinate(ijk);
	if (z == ijk[2]) {
		return;
	}
	ijk[2] = z;
	m_style[DEFAULT_IMAGE]->SetCurrentFocalPointWithImageCoordinate(ijk[0], ijk[1], ijk[2]);


	emit signalChangeSliceZ(z);
}

void Core::slotChangeFocalPointWithImageCoordinate(int i, int j, int k)
{
	emit signalChangeSliceX(i);
	emit signalChangeSliceY(j);
	emit signalChangeSliceZ(k);

}

void Core::slotNavigationMode()
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_style[i]->SetInteractorStyleToNavigation();
	}

	//this->slotRuler(false);
}

void Core::slotWindowLevelMode()
{

	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_style[i]->SetInteractorStyleToWindowLevel();
	}
	//this->slotRuler(false);
}

void Core::slotBrushMode()
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_style[i]->SetInteractorStyleToPaintBrush();
	}
	//Update ui
	//m_moduleWidget->SetPage(2);

	//this->slotRuler(false);
}

void Core::slotSetBrushSize(int size)
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		if (m_style[i] != NULL) {
			m_style[i]->GetPaintBrush()->SetBrushSize(size);
		}
	}
}

void Core::slotSetBrushShape(int shape)
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		if (m_style[i] != NULL) {
			m_style[i]->GetPaintBrush()->SetBrushShape(shape);
		}
	}
}

void Core::slotSetImageLayerColor(int layer) {
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		if (m_style[i] != NULL) {
			m_style[i]->GetPaintBrush()->SetPaintBrushLabel(layer + 1);
			m_style[i]->GetPolygonDraw()->SetVesselWallLabel(layer + 1);
		}
	}
}

void Core::slotGenerateCenterlineBtn()
{
	// Extract VOI
	vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	voi->SetInputData(this->m_imageManager->getOverlay()->GetVTKOutput());
	voi->SetVOI(this->m_imageManager->getOverlay()->GetDisplayExtent());
	voi->Update();

	// Threshold the iamge
	vtkSmartPointer<vtkImageThreshold> thres = vtkSmartPointer<vtkImageThreshold>::New();
	thres->SetInputData(voi->GetOutput());
	thres->ThresholdBetween(LABEL_LUMEN - 0.1, LABEL_LUMEN + 0.1);
	thres->SetOutValue(0);
	thres->SetInValue(10);
	thres->Update();

	/// Obtain centerline
	SurfaceCreator* surfaceCreator = new SurfaceCreator();
	surfaceCreator->SetInput(thres->GetOutput());
	surfaceCreator->SetValue(10);
	surfaceCreator->SetSmoothIteration(15);
	surfaceCreator->SetDiscrete(false);
	surfaceCreator->SetResamplingFactor(1.0);
	surfaceCreator->Update();

	// Clip at the end of the boundaries
	vtkSmartPointer<vtkPolyData> surface = vtkSmartPointer<vtkPolyData>::New();
	surface->DeepCopy(surfaceCreator->GetOutput());
	vtkSmartPointer<vtkPlane> plane[2];
	plane[0] = vtkSmartPointer<vtkPlane>::New();
	plane[1] = vtkSmartPointer<vtkPlane>::New();
	plane[0]->SetNormal(0, 0, -1);
	plane[1]->SetNormal(0, 0, 1);
	double zCoord1 = (this->m_imageManager->getOverlay()->GetDisplayExtent()[5] - 1) *
		this->m_imageManager->getListOfViewerInputImages()[0]->GetSpacing()[2] +
		this->m_imageManager->getListOfViewerInputImages()[0]->GetOrigin()[2];
	double zCoord2 = (this->m_imageManager->getOverlay()->GetDisplayExtent()[4] + 1) *
		this->m_imageManager->getListOfViewerInputImages()[0]->GetSpacing()[2] +
		this->m_imageManager->getListOfViewerInputImages()[0]->GetOrigin()[2];
	plane[0]->SetOrigin(0, 0, zCoord1);
	plane[1]->SetOrigin(0, 0, zCoord2);
	for (int i = 0; i < 2; i++)
	{
		vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
		clipper->SetClipFunction(plane[i]);
		clipper->SetInputData(surface);
		clipper->Update();
		surface->DeepCopy(clipper->GetOutput());
	}


	Centerline* cl = new Centerline;
	try {
		cl->SetSurface(surface);
		cl->Update();
	}
	catch (std::exception &e) {
		QString errorMsg(e.what());
		QString msg("\nCenterline calculations failed. Some functions might not be available. Please select an ROI with cylindrical structures. (Branches are allowed)");
		this->DisplayErrorMessage((errorMsg + msg).toStdString());

		///Volume Render
		GPUVolumeRenderingFilter* volumeRenderingFilter =
			GPUVolumeRenderingFilter::New();

		volumeRenderingFilter->SetInputData(voi->GetOutput());
		volumeRenderingFilter->SetLookUpTable(this->m_2DimageViewer[0]->GetLookupTable());
		volumeRenderingFilter->GetVolume()->SetPickable(1);
		volumeRenderingFilter->Update();

		this->m_3DDataRenderer->AddVolume(volumeRenderingFilter->GetVolume());

		this->m_3DimageViewer->GetRenderers()->GetFirstRenderer()->SetBackground(0.3, 0.3, 0.3);
		this->m_3DimageViewer->GetRenderers()->GetFirstRenderer()->ResetCamera();
		this->m_3DimageViewer->Render();
		this->m_style3D->SetInteractorStyleTo3DDistanceWidget();

		return;
	}


	if (m_centerlinePD != NULL) {
		this->m_centerlinePD->Delete();
		this->m_centerlinePD = NULL;
	}
	this->m_centerlinePD = vtkPolyData::New();
	this->m_centerlinePD->DeepCopy(cl->GetCenterline());

	delete cl;

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(this->m_centerlinePD);
	mapper->SetLookupTable(this->GetLookupTable());
	mapper->SetScalarRange(0, 6); // Change this too if you change the look up table!
	mapper->Update();

	vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
	//Actor->GetProperty()->SetColor(overlayColor[0][0]/255.0, overlayColor[0][1] / 255.0, overlayColor[0][2] / 255.0);
	Actor->SetMapper(mapper);
	Actor->GetProperty()->SetOpacity(0.9);
	Actor->GetProperty()->SetRepresentationToSurface();
	Actor->SetPickable(1);
	this->m_3DDataRenderer->AddActor(Actor);
}



void Core::slotUpdate3DLabelBtn()
{
	this->m_3DimageViewer->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();

	// Extract VOI
	vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	voi->SetInputData(this->m_imageManager->getOverlay()->GetVTKOutput());
	voi->SetVOI(this->m_imageManager->getOverlay()->GetDisplayExtent());
	voi->Update();

	///Volume Render
	GPUVolumeRenderingFilter* volumeRenderingFilter =
		GPUVolumeRenderingFilter::New();

	volumeRenderingFilter->SetInputData(voi->GetOutput());
	volumeRenderingFilter->SetLookUpTable(this->m_2DimageViewer[0]->GetLookupTable());
	volumeRenderingFilter->GetVolume()->SetPickable(1);
	//volumeRenderingFilter->GetVolumeProperty()->SetInterpolationTypeToLinear();
	volumeRenderingFilter->Update();

	this->m_3DDataRenderer->AddVolume(volumeRenderingFilter->GetVolume());
	this->m_3DimageViewer->GetRenderers()->GetFirstRenderer()->SetBackground(0.3, 0.3, 0.3);
	this->m_3DimageViewer->GetRenderers()->GetFirstRenderer()->ResetCamera();
	this->m_3DimageViewer->Render();
	this->m_style3D->SetInteractorStyleTo3DDistanceWidget();
	//delete surfaceCreator;

	/*vtkRenderWindow* rwin = vtkRenderWindow::New();
	rwin->AddRenderer(vtkRenderer::New());
	rwin->GetRenderers()->GetFirstRenderer()->AddVolume(volumeRenderingFilter->GetVolume());
	vtkRenderWindowInteractor* interacotr = vtkRenderWindowInteractor::New();
	rwin->SetInteractor(interacotr);
	interacotr->Initialize();
	rwin->Render();
	interacotr->Start();*/
}

void Core::slotContourMode()
{
	for (int i = 0; i < VIEWER_NUM; i++)
	{
		m_style[i]->SetInteractorStyleToPolygonDraw();
		//m_style[i]->SetMode(4);
	}
	//m_moduleWidget->SetPage(1);

	//this->slotRuler(false);
}

void Core::slotEnableAutoLumenSegmentation(bool flag)
{
	for (int i = 0; i < 3; i++)
	{
		m_style[i]->GetPolygonDraw()->EnableAutoLumenSegmentation(flag);
	}
}

void Core::slotSetContourFilterGenerateValues(int generateValues)
{
	for (int i = 0; i < 3; ++i) {
		m_style[i]->GetPolygonDraw()->
			SetContourFilterGenerateValues(generateValues);
		m_style[i]->GetPolygonDraw()->
			GenerateLumenWallContourWidget();

	}
}

void Core::slotSetLineInterpolatorToSmoothCurve(bool flag)
{
	if (flag) {
		for (int i = 0; i < 3; i++)
		{
			m_style[i]->GetPolygonDraw()->SetLineInterpolator(0);
		}
	}
}

void Core::slotSetLineInterpolatorToPolygon(bool flag)
{
	if (flag) {
		for (int i = 0; i < 3; i++)
		{
			m_style[i]->GetPolygonDraw()->SetLineInterpolator(1);
		}
	}
}

void Core::slotRuler(bool b)
{
	for (int i = 0; i < VIEWER_NUM; ++i) {
		m_style[i]->SetInteractorStyleToRuler();
	}
	//for (int i = 0; i< VIEWER_NUM; i++)
	//{
	//	m_2DimageViewer[i]->SetRulerEnabled(b);
	//}

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

	//this->slotRuler(false);
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
	for (int i = 0; i < this->m_imageManager->getListOfViewerInputImages().size(); ++i) {
		if (m_imageManager->getListOfViewerInputImages()[i] != NULL) {
			vtkSmartPointer<vtkExtractVOI> extractVOIFilter =
				vtkSmartPointer<vtkExtractVOI>::New();
			extractVOIFilter->SetInputData(m_imageManager->getListOfViewerInputImages()[i]);
			extractVOIFilter->SetVOI(newExtent);
			extractVOIFilter->Update();
			this->m_imageManager->getListOfViewerInputImages()[i]->DeepCopy(
				extractVOIFilter->GetOutput());

		}
	}
	slotChangeView(MULTIPLANAR_VIEW);
	//this->m_imageManager->getOverlay().SetDisplayExtent(newExtent);

	for (int i = 0; i < VIEWER_NUM; i++)
	{
		this->m_2DimageViewer[i]->GetRenderer()->ResetCamera();
		this->m_2DimageViewer[i]->GetInputLayer()->Modified();
		this->m_2DimageViewer[i]->Render();

	}

	//ui->actionMultiPlanarView->trigger();
	//ui->actionNavigation->trigger();

	//this->slotChangeSlice(0, 0, 0);
}
void Core::slotResetROI()
{
	for (int i = 0; i < m_imageManager->getListOfViewerInputImages().size(); ++i)
	{
		if (m_imageManager->getListOfViewerInputImages()[i] != NULL) {
			m_imageManager->getListOfViewerInputImages()[i]->DeepCopy(
				m_imageManager->getListOfVtkImages()[i]);
		}
	}
	slotChangeView(MULTIPLANAR_VIEW);
	//this->imageManager.getOverlay().DisplayExtentOff();


	//ui->actionMultiPlanarView->trigger();
	//ui->actionNavigation->trigger();

	//this->slotChangeSlice(0, 0, 0);
}
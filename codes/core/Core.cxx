#include "Core.h"

#include "SurfaceCreator.h"
#include "Centerline.h"
#include <GPUVolumeRenderingFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryDilateImageFilter.h>
#include <vtkClipPolyData.h>
#include <vtkExtractVOI.h>
#include <vtkRendererCollection.h>
#include <vtkImageThreshold.h>
#include <QMessageBox>
#include <QAction>
Core::Core(QWidget* parent)
	:Core(parent, parent)
{
	
}

Core::Core(QObject* parent, QWidget* mainWindow)
	:QObject(parent)
{
	m_imageManager = new  MyImageManager(this);
	m_ioManager = new IOManager(mainWindow);
	m_widgetManager = new MyWidgetManager(this);
	m_lumenExtractionFilter = new LumenExtraction();

	// default parameters for lumen extractions
	this->m_lumenExtractionFilter->SetNumberOfIterations(30);
	this->m_lumenExtractionFilter->SetDilationValue(4);
	this->m_lumenExtractionFilter->SetMultiplier(2.3);
	this->m_lumenExtractionFilter->SetInitialNeighborhoodRadius(1);

	// enable registration
	m_ioManager->enableRegistration(true);

	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
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
	this->m_3DDataRenderer->SetBackground(0.3, 0.3, 0.3);
	this->m_3DimageViewer->AddRenderer(this->m_3DDataRenderer);
	this->m_3Dinteractor->Initialize();

	vtkROIWidget* roi = this->m_widgetManager->GetROIWidget();
	for (int i = 0; i < 3;i++)
	{
		roi->SetBorderWidgetsInteractor(i, this->m_2DimageViewer[i]->GetRenderWindow()->GetInteractor());
	}
	roi->SetInteractor(this->m_3Dinteractor);

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

InteractorStyleSwitch* Core::Get2DInteractorStyle(int i)
{
	return this->m_style[i];
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
	if (num < 0 || num > NUMBER_OF_2DVIEWERS) {
		return NULL;
	}
	else if (num == NUMBER_OF_2DVIEWERS) {
		return m_3DimageViewer;
	}
	else {
		return m_2DimageViewer[num]->GetRenderWindow();
	}
}

void Core::RenderAllViewer()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
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

MyWidgetManager* Core::GetMyWidgetManager()
{
	return this->m_widgetManager;
}

void Core::slotAddOverlayToImageViewer() {
	int *extent1 = this->m_imageManager->getOverlay()->GetOutput()->GetExtent();
	int *extent2 = this->m_imageManager->getListOfViewerInputImages()[0]->GetExtent();
	if (!std::equal(extent1, extent1 + 6, extent2)) {
		this->DisplayErrorMessage("Selected segmentation has wrong spacing!");
		return;
	}
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		// The tableRange of LookupTable Change when the vtkImageViewer2::Render was call
		// Very strange
		//this->LookupTable->SetTableRange(0, 6);
		m_2DimageViewer[i]->SetOverlay(m_imageManager->getOverlay());
		//m_2DimageViewer[i]->GetOverlayActor()->SetVisibility(true);
	}
	this->m_imageManager->getOverlay()->GetOutput()->Modified();
}

void Core::slotVisualizeAll2DViewers()
{
	// Validate Patient Id and Information, if they are different, 
	// a error message will pop up.
	slotValidatePatientInformation();
	slotChangeView(MULTIPLANAR_VIEW);
	if (m_firstInitialize) {
		for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
			m_2DimageViewer[i]->SetupInteractor(m_interactor[i]);
			m_style[i]->SetImageViewer(m_2DimageViewer[i]);
			m_interactor[i]->SetInteractorStyle(m_style[i]);
		}
		slotAddOverlayToImageViewer();
		m_firstInitialize = false;
		emit signalMultiPlanarView();
	}

	// reset slice to all viewer
	const int* extent = m_2DimageViewer[DEFAULT_IMAGE]->GetInput()->GetExtent();
	m_style[DEFAULT_IMAGE]->GetNavigation()->SetCurrentFocalPointWithImageCoordinate(
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

	int index = this->m_imageManager->GetModalityIndex(modalityName);
	if (m_viewMode == MULTIPLANAR_VIEW) {
		for (int j = 0; j < NUMBER_OF_2DVIEWERS; ++j) {
			m_2DimageViewer[j]->SetInputData(
				this->m_imageManager->getListOfViewerInputImages()[index]);
			m_2DimageViewer[j]->Render();
			m_2DimageViewer[j]->InitializeHeader(m_imageManager->getModalityName(index));
		}
	}
	else {
		m_2DimageViewer[viewerNum]->SetInputData(
			this->m_imageManager->getListOfViewerInputImages()[index]);
		m_2DimageViewer[viewerNum]->Render();
		m_2DimageViewer[viewerNum]->InitializeHeader(m_imageManager->getModalityName(index));
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

void Core::slotChangeView(int viewMode)
{

	m_viewMode = viewMode;
	// SEGMENTATION_VIEW
	if (viewMode) {
		// i1 for looping all 5 vtkImage, while i2 for looping all 3 m_2DimageViewer
		for (int i1 = 0, i2 = 0; i2 < NUMBER_OF_2DVIEWERS; ++i2)
		{
			for (; i1 < this->m_imageManager->getListOfViewerInputImages().size() && i2 < NUMBER_OF_2DVIEWERS;
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
			if (i1 >= m_imageManager->getListOfVtkImages().size() && i2 < NUMBER_OF_2DVIEWERS ) {
				this->m_2DimageViewer[i2]->GetRenderWindow()->GetInteractor()->Disable();
				// disable view props
				m_2DimageViewer[i2]->SetAllBlack(true);

			}

		}

	}
	// MULTIPLANAR_VIEW
	else {
		for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
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
			m_2DimageViewer[i]->SetAllBlack(false);

		}

	}
	RenderAllViewer();
}


void Core::slotNavigationMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->SetInteractorStyleToNavigation();
	}
}

void Core::slotWindowLevelMode()
{

	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->SetInteractorStyleToWindowLevel();
	}
	this->ModeChangeUpdate(WINDOW_LEVEL_MODE);
}

void Core::slotBrushMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->SetInteractorStyleToPaintBrush();
	}
	
	this->ModeChangeUpdate(BRUSH_MODE);
}

void Core::slotSetBrushSize(int size)
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		if (m_style[i] != NULL) {
			m_style[i]->GetPaintBrush()->SetBrushSize(size);
		}
	}
}

void Core::slotSetBrushShape(int shape)
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		if (m_style[i] != NULL) {
			m_style[i]->GetPaintBrush()->SetBrushShape(shape);
		}
	}
}

void Core::slotSetImageLayerColor(int layer) {
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		if (m_style[i] != NULL) {
			m_style[i]->GetPaintBrush()->SetPaintBrushLabel(layer + 1);
			//m_style[i]->GetPolygonDraw()->SetVesselWallLabel(layer + 1);
		}
	}
}

void Core::slotSetPaintBrushToEraser(bool flag)
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		if (m_style[i] != NULL) {

			m_style[i]->GetPaintBrush()->EnableEraserMode(flag);
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
	thres->SetInValue(VTK_SHORT_MAX);
	thres->SetOutputScalarTypeToUnsignedShort();
	thres->Update();

	/// Obtain centerline
	// Dilate the image fisrst
	typedef itk::Image<ushort, 3> ImageTypeForDilation;
	typedef itk::BinaryBallStructuringElement<ImageTypeForDilation::PixelType, 3>  StructuringElementType;
	typedef itk::BinaryDilateImageFilter<ImageTypeForDilation, ImageTypeForDilation, StructuringElementType> DilateFilterType;
	typedef itk::VTKImageToImageFilter<ImageTypeForDilation> Vtk2ItkConnectorType;
	typedef itk::ImageToVTKImageFilter<ImageTypeForDilation> Itk2VtkConnectorType;
	Vtk2ItkConnectorType::Pointer v2i = Vtk2ItkConnectorType::New();
	Itk2VtkConnectorType::Pointer i2v = Itk2VtkConnectorType::New();
	StructuringElementType structuringElement;
	structuringElement.SetRadius(2);
	structuringElement.CreateStructuringElement();
	v2i->SetInput(thres->GetOutput());
	v2i->Update();
	//  Dilation
	DilateFilterType::Pointer dilationFilter = DilateFilterType::New();
	dilationFilter->SetInput(v2i->GetOutput());
	dilationFilter->SetKernel(structuringElement);
	dilationFilter->SetDilateValue(1);
	dilationFilter->Update();
	i2v->SetInput(dilationFilter->GetOutput());
	i2v->Update();
	
	// Generate surface for centerline calculation
	SurfaceCreator* surfaceCreator = new SurfaceCreator();
	surfaceCreator->SetInput(i2v->GetOutput());
	surfaceCreator->SetValue(15);
	surfaceCreator->SetSmoothIteration(5);
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
	catch (Centerline::ERROR_CODE &e) {
		QString msg("\nCenterline calculations failed. Some functions might not be available. Please select an ROI with cylindrical structures. (Branches are allowed)");
		this->DisplayErrorMessage((msg).toStdString());

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

int* Core::ConvertBoundsToExtent(double* bounds, bool clampping)
{
	// Error check
	if (this->m_imageManager->getNumberOfImages() == 0)
		return nullptr;

	int imExtent[6];
	double spacing[3], origin[3];
	this->m_imageManager->getListOfVtkImages().at(0)->GetSpacing(spacing);
	this->m_imageManager->getListOfVtkImages().at(0)->GetOrigin(origin);
	this->m_imageManager->getListOfVtkImages().at(0)->GetExtent(imExtent);

	int* extent = (int*)malloc(sizeof(int) * 6);
	extent[0] = ceil((bounds[0] - origin[0]) / spacing[0]);
	extent[1] = floor((bounds[1] - origin[0]) / spacing[0]);
	extent[2] = ceil((bounds[2] - origin[1]) / spacing[1]);
	extent[3] = floor((bounds[3] - origin[1]) / spacing[1]);
	extent[4] = ceil((bounds[4] - origin[2]) / spacing[2]);
	extent[5] = floor((bounds[5] - origin[2]) / spacing[2]);

	if (clampping)
		for (int i = 0; i < 3;i++)
		{
			extent[2*i] = { extent[2*i] < imExtent[2*i] ? imExtent[2*i] : extent[2*i] };
			extent[2*i+1] = { extent[2*i+1] > imExtent[2*i+1] ? imExtent[2*i+1] : extent[2*i+1] };
		}

	return extent;
}

double* Core::ConvertExtentToBounds(int* extent)
{
	// Error check
	if (this->m_imageManager->getNumberOfImages() == 0)
		return nullptr;

	double spacing[3], origin[3];
	this->m_imageManager->getListOfVtkImages().at(0)->GetSpacing(spacing);
	this->m_imageManager->getListOfVtkImages().at(0)->GetOrigin(origin);

	double* bounds = (double*)malloc(sizeof(double) * 6);
	bounds[0] = extent[0] * spacing[0] + origin[0];
	bounds[1] = extent[1] * spacing[0] + origin[0];
	bounds[2] = extent[2] * spacing[1] + origin[1];
	bounds[3] = extent[3] * spacing[1] + origin[1];
	bounds[4] = extent[4] * spacing[2] + origin[2];
	bounds[5] = extent[5] * spacing[2] + origin[2];

	return bounds;
}

void Core::slotSetExtractLumenSeedList(std::vector<int *> seedList)
{
	this->m_lumenExtractionFilter->ClearAllSeeds();
	for each (int* seedIndex in seedList)
	{
		this->m_lumenExtractionFilter->AddSeed(seedIndex);
	}
}

void Core::slotValidatePatientInformation()
{
	if (m_imageManager->getNumberOfImages() == 1) {
		return;
	}
	bool allTheSame = true;
	QMap<QString, QString>* header =
		m_imageManager->getListOfDICOMHeader()[Core::DEFAULT_IMAGE];
	if (header == NULL) {
		return;
	}
	QString patientName = header->value("0010|0010");
	QString patientID = header->value("0010|0020");
	//qDebug() << patientName;
	//qDebug() << patientID;
	for (int i = 1; i < m_imageManager->getListOfViewerInputImages().size(); ++i) {
		if (m_imageManager->getListOfViewerInputImages()[i] == NULL) {
			continue;
		}
		QString _patientName = m_imageManager->getListOfDICOMHeader()[i]->
			value("0010|0010");
		QString _patientID = m_imageManager->getListOfDICOMHeader()[i]->
			value("0010|0020");
		//qDebug() << _patientName;
		//qDebug() << _patientID;
		if (patientName != _patientName || patientID != _patientID) {
			allTheSame = false;
			DisplayErrorMessage("Attention! Patiention Name or Patiention Id of your images"
				"might be different. ");
			break;
		}
	}
}

void Core::slotChangeOpacity(int layer, int opacity)
{
	double* value = GetLookupTable()->GetTableValue(layer);
	value[3] = opacity * 0.01;
	GetLookupTable()->SetTableValue(layer, value);
	GetLookupTable()->Build();
	RenderAllViewer();
}

void Core::ModeChangeUpdate(INTERACTION_MODE index)
{
	if (index != ROI_MODE)
	{
		// turn off ROI widget
		this->m_widgetManager->DisableROIWidget();
	}

	if (index != RULER_MODE)
	{
		// turn off Ruler widget
		for (int i = 0; i < 3;i++)
		{
			this->m_style[i]->GetRuler()->SetDistanceWidgetEnabled(false);
		}
	}

	if (index != POLYGON_CONTOUR_MODE)
	{
		// turn off contour widget
		for (int i = 0; i < 3;i++)
		{
			this->m_style[i]->GetPolygonDraw()->SetPolygonModeEnabled(false);
		}
	}

	this->RenderAllViewer();
}

void Core::slotContourMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->SetInteractorStyleToPolygonDraw();
	}
	this->ModeChangeUpdate(POLYGON_CONTOUR_MODE);
}

void Core::slotFillContour()
{
	for (int i = 0; i < 3; i++)
	{
		m_style[i]->GetPolygonDraw()->FillPolygon();
	}
}

void Core::slotClearContour()
{
	for (int i = 0; i < 3; i++)
	{
		m_style[i]->GetPolygonDraw()->SetPolygonModeEnabled(false);
		m_style[i]->GetPolygonDraw()->SetPolygonModeEnabled(true);

	}
}

void Core::slotEnableAutoLumenSegmentation(bool flag)
{
	for (int i = 0; i < 3; i++)
	{
		//m_style[i]->GetPolygonDraw()->EnableAutoLumenSegmentation(flag);
	}
}

void Core::slotSetContourFilterGenerateValues(int generateValues)
{
	for (int i = 0; i < 3; ++i) {
		//m_style[i]->GetPolygonDraw()->
		//	SetContourFilterGenerateValues(generateValues);
		//m_style[i]->GetPolygonDraw()->
		//	GenerateLumenWallContourWidget();

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

void Core::slotExtractLumen()
{
	try
	{
		this->m_lumenExtractionFilter->Update();
		vtkSmartPointer<vtkImageData> lumenImage =
			vtkSmartPointer<vtkImageData>::New();
		this->m_lumenExtractionFilter->GetLumenVtkImage(lumenImage);
		this->m_imageManager->getOverlay()->ReplacePixels(lumenImage->GetExtent(), lumenImage);
		this->RenderAllViewer();
	}
	catch (...) {
		this->DisplayErrorMessage("Unknown Error!");
	}
}

void Core::slotSetExtractLumenInputImage(vtkImageData* im)
{
	this->m_lumenExtractionFilter->SetInputData(im);
}

void Core::slotSetExtractLumenDilationValue(int val)
{
	this->m_lumenExtractionFilter->SetDilationValue(val);
}

void Core::slotSetExtractLumenInitialNeighborhoodRadius(int val)
{
	this->m_lumenExtractionFilter->SetInitialNeighborhoodRadius(val);
}

void Core::slotSetExtractLumenMultiplier(double val)
{
	this->m_lumenExtractionFilter->SetMultiplier(val);
}

void Core::slotDrawSegmentation()
{
	this->m_style[2]->GetSmartContour2()->FillAllPolygons();
}

void Core::slotExtractLumenDilateLabel(vtkImageData* im)
{
	this->m_lumenExtractionFilter->LabelDilation(im);
	vtkSmartPointer<vtkImageData> vesselWallImage =
		vtkSmartPointer<vtkImageData>::New();

	this->m_lumenExtractionFilter->GetDilatedVtkImage(vesselWallImage);
	for (int i = 0; i < 6; i++)
	{
		cout << im->GetExtent()[i] << ' ' << vesselWallImage->GetExtent()[i];
	}
	cout << endl;
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
		m_style[i]->GetSmartContour2()->SetLumenImage(im);
		m_style[i]->GetSmartContour2()->SetVesselWallImage(vesselWallImage);
	}
}

void Core::slotRulerMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
		m_style[i]->SetInteractorStyleToRuler();
		for (int j = 0; j < NUMBER_OF_2DVIEWERS; ++j) {
			m_style[i]->GetRuler()->AddSynchronalRuler(m_style[j]->GetRuler());
		}
	}

	this->ModeChangeUpdate(RULER_MODE);
}

void Core::slotEnableMaximumWallThickneesLabel(bool flag)
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
		m_style[i]->GetRuler()->EnableMaximumWallThickneesLabel(flag);
	}
}

void Core::slotROIMode()
{
	// force user into ROI mode
	this->slotNavigationMode();

	if (this->m_imageManager->getNumberOfImages() != 0 && !this->m_widgetManager->GetROIWidget()->GetEnabled())
	{
		double bounds[6];
		this->m_imageManager->getListOfViewerInputImages().at(0)->GetBounds(bounds);
		this->m_widgetManager->EnableROIWidget(
			this->m_2DimageViewer, 
			this->m_3DimageViewer, 
			bounds, 
			this->m_2DimageViewer[0]->GetFocalPointWithWorldCoordinate());
		this->m_widgetManager->GetROIWidget()->GetRepresentation()->VisibilityOff();
	}

	this->ModeChangeUpdate(ROI_MODE);
}

void Core::slotSmartContourMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->SetInteractorStyleToSmartContour();
	}
	this->ModeChangeUpdate(SMARTCONTOUR_MODE);

}

void Core::slotSmartContour2Mode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->SetInteractorStyleToSmartContour2();
	}
	this->ModeChangeUpdate(SMARTCONTOUR2_MODE);

}

void Core::slotChangeROI()
{
	/// Depicted


}

void Core::slotSelectROI()
{

	int newExtent[6];
	double newBounds[6];
	double* bounds = this->m_widgetManager->GetROIWidget()->GetRepresentation()->GetBounds();
	memcpy(newExtent, this->ConvertBoundsToExtent(bounds), sizeof(int) * 6);

	// Convert clamped value back to bounds for ROI widget
	memcpy(newBounds, this->ConvertExtentToBounds(newExtent), sizeof(double) * 6);
	this->m_widgetManager->GetROIWidget()->GetRepresentation()->PlaceWidget(newBounds);

	for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
		m_2DimageViewer[i]->SetImageVOI(newExtent);
		m_2DimageViewer[i]->SetOverlayVOI(newExtent);
		m_2DimageViewer[i]->GetRenderer()->ResetCamera();
	}
	m_style[DEFAULT_IMAGE]->GetNavigation()->Initialization();


}
void Core::slotResetROI()
{

	// Extract VOI of the vtkImage data
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
		m_2DimageViewer[i]->ResetImageVOI();
		m_2DimageViewer[i]->ResetOverlayVOI();
		m_2DimageViewer[i]->GetRenderer()->ResetCamera();
	}
	m_style[DEFAULT_IMAGE]->GetNavigation()->Initialization();

}
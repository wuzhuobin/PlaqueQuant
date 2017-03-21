#include "Core.h"

#include "SurfaceCreator.h"
#include "Centerline.h"

#include <vtkRenderWindow.h>
#include <vtkProperty.h>

#include <GPUVolumeRenderingFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkClipPolyData.h>
#include <vtkExtractVOI.h>
#include <vtkRendererCollection.h>
#include <vtkImageThreshold.h>
#include <QMessageBox>
#include <QAction>
#include "vtkXMLPolyDataWriter.h"
#include "vtkConnectivityFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
Core::Core(QWidget* parent)
	:Core(parent, parent)
{
}

Core::Core(QObject* parent, QWidget* mainWindow)
	:QObject(parent)
{
	m_imageManager = new  MyImageManager(this);
	m_ioManager = new IOManager(mainWindow);

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

	m_ioManager->setMyImageManager(m_imageManager);

	connect(m_ioManager, SIGNAL(finishOpenMultiImages()),
		this, SLOT(slotVisualizeAll2DViewers()));
	connect(m_ioManager, SIGNAL(finishOpenSegmentation()),
		this, SLOT(slotAddOverlayToImageViewer()));
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



void Core::slotAddOverlayToImageViewer() {
	int *extent1 = this->m_imageManager->getOverlay()->GetOutput()->GetExtent();
	int *extent2 = this->m_imageManager->getListOfVtkImages()[0]->GetExtent();
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

	if (m_firstInitialize) {
		for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
			m_2DimageViewer[i]->SetupInteractor(m_interactor[i]);
			m_style[i]->SetImageViewer(m_2DimageViewer[i]);
			m_interactor[i]->SetInteractorStyle(m_style[i]);
		}
		m_firstInitialize = false;
	}
	slotAddOverlayToImageViewer();
	slotChangeView(MULTIPLANAR_VIEW);
	emit signalMultiPlanarView();

	// some special setting for every InteractorStyle
	// reset slice to all viewer
	const int* extent = m_2DimageViewer[DEFAULT_IMAGE]->GetInput()->GetExtent();
	m_style[DEFAULT_IMAGE]->GetNavigation()->SetCurrentFocalPointWithImageCoordinate(
		(extent[1] - extent[0]) / 2,
		(extent[3] - extent[2]) / 2,
		(extent[5] - extent[4]) / 2);
	m_style[DEFAULT_IMAGE]->GetSeedsPlacer()->SetTargetImages(
		m_imageManager->getListOfVtkImages(),
		m_imageManager->getListOfModalityNames());

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
				this->m_imageManager->getListOfVtkImages()[index]);
			m_2DimageViewer[j]->Render();
			m_2DimageViewer[j]->InitializeHeader(m_imageManager->getModalityName(index));
		}
	}
	else {
		m_2DimageViewer[viewerNum]->SetInputData(
			this->m_imageManager->getListOfVtkImages()[index]);
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
			for (; i1 < this->m_imageManager->getListOfVtkImages().size() && i2 < NUMBER_OF_2DVIEWERS;
				++i1) {
				// skip the NULL image
				if (this->m_imageManager->getListOfVtkImages()[i1] != NULL) {
					// SetupInteractor should be ahead of InitializeHeader
					this->m_2DimageViewer[i2]->SetInputData(
						this->m_imageManager->getListOfVtkImages()[i1]);
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
				this->m_imageManager->getListOfVtkImages()[DEFAULT_IMAGE]);
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
}

void Core::slotBrushMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->SetInteractorStyleToPaintBrush();
	}
	
}


void Core::slotGenerateCenterlineBtn()
{
	// Extract VOI
	//vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	//voi->SetInputData(this->m_imageManager->getOverlay()->GetVTKOutput());
	//voi->SetVOI(this->m_imageManager->getOverlay()->GetDisplayExtent());
	//voi->Update();

	double* origin = m_2DimageViewer[DEFAULT_IMAGE]->GetInputLayer()->GetOrigin();

	// Threshold the iamge
	vtkSmartPointer<vtkImageThreshold> thres = vtkSmartPointer<vtkImageThreshold>::New();
	thres->SetInputData(m_2DimageViewer[DEFAULT_IMAGE]->GetInputLayer());
	thres->ThresholdBetween(LABEL_LUMEN, LABEL_LUMEN);
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

	// There is a bug in marching cube which causes the output mesh to be sliced to seperate pieces at (0,0,0)
	// So this work around move the image away from that point sufficiently far away.
	vtkImageData* test = i2v->GetOutput();
	test->SetOrigin(250, 250, 250);

	vtkSmartPointer<vtkMarchingCubes> marchingcubeFilter
		= vtkSmartPointer<vtkMarchingCubes>::New();
	marchingcubeFilter->SetInputData(i2v->GetOutput());
	marchingcubeFilter->SetValue(0, VTK_SHORT_MAX - 1);
	marchingcubeFilter->ComputeScalarsOff();
	marchingcubeFilter->ComputeGradientsOff();
	marchingcubeFilter->ComputeNormalsOff();
	marchingcubeFilter->Update();

	vtkSmartPointer<vtkPolyDataConnectivityFilter> connFilter
		= vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connFilter->SetInputConnection(marchingcubeFilter->GetOutputPort());
	connFilter->SetExtractionModeToLargestRegion();
	connFilter->Update();

	// Transform the result back to original space
	vtkSmartPointer<vtkTransformPolyDataFilter> trans 
		= vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	trans->SetInputConnection(connFilter->GetOutputPort());
	trans->SetTransform(vtkSmartPointer<vtkTransform>::New());
	vtkTransform::SafeDownCast(trans->GetTransform())->Translate(-250, -250, -250);
	vtkTransform::SafeDownCast(trans->GetTransform())->Translate(origin);
	trans->Update();


	vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	writer->SetInputData(trans->GetOutput());
	writer->SetFileName("C:/Users/lwong/Downloads/output.vtp");
	writer->Update();
	writer->Write();

	// Clip at the end of the boundaries
	vtkSmartPointer<vtkPolyData> surface = vtkSmartPointer<vtkPolyData>::New();
	surface->DeepCopy(trans->GetOutput());
	vtkSmartPointer<vtkPlane> plane[2];
	plane[0] = vtkSmartPointer<vtkPlane>::New();
	plane[1] = vtkSmartPointer<vtkPlane>::New();
	plane[0]->SetNormal(0, 0, -1);
	plane[1]->SetNormal(0, 0, 1);
	double zCoord1 = (this->m_imageManager->getOverlay()->GetDisplayExtent()[5] - 1) *
		this->m_imageManager->getOverlay()->GetVTKOutput()->GetSpacing()[2] +
		this->m_imageManager->getOverlay()->GetVTKOutput()->GetOrigin()[2];
	double zCoord2 = (this->m_imageManager->getOverlay()->GetDisplayExtent()[4] + 1) *
		this->m_imageManager->getOverlay()->GetVTKOutput()->GetSpacing()[2] +
		this->m_imageManager->getOverlay()->GetVTKOutput()->GetOrigin()[2];
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

	writer->SetInputData(surface);
	writer->SetFileName("C:/Users/lwong/Downloads/output_clipped.vtp");
	writer->Update();
	writer->Write();


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

		volumeRenderingFilter->SetInputData(m_2DimageViewer[DEFAULT_IMAGE]->GetInputLayer());
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

		volumeRenderingFilter->SetInputData(m_2DimageViewer[DEFAULT_IMAGE]->GetInputLayer());
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

	volumeRenderingFilter->SetInputData(m_2DimageViewer[DEFAULT_IMAGE]->GetInputLayer());
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
	for (int i = 1; i < m_imageManager->getListOfVtkImages().size(); ++i) {
		if (m_imageManager->getListOfVtkImages()[i] == NULL) {
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

void Core::slotContourMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->SetInteractorStyleToPolygonDraw();
	}
}

void Core::slotRulerMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
		m_style[i]->SetInteractorStyleToRuler();
	}
}

void Core::slotObliqueViewMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS;i++)
	{
		m_style[i]->GetObliqueView()->SetCurrentSeedsType(InteractorStyleSeedsPlacer::ObliqueViewSeeds);
		m_style[i]->SetInteractorStyleToObliqueView();
	}
}

void Core::slotROIMode()
{
	// force user into ROI mode
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; ++i) {
		m_style[i]->SetInteractorStyleToROI();
	}
}

void Core::slotSeedsPlacerMode()
{
	for (int i = 0; i < NUMBER_OF_2DVIEWERS; i++)
	{
		m_style[i]->GetSeedsPlacer()->SetCurrentSeedsType(InteractorStyleSeedsPlacer::SegmentationSeeds);
		m_style[i]->SetInteractorStyleToSeedsPlacer();

	}
}

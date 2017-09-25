/*!
 * \file Yolk3DSeries.cpp
 * \date 2017/09/01 16:28
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong,
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief
 *
 *   This class loads a 3D Projection DICOM series and yolk the series with the
 *   specified viewer.
 *
 * \note
*/


// me
#include "IVtkImageData.h"

#include "Yolk3DSeries.h"
#include "ui_Yolk3DSeries.h"
#include "RegistrationWizard.h"

#include <QVTKWidget2.h>
#include <QDebug>
#include <QList>

#include <itkVector.h>
#include <itkImageFileReader.h>
typedef itk::ImageFileReader<IVtkImageData::itkImageType>
ImageFileReader;
#include <itkOrientImageFilter.h>
typedef itk::OrientImageFilter<IVtkImageData::itkImageType, IVtkImageData::itkImageType>
OrientImageFilter;
#include <itkChangeInformationImageFilter.h>
typedef itk::ChangeInformationImageFilter<IVtkImageData::itkImageType>
ChangeInformationImageFilter;
#include <itkGDCMImageIO.h>
using itk::GDCMImageIO;
using itk::MetaDataDictionary;
#include <itkMetaDataDictionary.h>
//#include <itkMetaDataObject.h>



#include "vtkInteractorStyleImage.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkLinearTransform.h"
#include "vtkProperty.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCutter.h>
#include <vtkDICOMImageReader.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPlane.h>
#include <vtkPlaneSource.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkImageViewer2.h>
#include <vtkSmartPointer.h>
#include <QVTKInteractor.h>


class Yolk3DSeriesViewer : public vtkImageViewer2
{
public:
	vtkTypeMacro(Yolk3DSeriesViewer, vtkImageViewer2);
	static Yolk3DSeriesViewer* New() { return new Yolk3DSeriesViewer; }
protected:
	virtual void UpdateOrientation() VTK_OVERRIDE {
		// Set the camera position

		vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;
		if (cam)
		{
			switch (this->SliceOrientation)
			{
			case vtkImageViewer2::SLICE_ORIENTATION_XY:
				cam->SetFocalPoint(0, 0, 0);
				cam->SetPosition(0, 0, -1); // -1 if medical ?
				cam->SetViewUp(0, -1, 0);
				break;

			case vtkImageViewer2::SLICE_ORIENTATION_XZ:
				cam->SetFocalPoint(0, 0, 0);
				cam->SetPosition(0, -1, 0); // 1 if medical ?
				cam->SetViewUp(0, 0, 1);
				break;

			case vtkImageViewer2::SLICE_ORIENTATION_YZ:
				cam->SetFocalPoint(0, 0, 0);
				cam->SetPosition(1, 0, 0); // -1 if medical ?
				cam->SetViewUp(0, 0, 1);
				break;
			}
		}
	}

};

Yolk3DSeries::Yolk3DSeries(QWidget* parent /*= nullptr*/)
	:QWidget(parent)
{
	this->ui = new Ui::Yolk3DSeries;
	this->ui->setupUi(this);
	/* Create UI */
	this->setMinimumSize(800, 600);

	this->imageViewer = vtkSmartPointer<Yolk3DSeriesViewer>::New();
	this->imageViewer->SetRenderWindow(this->ui->widgetViewer->GetRenderWindow());
	this->imageViewer->SetupInteractor(this->ui->widgetViewer->GetInteractor());

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = 
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	//this->ui->widgetViewer->GetInteractor()->SetInteractorStyle(style);
	this->imageViewer->GetImageActor()->VisibilityOff();

	this->planeSource = vtkSmartPointer<vtkPlaneSource>::New();
	this->planeSource->SetOrigin(0, 0, 0);

	//this->planeTransform = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	//this->planeTransform->SetInputConnection(this->planeSource->GetOutputPort());
	this->planeSource->SetCenter(0, 0, 0);
	this->planeSource->SetNormal(0, 0, 1);

	vtkSmartPointer<vtkPolyDataMapper> planeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	planeMapper->SetInputConnection(this->planeSource->GetOutputPort());
	planeMapper->ScalarVisibilityOff();

	this->planeActor = vtkSmartPointer<vtkActor>::New();
	this->planeActor->SetMapper(planeMapper);
	this->planeActor->VisibilityOff();
	this->planeActor->GetProperty()->SetColor(1, 1, 0);

	this->imageViewer->GetRenderer()->AddActor(planeActor);

	this->planeSource2 = vtkSmartPointer<vtkPlaneSource>::New();
	this->planeSource2->SetOrigin(0, 0, 0);
	this->planeSource2->SetPoint1(1000, 0, 0);
	this->planeSource2->SetPoint2(0, 1000, 0);

	this->imageDirection = vtkSmartPointer<vtkMatrix4x4>::New();
	this->imageDirection->Identity();

	vtkSmartPointer<vtkTransform> transform2 =
		vtkSmartPointer<vtkTransform>::New();
	transform2->SetMatrix(this->imageDirection);

	this->planeTransform2 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	this->planeTransform2->SetInputConnection(this->planeSource2->GetOutputPort());
	this->planeTransform2->SetTransform(transform2);
	//this->planeSource->SetCenter(0, 0, 0);
	//this->planeSource->SetNormal(0, 0, 1);

	vtkSmartPointer<vtkPolyDataMapper> planeMapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	planeMapper2->SetInputConnection(this->planeTransform2->GetOutputPort());
	planeMapper2->ScalarVisibilityOff();

	this->planeActor2 = vtkSmartPointer<vtkActor>::New();
	this->planeActor2->SetMapper(planeMapper2);
	this->planeActor2->VisibilityOff();
	this->planeActor2->GetProperty()->SetColor(0, 1, 0);

	this->imageViewer->GetRenderer()->AddActor(planeActor2);


	this->plane = vtkSmartPointer<vtkPlane>::New();
	
	this->cutter = vtkSmartPointer<vtkCutter>::New();
	this->cutter->SetCutFunction(this->plane);
	this->cutter->SetInputConnection(this->planeSource->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> lineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	lineMapper->SetInputConnection(this->cutter->GetOutputPort());
	lineMapper->ScalarVisibilityOff();
	
	this->lineActor = vtkSmartPointer<vtkActor>::New();
	this->lineActor->SetMapper(lineMapper);
	this->lineActor->GetProperty()->SetColor(1, 0, 0);
	this->lineActor->VisibilityOff();

	this->imageViewer->GetRenderer()->AddActor(this->lineActor);
	


	/* Create renderers */
	//this->m_renwin	= this->ui->widgetViewer->GetRenderWindow();
	//this->m_renwin->SetNumberOfLayers(2);
	//this->m_ren = vtkRenderer::New();
	//this->m_frontRen = vtkRenderer::New();
	//this->m_ren->SetLayer(0);
	//this->m_frontRen->SetLayer(1);
	//this->m_frontRen->SetActiveCamera(this->m_ren->GetActiveCamera());
	////this->m_frontRen->SetBackground(0.5, 0.5, 0.5);
	////this->m_ren->SetBackground(0.5, 0.5, 0.5);
	//this->m_renwin->AddRenderer(this->m_frontRen);
	//this->m_renwin->AddRenderer(this->m_ren);
	//this->m_renwin->GetInteractor()->SetInteractorStyle(vtkInteractorStyleImage::New());
	//this->m_renwin->GetInteractor()->GetInteractorStyle()->SetCurrentRenderer(this->m_ren);
	//this->m_imageActor = vtkImageActor::New();

	///* Create reference for debuging */
	//vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	//axes->SetUserMatrix(vtkMatrix4x4::New());
	//axes->GetUserMatrix()->SetElement(0, 0, 100);
	//axes->GetUserMatrix()->SetElement(1, 1, 100);
	//axes->GetUserMatrix()->SetElement(2, 2, 100);
	//this->m_ren->AddActor(axes);

	/* Create line actors */
/*	this->m_mapper		= vtkPolyDataMapper::New();
	this->m_cutter		= vtkCutter::New();
	this->m_lineActor	= vtkActor::New();
	this->m_cutfunction = vtkPlane::New();
	this->m_transfilter = vtkTransformPolyDataFilter::New();
	this->m_mapper->SetInputConnection(this->m_cutter->GetOutputPort());
	this->m_cutter->SetInputConnection(m_transfilter->GetOutputPort());
	this->m_cutter->SetCutFunction(this->m_cutfunction);
	this->m_transfilter->SetInputConnection(this->planeSource->GetOutputPort());


	this->m_lineActor->SetMapper(this->m_mapper);
	this->m_lineActor->GetProperty()->SetColor(1, 1, 0);
	this->m_lineActor->GetProperty()->ShadingOff();
	this->m_lineActor->GetProperty()->SetLineWidth(2);

*/
}

Yolk3DSeries::~Yolk3DSeries()
{
	///* Clean line actor */
	//if (this->m_frontRen->HasViewProp(this->m_lineActor))
	//{
	//	this->m_frontRen->RemoveActor(this->m_lineActor);
	//}
	//this->m_lineActor->Delete();
	//this->m_mapper->Delete();

	///* Clean dependence to actor */
	//if (this->m_imageActor->GetInput())
	//{
	//	this->m_imageActor->SetInputData(NULL);
	//}

	///* Clean current list */
	////while (this->m_3dimageList.size())
	////{
	////	this->m_3dimageList.last()->Delete();
	////	this->m_3dimageList.erase(this->m_3dimageList.end());
	////}

	//this->m_imageActor->Delete();

	//this->m_imageDirection->Delete();

	delete this->ui;
}

//void Yolk3DSeries::connectViewer(MyImageViewer* viewer)
//{
//	connect(viewer, SIGNAL(SliceChanged(int)), this, SLOT(slotUpdate()), Qt::UniqueConnection);
//}
//
//void Yolk3DSeries::disconnectViewer(MyImageViewer* viewer)
//{
//	disconnect(viewer, SIGNAL(SliceChanged(int)), this, SLOT(slotUpdate()));
//}

void Yolk3DSeries::set3DSeries(QStringList filenames)
{
	/* Clean dependence to actor */
	if (this->m_imageActor->GetInput())
	{
		this->m_imageActor->SetInputData(NULL);
	}

	/* Clean current list */
	while (this->m_3dimageList.size())
	{
		this->m_3dimageList.last()->Delete();
		this->m_3dimageList.erase(this->m_3dimageList.end());
	}

	this->readSeries(filenames);

	this->setSlice(0);
}

void Yolk3DSeries::setSlice(int slice)
{
	if (!this->imageSlice.contains(slice)) {
		return;
	}

	this->imageSlice[slice]->Print(cout);
	this->imageViewer->SetInputData(this->imageSlice[slice]);
	this->imageViewer->Render();
	//assert(this->m_3dimageList.keys().contains(sliceNum), "Slice dose not exist!");

	//if (!this->m_3dimageList.contains(sliceNum))
	//{
	//	qDebug() << "Slice does not exist.";
	//	return;
	//}
	//int s = sliceNum;

	//int* extent = this->m_3dimageList[s]->GetExtent();
	//double* bounds = this->m_3dimageList[s]->GetBounds();
	//int lengths[3];
	//lengths[0] = extent[1] - extent[0];
	//lengths[1] = extent[3] - extent[2];
	//lengths[2] = extent[5] - extent[4];

	//int minDirection = std::min_element(lengths, lengths + 3) - lengths;

	///* Defines initial actor and plane source position and direction */
	//this->m_normalByExtent[0] = 0;
	//this->m_normalByExtent[1] = 0;
	//this->m_normalByExtent[2] = 0;
	//this->m_normalByExtent[3] = 0;
	//this->m_viewUpByExtent[0] = 0;
	//this->m_viewUpByExtent[1] = 0;
	//this->m_viewUpByExtent[2] = 0;
	//this->m_viewUpByExtent[3] = 0;
	//switch (minDirection)
	//{
	//case 0:
	//	this->m_normalByExtent[0] = 1;
	//	this->m_viewUpByExtent[2] = 1;

	//	this->planeSource->SetPoint1(0, bounds[3] - bounds[2], 0);
	//	this->planeSource->SetPoint2(0, 0, bounds[5] - bounds[4]);
	//	break;
	//case 1:
	//	this->m_normalByExtent[1] = 1;
	//	this->m_viewUpByExtent[2] = 1;

	//	this->planeSource->SetPoint1(bounds[1] - bounds[0], 0, 0);
	//	this->planeSource->SetPoint2(0, 0, bounds[5] - bounds[4]);
	//	break;
	//case 2:
	//	this->m_normalByExtent[2] = 1;
	//	this->m_viewUpByExtent[1] = 1;

	//	this->planeSource->SetPoint1(bounds[1] - bounds[0], 0, 0);
	//	this->planeSource->SetPoint2(0 ,bounds[3] - bounds[2], 0);
	//	break;
	//default:
	//	return;
	//	break;
	//}
	//this->planeSource->Update();


	//vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
	//mat->DeepCopy(this->m_matrixList[s]);

	//memcpy(this->m_normalByExtent,mat->MultiplyDoublePoint(this->m_normalByExtent), sizeof(double) * 4);
	//memcpy(this->m_viewUpByExtent, mat->MultiplyDoublePoint(this->m_viewUpByExtent), sizeof(double) * 4);
	//vtkMath::Normalize(this->m_normalByExtent);

	//this->m_imageActor->SetInputData(this->m_3dimageList[s]);
	//this->m_imageActor->SetDisplayExtent(this->m_3dimageList[s]->GetExtent());
	//this->m_imageActor->Update();
	//this->m_imageActor->SetUserMatrix(mat);
	//if (!this->m_ren->HasViewProp(this->m_imageActor))
	//{
	//	this->m_ren->AddActor(this->m_imageActor);
	//}

	/////* Align cutter's plane source transform */
	//if (this->m_transfilter)
	//{
	//	this->m_transfilter->SetTransform(this->m_imageActor->GetUserTransform());
	//}

	////	this->m_cutter->GetCutFunction()->SetTransform(this->m_imageActor->GetUserTransform());

	///* Handle camera */
	//double dist = double(lengths[0] > lengths[1] ? lengths[0] : (lengths[1] > lengths [2] ? lengths[1] : lengths[2]) );
	//double *position = this->m_imageActor->GetCenter();
	//double cameraOffset[3];
	//memcpy(cameraOffset, this->m_normalByExtent, sizeof(double) * 3);
	//vtkMath::MultiplyScalar(cameraOffset, dist * 0.8);
	//vtkMath::Add(position, cameraOffset, cameraOffset);
	//this->m_ren->GetActiveCamera()->SetPosition(cameraOffset);
	//this->m_ren->GetActiveCamera()->SetFocalPoint(position);
	//this->m_ren->GetActiveCamera()->SetViewUp(this->m_viewUpByExtent);

	//this->m_ren->ResetCameraClippingRange();
	//this->m_renwin->Render();
}

void Yolk3DSeries::setImageDirection(vtkMatrix4x4 * direction)
{
	this->imageDirection->DeepCopy(direction);
}

//void Yolk3DSeries::slotUpdate()
//{
//	MyImageViewer *viewer = dynamic_cast<MyImageViewer *>(this->sender());
//	if (NULL != viewer)f
//	{
//		this->updateByViewer();
//	}
//
//	this->m_renwin->Render();
//}

void Yolk3DSeries::on_pushButtonLoad_clicked()
{
	RegistrationWizard rw("D:\\work\\IADE\\T2propeller&MRA\\MIP", 1);
	rw.setImageModalityNames(0, "MIP Image");
	if (rw.exec() == QWizard::Accepted) {
		QStringList fileName = rw.getFileNames(0).split(";");
		fileName.pop_back();

		this->fileNames = fileName;
		//set3DSeries(fileName);

	}

	if (this->fileNames.isEmpty()) {
		qCritical() << "File names is empty.";
		return;
	}
	qDebug() << fileNames.length();
	this->imageSlice.clear();
	for (int i = 0; i < fileNames.length(); ++i) {

		GDCMImageIO::Pointer gdcmIO = GDCMImageIO::New();

		ImageFileReader::Pointer imageFileReader = ImageFileReader::New();
		imageFileReader->SetFileName(fileNames[i].toStdString());
		imageFileReader->SetImageIO(gdcmIO);
		try
		{
			imageFileReader->Update();
		}
		catch (const itk::ExceptionObject& e)
		{
			qWarning() << e.what(); 
			continue;
		}

		OrientImageFilter::Pointer orientImageFilter = OrientImageFilter::New();
		orientImageFilter->SetInput(imageFileReader->GetOutput());
		orientImageFilter->UseImageDirectionOn();
		orientImageFilter->SetDesiredCoordinateOrientation(
			itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
		orientImageFilter->Update();
		
		IVtkImageData::itkImageType::PointType zeroOrigin;
		zeroOrigin.Fill(0);

		IVtkImageData::itkImageType::DirectionType zeroDirection;
		zeroDirection.Fill(0);


		ChangeInformationImageFilter::Pointer changeInformationImageFilter =
			ChangeInformationImageFilter::New();
		changeInformationImageFilter->SetInput(imageFileReader->GetOutput());
		changeInformationImageFilter->ChangeOriginOn();
		changeInformationImageFilter->SetOutputOrigin(zeroOrigin);
		changeInformationImageFilter->SetOutputDirection(zeroDirection);
		changeInformationImageFilter->Update();

		vtkSmartPointer<IVtkImageData> image =
			vtkSmartPointer<IVtkImageData>::New();
		image->Graft(changeInformationImageFilter->GetOutput());

		const IVtkImageData::itkImageType::PointType& origin =
			imageFileReader->GetOutput()->GetOrigin();
		const IVtkImageData::itkImageType::DirectionType& direction =
			imageFileReader->GetOutput()->GetDirection();

		//const IVtkImageData::itkImageType::PointType& origin =
		//	orientImageFilter->GetOutput()->GetOrigin();
		//const IVtkImageData::itkImageType::DirectionType& direction =
		//	orientImageFilter->GetOutput()->GetDirection();

		vtkSmartPointer<vtkMatrix4x4> matrix =
			vtkSmartPointer<vtkMatrix4x4>::New();

		for (vtkIdType i = 0; i < 3; i++)
		{
			for (vtkIdType j = 0; j < 3; j++)
			{
				matrix->SetElement(i, j, direction[i][j]);
			}
		}
		for (vtkIdType i = 0; i < 3; i++)
		{
			matrix->SetElement(i, 3, origin[i]);
		}

		MetaDataDictionary dict = gdcmIO->GetMetaDataDictionary();
		std::string slice;



		if (!itk::ExposeMetaData(dict, "0020|0013", slice)) {
			qWarning() << "a slice is skipped. ";
			continue;
		}
		this->imageSlice.insert(std::stoi(slice), image);
		this->imageMatrixes.insert(std::stoi(slice), matrix);
	}

	on_spinBoxSlice_valueChanged(1);
	this->imageViewer->SetSliceOrientationToXZ();
	this->imageViewer->SetSliceOrientationToXY();
	this->imageViewer->GetImageActor()->VisibilityOn();
	//this->planeActor->VisibilityOn();
	//this->planeActor2->VisibilityOn();
	this->lineActor->VisibilityOn();
	this->imageViewer->GetRenderer()->ResetCamera();
	this->imageViewer->GetRenderer()->ResetCameraClippingRange();
	this->imageViewer->GetRenderer()->Render();

}

void Yolk3DSeries::on_spinBoxSlice_valueChanged(int slice)
{
	if (!this->imageSlice.contains(slice)) {
		return;
	}
	if (!this->imageMatrixes.contains(slice)) {
		return;
	}

	vtkSmartPointer<vtkTransform> tranform = 
		vtkSmartPointer<vtkTransform>::New();
	tranform->SetMatrix(this->imageMatrixes[slice]);
	tranform->Inverse();

	double bounds[6];
	double point1[4] = { 0, 0, 0, 1 };
	double point2[4] = { 0, 0, 0, 1 };
	double origin[4] = { 0, 0, 0, 1 };
	this->imageSlice[slice]->GetBounds(bounds);

	if (this->imageSlice[slice]->GetDimensions()[0] == 1)
	{
		point1[1] = bounds[3] - bounds[2];
		point2[2] = bounds[5] - bounds[4];
	}
	else if (this->imageSlice[slice]->GetDimensions()[1] == 1)
	{
		point1[2] = bounds[5] - bounds[4];
		point2[0] = bounds[1] - bounds[0];
	}
	else
	{
		point1[0] = bounds[1] - bounds[0];
		point2[1] = bounds[3] - bounds[2];
	}

	this->imageMatrixes[slice]->MultiplyPoint(point1, point1);
	this->imageMatrixes[slice]->MultiplyPoint(point2, point2);
	this->imageMatrixes[slice]->MultiplyPoint(origin, origin);
	this->planeSource->SetPoint1(point1);
	this->planeSource->SetPoint2(point2);
	this->planeSource->SetOrigin(origin);
	this->planeSource->Print(cout);
	//this->planeTransform->SetTransform(tranform);

	//double center[4] = { 0, 0, 0, 1 };
	//double normal[4] = { 0, 0, 0, 0 };
	//
	//this->planeSource->GetNormal(normal);
	//cout << "Normal before" << endl;
	//print_vector(cout, normal, 4);
	//cout << endl;
	//this->imageMatrixes[slice]->MultiplyPoint(normal, normal);
	////this->planeSource->SetNormal(normal);
	//cout << "Normal after" << endl;
	//print_vector(cout, normal, 4);
	//cout << endl;


	//this->planeSource->GetCenter(center);
	//cout << "Center before" << endl;
	//print_vector(cout, center, 4);
	//cout << endl;

	////center[0] += this->imageMatrixes[slice]->GetElement(0, 3);
	////center[1] += this->imageMatrixes[slice]->GetElement(1, 3);
	////center[2] += this->imageMatrixes[slice]->GetElement(2, 3);
	//this->imageMatrixes[slice]->MultiplyPoint(center, center);
	////this->planeSource->SetCenter(center);
	//cout << "Center after" << endl;
	//print_vector(cout, center, 4);
	//cout << endl;

	//cout << "Matrix: " << endl;
	//this->imageMatrixes[slice]->Print(cout);

	//tranform->Inverse();
	//this->planeActor->SetUserTransform(tranform);
	this->lineActor->SetUserTransform(tranform);

	this->imageViewer->SetInputData(this->imageSlice[slice]);
	//this->imageViewer->GetImageActor()->SetPosition(position);
	//this->imageViewer->GetImageActor()->SetOrientation(orientation);
	this->imageViewer->Render();
}

void Yolk3DSeries::SetWorldCoordinate(double x, double y, double z, unsigned int i)
{
	double origin[4] = {
		x - this->imageDirection->GetElement(0, 3), 
		y - this->imageDirection->GetElement(1, 3), 
		z - this->imageDirection->GetElement(2, 3), 
		1 };


	double normal[4] = { 0 ,0, 0, 0 };
	normal[i] = 1;
	

	cout << "Before" << endl;
	cout << "Normal: ";
	print_vector(cout, normal, 4);
	cout << "Origin: ";
	print_vector(cout, origin, 4);

	this->imageDirection->MultiplyPoint(origin, origin);
	this->imageDirection->MultiplyPoint(normal, normal);
	this->planeSource2->SetCenter(origin);
	this->planeSource2->SetNormal(normal);
	this->plane->SetOrigin(origin);
	this->plane->SetNormal(normal);


	cout << "After" << endl;
	cout << "Normal: ";
	print_vector(cout, normal, 4);
	cout << "Origin: ";
	print_vector(cout, origin, 4);

	this->imageViewer->Render();
	//

	//if (this->m_cutter->GetCutFunction())
	//{
	//	this->m_cutfunction->SetOrigin(origin);
	//}

	//switch (i)
	//{
	//case 0:
	//	this->m_cutfunction->SetNormal(1, 0, 0);
	//	break;
	//case 1:
	//	this->m_cutfunction->SetNormal(0, 1, 0);
	//	break;
	//case 2:
	//	this->m_cutfunction->SetNormal(0, 0, 1);
	//	break;
	//default:
	//	qCritical() << "sender object viewer has wrong slice orientation!";
	//	return;
	//};

	//if (!this->m_frontRen->HasViewProp(this->m_lineActor))
	//{
	//	drawLineByPlane(this->m_cutfunction->GetNormal(), origin);
	//}
	//this->m_renwin->Render();
}

void Yolk3DSeries::updateBy3DSeries()
{
	this->setSlice(0);
}

void Yolk3DSeries::readSeries(QStringList filenames)
{
	/// Read files
	typedef itk::GDCMImageIO IOType;
	IOType::Pointer gdcmio = IOType::New();


	vtkSmartPointer<vtkDICOMImageReader> reader
		= vtkSmartPointer<vtkDICOMImageReader>::New();

	for (QList<QString>::iterator iter = filenames.begin();
		iter != filenames.end(); iter++)
	{
		vtkImageData* l_im = vtkImageData::New();
		reader->SetFileName(iter->toStdString().c_str());
		reader->Update();
		l_im->DeepCopy(reader->GetOutput());

		gdcmio->SetFileName(iter->toStdString().c_str());
		gdcmio->ReadImageInformation();

		itk::MetaDataDictionary dict = gdcmio->GetMetaDataDictionary();

		/// Check slice number
		std::string tempstr;
		itk::ExposeMetaData<std::string>(dict, "0020|0013", tempstr); // position
		int s = QString::fromStdString(tempstr).toInt();

		this->m_3dimageList[s - 1] = l_im;

		/// Construct transform matrix for image actor ///
		double* matrix = new double[16];

		/* Position */
		itk::ExposeMetaData<std::string>(dict, "0020|0032", tempstr); // position
#ifdef _MSC_VER
		sscanf_s(tempstr.c_str(), "%lf\\%lf\\%lf", matrix + 3, matrix + 7, matrix + 11);
#else
		sscanf(tempstr.c_str(), "%lf\\%lf\\%lf", matrix + 3, matrix + 7, matrix + 11);
#endif

		/* Handle direction part */
		itk::ExposeMetaData<std::string>(dict, "0020|0037", tempstr); // direction
#ifdef _MSC_VER
		sscanf_s(tempstr.c_str(), "%lf\\%lf\\%lf\\%lf\\%lf\\%lf", matrix, matrix + 4, matrix + 8,
																  matrix + 1, matrix + 5, matrix + 9);
#else
		sscanf(tempstr.c_str(), "%lf\\%lf\\%lf\\%lf\\%lf\\%lf", matrix, matrix + 4, matrix + 8,
			matrix + 1, matrix + 5, matrix + 9);
#endif

		int nrow;
		itk::ExposeMetaData<std::string>(dict, "0028|0010", tempstr); // number of rows
#ifdef _MSC_VER
		sscanf_s(tempstr.c_str(), "%d", &nrow);
#else
		sscanf(tempstr.c_str(), "%d", nrow);
#endif
		itk::Vector<double, 3> dx;
		itk::Vector<double, 3> dy;
		itk::Vector<double, 3> dz;
		dx[0] = matrix[0];
		dx[1] = matrix[4];
		dx[2] = matrix[8];
		dy[0] = matrix[1];
		dy[1] = matrix[5];
		dy[2] = matrix[9];
		dz = itk::CrossProduct(dx, dy);
		matrix[2] = dz[0];
		matrix[6] = dz[1];
		matrix[10] = dz[2];
		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1;

		/* Adjust for ITK axes flip. */
		/* >>> IMPORTANT <<< :
		 *	This assume positive-z along axial of original 3D projection series, so that the difference
		 *  is only a z-flip at the position of the image!
		 */
		matrix[1] = -matrix[1];
		matrix[5] = -matrix[5];
		matrix[9] = -matrix[9];

		/* Adjust for DICOM y directional offset due to upper axis flip */
		dy.Normalize();
		dy *= nrow * l_im->GetSpacing()[1];
		matrix[3] += dy[0];
		matrix[7] += dy[1];
		matrix[11] += dy[2];

		/// Push to matrix list
		this->m_matrixList[s - 1] = matrix;
	}
}


void Yolk3DSeries::drawLineByPlane(const double* normal, const double* pos)
{
	///* Create a plane */
	//double* bounds = this->m_imageActor->GetBounds();

	////this->m_transfilter->SetTransform(this->m_imageActor->GetUserTransform());
	////this->m_transfilter->Update();

	//this->m_cutter->SetCutFunction(this->m_cutfunction);
	//this->m_cutter->Update();

	//this->m_mapper->Update();

	//if (!this->m_frontRen->HasViewProp(this->m_lineActor))
	//{
	//	this->m_frontRen->AddActor(this->m_lineActor);
	//}
}

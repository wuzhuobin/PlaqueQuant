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
#include <itkChangeInformationImageFilter.h>
typedef itk::ChangeInformationImageFilter<IVtkImageData::itkImageType>
ChangeInformationImageFilter;
#include <itkGDCMImageIO.h>
using itk::GDCMImageIO;
#include <itkMetaDataDictionary.h>
using itk::MetaDataDictionary;
#include <itkMetaDataObject.h>


// vtk
#include <vtkPassArrays.h>
//#include "vtkLinearTransform.h"
#include "vtkProperty.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCutter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPlane.h>
#include <vtkPlaneSource.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
//#include <vtkTransformPolyDataFilter.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
//#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkImageViewer2.h>
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

	//vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = 
	//	vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	//this->ui->widgetViewer->GetInteractor()->SetInteractorStyle(style);
	this->imageViewer->GetImageActor()->VisibilityOff();

	this->planeSource = vtkSmartPointer<vtkPlaneSource>::New();
	this->planeSource->SetOrigin(0, 0, 0);
	this->planeSource->SetCenter(0, 0, 0);
	this->planeSource->SetNormal(0, 0, 1);

	//vtkSmartPointer<vtkPolyDataMapper> planeMapper =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//planeMapper->SetInputConnection(this->planeSource->GetOutputPort());
	//planeMapper->ScalarVisibilityOff();

	//this->planeActor = vtkSmartPointer<vtkActor>::New();
	//this->planeActor->SetMapper(planeMapper);
	//this->planeActor->VisibilityOff();
	//this->planeActor->GetProperty()->SetColor(1, 1, 0);

	//this->imageViewer->GetRenderer()->AddActor(planeActor);

	//this->planeSource2 = vtkSmartPointer<vtkPlaneSource>::New();
	//this->planeSource2->SetOrigin(0, 0, 0);
	//this->planeSource2->SetPoint1(1000, 0, 0);
	//this->planeSource2->SetPoint2(0, 1000, 0);

	this->imageDirection = vtkSmartPointer<vtkMatrix4x4>::New();
	this->imageDirection->Identity();

	//vtkSmartPointer<vtkTransform> transform2 =
	//	vtkSmartPointer<vtkTransform>::New();
	//transform2->SetMatrix(this->imageDirection);

/*	this->planeTransform2 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	this->planeTransform2->SetInputConnection(this->planeSource2->GetOutputPort());
	this->planeTransform2->SetTransform(transform2);

	vtkSmartPointer<vtkPolyDataMapper> planeMapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	planeMapper2->SetInputConnection(this->planeTransform2->GetOutputPort());
	planeMapper2->ScalarVisibilityOff();

	this->planeActor2 = vtkSmartPointer<vtkActor>::New();
	this->planeActor2->SetMapper(planeMapper2);
	this->planeActor2->VisibilityOff();
	this->planeActor2->GetProperty()->SetColor(0, 1, 0);

	this->imageViewer->GetRenderer()->AddActor(planeActor2)*/;


	this->plane = vtkSmartPointer<vtkPlane>::New();
	
	this->cutter = vtkSmartPointer<vtkCutter>::New();
	this->cutter->SetCutFunction(this->plane);
	this->cutter->SetInputConnection(this->planeSource->GetOutputPort());
	this->cutter->GenerateCutScalarsOff();

	vtkSmartPointer<vtkPassArrays> passArrays =
		vtkSmartPointer<vtkPassArrays>::New();
	passArrays->SetInputConnection(this->cutter->GetOutputPort());
	passArrays->UseFieldTypesOn();
	passArrays->AddFieldType(vtkDataObject::POINT);
	passArrays->AddFieldType(vtkDataObject::CELL);
	passArrays->AddFieldType(vtkDataObject::FIELD);
	passArrays->ClearArrays();

	vtkSmartPointer<vtkPolyDataMapper> lineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	lineMapper->SetInputConnection(passArrays->GetOutputPort());
	lineMapper->ScalarVisibilityOff();
	
	this->lineActor = vtkSmartPointer<vtkActor>::New();
	this->lineActor->SetMapper(lineMapper);
	this->lineActor->GetProperty()->SetColor(1, 0, 0);
	this->lineActor->VisibilityOff();

	//vtkSmartPointer<vtkRenderer> renderer =
	//	vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(this->lineActor);
	//renderer->SetLayer(1);
	//renderer->SetActiveCamera(this->imageViewer->GetRenderer()->GetActiveCamera());

	//this->imageViewer->GetRenderWindow()->SetNumberOfLayers(2);
	//this->imageViewer->GetRenderer()->SetLayer(0);
	//this->imageViewer->GetRenderWindow()->AddRenderer(renderer);
	this->imageViewer->GetRenderer()->AddActor(this->lineActor);
}

Yolk3DSeries::~Yolk3DSeries()
{
	delete this->ui;
}

void Yolk3DSeries::setImageDirection(vtkMatrix4x4 * direction)
{
	this->imageDirection->DeepCopy(direction);
}



void Yolk3DSeries::on_pushButtonLoad_clicked()
{
	RegistrationWizard rw(1);
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

	this->ui->spinBoxSlice->setMinimum(this->imageSlice.keys().last());
	this->ui->spinBoxSlice->setMaximum(this->imageSlice.keys().first());

	this->ui->spinBoxSlice->setValue(1);
	this->imageViewer->SetSliceOrientationToXZ();
	this->imageViewer->SetSliceOrientationToXY();
	double* range = this->imageViewer->GetInput()->GetScalarRange();
	this->imageViewer->SetColorLevel((range[1] + range[0])*0.5);
	this->imageViewer->SetColorWindow(range[1] - range[0]);
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
	//this->planeSource->Print(cout);

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
	

	//cout << "Before" << endl;
	//cout << "Normal: ";
	//print_vector(cout, normal, 4);
	//cout << "Origin: ";
	//print_vector(cout, origin, 4);

	this->imageDirection->MultiplyPoint(origin, origin);
	this->imageDirection->MultiplyPoint(normal, normal);
	//this->planeSource2->SetCenter(origin);
	//this->planeSource2->SetNormal(normal);
	this->plane->SetOrigin(origin);
	this->plane->SetNormal(normal);


	//cout << "After" << endl;
	//cout << "Normal: ";
	//print_vector(cout, normal, 4);
	//cout << "Origin: ";
	//print_vector(cout, origin, 4);

	this->imageViewer->Render();

}

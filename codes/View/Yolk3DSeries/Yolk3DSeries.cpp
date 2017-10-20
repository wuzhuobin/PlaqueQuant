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
#include <vtkCommand.h>
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

class Yolk3DSeriesCommand : public vtkCommand
{
public:
	static Yolk3DSeriesCommand* New() { return new Yolk3DSeriesCommand; }
	vtkTypeMacro(Yolk3DSeriesCommand, vtkCommand);
	Yolk3DSeries* Self = nullptr;
	virtual void Execute(vtkObject *caller, unsigned long eventId,
		void *callData) VTK_OVERRIDE {

		this->AbortFlagOn();
		int slice = this->Self->getUi()->spinBoxSlice->value();
		switch (eventId)
		{
		case vtkCommand::MouseWheelBackwardEvent:
			this->Self->getUi()->spinBoxSlice->setValue(--slice);
			break;
		case vtkCommand::MouseWheelForwardEvent:
			this->Self->getUi()->spinBoxSlice->setValue(++slice);
			break;
		default:
			break;
		}
	}


};

Yolk3DSeries::Yolk3DSeries(QWidget* parent /*= nullptr*/)
	:QWidget(parent)
{
	this->ui = new Ui::Yolk3DSeries;
	this->ui->setupUi(this);
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	/* Create UI */
	this->setMinimumSize(800, 600);

	Yolk3DSeriesCommand* command = Yolk3DSeriesCommand::New();
	command->Self = this;


	this->imageViewer = vtkSmartPointer<Yolk3DSeriesViewer>::New();
	this->imageViewer->SetRenderWindow(this->ui->widgetViewer->GetRenderWindow());
	this->imageViewer->SetupInteractor(this->ui->widgetViewer->GetInteractor());
	this->ui->widgetViewer->GetInteractor()->AddObserver(vtkCommand::MouseWheelBackwardEvent, command, 1);
	this->ui->widgetViewer->GetInteractor()->AddObserver(vtkCommand::MouseWheelForwardEvent, command, 1);
	command->Delete();

	//vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
	//	vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	//this->ui->widgetViewer->GetInteractor()->SetInteractorStyle(style);
	this->imageViewer->GetImageActor()->VisibilityOff();

	this->planeSource = vtkSmartPointer<vtkPlaneSource>::New();
	this->planeSource->SetOrigin(0, 0, 0);
	this->planeSource->SetCenter(0, 0, 0);
	this->planeSource->SetNormal(0, 0, 1);

	vtkSmartPointer<vtkPolyDataMapper> planeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	planeMapper->SetInputConnection(this->planeSource->GetOutputPort());
	planeMapper->ScalarVisibilityOff();

	//this->planeActor = vtkSmartPointer<vtkActor>::New();
	//this->planeActor->SetMapper(planeMapper);
	////this->planeActor->VisibilityOff();
	//this->planeActor->GetProperty()->SetColor(1, 1, 0);

	//this->imageViewer->GetRenderer()->AddActor(planeActor);

	//this->planeSource2 = vtkSmartPointer<vtkPlaneSource>::New();
	//this->planeSource2->SetOrigin(0, 0, 0);
	//this->planeSource2->SetPoint1(1000, 0, 0);
	//this->planeSource2->SetPoint2(0, 1000, 0);

	this->imageDirection = vtkSmartPointer<vtkMatrix4x4>::New();
	this->imageDirection->Identity();

	this->inverseImageDirection = vtkSmartPointer<vtkMatrix4x4>::New();
	this->inverseImageDirection->Identity();

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


	for (int i = 0; i < 3; i++)
	{
		this->plane[i] = vtkSmartPointer<vtkPlane>::New();

		this->cutter[i] = vtkSmartPointer<vtkCutter>::New();
		this->cutter[i]->SetCutFunction(this->plane[i]);
		this->cutter[i]->SetInputConnection(this->planeSource->GetOutputPort());
		this->cutter[i]->GenerateCutScalarsOff();

		this->passArrays[i] =
			vtkSmartPointer<vtkPassArrays>::New();
		this->passArrays[i]->SetInputConnection(this->cutter[i]->GetOutputPort());
		this->passArrays[i]->UseFieldTypesOn();
		this->passArrays[i]->AddFieldType(vtkDataObject::POINT);
		this->passArrays[i]->AddFieldType(vtkDataObject::CELL);
		this->passArrays[i]->AddFieldType(vtkDataObject::FIELD);
		this->passArrays[i]->ClearArrays();
	}

	this->lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	this->lineMapper->SetInputConnection(passArrays[0]->GetOutputPort());
	this->lineMapper->ScalarVisibilityOff();

	this->lineActor = vtkSmartPointer<vtkActor>::New();
	this->lineActor->SetMapper(this->lineMapper);
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
	this->inverseImageDirection->DeepCopy(direction);
	//this->inverseImageDirection->Invert();
	vtkTransform* trans = vtkTransform::New();
	cout << "imageDirection" << endl;
	this->imageDirection->Print(cout);
	cout << "beforce" << endl;
	cout << "inverseImageDirection" << endl;
	this->inverseImageDirection->Print(cout);
	trans->SetMatrix(this->imageDirection);
	trans->PostMultiply();
	trans->Inverse();
	trans->Translate(
		this->imageDirection->GetElement(0, 3),
		this->imageDirection->GetElement(1, 3),
		this->imageDirection->GetElement(2, 3) );
	trans->GetMatrix(this->inverseImageDirection);
	cout << "after" << endl;
	cout << "inverseImageDirection" << endl;
	this->inverseImageDirection->Print(cout);
	//this->inverseImageDirection->SetElement(0, 3, 0);
	//this->inverseImageDirection->SetElement(1, 3, 0);
	//this->inverseImageDirection->SetElement(2, 3, 0);

}
vtkMatrix4x4 * Yolk3DSeries::getImageDirection()
{
	return this->imageDirection;
}

vtkMatrix4x4 * Yolk3DSeries::getInverseImageDirection()
{
	return this->inverseImageDirection;
}

//
//void Yolk3DSeries::setImageDirection(vtkMatrix4x4 * direction)
//{
//	this->imageDirection->DeepCopy(direction);
//}



void Yolk3DSeries::on_pushButtonLoad_clicked()
{
	RegistrationWizard rw("", 1);
	QStringList fileNames;
	rw.setImageModalityNames(0, "MIP Image");
	if (rw.exec() == QWizard::Accepted) {
		QStringList fileName = rw.getFileNames(0).split(";");
		fileName.pop_back();

		fileNames = fileName;
		//set3DSeries(fileName);

	}

	if (fileNames.isEmpty()) {
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

	QList<int> allSlices = this->imageSlice.keys();
	std::pair<QList<int>::const_iterator, QList<int>::const_iterator> sliceRange = std::minmax_element(allSlices.cbegin(), allSlices.cend());

	this->ui->spinBoxSlice->setMinimum(*(sliceRange.first));
	this->ui->spinBoxSlice->setMaximum(*(sliceRange.second));

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
	tranform->PostMultiply();
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


void Yolk3DSeries::slotClose()
{
	this->close();
}

vtkAlgorithmOutput * Yolk3DSeries::getLineOutput(int i)
{
	return this->passArrays[i]->GetOutputPort();
}

//vtkPolyDataMapper * Yolk3DSeries::getLineMapper()
//{
//	return this->lineMapper;
//}

void Yolk3DSeries::setImageCoordinate(int x, int y, int z, unsigned int direction)
{



	for (int i = 0; i < 3; i++)
	{
		double normal[4] = { 0 ,0, 0, 0 };
		double origin[4] = {
		x - this->imageDirection->GetElement(0, 3),
		y - this->imageDirection->GetElement(1, 3),
		z - this->imageDirection->GetElement(2, 3),
		1 };
		normal[i] = 1;

		this->imageDirection->MultiplyPoint(origin, origin);
		this->imageDirection->MultiplyPoint(normal, normal);

		this->plane[i]->SetOrigin(origin);
		this->plane[i]->SetNormal(normal);
	}

	this->lineMapper->SetInputConnection(this->passArrays[direction]->GetOutputPort());
	//cout << "Before" << endl;
	//cout << "Normal: ";
	//print_vector(cout, normal, 4);
	//cout << "Origin: ";
	//print_vector(cout, origin, 4);
	//this->planeSource2->SetCenter(origin);
	//this->planeSource2->SetNormal(normal);
	//this->plane->SetOrigin(origin);
	//this->plane->SetNormal(normal);


	//cout << "After" << endl;
	//cout << "Normal: ";
	//print_vector(cout, normal, 4);
	//cout << "Origin: ";
	//print_vector(cout, origin, 4);

	this->imageViewer->Render();

}

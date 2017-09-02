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
#include "Yolk3DSeries.h"

#include <QGridLayout>
#include <QStringList>
#include <QStringListIterator>
#include <QVTKWidget2.h>
#include <QDebug>

#include <itkVector.h>
#include <itkImageFileReader.h>
#include <itkGDCMImageIO.h>
#include <itkMetaDataDictionary.h>

#include "vtkInteractorStyleImage.h"
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

Yolk3DSeries::Yolk3DSeries(QObject* parent /*= nullptr*/)
{
	/* Create UI */
	this->setMinimumSize(800, 600);
	this->m_qvtkWidget		= new QVTKWidget2(this);
	QGridLayout* layout		= new QGridLayout(this);
	layout->addWidget(this->m_qvtkWidget);


	/* Create renderers */
	this->m_renwin	= this->m_qvtkWidget->GetRenderWindow();
	this->m_renwin->SetNumberOfLayers(2);
	this->m_ren = vtkRenderer::New();
	this->m_frontRen = vtkRenderer::New();
	this->m_ren->SetLayer(0);
	this->m_frontRen->SetLayer(1);
	this->m_frontRen->SetActiveCamera(this->m_ren->GetActiveCamera());
	//this->m_frontRen->SetBackground(0.5, 0.5, 0.5);
	//this->m_ren->SetBackground(0.5, 0.5, 0.5);
	this->m_renwin->AddRenderer(this->m_frontRen);
	this->m_renwin->AddRenderer(this->m_ren);
	this->m_renwin->GetInteractor()->SetInteractorStyle(vtkInteractorStyleImage::New());
	this->m_renwin->GetInteractor()->GetInteractorStyle()->SetCurrentRenderer(this->m_ren);
	this->m_imageActor = vtkImageActor::New();

	///* Create reference for debuging */
	//vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	//axes->SetUserMatrix(vtkMatrix4x4::New());
	//axes->GetUserMatrix()->SetElement(0, 0, 100);
	//axes->GetUserMatrix()->SetElement(1, 1, 100);
	//axes->GetUserMatrix()->SetElement(2, 2, 100);
	//this->m_ren->AddActor(axes);

	/* Create line actors */
	this->m_cutter		= vtkCutter::New();
	this->m_lineActor	= vtkActor::New();
	this->m_planeSource = vtkPlaneSource::New();
	this->m_transfilter = vtkTransformPolyDataFilter::New();

	this->m_mapper		= vtkPolyDataMapper::New();
	this->m_lineActor->SetMapper(this->m_mapper);
	this->m_lineActor->GetProperty()->SetColor(1, 1, 0);
	this->m_lineActor->GetProperty()->ShadingOff();
	this->m_lineActor->GetProperty()->SetLineWidth(2);
}

Yolk3DSeries::~Yolk3DSeries()
{
	/* Clean line actor */
	if (this->m_frontRen->HasViewProp(this->m_lineActor))
	{
		this->m_frontRen->RemoveActor(this->m_lineActor);
	}
	this->m_lineActor->Delete();
	this->m_mapper->Delete();

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

	this->m_imageActor->Delete();
}

void Yolk3DSeries::connectViewer(MyImageViewer* viewer)
{
	connect(viewer, SIGNAL(FocalPointWithImageCoordinateChanged(int, int, int)), this, SLOT(slotUpdate()), Qt::UniqueConnection);
}

void Yolk3DSeries::disconnectViewer(MyImageViewer* viewer)
{
	disconnect(viewer, SIGNAL(FocalPointWithImageCoordinateChanged(int, int, in)), this, SLOT(slotUpdate()));
}

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

void Yolk3DSeries::setSlice(int sliceNum)
{
	assert(this->m_3dimageList.keys().contains(sliceNum), "Slice dose not exist!");

	int s = sliceNum;

	int* extent = this->m_3dimageList[s]->GetExtent();
	double* bounds = this->m_3dimageList[s]->GetBounds();
	int lengths[3];
	lengths[0] = extent[1] - extent[0];
	lengths[1] = extent[3] - extent[2];
	lengths[2] = extent[5] - extent[4];

	int minDirection = std::min_element(lengths, lengths + 3) - lengths;

	/* Defines initial actor and plane source position and direction */
	this->m_normalByExtent[0] = 0;
	this->m_normalByExtent[1] = 0;
	this->m_normalByExtent[2] = 0;
	this->m_normalByExtent[3] = 0;
	this->m_viewUpByExtent[0] = 0;
	this->m_viewUpByExtent[1] = 0;
	this->m_viewUpByExtent[2] = 0;
	this->m_viewUpByExtent[3] = 0;
	switch (minDirection)
	{
	case 0:
		this->m_normalByExtent[0] = 1;
		this->m_viewUpByExtent[2] = 1;

		this->m_planeSource->SetPoint1(0, bounds[3] - bounds[2], 0);
		this->m_planeSource->SetPoint2(0, 0, bounds[5] - bounds[4]);
		break;
	case 1:
		this->m_normalByExtent[1] = 1;
		this->m_viewUpByExtent[2] = 1;

		this->m_planeSource->SetPoint1(bounds[1] - bounds[0], 0, 0);
		this->m_planeSource->SetPoint2(0, 0, bounds[5] - bounds[4]);
		break;
	case 2:
		this->m_normalByExtent[2] = 1;
		this->m_viewUpByExtent[1] = 1;

		this->m_planeSource->SetPoint1(bounds[1] - bounds[0], 0, 0);
		this->m_planeSource->SetPoint2(0 ,bounds[3] - bounds[2], 0);
		break;
	default:
		return;
		break;
	}
	this->m_planeSource->Update();


	vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
	mat->DeepCopy(this->m_matrixList[s]);

	memcpy(this->m_normalByExtent,mat->MultiplyDoublePoint(this->m_normalByExtent), sizeof(double) * 4);
	memcpy(this->m_viewUpByExtent, mat->MultiplyDoublePoint(this->m_viewUpByExtent), sizeof(double) * 4);
	vtkMath::Normalize(this->m_normalByExtent);

	this->m_imageActor->SetInputData(this->m_3dimageList[s]);
	this->m_imageActor->SetDisplayExtent(this->m_3dimageList[s]->GetExtent());
	this->m_imageActor->Update();
	this->m_imageActor->SetUserMatrix(mat);
	if (!this->m_ren->HasViewProp(this->m_imageActor))
	{
		this->m_ren->AddActor(this->m_imageActor);
	}

	///* Align cutter's plane source transform */
	if (this->m_transfilter)
		this->m_transfilter->SetTransform(this->m_imageActor->GetUserTransform());
	//	this->m_cutter->GetCutFunction()->SetTransform(this->m_imageActor->GetUserTransform());

	/* Handle camera */
	double dist = double(lengths[0] > lengths[1] ? lengths[0] : (lengths[1] > lengths [2] ? lengths[1] : lengths[2]) );
	double *center = this->m_imageActor->GetCenter();
	double cameraOffset[3];
	memcpy(cameraOffset, this->m_normalByExtent, sizeof(double) * 3);
	vtkMath::MultiplyScalar(cameraOffset, dist * 0.8);
	vtkMath::Add(center, cameraOffset, cameraOffset);
	this->m_ren->GetActiveCamera()->SetPosition(cameraOffset);
	this->m_ren->GetActiveCamera()->SetFocalPoint(center);
	this->m_ren->GetActiveCamera()->SetViewUp(this->m_viewUpByExtent);

	this->m_ren->ResetCameraClippingRange();
	this->m_renwin->Render();
}

void Yolk3DSeries::slotUpdate()
{
	MyImageViewer *viewer = dynamic_cast<MyImageViewer *>(this->sender());
	if (NULL != viewer)
	{
		this->updateByViewer();
	}
}

void Yolk3DSeries::slotSetSlice(int s)
{
	this->setSlice(s);
}

void Yolk3DSeries::updateByViewer()
{
	MyImageViewer* viewer = static_cast<MyImageViewer*>(this->sender());
	double* coord = viewer->GetFocalPointWithWorldCoordinate();
	if (this->m_cutter->GetCutFunction())
	{
		vtkPlane::SafeDownCast(this->m_cutter->GetCutFunction())->SetOrigin(coord);
	}

	switch (viewer->GetSliceOrientation())
	{
	case 0:
		vtkPlane::SafeDownCast(this->m_cutter->GetCutFunction())->SetNormal(1, 0, 0);
		break;
	case 1:
		vtkPlane::SafeDownCast(this->m_cutter->GetCutFunction())->SetNormal(0, 1, 0);
		break;
	case 2:
		vtkPlane::SafeDownCast(this->m_cutter->GetCutFunction())->SetNormal(0, 0, 1);
		break;
	default:
		qCritical() << "Sender object viewer has wrong slice orientation!";
		return;
	};
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

		/// Push to matrix list
		this->m_matrixList[s - 1] = matrix;
	}
}

void Yolk3DSeries::drawLineByPlane(vtkPlane* plane)
{
	/* Create a plane */
	double* bounds = this->m_imageActor->GetBounds();

	this->m_transfilter->SetInputConnection(this->m_planeSource->GetOutputPort());
	this->m_transfilter->SetTransform(this->m_imageActor->GetUserTransform());
	this->m_transfilter->Update();

	this->m_cutter->SetInputConnection(m_transfilter->GetOutputPort());
	this->m_cutter->SetCutFunction(plane);
	this->m_cutter->Update();

	this->m_mapper->SetInputConnection(this->m_cutter->GetOutputPort());
	this->m_mapper->Update();

	if (!this->m_frontRen->HasViewProp(this->m_lineActor))
	{
		this->m_frontRen->AddActor(this->m_lineActor);
	}
}

void Yolk3DSeries::drawLineByPlane(const double* normal, const double* pos)
{
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetNormal(normal[0], normal[1], normal[2]);
	plane->SetOrigin(pos[0], pos[1], pos[2]);

	this->drawLineByPlane(plane);
}

#include "Overlay.h"

#include <itkVTKImageToImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkOrientImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <itkImageSeriesReader.h>
#include <itkImageDuplicator.h>

#include <vtkImageMagnitude.h>
#include <vtkImageCast.h>
#include <vtkSimpleImageToImageFilter.h>
#include <vtkExtractVOI.h>

#include "MeasurementFor3D.h"
#include "MeasurementFor2D.h"
#include "MaximumWallThickness.h"

typedef itk::OrientImageFilter<ImageType, ImageType> OrienterType;
typedef itk::ImageToVTKImageFilter<ImageType>		ConnectorType;
typedef itk::ImageSeriesReader< ImageType >			ReaderType;
typedef itk::ImageDuplicator<ImageType>				DuplicatorType;


Overlay::Overlay(QObject* parent) : QObject(parent)
{
	m_itkOverlay = NULL;
	m_vtkOverlay = NULL;

	// using m_overlayColor to build the lookupTable
	m_overlayColor[0] = zeros;
	m_overlayColor[1] = lumen;
	m_overlayColor[2] = vesselWall;
	m_overlayColor[3] = calcification;
	m_overlayColor[4] = hemorrhage;
	m_overlayColor[5] = lrnc;
	m_overlayColor[6] = lm;

	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_lookupTable->SetNumberOfTableValues(7);
	m_lookupTable->SetTableRange(0.0, 6);
	for (int i = 0; i < 7; ++i) {
		double rgba[4];
		for (int j = 0; j < 4; ++j) {
			rgba[j] = m_overlayColor[i][j] / 255;
		}
		m_lookupTable->SetTableValue(i, rgba);
	}
	m_lookupTable->Build();
}

Overlay::~Overlay()
{
}

void Overlay::SetInputImageData(ImageType::Pointer imageData)
{
	//re-orient
	OrienterType::Pointer orienter = OrienterType::New();
	orienter->UseImageDirectionOn();
	orienter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orienter->SetInput(imageData);
	orienter->Update();

	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(orienter->GetOutput());
	connector->Update();

	SetInputImageData(connector->GetOutput());
}

void Overlay::SetInputImageData(vtkImageData* imageData)
{
	if (imageData->GetScalarType() == VTK_DOUBLE) {
		m_vtkOverlay = imageData;
	}
	else {
		vtkSmartPointer<vtkImageCast> imageCast =
			vtkSmartPointer<vtkImageCast>::New();
		imageCast->SetOutputScalarTypeToDouble();
		imageCast->SetInputData(imageData);
		imageCast->Update();
		m_vtkOverlay = imageCast->GetOutput();
	}
	Measure2D();
	emit signalOverlayUpdated();
}


void Overlay::SetInputImageData(QString fileName)
{
	SetInputImageData(fileName.toStdString().c_str());
}

void Overlay::SetInputImageData(const char* fileName)
{
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(fileName);
	reader->Update();
	SetInputImageData(reader->GetOutput());

}

void Overlay::Initialize(ImageType::Pointer itkinputimage, int dim[3], double spacing[3], double origin[3], int type)
{
	DuplicatorType::Pointer duplicator = DuplicatorType::New();
	duplicator->SetInputImage(itkinputimage);
	try
	{
		duplicator->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		return; // Since the goal of the example is to catch the exception, we declare this a success.
	}
	ImageType::Pointer m_itkOverlay = duplicator->GetOutput();

	itk::ImageRegionIterator<ImageType> itr(m_itkOverlay, m_itkOverlay->GetLargestPossibleRegion());

	//Set all pixel to zero
	while (!itr.IsAtEnd())
	{
		itr.Set(0);
		++itr;
	}

	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(m_itkOverlay);
	try
	{
		connector->Update();
	}
	catch (itk::ExceptionObject &err)
	{
		std::cerr << err << std::endl;
	}

	m_vtkOverlay->DeepCopy(connector->GetOutput());

}

void Overlay::SetDisplayExtent(int *extent)
{
	memcpy(this->DisplayExtent, extent, sizeof(int) * 6);
}

int * Overlay::GetDisplayExtent()
{
	return this->DisplayExtent;
}

void Overlay::GetDisplayExtent(int *extent)
{
	memcpy(extent, this->DisplayExtent, sizeof(int) * 6);
}

void Overlay::DisplayExtentOn()
{
	
}

void Overlay::DisplayExtentOff()
{

}

void Overlay::SetPixel(int pos[3], double value)
{
		double * pixel = static_cast<double *>(m_vtkOverlay->GetScalarPointer(pos[0], pos[1], pos[2]));
		if (pixel == nullptr) {
			return;
		}
		//if (value != 0) {
		//	cout << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
		//}
		pixel[0] = value;
		//cout << pixel[0] << endl;
}

void Overlay::SetPixel(int pos[3], unsigned char value)
{
	unsigned char * pixel = static_cast<uchar *>(m_vtkOverlay->GetScalarPointer(pos[0], pos[1], pos[2]));
	if (pixel == nullptr)
		return;
	pixel[0] = value;
	//cout << pixel[0] << endl;
}
bool Overlay::Update()
{
	if (!m_vtkOverlay)
		return EXIT_FAILURE;
	else
		return EXIT_SUCCESS;
}

void Overlay::Initialize(vtkImageData * img)
{
	m_vtkOverlay = vtkSmartPointer<vtkImageData>::New();
	m_vtkOverlay->SetExtent(img->GetExtent());
	m_vtkOverlay->SetOrigin(img->GetOrigin());
	m_vtkOverlay->SetSpacing(img->GetSpacing());
	m_vtkOverlay->AllocateScalars(img->GetScalarType(), img->GetNumberOfScalarComponents());
	SetInputImageData(m_vtkOverlay);
	//m_vtkOverlay->DeepCopy(img);
	//
	//vtkSmartPointer<vtkImageCast> castfilter = vtkSmartPointer<vtkImageCast>::New();
	//castfilter->SetInputData(m_vtkOverlay);
	//castfilter->SetOutputScalarTypeToDouble();
	//castfilter->Update();

	//m_vtkOverlay = castfilter->GetOutput();

	//const int* extent = m_vtkOverlay->GetExtent();


	//for (int i = extent[0]; i <= extent[1]; i++)
	//{
	//	for (int j = extent[2]; j <= extent[3]; j++)
	//	{
	//		for (int k = extent[4]; k <= extent[5]; k++)
	//		{
	//			double* pixel = static_cast<double*>(this->m_vtkOverlay->GetScalarPointer(i, j, k));
	//			*pixel = 0;
	//		}
	//	}
	//}

	this->m_vtkOverlay->GetExtent(this->DisplayExtent);
}

void Overlay::SetPixels(int* extent, vtkImageData* image)
{
	for (int orientation = 0; orientation < 3; ++orientation) {
		// find correct orientation
		if (extent[orientation*2] == extent[orientation*2 + 1]) {
			for (int x = extent[0]; x <= extent[1]; ++x) {
				for (int y = extent[2]; y <= extent[3]; ++y) {
					for (int z = extent[4]; z <= extent[5]; ++z) {
						int pos[3] = { x,y,z };
						double pixelval = 0;
						int _tmp = pos[orientation];
						pos[orientation] = 0;
						unsigned char* val = static_cast<unsigned char*>(
							image->GetScalarPointer(pos));
						if (val == nullptr)
							continue;
						if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0) {
							for (int i = 0; i < m_lookupTable->GetNumberOfColors(); ++i) {
								double rgba[4];
								uchar rgbaUCHAR[4];
								m_lookupTable->GetIndexedColor(i, rgba);
								m_lookupTable->GetColorAsUnsignedChars(rgba, rgbaUCHAR); // convert double to uchar

								if (val[0] == rgbaUCHAR[0] && val[1] == rgbaUCHAR[1] && 
									val[2] == rgbaUCHAR[2]) {
									pixelval = i;
									break;
								}

							}

						}
						pos[orientation] = _tmp;
						SetPixel(pos, pixelval);
					}
				}
			}
			break;
		}
	}
	m_vtkOverlay->Modified();
	emit signalOverlayUpdated();
}

void Overlay::Measure3D()
{
	if (m_3DMeasurements != nullptr) {
		delete m_3DMeasurements;
	}
	m_3DMeasurements = new double[m_lookupTable->GetNumberOfColors()];

	MeasurementFor3D m_measurementFor3D;
	m_measurementFor3D.SetInputData(m_vtkOverlay);
	m_measurementFor3D.SetLookupTable(m_lookupTable);
	m_measurementFor3D.Update();
	m_measurementFor3D.GetVolumes(m_3DMeasurements);

	
}

void Overlay::Measure2D()
{
	m_2DMeasurements.clear();

	const int* extent = m_vtkOverlay->GetExtent();
	for (int z = extent[4]; z <= extent[5]; ++z) {
		Measure2D(z);
	}
}

void Overlay::Measure2D(int slice)
{
	QStringList _2DMeasurements;
	_2DMeasurements += "Undefined";
	_2DMeasurements += "Undefined";
	_2DMeasurements += "Undefined";
	_2DMeasurements += "Undefined";
	_2DMeasurements += "Normal";
	// to ensure it won't be out of index
	while (slice >= m_2DMeasurements.size()) {
		m_2DMeasurements += _2DMeasurements;
	}

	const int* extent = m_vtkOverlay->GetExtent();
	// Extract slice image
	vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	voi->SetInputData(m_vtkOverlay);
	voi->SetVOI(extent[0], extent[1], extent[2], extent[3], slice, slice);
	voi->Update();
	slice = slice - extent[4];
	// Calculate areas
	vtkSmartPointer<MeasurementFor2D> m2d = vtkSmartPointer<MeasurementFor2D>::New();
	m2d->SetSliceImage(voi->GetOutput());

	// Error checking
	//double lumenArea, vesselArea, NMI, distance; // #todo: Allow multiple branch
	try {
		m2d->Update();
		_2DMeasurements[0] = QString::number(m2d->GetOutput(0).LumenArea);
		_2DMeasurements[1] = QString::number(m2d->GetOutput(0).VesselWallArea);
		_2DMeasurements[3] = QString::number(
			m2d->GetOutput(0).VesselWallArea /
			(m2d->GetOutput(0).LumenArea + m2d->GetOutput(0).VesselWallArea));
	}
	catch (MeasurementFor2D::ERROR_CODE e) {
		cerr << "MeasurementFor2D error: " << e << endl;
	}

	// calculate wall thickness
	vtkSmartPointer<MaximumWallThickness> mwt =
		vtkSmartPointer<MaximumWallThickness>::New();
	mwt->SetLumemIntensity(1);
	mwt->SetVesselIntensity(2);
	mwt->SetSliceImage(voi->GetOutput());
	try {
		mwt->Update();
		_2DMeasurements[2] = QString::number(mwt->GetDistanceLoopPairVect().at(0).Distance);
	}
	catch (MaximumWallThickness::ERROR_CODE e) {
		cerr << "MaximumWallThickness error: " << e << endl;
		switch (e) {
		case MaximumWallThickness::ERROR_EXTRACT_LOOP:
			_2DMeasurements[4] = "Extract loop failed!";
			break;
		case MaximumWallThickness::ERROR_EXTRACT_SLICE:
			_2DMeasurements[4] = "Extract slice failed!";
			break;
		case MaximumWallThickness::ERROR_INPUT_NOT_A_SLICE:
			_2DMeasurements[4] = "Input is not a slice!";
			break;
		case MaximumWallThickness::ERROR_THICKNESS_CAL:
			_2DMeasurements[4] = "";
			break;
		case MaximumWallThickness::ERROR_UNDEFINED_BRANCH:
			_2DMeasurements[4] = "Cannot pair up vessel wall and lumen!";
			break;
		case MaximumWallThickness::ERROR_VALUE_TRANSFORM:
			_2DMeasurements[4] = "Type casting failed!";
			break;
		default:
			break;
		}
	}
	m_2DMeasurements[slice] = _2DMeasurements;
}

vtkImageData* Overlay::GetOutput()
{
	if (!m_vtkOverlay)
		return NULL;
	else
		return this->m_vtkOverlay;
}

vtkImageData * Overlay::GetVTKOutput()
{
	if (!m_vtkOverlay)
		return NULL;
	else
		return this->m_vtkOverlay;
}

ImageType::Pointer Overlay::GetITKOutput(ImageType::Pointer format)
{

	//if (!m_vtkOverlay)
	//	return NULL;
	//else {
	itk::VTKImageToImageFilter<itk::Image<double, 3>>::Pointer vtkImageToImageFilter =
		itk::VTKImageToImageFilter<itk::Image<double, 3>>::New();
	vtkImageToImageFilter->SetInput(this->GetOutput());
	vtkImageToImageFilter->Update();

	itk::CastImageFilter<itk::Image<double, 3>, ImageType>::Pointer castImageFilter =
		itk::CastImageFilter<itk::Image<double, 3>, ImageType>::New();
	castImageFilter->SetInput(vtkImageToImageFilter->GetOutput());
	castImageFilter->Update();

	//re-orient
	OrienterType::Pointer orienter = OrienterType::New();
	orienter->UseImageDirectionOn();
	orienter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orienter->SetInput(format);
	orienter->Update();

	m_itkOverlay = castImageFilter->GetOutput();
	m_itkOverlay->SetDirection(orienter->GetOutput()->GetDirection());
	m_itkOverlay->SetOrigin(orienter->GetOutput()->GetOrigin());
	//}
	return m_itkOverlay;

}

vtkLookupTable * Overlay::GetLookupTable()
{
	return m_lookupTable;
}

double * Overlay::Get3DMeasurements()
{
	Measure3D();

	return m_3DMeasurements;
}

QStringList Overlay::Get3DMeasurementsStrings()
{
	m_volumeStrings.clear();
	for (int i = 0; i < m_lookupTable->GetNumberOfColors(); ++i) {
		m_volumeStrings.append(QString::number(m_3DMeasurements[i]));
	}
	return m_volumeStrings;
}

QStringList Overlay::Get2DMeasurementsStrings(int slice)
{
	const int* extent = m_vtkOverlay->GetExtent();
	if (slice < extent[4]) {
		Measure2D(extent[4]);
		return m_2DMeasurements[0];
	}
	else if (slice > extent[5]) {
		Measure2D(extent[5]);
		return m_2DMeasurements[m_2DMeasurements.size() - 1];
	}
	else {
		Measure2D(slice);
		return m_2DMeasurements[slice - extent[4]];
	}
}
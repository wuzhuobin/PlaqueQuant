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
		return;
	}
	vtkSmartPointer<vtkImageCast> imageCast =
		vtkSmartPointer<vtkImageCast>::New();
	imageCast->SetOutputScalarTypeToDouble();
	imageCast->SetInputData(imageData);
	imageCast->Update();
	m_vtkOverlay = imageCast->GetOutput();

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
	m_vtkOverlay->DeepCopy(img);
	
	vtkSmartPointer<vtkImageCast> castfilter = vtkSmartPointer<vtkImageCast>::New();
	castfilter->SetInputData(m_vtkOverlay);
	castfilter->SetOutputScalarTypeToDouble();
	castfilter->Update();

	m_vtkOverlay = castfilter->GetOutput();

	const int* extent = m_vtkOverlay->GetExtent();


	for (int i = extent[0]; i <= extent[1]; i++)
	{
		for (int j = extent[2]; j <= extent[3]; j++)
		{
			for (int k = extent[4]; k <= extent[5]; k++)
			{
				double* pixel = static_cast<double*>(this->m_vtkOverlay->GetScalarPointer(i, j, k));
				*pixel = 0;
			}
		}
	}

	this->m_vtkOverlay->GetExtent(this->DisplayExtent);
	emit signalOverlayUpdated();
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
	Measure();
	emit signalOverlayUpdated();
}

void Overlay::Measure()
{
	m_measurementFor3D.SetInputData(m_vtkOverlay);
	m_measurementFor3D.SetLookupTable(m_lookupTable);
	m_measurementFor3D.Update();
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

double * Overlay::GetVolumes()
{
	return m_measurementFor3D.GetVolumes();;
}

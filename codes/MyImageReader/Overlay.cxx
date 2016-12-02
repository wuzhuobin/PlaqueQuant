#include "Overlay.h"


#include <itkImageSeriesReader.h>
#include <itkImageDuplicator.h>
#include <itkImageToVTKImageFilter.h>

#include <vtkImageMagnitude.h>
#include <vtkImageCast.h>
#include <vtkSimpleImageToImageFilter.h>
#include <vtkExtractVOI.h>
#include <vtkDoubleArray.h>
#include <vtkImageIterator.h>

typedef itk::ImageSeriesReader< Overlay::OverlayImageType >			ReaderType;
typedef itk::ImageDuplicator<Overlay::OverlayImageType>				DuplicatorType;
typedef itk::ImageToVTKImageFilter<Overlay::OverlayImageType>		ConnectorType;


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
	m_lookupTable->SetTableRange(0.0, 6.0);
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
	for (int i = 0; i < m_measurementFor2D.size(); ++i) {
		delete m_measurementFor2D[i];
	}
	for (QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>::iterator it = m_vesselWallPolyData.begin();
		it != m_vesselWallPolyData.end(); ++it) {
		delete *it;
	}
	for (QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>::iterator it = m_lumenPolyData.begin();
		it != m_lumenPolyData.end(); ++it) {
		delete *it;
	}
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


void Overlay::SetInputImageData(OverlayImageType::Pointer imageData)
{
	typedef itk::OrientImageFilter<OverlayImageType, OverlayImageType> OrienterType;
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
	vtkSmartPointer<vtkImageCast> imageCast =
		vtkSmartPointer<vtkImageCast>::New();
	imageCast->SetOutputScalarTypeToUnsignedChar();
	//imageCast->SetOutputScalarTypeToDouble();
	imageCast->SetInputData(imageData);
	imageCast->Update();
	m_vtkOverlay = imageCast->GetOutput();
	//Measure2D();
	//Measure3D();
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

void Overlay::Initialize(OverlayImageType::Pointer itkinputimage, int dim[3], double spacing[3], double origin[3], int type)
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
	OverlayImageType::Pointer m_itkOverlay = duplicator->GetOutput();

	itk::ImageRegionIterator<OverlayImageType> itr(m_itkOverlay, m_itkOverlay->GetLargestPossibleRegion());

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

void Overlay::SetPixel(int pos[3], double value)
{
		double * pixel = static_cast<double *>(m_vtkOverlay->GetScalarPointer(pos[0], pos[1], pos[2]));
		if (pixel == nullptr) {
			return;
		}
		pixel[0] = value;
}

void Overlay::SetPixel(int pos[3], unsigned char value)
{
	unsigned char * pixel = static_cast<unsigned char *>(m_vtkOverlay->GetScalarPointer(pos[0], pos[1], pos[2]));
	if (pixel == nullptr)
		return;
	pixel[0] = value;
}
void Overlay::Initialize(vtkImageData * img)
{
	m_vtkOverlay = vtkSmartPointer<vtkImageData>::New();
	m_vtkOverlay->SetExtent(img->GetExtent());
	m_vtkOverlay->SetOrigin(img->GetOrigin());
	m_vtkOverlay->SetSpacing(img->GetSpacing());
	m_vtkOverlay->AllocateScalars(VTK_UNSIGNED_CHAR, img->GetNumberOfScalarComponents());
	
	// Retrieve the entries from the image data and print them to the screen
	vtkImageIterator<unsigned char> it(m_vtkOverlay, m_vtkOverlay->GetExtent());
	while (!it.IsAtEnd()) {
		for (unsigned char* valIt = it.BeginSpan(); valIt != it.EndSpan(); ++valIt) {
			*valIt = 0;
		}
		it.NextSpan();
	}
	SetInputImageData(m_vtkOverlay);


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
						unsigned char pixelval = 0;
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

void Overlay::SetPixels(vtkPoints* points, unsigned char label)
{
	for (vtkIdType i = 0; i < points->GetNumberOfPoints(); ++i) {
		const double* ijk_double = points->GetPoint(i);
		int ijk[3];
		std::copy(ijk_double, ijk_double + 3, ijk);
		SetPixel(ijk, label);
	}

	m_vtkOverlay->Modified();
	emit signalOverlayUpdated();
}

void Overlay::ReplacePixels(int* extent, vtkImageData* image)
{
	int inImExtent[6];
	image->GetExtent(inImExtent);

	if (!std::equal(extent, extent + 6, inImExtent)) {
		// if the extent is different something is wrong
		return;
	}

	for (int orientation = 0; orientation < 3; ++orientation) {
		// find correct orientation
		for (int x = extent[0]; x <= extent[1]; ++x) {
			for (int y = extent[2]; y <= extent[3]; ++y) {
				for (int z = extent[4]; z <= extent[5]; ++z) {
					int scalerLength = image->GetScalarSize();
					int pos[3] = { x,y,z };
					double pixelval = 0;
					unsigned char* val = static_cast<unsigned char*>(
						image->GetScalarPointer(pos[0], pos[1], pos[2]));
					SetPixel(pos, *val);

				}
			}
		}
	}
	m_vtkOverlay->Modified();
	emit signalOverlayUpdated();
}

void Overlay::vtkShallowCopyImage(vtkImageData * image)
{
	m_vtkOverlay->ShallowCopy(image);
	emit signalOverlayUpdated();
}

void Overlay::Measure3D()
{
	m_measurementFor3D.SetInputData(m_vtkOverlay);
	m_measurementFor3D.SetLookupTable(m_lookupTable);
	m_measurementFor3D.Update();
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
	const int* extent = m_vtkOverlay->GetExtent();
	if (slice < extent[4] || slice > extent[5]) {
		return;
	}
	QStringList _2DMeasurements;
	_2DMeasurements += "Undefined";
	_2DMeasurements += "Undefined";
	_2DMeasurements += "Undefined";
	_2DMeasurements += "Undefined";
	_2DMeasurements += "Normal";
	//// to ensure it won't be out of index
	//while (slice >= m_2DMeasurements.size()) {
	//	m_2DMeasurements += _2DMeasurements;
	//}
	//while (slice >= m_measurementFor2D.size()) {
	//	m_measurementFor2D += new MeasurementFor2DIntegrated();
	//}
	m_2DMeasurements.insert(slice, _2DMeasurements);
	m_measurementFor2D[slice] = new MeasurementFor2DIntegrated();
	// Extract slice image
	vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	voi->SetInputData(m_vtkOverlay);
	voi->SetVOI(extent[0], extent[1], extent[2], extent[3], slice, slice);
	voi->Update();
	//slice = slice - extent[4];
	// Calculate areas
	m_measurementFor2D[slice]->m2d->SetSliceImage(voi->GetOutput());

	// Error checking
	//double lumenArea, vesselArea, NMI, distance; // #todo: Allow multiple branch
	try {
		m_measurementFor2D[slice]->m2d->Update();
		_2DMeasurements[0] = QString::number(
			m_measurementFor2D[slice]->m2d->GetOutput(0).LumenArea);
		_2DMeasurements[1] = QString::number(
			m_measurementFor2D[slice]->m2d->GetOutput(0).VesselWallArea);
		_2DMeasurements[3] = QString::number(
			m_measurementFor2D[slice]->m2d->GetOutput(0).VesselWallArea /
			(m_measurementFor2D[slice]->m2d->GetOutput(0).LumenArea + 
				m_measurementFor2D[slice]->m2d->GetOutput(0).VesselWallArea));
	}
	catch (MeasurementFor2D::ERROR_CODE e) {
		cerr << "MeasurementFor2D error: " << e << endl;
	}

	// calculate wall thickness
	m_measurementFor2D[slice]->mwt->SetLumemIntensity(1);
	m_measurementFor2D[slice]->mwt->SetVesselIntensity(2);
	m_measurementFor2D[slice]->mwt->SetSliceImage(voi->GetOutput());
	try {
		m_measurementFor2D[slice]->mwt->Update();
		_2DMeasurements[2] = QString::number(
			m_measurementFor2D[slice]->mwt->GetDistanceLoopPairVect().at(0).Distance);
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

vtkLookupTable * Overlay::GetLookupTable()
{
	return m_lookupTable;
}

MeasurementFor3D Overlay::GetMeasurementFor3D()
{
	return m_measurementFor3D;
}

double * Overlay::Get3DMeasurementsAsDouble()
{
	return m_measurementFor3D.GetVolumes();
}

QStringList Overlay::Get3DMeasurementsStrings()
{
	double* _3DMeasurements = Get3DMeasurementsAsDouble();
	m_volumeStrings.clear();
	for (int i = 0; i < m_lookupTable->GetNumberOfColors(); ++i) {
		m_volumeStrings.append(QString::number(_3DMeasurements[i]));
	}
	return m_volumeStrings;
}

Overlay::MeasurementFor2DIntegrated Overlay::GetMeasurementFor2D(int slice)
{
	return *(m_measurementFor2D[slice]);
}

QStringList Overlay::Get2DMeasurementsStrings(int slice)
{
	const int* extent = m_vtkOverlay->GetExtent();
	if (slice < extent[4]) {
		//Measure2D(extent[4]);
		return m_2DMeasurements[extent[4]];
	}
	else if (slice > extent[5]) {
		//Measure2D(extent[5]);
		return m_2DMeasurements[extent[5]];
	}
	else {
		//Measure2D(slice);
		return m_2DMeasurements[slice];
	}
}

QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* Overlay::GetVesselWallPolyData()
{
	return &m_vesselWallPolyData;
}

QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* Overlay::GetLumenPolyData()
{
	return &m_lumenPolyData;
}

std::map<int, QList<vtkSmartPointer<vtkPolyData>>*>* Overlay::GetVesselWallPolyDataSTD()
{
	return &(m_vesselWallPolyData.toStdMap());
}

std::map<int, QList<vtkSmartPointer<vtkPolyData>>*>* Overlay::GetLumenPolyDataSTD()
{
	return &(m_lumenPolyData.toStdMap());
}


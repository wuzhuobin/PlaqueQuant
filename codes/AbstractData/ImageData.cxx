#include "ImageData.h"
#include "vtkImageProperty.h"
#include "vtkImageMapper3D.h"
#include "itkShiftScaleImageFilter.h"
#include "itkExtractImageFilter.h"

Image::Image(QObject *parent) :
    QObject(parent), AbstractData()
{
	m_lut				= NULL;
	m_lutType			= 0;
}

Image::~Image()
{
	//m_transform->Delete();
	//m_userMatrix->Delete();
	//m_inverseMatrix->Delete();
}


void Image::InstallPipeline()
{

	//Calculate Window Level
	double minMax[2];
	m_imageData->GetScalarRange(minMax);

	if (minMax[1] - minMax[0] < 5)
	{
		minMax[0] = 0;
		minMax[1] = 3;

	}

	//Set LUT
	this->SetLUT(0);
}

vtkImageData* Image::GetImageData()
{
	return m_imageData;
}

void Image::SetImage(vtkImageData *im)
{
	this->m_imageData = im;
}

void Image::SetLUT(vtkLookupTable* lut )
{
	this->m_lut = lut;
}

int Image::GetLUT()
{
	return m_lutType;
}


vtkLookupTable* Image::GetLookUpTable()
{
	return m_lut;
}

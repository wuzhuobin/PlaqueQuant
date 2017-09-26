#ifndef LUMEN_EXTRACTION_H
#define LUMEN_EXTRACTION_H

#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "itkImage.h"
#include "itkConfidenceConnectedImageFilter.h"
#include <itkConnectedThresholdImageFilter.h>
#include "itkBinaryBallStructuringElement.h"
#include "itkImageToVTKImageFilter.h"
#include <itkVTKImageToImageFilter.h>



using namespace std;

#ifndef vtkDoubleType
#define vtkDoubleType double
#endif


class LumenExtraction
{
	// For Lumen extraction
	typedef float										PixelType;
	const static unsigned int							ImageDimension = 3;
	typedef itk::Image< PixelType, ImageDimension >		ImageType;
	typedef itk::ImageToVTKImageFilter<ImageType>		ConnectorType;

	typedef unsigned char							LabelPixelType;
	typedef itk::Image<LabelPixelType, 3>			LabelImageType;
	typedef itk::ImageToVTKImageFilter<ImageType>		ConnectorType;
	typedef itk::VTKImageToImageFilter<ImageType>	VTKImageToImageType;
	typedef itk::BinaryBallStructuringElement<	ImageType::PixelType, 2>                  StructuringElementType;
	typedef itk::CastImageFilter<ImageType, LabelImageType> ImageToLabelImageCastFilterType;
	typedef itk::CastImageFilter<LabelImageType, ImageType> LabelImageToImageCastFilterType;
	typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType > ConnectedThresholdImageFilterType;
	typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> ConfidenceConnectedFilterType;
public:
	LumenExtraction();
	~LumenExtraction();

	void ClearAllSeeds();
	void SetInputData(vtkImageData* image);
	void SetDilationValue(double dilationVal);
	void AddSeed(int* seed);
	void SetInitialNeighborhoodRadius(double radius);
	void SetMultiplier(double multiplier);
	void SetNumberOfIterations(double itr);
	void GetInputItkImage(ImageType::Pointer itkImage);
	void GetLumenItkImage(ImageType::Pointer itkLumenImage);
	void GetLumenVtkImage(vtkImageData* vtkLumenImage);
	void GetDilatedItkImage(ImageType::Pointer dilatedImage);
	void GetDilatedVtkImage(vtkImageData* dilatedImage);
	void Update();
	void LabelDilation(vtkImageData*);

	enum ERROR_CODE {
		ERROR_NO_SEEDS_IN_LIST = 0
	};

private:
	vtkImageData* m_input;
	double m_isoVal;
	vtkPolyData* m_lumenSurface;
	vtkPolyData* m_dilatedSurface;
	double m_dilationVal;
	ImageType::Pointer m_itkLumenImage;
	ImageType::Pointer m_inputItkImage;
	ImageType::Pointer m_dilatedItkImage;
	vtkImageData* m_dilatedVtkImage;
	vtkImageData* m_vtkLumenImage;
	std::vector<int*> m_seedList;
	double m_initialNeighborhoodRadius;
	double m_mulitplier;
	double m_iterations;

	void LabelDilation();
};

#endif
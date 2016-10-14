#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "vtkMarchingCubes.h"
#include "vtkPolyData.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"

#include "itkImage.h"
#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkDefaultDynamicMeshTraits.h"
#include "itkMesh.h"
#include "itkTriangleCell.h"
#include "itkTriangleMeshToBinaryImageFilter.h"
#include <itkVTKImageToImageFilter.h>		
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include <itkImageToVTKImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkIndex.h>
#include "itkConfidenceConnectedImageFilter.h"

#include <itkImageFileWriter.h>

#include <QList>

using namespace std;

#ifndef vtkDoubleType
#define vtkDoubleType double
#endif

typedef itk::Image<float, 3>					ImageType;
typedef unsigned char							LabelPixelType;
typedef itk::LabelObject< LabelPixelType, 3 >	LabelObjectType;
typedef itk::LabelMap<LabelObjectType>			LabelMapType;
typedef itk::Image<LabelPixelType, 3>			LabelImageType;
typedef itk::VTKImageToImageFilter<ImageType>	VTKImageToImageType;
typedef itk::BinaryBallStructuringElement<	ImageType::PixelType, 2>                  StructuringElementType;
typedef itk::ImageToVTKImageFilter<ImageType>       ConnectorType;
typedef itk::CastImageFilter<ImageType, LabelImageType> ImageToLabelImageCastFilterType;
typedef itk::CastImageFilter<LabelImageType, ImageType> LabelImageToImageCastFilterType;
typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType > ConnectedThresholdImageFilterType;
typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> ConfidenceConnectedFilterType;
typedef itk::ImageToVTKImageFilter<ImageType>				ConnectorType;


class LumenExtraction
{
public:
	LumenExtraction();
	~LumenExtraction();

	void SetInputData(vtkImageData* image);
	void SetDilationValue(double dilationVal);
	void AddSeed(double* seed);
	void SetInitialNeighborhoodRadius(double radius);
	void SetMultiplier(double multiplier);
	void SetNumberOfIterations(double itr);
	void GetInputItkImage(ImageType::Pointer itkImage);
	void GetLumenItkImage(ImageType::Pointer itkLumenImage);
	void GetLumenVtkImage(vtkImageData* vtkLumenImage);
	void GetDilatedItkImage(ImageType::Pointer dilatedImage);
	void GetDilatedVtkImage(vtkImageData* dilatedImage);
	void Update();

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
	QList<double*> m_seedList;
	double m_initialNeighborhoodRadius;
	double m_mulitplier;
	double m_iterations;

	void LabelDilation();
};
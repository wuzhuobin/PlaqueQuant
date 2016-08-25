#ifndef DEFINE_H
#define DEFINE_H

//Preprocessor definition
//_CRT_SECURE_NO_WARNINGS
//_SCL_SECURE_NO_WARNINGS

//ITK
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkSmartPointer.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkImageToVTKImageFilter.h"
#include "itkOrientImageFilter.h"
#include "itkImageFileReader.h"

#include <itkLabelImageToLabelMapFilter.h>
#include <itkLabelObject.h>
#include <itkLabelMap.h>
#include <itkMaskImageFilter.h>
#include <itkOtsuThresholdImageFilter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkInvertIntensityImageFilter.h>
#include "itkConnectedComponentImageFilter.h"
#include "itkLabelShapeKeepNObjectsImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itksys/SystemTools.hxx"
#include <sstream>
#include <itkCastImageFilter.h>
#include <itkLabelImageToLabelMapFilter.h>
#include <itkLabelMapToLabelImageFilter.h>
#include <itkMergeLabelMapFilter.h>
#include <itkChangeLabelImageFilter.h>
#include <itkSubtractImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>

#include <itkVTKImageToImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkImageDuplicator.h>
typedef float										PixelType;
const static unsigned int							ImageDimension = 3;
typedef itk::Image< PixelType, ImageDimension >		ImageType;		
typedef itk::GDCMSeriesFileNames					NamesGeneratorType;
typedef itk::ImageSeriesReader< ImageType >			ReaderType;
typedef itk::ImageFileReader<ImageType>				NiftiReaderType;
typedef itk::ImageFileWriter<ImageType>				NiftiWriterType;
typedef itk::GDCMImageIO							ImageIOType;
typedef itk::ImageToVTKImageFilter<ImageType>		ConnectorType;
typedef itk::OrientImageFilter<ImageType,ImageType> OrienterType;
typedef itk::ImageFileWriter<ImageType>				WriterType;
typedef itk::ImageDuplicator<ImageType>				DuplicatorType;
typedef itk::VTKImageToImageFilter<ImageType>		vtkImageToImageFilterType;

typedef itk::Image<float, 3>														FloatImageType;
typedef itk::Image< unsigned short, 3 >												UnsignedShortImageType;
typedef itk::LabelObject<unsigned long, 3>											LabelObjectType;
typedef itk::LabelMap<LabelObjectType>												LabelMapType;
typedef itk::LabelImageToLabelMapFilter<FloatImageType, LabelMapType>				LabelImageToLabelMapFilterType;
typedef itk::MaskImageFilter<FloatImageType, FloatImageType, FloatImageType>			MaskImageFilterType;
typedef itk::OtsuThresholdImageFilter<FloatImageType, FloatImageType>				OtsuThresholdImageFilterType;
typedef itk::OtsuMultipleThresholdsImageFilter<FloatImageType, FloatImageType>		OtsuMultipleThresholdsImageFilterType;
typedef itk::InvertIntensityImageFilter<FloatImageType, FloatImageType>				InvertIntensityImageFilterType;
typedef itk::ConnectedComponentImageFilter<FloatImageType, UnsignedShortImageType>	ConnectedComponentImageFilterType;
typedef itk::LabelShapeKeepNObjectsImageFilter<UnsignedShortImageType>				LabelShapeKeepNObjectsImageFilterType;
typedef itk::ChangeLabelImageFilter<FloatImageType, FloatImageType>					ChangeLabelImageFilterType;
typedef itk::SubtractImageFilter<FloatImageType, FloatImageType>						SubtractImageFilterType;
typedef itk::ImageToVTKImageFilter<FloatImageType>									ImageToVTKImageType;
typedef itk::BinaryThresholdImageFilter<FloatImageType, FloatImageType>								BinarThresholdImageFilterType;

//VTK
#include "vtkImageData.h"
#include "vtkPolyData.h"
#include "vtkNIFTIImageWriter.h"
#include <vtkNIFTIImageHeader.h>
#include "vtkNIFTIImageReader.h"


#endif //DEFINE_H
#ifndef IMAGEREGISTRATION_H
#define IMAGEREGISTRATION_H

#include <QObject>

#include "itkVersorRigid3DTransform.h"
#include "itkVersorRigid3DTransformOptimizer.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageRegistrationMethod.h"
#include "itkResampleImageFilter.h"
#include "itkCenteredTransformInitializer.h"



class ImageRegistration : public QObject
{
	Q_OBJECT;
	//Registration
	typedef float										PixelType;
	const static unsigned int							ImageDimension = 3;
	typedef itk::Image< PixelType, ImageDimension >		ImageType;
	typedef itk::VersorRigid3DTransform <double>									TransformType;		//TransformType
	typedef itk::VersorRigid3DTransformOptimizer									OptimizerType; 		//Optimizer
	typedef OptimizerType::ScalesType												OptimizerScalesType;
	typedef itk::MattesMutualInformationImageToImageMetric<ImageType, ImageType>	MetricType;			//Metric
	typedef itk::LinearInterpolateImageFunction <ImageType, double>					InterpolatorType;	//interpolation
	typedef itk::CenteredTransformInitializer<TransformType, ImageType, ImageType >	InitializerType;    //initializer
	typedef itk::ImageRegistrationMethod <ImageType, ImageType>						RegistrationType;	//registration method
	typedef itk::ResampleImageFilter <ImageType, ImageType>							ResampleFilterType;	//Resampler


public:
	ImageRegistration(QObject* parent = NULL);
	~ImageRegistration();

	void SetFixedImage(ImageType::Pointer fixedImage);
	void SetMovingImage(ImageType::Pointer movingImage);

	bool Update();	
	TransformType::Pointer   GetOutputTransform();
	ImageType::DirectionType GetOutputDirection();
	ImageType::PointType	 GetOutputOrigin();
	ImageType::Pointer		 GetOutput();

private:
	ImageType::Pointer	   m_fixedImage;
	ImageType::Pointer	   m_movingImage;
	ImageType::Pointer	   m_outputImage;
	TransformType::Pointer m_finalTransform;
};

#endif // IMAGEREGISTRATION_H

#include "ImageRegistration.h"

#include <itkCenteredTransformInitializer.h>
#include "itkVersorRigid3DTransformOptimizer.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageRegistrationMethod.h"
#include "itkResampleImageFilter.h"

typedef itk::VersorRigid3DTransformOptimizer									OptimizerType; 		//Optimizer
typedef OptimizerType::ScalesType												OptimizerScalesType;
typedef itk::MattesMutualInformationImageToImageMetric<ImageRegistration::ImageType, ImageRegistration::ImageType>	MetricType;			//Metric
typedef itk::LinearInterpolateImageFunction <ImageRegistration::ImageType, double>					InterpolatorType;	//interpolation
typedef itk::CenteredTransformInitializer<ImageRegistration::TransformType, ImageRegistration::ImageType, ImageRegistration::ImageType >	InitializerType;    //initializer
typedef itk::ImageRegistrationMethod <ImageRegistration::ImageType, ImageRegistration::ImageType>						RegistrationType;	//registration method
typedef itk::ResampleImageFilter <ImageRegistration::ImageType, ImageRegistration::ImageType>							ResampleFilterType;	//Resampler


ImageRegistration::ImageRegistration(QObject* parent) : QObject(parent)
{
	m_fixedImage	 = NULL;
	m_movingImage	 = NULL;
	m_finalTransform = NULL;
	m_outputImage	 = NULL;
}

ImageRegistration::~ImageRegistration()
{
}

void ImageRegistration::SetFixedImage( ImageType::Pointer fixedImage )
{
	m_fixedImage = fixedImage;	
}

void ImageRegistration::SetMovingImage( ImageType::Pointer movingImage )
{
	m_movingImage = movingImage;
}

bool ImageRegistration::Update()
{
	if (!m_fixedImage| !m_movingImage)
		return EXIT_FAILURE;

	//initialization
	MetricType::Pointer         metric        = MetricType::New();
	OptimizerType::Pointer      optimizer     = OptimizerType::New();
	InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
	TransformType::Pointer		transform	  = TransformType::New();
	InitializerType::Pointer    initializer	  = InitializerType::New();
	RegistrationType::Pointer   registration  = RegistrationType::New();

	//link metric, optimizer, interpolator and transform to registration
	metric->SetNumberOfSpatialSamples(100000);

	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	registration->SetInterpolator(interpolator);
	registration->SetTransform(transform);

	//set the fixed and moving images
	registration->SetFixedImage(m_fixedImage);
	registration->SetMovingImage(m_movingImage);
	registration->SetFixedImageRegion(m_fixedImage->GetBufferedRegion());

	//Initialize transform
	typedef TransformType::VersorType VersorType;
	typedef VersorType::VectorType VectorType;
	VersorType rotation;
	VectorType axis;
	axis[0] = 0.0;
	axis[1] = 0.0;
	axis[2] = 1.0;
	const double angle = 0;
	rotation.Set(axis, angle);
	transform->SetMatrix(rotation.GetMatrix());

	initializer->SetTransform(transform);
	initializer->SetFixedImage(m_fixedImage);
	initializer->SetMovingImage(m_movingImage);
	initializer->GeometryOn();
	initializer->InitializeTransform();

	//registration get the initial transform parameters
	registration->SetInitialTransformParameters(transform->GetParameters());

	//set the optimizer
	OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());
	double translationScale = 1.0 /1000.0;
	optimizerScales[0] = 1.0;
	optimizerScales[1] = 1.0;
	optimizerScales[2] = 1.0;
	optimizerScales[3] = translationScale;
	optimizerScales[4] = translationScale;
	optimizerScales[5] = translationScale;

	optimizer->SetScales(optimizerScales);

	//set step length and iterations
	optimizer->SetMaximumStepLength(0.2000);
	//optimizer->SetMaximumStepLength(0.005);
	optimizer->SetMinimumStepLength(0.001);
	optimizer->SetNumberOfIterations(1500);
	optimizer->SetRelaxationFactor(0.5);

	//print out error messages if the registration fails
	try
	{
		registration->Update();
		std::cout << "Optimizer stop condition:"
			<< registration->GetOptimizer()->GetStopConditionDescription()
			<<std::endl;
	}
	catch (itk::ExceptionObject &err)
	{
		std::cerr << "ExceptionObject caught!" << std::endl;
		std::cerr << err << std::endl;
		return EXIT_FAILURE;
	}

	const TransformType::ParametersType finalParameters = registration->GetLastTransformParameters();

	//get the result of the optimizer
	TransformType::Pointer finalTransform = TransformType::New();
	transform->SetParameters(finalParameters);
	finalTransform->SetCenter(transform->GetCenter());  //for affine transform
	finalTransform->SetParameters(finalParameters);
	finalTransform->SetFixedParameters(transform->GetFixedParameters());

	finalTransform->Print(std::cout);

	//reserve the finalTransform
	m_finalTransform = finalTransform;

	//get the image by applying the transform to the fixed image using resampling
	ResampleFilterType::Pointer resampler = ResampleFilterType::New();
	resampler->SetTransform(finalTransform);
	resampler->SetInput(m_movingImage);

	//set the parameters from the fixed image
	resampler->SetSize(m_fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(m_fixedImage->GetOrigin());
	resampler->SetOutputSpacing(m_fixedImage->GetSpacing());
	resampler->SetOutputDirection(m_fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(0);
	resampler->Update();

	m_outputImage = resampler->GetOutput();

	//return the registered image
	return EXIT_SUCCESS;
}

ImageRegistration::TransformType::Pointer ImageRegistration::GetOutputTransform()
{
	if (m_finalTransform)
	{
		return m_finalTransform;
	}
	
	return NULL;
}

ImageRegistration::ImageType::PointType ImageRegistration::GetOutputOrigin()
{
	return m_outputImage->GetOrigin();
	return NULL;
}

ImageRegistration::ImageType::DirectionType ImageRegistration::GetOutputDirection()
{
	return m_outputImage->GetDirection();
}

ImageRegistration::ImageType::Pointer ImageRegistration::GetOutput()
{
	return m_outputImage;
}
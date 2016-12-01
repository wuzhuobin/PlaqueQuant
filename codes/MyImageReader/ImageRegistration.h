#ifndef IMAGEREGISTRATION_H
#define IMAGEREGISTRATION_H

#include <QObject>

#include <itkImage.h>
#include <itkVersorRigid3DTransform.h>


typedef itk::Image< float, 3 > ImageType;
typedef itk::VersorRigid3DTransform <double> TransformType;		//TransformType
class ImageRegistration : public QObject
{
	Q_OBJECT

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

#include "MyImageManager.h"



#include <string>
#include <vector>

using namespace itk;
MyImageManager::MyImageManager(QObject* parent)
	:QObject(parent)
{
}

MyImageManager::~MyImageManager()
{
	for (QMap<Image<float, 3>::Pointer, QMap<QString, QString>>::iterator it;
		it != this->mapOfDICOMHeader.end(); ++it) {
		delete &it.value();
	}
	this->mapOfDICOMHeader.clear();
}

void MyImageManager::enableRegistration(bool flag)
{
	this->registrationFlag = flag;
}

void MyImageManager::addFileNames(QStringList fileNames)
{
	this->listOfFileNames.append(fileNames);
}

QList<QStringList> MyImageManager::getListOfFileNames()
{
	return this->listOfFileNames;
}

ImageType::Pointer MyImageManager::imageAlignment(ImageType::Pointer alignedTo, ImageType::Pointer toBeAligned)
{
	MyImageManager::registration.SetFixedImage(alignedTo);
	MyImageManager::registration.SetMovingImage(toBeAligned);
	MyImageManager::registration.Update();

	return registration.GetOutput();
}

int MyImageManager::getNumberOfImages()
{
	return this->listOfItkImages.size();
}

const QList<vtkSmartPointer<vtkImageData>> MyImageManager::getListOfVtkImages()
{
	return this->listOfVtkImages;
}

const QList<Image<float, 3>::Pointer> MyImageManager::getListOfItkImages()
{
	return this->listOfItkImages;
}

const QMap<QString, Image<float, 3>::Pointer> MyImageManager::getMapOfItkImages()
{
	return this->mapOfItkImages;
}

const QMap<QString, vtkSmartPointer<vtkImageData>> MyImageManager::getMapOfVtkImages()
{
	return this->mapOfVtkImages;
}

const QMap<QString, QString> MyImageManager::getDICOMHeader(Image<float, 3>::Pointer itkImage)
{
	return this->mapOfDICOMHeader[itkImage];
}

#include "MyImageManager.h"

using namespace itk;
MyImageManager::MyImageManager(QObject* parent)
	:QObject(parent)
{
}

MyImageManager::~MyImageManager()
{
	for (QMap<Image<float, 3>::Pointer, QMap<QString, QString>*>::iterator it =
		this->mapOfDICOMHeader.begin();	it != this->mapOfDICOMHeader.end(); ++it) {
		delete it.value();
	}
	this->mapOfDICOMHeader.clear();
	
	for (QList<QMap<QString, QString>*>::iterator it = this->listOfDICOMHeader.begin();
		it != this->listOfDICOMHeader.end(); ++it) {
		delete *it;
	}
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

const QList<QMap<QString, QString>*> MyImageManager::getListOfDICOMHeader()
{
	return this->listOfDICOMHeader;
}

const QMap<QString, Image<float, 3>::Pointer> MyImageManager::getMapOfItkImages()
{
	return this->mapOfItkImages;
}

const QMap<QString, vtkSmartPointer<vtkImageData>> MyImageManager::getMapOfVtkImages()
{
	return this->mapOfVtkImages;
}

const QMap<QString, QString>* MyImageManager::getDICOMHeader(Image<float, 3>::Pointer itkImage)
{
	return this->mapOfDICOMHeader[itkImage];
}

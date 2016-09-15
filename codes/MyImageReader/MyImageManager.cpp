#include "MyImageManager.h"

using namespace itk;
MyImageManager::MyImageManager(QObject* parent)
	:overlay(parent), QObject(parent)
{
}

MyImageManager::~MyImageManager()
{
	allClear();
}

int MyImageManager::getNumberOfImages()
{
	int num = 0;
	for (int i = 0; i < this->listOfItkImages.size(); ++i) {
		if (this->listOfItkImages[i].IsNotNull())
			++num;
	}
	return num;
}

QList<vtkSmartPointer<vtkImageData>> MyImageManager::getListOfViewerInputImages()
{
	return this->listOfVtkViewerInputImages;
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

const QList<QString> MyImageManager::getListOfModalityNames()
{
	return this->listOfModalityNames;
}

QString MyImageManager::getModalityName(int i)
{
	return this->listOfModalityNames[i];
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

Overlay& MyImageManager::getOverlay()
{
	return this->overlay;
}

void MyImageManager::allClear()
{

	for (QMap<Image<float, 3>::Pointer, QMap<QString, QString>*>::iterator it =
		this->mapOfDICOMHeader.begin();	it != this->mapOfDICOMHeader.end(); ++it) {
		delete it.value();
	}
	for (QList<QMap<QString, QString>*>::iterator it = this->listOfDICOMHeader.begin();
		it != this->listOfDICOMHeader.end(); ++it) {
		delete *it;
	}

	this->listOfItkImages.clear();
	this->listOfVtkViewerInputImages.clear();
	this->listOfVtkImages.clear();
	this->listOfDICOMHeader.clear();
	this->listOfModalityNames.clear();

	this->mapOfDICOMHeader.clear();
	this->mapOfItkImages.clear();
	this->mapOfVtkImages.clear();

}
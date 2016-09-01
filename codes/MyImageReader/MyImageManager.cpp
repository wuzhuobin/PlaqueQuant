#include "MyImageManager.h"

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkOrientImageFilter.h>
#include <itkMetaDataDictionary.h>
#include <itkGDCMImageIO.h>

#include <string>
#include <vector>

using namespace itk;
MyImageManager::MyImageManager(QList<QStringList> listOfFileNames,
	bool registrationFlag, QObject* parent)
	:listOfFileNames(listOfFileNames), registrationFlag(registrationFlag),
	registration(parent), QObject(parent)
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
bool MyImageManager::loadImages()
{
	//for (int i = 0; i < this->listOfFileNames.size(); ++i) {
	for(QList<QStringList>::const_iterator cit = listOfFileNames.constBegin(); 
		cit!=listOfFileNames.constEnd();++cit){
		QStringList fileNames = *cit;
		ImageType::Pointer _image;
		// QMAP saving DICOM imformation
		QMap<QString, QString>* DICOMHeader = new QMap<QString, QString>;
		// load Nifti Data
		if (fileNames.size() == 1 && fileNames[0].contains(".nii")) {
			ImageFileReader<Image<float, 3>>::Pointer reader = 
				ImageFileReader<Image<float, 3>>::New();
			reader->SetFileName(fileNames[0].toStdString());
			reader->Update();
			_image = reader->GetOutput();
		}
		// Load Dicom Data
		else {
			std::vector<std::string> _fileNames;
			for (QStringList::const_iterator constIterator = fileNames.constBegin();
				constIterator != fileNames.constEnd(); ++constIterator){
				_fileNames.push_back(constIterator->toStdString());
			}
			GDCMImageIO::Pointer dicomIO = GDCMImageIO::New();

			ImageSeriesReader<Image<float, 3>>::Pointer reader = 
				ImageSeriesReader<Image<float, 3>>::New();
			reader->SetFileNames(_fileNames);
			reader->SetImageIO(dicomIO);
			reader->Update();
			_image = reader->GetOutput();

			const  MetaDataDictionary& dictionary = dicomIO->GetMetaDataDictionary();
			for (MetaDataDictionary::ConstIterator cit = dictionary.Begin();
				cit != dictionary.End(); ++cit) {
				MetaDataObjectBase::Pointer  entry = cit->second;
				MetaDataObject<std::string>::Pointer entryvalue =
					dynamic_cast<MetaDataObject<std::string>*>(entry.GetPointer());
				if (entryvalue)
				{
					DICOMHeader->insert(QString::fromStdString(cit->first),
						QString::fromStdString(entryvalue->GetMetaDataObjectValue()));
				}
			}
		}
		// using the same orientation ITK_COORDINATE_ORIENTATION_RAI
		OrientImageFilter<Image<float, 3>, Image<float, 3>>::Pointer orienter =
			OrientImageFilter<Image<float, 3>, Image<float, 3>>::New();
		orienter->UseImageDirectionOn();
		orienter->SetDesiredCoordinateOrientation(
			itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
		orienter->SetInput(_image);
		orienter->Update();
		_image = orienter->GetOutput();
		mapOfDICOMHeader.insert(_image, *DICOMHeader);
		// Image Registration
		if (cit != listOfFileNames.constBegin() && this-registrationFlag) {
			_image = imageAlignment(this->mapOfItkImages[0], _image);
		}
		this->listOfItkImages.append(_image);
		this->mapOfItkImages.insert(fileNames[0], _image);

		ImageToVTKImageFilter<Image<float, 3>>::Pointer connector = 
			ImageToVTKImageFilter<Image<float, 3>>::New();
		connector->SetInput(_image);
		this->listOfVtkImages.append(connector->GetOutput());
		this->mapOfVtkImages.insert(fileNames[0], connector->GetOutput());

	}
	return true;
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

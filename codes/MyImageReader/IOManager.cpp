#include "IOManager.h"


#include "MyImageManager.h"

#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkOrientImageFilter.h>
#include <itkMetaDataDictionary.h>
#include <itkGDCMImageIO.h>

using namespace itk;
IOManager::IOManager(QObject* parent) 
	:registration(parent), QObject(parent)
{

}

void IOManager::slotEnableRegistration(bool flag)
{
	this->registrationFlag = flag;
}

void IOManager::setListOfFileNames(QList<QStringList> listOfFileNames)
{
	this->listOfFileNames = listOfFileNames;
}

const QList<QStringList> IOManager::getListOfFileNames()
{
	return this->listOfFileNames;
}

bool IOManager::LoadImageData(QStringList fileNames)
{
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
			constIterator != fileNames.constEnd(); ++constIterator) {
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
	this->myImageManager->mapOfDICOMHeader.insert(_image, *DICOMHeader);
	// Image Registration
	if (this->registrationFlag) {
		_image = imageAlignment(this->myImageManager->mapOfItkImages[0], _image);
	}
	this->myImageManager->listOfItkImages.append(_image);
	this->myImageManager->mapOfItkImages.insert(fileNames[0], _image);

	ImageToVTKImageFilter<Image<float, 3>>::Pointer connector =
		ImageToVTKImageFilter<Image<float, 3>>::New();
	connector->SetInput(_image);
	this->myImageManager->listOfVtkImages.append(connector->GetOutput());
	this->myImageManager->mapOfVtkImages.insert(fileNames[0], connector->GetOutput());
	return true;
}

ImageType::Pointer IOManager::imageAlignment(ImageType::Pointer alignedTo, ImageType::Pointer toBeAligned)
{
	this->registration.SetFixedImage(alignedTo);
	this->registration.SetMovingImage(toBeAligned);
	this->registration.Update();

	return registration.GetOutput();
}

void IOManager::setMyImageManager(MyImageManager * myImageManager)
{
	this->myImageManager = myImageManager;
}

void IOManager::slotOpenMultiImages()
{
	bool _flag = this->registrationFlag;
	for (QList<QStringList>::const_iterator cit = this->listOfFileNames.constBegin();
		cit != this->listOfFileNames.constEnd(); ++cit) {
		// Never do registration for the first image
		if (cit == listOfFileNames.constBegin()) {
			this->registrationFlag = false;
		}
		else {
			this->registrationFlag = _flag;
		}
		slotOpenOneImage(*cit);
	}
}

void IOManager::slotOpenOneImage(QStringList fileNames)
{
	this->LoadImageData(fileNames);
}

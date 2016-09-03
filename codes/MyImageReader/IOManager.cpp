#include "IOManager.h"

#include "MyImageManager.h"
#include "ImageRegistration.h"
#include "Define.h"

#include <itkImageFileReader.h>
#include <itkMetaDataDictionary.h>
#include <itkOrientImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkImageSeriesReader.h>
#include <itkImageToVTKImageFilter.h>

using namespace itk;
IOManager::IOManager(QObject* parent) 
	:registration(parent), QObject(parent)
{
}

IOManager::~IOManager()
{
}

void IOManager::enableRegistration(bool flag)
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
	ImageType::Pointer _itkImage;
	vtkSmartPointer<vtkImageData> _vtkImage;
	// QMAP saving DICOM imformation
	QMap<QString, QString>* DICOMHeader;
	if (fileNames.isEmpty()) {
		_itkImage = NULL;
		_vtkImage = NULL;
		DICOMHeader = NULL;
	}
	// load Nifti Data
	else if (fileNames.size() == 1 && fileNames[0].contains(".nii")) {
		ImageFileReader<Image<float, 3>>::Pointer reader =
			ImageFileReader<Image<float, 3>>::New();
		reader->SetFileName(fileNames[0].toStdString());
		reader->Update();
		_itkImage = reader->GetOutput();
		DICOMHeader = NULL;
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
		_itkImage = reader->GetOutput();

		DICOMHeader = new QMap<QString, QString>;
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
	if (_itkImage.IsNotNull()) {
		// using the same orientation ITK_COORDINATE_ORIENTATION_RAI
		OrientImageFilter<Image<float, 3>, Image<float, 3>>::Pointer orienter =
			OrientImageFilter<Image<float, 3>, Image<float, 3>>::New();
		orienter->UseImageDirectionOn();
		orienter->SetDesiredCoordinateOrientation(
			itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
		orienter->SetInput(_itkImage);
		orienter->Update();
		_itkImage = orienter->GetOutput();
		// Image Registration
		if (this->registrationFlag) {
			_itkImage = imageAlignment(this->myImageManager->listOfItkImages[0], _itkImage);
		}

		ImageToVTKImageFilter<Image<float, 3>>::Pointer connector =
			ImageToVTKImageFilter<Image<float, 3>>::New();
		connector->SetInput(_itkImage);
		connector->Update();
		_vtkImage = connector->GetOutput();
	}
	this->myImageManager->listOfItkImages.append(_itkImage);
	this->myImageManager->listOfVtkImages.append(_vtkImage);
	this->myImageManager->listOfDICOMHeader.append(DICOMHeader);
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

void IOManager::setUniqueKeys(QStringList keys)
{
	this->uniqueKeys = keys;
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

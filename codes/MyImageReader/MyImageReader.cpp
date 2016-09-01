#include "MyImageReader.h"

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
//#include <itkImageToVTKImageFilter.h>
#include <itkOrientImageFilter.h>
#include <itkMetaDataDictionary.h>
#include <itkGDCMImageIO.h>

#include <string>
#include <vector>

using namespace itk;
MyImageReader::MyImageReader(QList<QStringList> listOfFileNames,
	bool registrationFlag, QObject* parent)
	:listOfFileNames(listOfFileNames), registrationFlag(registrationFlag),
	registration(parent), QObject(parent)
{
}

MyImageReader::~MyImageReader()
{
}
#include <qdebug.h>
bool MyImageReader::loadImages()
{
	//for (int i = 0; i < this->listOfFileNames.size(); ++i) {
	for(QList<QStringList>::const_iterator cit = listOfFileNames.constBegin(); 
		cit!=listOfFileNames.constEnd();++cit){
		QStringList fileNames = *cit;
		ImageType::Pointer _image;
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
			QMap<QString, QString>* DICOMHeader = new QMap<QString, QString>;
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
			mapOfDICOMHeader.insert(_image, *DICOMHeader);
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
	qDebug() << listOfItkImages[0];
	qDebug() << mapOfItkImages.first();
	qDebug() << mapOfDICOMHeader.firstKey();
	return true;
}


void MyImageReader::enableRegistration(bool flag)
{
	this->registrationFlag = flag;
}

QList<QStringList> MyImageReader::getListOfFileNames()
{
	return this->listOfFileNames;
}

ImageType::Pointer MyImageReader::imageAlignment(ImageType::Pointer alignedTo, ImageType::Pointer toBeAligned)
{
	MyImageReader::registration.SetFixedImage(alignedTo);
	MyImageReader::registration.SetMovingImage(toBeAligned);
	MyImageReader::registration.Update();

	return registration.GetOutput();
}

QList<vtkSmartPointer<vtkImageData>> MyImageReader::getListOfVtkImages()
{
	return this->listOfVtkImages;
}

QList<Image<float, 3>::Pointer> MyImageReader::getListOfItkImages()
{
	return this->listOfItkImages;
}

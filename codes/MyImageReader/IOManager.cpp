#include "IOManager.h"

#include "MyImageManager.h"
#include "ImageRegistration.h"

#include <QFileDialog>

#include <itkImageFileReader.h>
#include <itkMetaDataDictionary.h>
#include <itkOrientImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkImageSeriesReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkImageFileWriter.h>

#include "RegistrationWizard.h"

using namespace itk;
IOManager::IOManager(QWidget* parent) 
	:registration(parent), QWidget(parent)
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

void IOManager::addToListOfFileNames(QStringList fileNames)
{
	this->listOfFileNames.append(fileNames);
}

const QList<QStringList> IOManager::getListOfFileNames()
{
	return this->listOfFileNames;
}

bool IOManager::LoadImageData(QStringList fileNames)
{
	ImageType::Pointer _itkImage = NULL;
	vtkSmartPointer<vtkImageData> _vtkImage = NULL;
	vtkSmartPointer<vtkImageData> _vtkImageCopy = NULL;
	// QMAP saving DICOM imformation
	QMap<QString, QString>* DICOMHeader = NULL;
	if (fileNames.isEmpty()) {
		_itkImage = NULL;
		_vtkImage = NULL;
		DICOMHeader = NULL;
		_vtkImageCopy = NULL;
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
		// using the same m_orientation ITK_COORDINATE_ORIENTATION_RAI
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
		_vtkImageCopy = vtkSmartPointer<vtkImageData>::New();
		_vtkImageCopy->DeepCopy(_vtkImage);
	}

	this->myImageManager->listOfItkImages.append( _itkImage);
	this->myImageManager->listOfVtkViewerInputImages.append(_vtkImageCopy);
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

void IOManager::slotOpenWithWizard()
{
	this->slotOpenWithWizard("");
}

void IOManager::slotOpenWithWizard(QString dir)
{
	RegistrationWizard wizard(dir);
	if (wizard.exec() == QWizard::Rejected)
		return;
	this->listOfFileNames.clear();
	for (int i = 0; i < 5; ++i) {
		if (wizard.getFileNamesN(i + 1) == NULL) {
			addToListOfFileNames(QStringList());
		}
		else {
			this->addToListOfFileNames(*wizard.getFileNamesN(i + 1));
		}
	}

	//QStringList* wizardFileNames[5] = {
	//	wizard.getFileNames1(), wizard.getFileNames2(), wizard.getFileNames3(),
	//	wizard.getFileNames4(), wizard.getFileNames5() };
	//for (int i = 0; i < 5; ++i) {
	//	if (wizardFileNames[i] == NULL) {
	//		wizardFileNames[i];
	//	}
	//	this->addToListOfFileNames(*wizardFileNames[i]);
	//}
	slotOpenMultiImages();
}

void IOManager::slotOpenMultiImages()
{
	// initialize modality names
	this->myImageManager->allClear();
	this->myImageManager->listOfModalityNames += "CUBE T1";
	this->myImageManager->listOfModalityNames += "CUBE T2";
	this->myImageManager->listOfModalityNames += "CUBE T1+C";
	this->myImageManager->listOfModalityNames += "2D DIR/QIR";
	this->myImageManager->listOfModalityNames += "MPRAGE";

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
	this->myImageManager->overlay->Initialize(
		this->myImageManager->listOfVtkViewerInputImages[0]);

	emit finishOpenMultiImages();
}

void IOManager::slotOpenOneImage(QStringList fileNames)
{
	this->LoadImageData(fileNames);
	emit finishOpenOneImage();
}

void IOManager::slotOpenSegmentationWithDiaglog()
{
	QString path = QFileDialog::getOpenFileName(this, QString(tr("Open Segmentation")), 
		".", tr("NlFTI Images (*.nii)"));
	if (path.isEmpty()) return;
	this->slotOpenSegmentation(path);
	emit finishOpenSegmentation();
}

void IOManager::slotOpenSegmentation(QString fileName)
{
	ImageType::Pointer _itkImage;
	//vtkSmartPointer<vtkImageData> _vtkImage;
	
	ImageFileReader<Image<float, 3>>::Pointer reader =
		ImageFileReader<Image<float, 3>>::New();
	reader->SetFileName(fileName.toStdString());
	reader->Update();
	_itkImage = reader->GetOutput();
	this->myImageManager->overlay->SetInputImageData(_itkImage);

	//if (_itkImage.IsNotNull()) {
	//	// using the same m_orientation ITK_COORDINATE_ORIENTATION_RAI
	//	OrientImageFilter<Image<float, 3>, Image<float, 3>>::Pointer orienter =
	//		OrientImageFilter<Image<float, 3>, Image<float, 3>>::New();
	//	orienter->UseImageDirectionOn();
	//	orienter->SetDesiredCoordinateOrientation(
	//		itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	//	orienter->SetInput(_itkImage);
	//	orienter->Update();
	//	_itkImage = orienter->GetOutput();
	//	ImageToVTKImageFilter<Image<float, 3>>::Pointer connector =
	//		ImageToVTKImageFilter<Image<float, 3>>::New();
	//	connector->SetInput(_itkImage);
	//	connector->Update();
	//	_vtkImage = connector->GetOutput();
	//}
}

void IOManager::slotSaveSegmentaitonWithDiaglog()
{
	QString path = QFileDialog::getSaveFileName(this, QString(tr("Save Segmentation")),
		".", tr("NlFTI Images (*.nii)"));
	if (path.isEmpty())	return;
	slotSaveSegmentation(path);
	
}

void IOManager::slotSaveSegmentation(QString path)
{
	ImageFileWriter<Image<float, 3>>::Pointer writer =
		ImageFileWriter<Image<float, 3>>::New();
	writer->SetInput(this->myImageManager->overlay->GetITKOutput(
		this->myImageManager->listOfItkImages[0]));
	writer->SetFileName(path.toStdString().c_str());
	writer->Write();
}

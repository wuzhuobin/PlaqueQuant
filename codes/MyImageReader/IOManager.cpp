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

#include <vtkAppendPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPointData.h>

#include "RegistrationWizard.h"

using namespace itk;
IOManager::IOManager(QObject* parent, QWidget* mainWindow)
	:registration(mainWindow), QObject(parent)
{
}
IOManager::IOManager(QWidget* parent)
	: IOManager(parent, parent)
{
}

IOManager::~IOManager()
{
}

void IOManager::enableRegistration(bool flag)
{
	this->registrationFlag = flag;
}

void IOManager::addToListOfFileNames(QStringList fileNames)
{
	this->listOfFileNames.append(fileNames);
}

void IOManager::cleanListsOfFileNames()
{
	this->listOfFileNames.clear();
}

const QList<QStringList> IOManager::getListOfFileNames()
{
	return this->listOfFileNames;
}

void IOManager::setFilePath(QString filePath)
{
	this->filePath = filePath;
}

const QString IOManager::getFilePath()
{
	return this->filePath;
}

bool IOManager::loadImageData(QStringList fileNames)
{
	ImageType::Pointer _itkImage = nullptr;
	vtkSmartPointer<vtkImageData> _vtkImage = nullptr;
	vtkSmartPointer<vtkImageData> _vtkImageCopy = nullptr;
	// QMAP saving DICOM imformation
	QMap<QString, QString>* DICOMHeader = nullptr;
	if (fileNames.isEmpty()) {
		_itkImage = nullptr;
		_vtkImage = nullptr;
		DICOMHeader = nullptr;
		_vtkImageCopy = nullptr;
	}
	// load Nifti Data
	else if (fileNames.size() == 1 && fileNames[0].contains(".nii")) {
		ImageFileReader<Image<float, 3>>::Pointer reader =
			ImageFileReader<Image<float, 3>>::New();
		reader->SetFileName(fileNames[0].toStdString());
		reader->Update();
		_itkImage = reader->GetOutput();
		DICOMHeader = nullptr;
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
	}

	this->myImageManager->listOfItkImages.append( _itkImage);
	this->myImageManager->listOfVtkImages.append(_vtkImage);
	this->myImageManager->listOfDICOMHeader.append(DICOMHeader);

	return true;
}

IOManager::ImageType::Pointer IOManager::imageAlignment(ImageType::Pointer alignedTo, ImageType::Pointer toBeAligned)
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

//void IOManager::setUniqueKeys(QStringList keys)
//{
//	this->uniqueKeys = keys;
//}

void IOManager::slotOpenWithWizard()
{
	this->slotOpenWithWizard("");
}

void IOManager::slotOpenWithWizard(QString dir)
{
	RegistrationWizard wizard(dir);
	if (wizard.exec() == QWizard::Rejected)
		return;
	filePath = wizard.getDirectory();
	this->listOfFileNames.clear();
	for (int i = 0; i < 5; ++i) {
		if (wizard.getFileNamesN(i + 1) == nullptr) {
			addToListOfFileNames(QStringList());
		}
		else {
			this->addToListOfFileNames(*wizard.getFileNamesN(i + 1));
		}
	}

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
		this->myImageManager->listOfVtkImages[0]);

	emit finishOpenMultiImages();
}

void IOManager::slotOpenOneImage(QStringList fileNames)
{
	loadImageData(fileNames);
	emit finishOpenOneImage();
}

void IOManager::slotOpenSegmentationWithDiaglog()
{
	QString path = QFileDialog::getOpenFileName(dynamic_cast<QWidget*>(this->parent()),
		QString(tr("Open Segmentation")), ".", tr("NlFTI Images (*.nii)"));
	if (path.isEmpty()) return;
	this->slotOpenSegmentation(path);
	emit finishOpenSegmentation();
}

void IOManager::slotOpenSegmentation(QString fileName)
{
	Overlay::OverlayImageType::Pointer _itkImage;
	ImageFileReader<Overlay::OverlayImageType>::Pointer reader =
		ImageFileReader<Overlay::OverlayImageType>::New();
	reader->SetFileName(fileName.toStdString());
	reader->Update();
	_itkImage = reader->GetOutput();
	this->myImageManager->overlay->SetInputImageData(_itkImage);

	//if (_itkImage.IsNotnullptr()) {
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
	QString path = QFileDialog::getSaveFileName(dynamic_cast<QWidget*>(this->parent()), 
		QString(tr("Save Segmentation")), ".", tr("NlFTI Images (*.nii)"));
	if (path.isEmpty())	return;
	slotSaveSegmentation(path);
	
}

void IOManager::slotSaveSegmentation(QString path)
{
	ImageFileWriter<Overlay::OverlayImageType>::Pointer writer =
		ImageFileWriter<Overlay::OverlayImageType>::New();
	writer->SetInput(this->myImageManager->overlay->GetITKOutput<ImageType>(
		this->myImageManager->listOfItkImages[0]));
	writer->SetFileName(path.toStdString().c_str());
	writer->Write();
}

void IOManager::slotSaveContourWithDiaglog()
{
	QString path = QFileDialog::getSaveFileName(dynamic_cast<QWidget*>(this->parent()),
		QString(tr("Save Contours")), ".", tr("Serial vtkPolyData(*.vtp)"));
	if (path.isEmpty())	return;
	slotSaveContour(path);
}

void IOManager::slotSaveContour(QString fileName)
{
	vtkSmartPointer<vtkAppendPolyData> append =
		vtkSmartPointer<vtkAppendPolyData>::New();


	Overlay* overlay = this->myImageManager->getOverlay();
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>*  contours[2] =
	{ overlay->GetLumenPolyData(), overlay->GetVesselWallPolyData() };
	for (int i = 0; i < 2; ++i) {



		for (QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>::const_iterator cit1 =
			contours[i]->cbegin(); cit1 != contours[i]->cend();++cit1) {
			
			QList<vtkSmartPointer<vtkPolyData>>* _list = cit1.value();
			for (QList<vtkSmartPointer<vtkPolyData>>::const_iterator cit2 = _list->cbegin();
				cit2 != _list->cend(); ++cit2) {

				// set point data for classification
				vtkSmartPointer<vtkUnsignedCharArray> scalars =
					vtkSmartPointer<vtkUnsignedCharArray>::New();
				scalars->SetNumberOfValues((*cit2)->GetNumberOfPoints());
				scalars->SetNumberOfComponents(1);
				scalars->FillComponent(0, i + 1);

				(*cit2)->GetPointData()->SetScalars(scalars);
				append->AddInputData(*cit2);
			}


		}
	}
	append->Update();

	vtkSmartPointer<vtkXMLPolyDataWriter> writer =
		vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	writer->SetInputConnection(append->GetOutputPort());
	writer->SetFileName(fileName.toStdString().c_str());
	writer->Write();
}

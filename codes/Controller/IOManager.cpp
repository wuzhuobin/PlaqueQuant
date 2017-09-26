#include "IOManager.h"

//#include "MyImageManager.h"
#include "ImageRegistration.h"

#include <QFileDialog>
#include <qdebug.h>

#include <itkImageFileReader.h>
#include <itkMetaDataDictionary.h>
#include <itkOrientImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileWriter.h>

#include <vtkAppendPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPointData.h>


using itk::GDCMImageIO;
using itk::ImageFileReader;
using itk::ImageSeriesReader;
using itk::OrientImageFilter;


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

void IOManager::slotAddToListOfFileNamesAndOpen(QStringList listOfFileNames)
{
	clearListsOfFileNames();
	
	for (int i = 0; i < listOfFileNames.size(); ++i) {
		QStringList fileNames = listOfFileNames.at(i).split(';');
		fileNames.pop_back();
		slotAddToListOfFileNames(fileNames);
	}

	slotOpenMultiImages();
}

void IOManager::slotAddToListOfFileNames(QStringList fileNames)
{
	this->listOfFileNames.append(fileNames);
}

void IOManager::slotAddToListOfFileNames(QStringList * fileNames)
{
	slotAddToListOfFileNames(*fileNames);
}

void IOManager::clearListsOfFileNames()
{
	this->listOfFileNames.clear();
}

const QList<vtkSmartPointer<IVtkImageData>> IOManager::getListOfImage() const
{
	return this->listOfImages;
}

const QList<QStringList> IOManager::getListOfFileNames() const
{
	return this->listOfFileNames;
}

const QList<GDCMImageIO::Pointer> IOManager::getListOfDicomIOs() const
{
	return this->listOfDicomIOs;
}

void IOManager::clearListOfDicoms()
{
	this->listOfDicomIOs.clear();
}

const QSharedPointer<Overlay> IOManager::getOverlay() const
{
	return overlay;
}


bool IOManager::loadImageData(QStringList fileNames)
{

	IVtkImageData::itkImageType::Pointer _itkImage = nullptr;
	GDCMImageIO::Pointer _dicomIO = nullptr;

	if (fileNames.isEmpty() || fileNames.value(0) == QString()) {
		_itkImage = nullptr;
		_dicomIO = nullptr;
	}
	// load Nifti Data
	else if (fileNames.size() == 1) {
		ImageFileReader<IVtkImageData::itkImageType>::Pointer reader;

		try {
			reader = ImageFileReader<IVtkImageData::itkImageType>::New();
			reader->SetFileName(fileNames[0].toStdString());
			reader->SetImageIO(_dicomIO);
			reader->Update();
			_itkImage = reader->GetOutput();

		}
		catch (itk::ImageFileReaderException& e) {
			qDebug() << "Fail to read Dicom header";
			qDebug() << e.what();
			qDebug() << "Description: " << e.GetDescription();
			reader = ImageFileReader<IVtkImageData::itkImageType>::New();
			reader->SetFileName(fileNames[0].toStdString());
			reader->Update();
			_itkImage = reader->GetOutput();
		}
	}
	// Load Dicom Data
	else {
		std::vector<std::string> _fileNames;
		for (QStringList::const_iterator constIterator = fileNames.constBegin();
			constIterator != fileNames.constEnd(); ++constIterator) {
			_fileNames.push_back(constIterator->toStdString());
		}
		_dicomIO = GDCMImageIO::New();

		ImageSeriesReader<IVtkImageData::itkImageType>::Pointer reader =
			ImageSeriesReader<IVtkImageData::itkImageType>::New();
		reader->SetFileNames(_fileNames);
		reader->SetImageIO(_dicomIO);
		reader->Update();
		_itkImage = reader->GetOutput();

	}
	if (_itkImage.IsNotNull()) {
		typedef itk::OrientImageFilter<IVtkImageData::itkImageType, IVtkImageData::itkImageType> OrientImageFilter;

		// using the same m_orientation ITK_COORDINATE_ORIENTATION_RAI
		OrientImageFilter::Pointer orienter =
			OrientImageFilter::New();
		orienter->UseImageDirectionOn();
		orienter->SetDesiredCoordinateOrientation(
			itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
		orienter->SetInput(_itkImage);
		orienter->Update();
		_itkImage = orienter->GetOutput();
		// Image Registration
		if (this->registrationFlag && 
			this->listOfImages.size() > 0 && 
			this->listOfImages[0]) {
			_itkImage = imageAlignment(this->listOfImages[0]->GetItkImage(), _itkImage);
		}
		vtkSmartPointer<IVtkImageData> _image =
			vtkSmartPointer<IVtkImageData>::New();
		_image->Graft(_itkImage);
		listOfImages << _image;
	}
	else {
		listOfImages << nullptr;
	}
	if (_dicomIO) {
		listOfDicomIOs << _dicomIO;
	}
	else {
		listOfDicomIOs << nullptr;
	}
	return true;
}

IVtkImageData::itkImageType::Pointer IOManager::imageAlignment(
	IVtkImageData::itkImageType::Pointer alignedTo, 
	IVtkImageData::itkImageType::Pointer toBeAligned)
{
	this->registration.SetFixedImage(alignedTo);
	this->registration.SetMovingImage(toBeAligned);
	this->registration.Update();

	return registration.GetOutput();
}

void IOManager::slotOpenMultiImages()
{
	slotCleanImagesAndDicomIOs();

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
		loadImageData(*cit);
	}

	emit signalFinishOpenMultiImages();
}

void IOManager::slotOpenOneImage(QStringList fileNames)
{
	bool _flag = this->registrationFlag;
	this->registrationFlag = false;
	loadImageData(fileNames);
	this->registrationFlag = _flag;
	emit signalFinishOpenOneImage();
}

void IOManager::slotCleanImagesAndDicomIOs()
{
	this->listOfDicomIOs.clear();
	this->listOfImages.clear();
}

void IOManager::slotInitializeOverlay()
{
	if (listOfImages.size() > 1 && listOfImages[0]){
		slotInitializeOverlay(listOfImages[0]->GetItkImage());
	}
}

void IOManager::slotInitializeOverlay(IVtkImageData::itkImageType::Pointer image)
{

	OverlayImageData::itkImageType::Pointer _overlay
		= OverlayImageData::itkImageType::New();
	_overlay->SetRegions(image->GetLargestPossibleRegion());
	_overlay->SetDirection(image->GetDirection());
	_overlay->SetOrigin(image->GetOrigin());
	_overlay->SetSpacing(image->GetSpacing());
	_overlay->Allocate();
	_overlay->FillBuffer(0);

	overlay = QSharedPointer<Overlay>(new Overlay(_overlay));

	emit signalFinishOpenOverlay();
}

void IOManager::slotOpenOverlay(QString fileName)
{
	//typedef itk::OrientImageFilter<OverlayImageData::itkImageType, OverlayImageData::itkImageType> OrientImageFilter;

	ImageFileReader<OverlayImageData::itkImageType>::Pointer reader =
		ImageFileReader<OverlayImageData::itkImageType>::New();
	reader->SetFileName(fileName.toStdString());
	reader->Update();
	OverlayImageData::itkImageType::Pointer _overlay = reader->GetOutput();

	// using the same m_orientation ITK_COORDINATE_ORIENTATION_RAI
	OrientImageFilter<OverlayImageData::itkImageType, OverlayImageData::itkImageType>::Pointer orienter =
		OrientImageFilter<OverlayImageData::itkImageType, OverlayImageData::itkImageType>::New();
	orienter->UseImageDirectionOn();
	orienter->SetDesiredCoordinateOrientation(
		itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orienter->SetInput(_overlay);
	orienter->Update();
	_overlay = orienter->GetOutput();

	overlay = QSharedPointer<Overlay>(new Overlay(_overlay));

	emit signalFinishOpenOverlay();

}
void IOManager::slotSaveOverlay(QString path)
{
	slotSaveOverlay(overlay->getData()->GetItkImage(), path);
}
void IOManager::slotSaveOverlay(OverlayImageData::itkImageType::Pointer input, QString path)
{
	typedef itk::ImageFileWriter<OverlayImageData::itkImageType> ImageFileWriter;
		ImageFileWriter::Pointer writer =
			ImageFileWriter::New();
		writer->SetInput(input);
		writer->SetFileName(path.toStdString().c_str());
		writer->Write();
}


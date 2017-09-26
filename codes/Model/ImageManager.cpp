#include "ImageManager.h"

#include <itkCastImageFilter.h>

ImageManager::ImageManager(unsigned int numOfImages, QObject * parent)
{
	setNumOfImages(numOfImages);
}

ImageManager::~ImageManager()
{
}

void ImageManager::setNumOfImages(unsigned int num)
{
	m_images.clear();
	m_modalityNames.clear();
	m_dicomIOs.clear();
	for (int i = 0; i < num; ++i) {
		m_images << nullptr;
		m_modalityNames << QString();
		m_dicomIOs << nullptr;
	}
}

unsigned int ImageManager::getNumOfImages() {
	return m_images.size();
}

//bool ImageManager::setOverlay()
//{
//	if (m_images[0] != nullptr) {
//		setOverlay(m_images[0]);
//		return true;
//	}
//	else
//		return false;
//}

//bool ImageManager::setOverlay(IVtkImageData * image)
//{
//	if (image != nullptr) {
//		setOverlay(image->GetItkImage());
//		return true;
//	}
//	else
//		return false;
//}

//bool ImageManager::setOverlay(IVtkImageData::itkImageType::Pointer image)
//{
//	typedef itk::CastImageFilter<IVtkImageData::itkImageType, OverlayImageData::itkImageType> CastImageFilter;
//	
//	if (image.IsNotNull()) {
//		CastImageFilter::Pointer castImageFilter = CastImageFilter::New();
//		castImageFilter->SetInput(image);
//		castImageFilter->Update();
//
//		m_overlay = QSharedPointer<Overlay>(new Overlay(castImageFilter->GetOutput()));
//
//		return true;
//	}
//	else
//		return false;
//}

//bool ImageManager::setOverlay(OverlayImageData::itkImageType::Pointer image)
//{
//
//	if (image.IsNotNull()) {
//		m_overlay = QSharedPointer<Overlay>(new Overlay(image));
//
//		return true;
//	}
//	else
//		return false;
//}

bool ImageManager::setOverlay(QSharedPointer<Overlay> overlay){
	if (overlay) {
		m_overlay = overlay;
		return true;
	}
	else {
		return false;
	}
}

//bool ImageManager::setImage(unsigned int i, IVtkImageData::itkImageType::Pointer image)
//{
//	if (i >= m_images.size()) {
//		return false;
//	}
//	if (!image) {
//		m_images[i] = nullptr;
//		return false;
//	}
//	vtkSmartPointer<IVtkImageData> _image =
//		vtkSmartPointer<IVtkImageData>::New();
//	_image->Graft(image);
//	m_images[i] = _image;
//	return true;
//}

//bool ImageManager::setImage(unsigned int i, vtkImageData * image)
//{
//	if (i >= m_images.size()) {
//		return false;
//	}
//	if (!image) {
//		m_images[i] = nullptr;
//		return false;
//	}
//	vtkSmartPointer<IVtkImageData> _image =
//		vtkSmartPointer<IVtkImageData>::New();
//	_image->ShallowCopy(image);
//	m_images[i] = _image;
//	return true;
//}

bool ImageManager::setImage(unsigned int i, IVtkImageData * image)
{
	if (i >= m_images.size()) {
		return false;
	}
	if (!image) {
		m_images[i] = nullptr;
		return false;
	}
	vtkSmartPointer<IVtkImageData> _image =
		vtkSmartPointer<IVtkImageData>::New();
	_image->ShallowCopy(image);
	m_images[i] = _image;
	return true;
}

bool ImageManager::setModalityName(unsigned int i, QString name)
{
	if (i >= m_modalityNames.size()) {
		return false;
	}
	m_modalityNames[i] = name;
	return true;
}

bool ImageManager::setDicomIO(unsigned int i, itk::GDCMImageIO::Pointer dicomIO)
{
	if (i >= m_dicomIOs.size()) {
		return false;
	}
	m_dicomIOs[i] = dicomIO;
	return true;
}

IVtkImageData * ImageManager::getImage(unsigned int i) const
{
	if (i >= m_images.size()) {
		return nullptr;
	}
	return m_images[i];
}

IVtkImageData * ImageManager::getImage(QString modalityName) const
{
	unsigned int index = modalityName.indexOf(modalityName);
	if (index < 0) {
		return nullptr;
	}
	return getImage(index);
}

Overlay * ImageManager::getOverlay() const
{
	return m_overlay.data();
}

const QString ImageManager::getModalityName(unsigned int i) const
{
	if (i >= m_modalityNames.size()) {
		return QString();
	}

	return m_modalityNames[i];
}

int ImageManager::getIndexOfModalityName(QString modalityName)
{
	return m_modalityNames.indexOf(modalityName);
}

const itk::GDCMImageIO::Pointer ImageManager::getDicomIO(unsigned int i) const
{
	if (i >= m_dicomIOs.size()) {
		return nullptr;
	}
	return m_dicomIOs[i];
}

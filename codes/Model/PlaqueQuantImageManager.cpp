#include "PlaqueQuantImageManager.h"

#include "PlaqueQuantOverlay.h"

#include <itkCastImageFilter.h>

PlaqueQuantImageManager::PlaqueQuantImageManager(unsigned int numOfImages, QObject * parent)
	:ImageManager(numOfImages, parent)
{
	setNumOfImages(numOfImages);
}

void PlaqueQuantImageManager::setNumOfImages(unsigned int num)
{
	ImageManager::setNumOfImages(num);
	for (int i = 0; i < num; ++i) {
		m_curvedImages << nullptr;
	}
}

//bool PlaqueQuantImageManager::setOverlay(IVtkImageData::itkImageType::Pointer image)
//{
//	typedef itk::CastImageFilter<IVtkImageData::itkImageType, OverlayImageData::itkImageType> CastImageFilter;
//
//	if (image.IsNotNull()) {
//		CastImageFilter::Pointer castImageFilter = CastImageFilter::New();
//		castImageFilter->SetInput(image);
//		castImageFilter->Update();
//
//		m_overlay = QSharedPointer<Overlay>(new PlaqueQuantOverlay(castImageFilter->GetOutput()));
//
//		return true;
//	}
//	else
//		return false;
//}

//bool PlaqueQuantImageManager::setOverlay(OverlayImageData::itkImageType::Pointer image)
//{
//
//	if (image.IsNotNull()) {
//		m_overlay = QSharedPointer<Overlay>(new PlaqueQuantOverlay(image));
//		return true;
//	}
//	else
//		return false;
//}

bool PlaqueQuantImageManager::setCurvedOverlay(vtkImageData * image)
{
	if (image) {
		m_curvedOverlay = QSharedPointer<Overlay>(new PlaqueQuantOverlay());
		m_curvedOverlay->getData()->ShallowCopy(image);
		return true;
	}
	else
		return false;
}

//bool PlaqueQuantImageManager::setCurvedOverlay(IVtkImageData::itkImageType::Pointer image)
//{
//	typedef itk::CastImageFilter<IVtkImageData::itkImageType, OverlayImageData::itkImageType> CastImageFilter;
//
//	if (image.IsNotNull()) {
//		CastImageFilter::Pointer castImageFilter = CastImageFilter::New();
//		castImageFilter->SetInput(image);
//		castImageFilter->Update();
//
//		m_overlay = QSharedPointer<Overlay>(new PlaqueQuantOverlay(castImageFilter->GetOutput()));
//
//		return true;
//	}
//	else
//		return false;
//}

//bool PlaqueQuantImageManager::setCurvedOverlay(OverlayImageData::itkImageType::Pointer image)
//{
//	if (image.IsNotNull()) {
//		m_overlay = QSharedPointer<Overlay>(new PlaqueQuantOverlay(image));
//		return true;
//	}
//	else
//		return false;
//}

//bool PlaqueQuantImageManager::setCurvedImage(unsigned int i, IVtkImageData::itkImageType::Pointer image)
//{
//	if (i >= m_curvedImages.size()) {
//		return false;
//	}
//	if (!image) {
//		m_curvedImages[i] = nullptr;
//		return false;
//	}
//	vtkSmartPointer<IVtkImageData> _image =
//		vtkSmartPointer<IVtkImageData>::New();
//	_image->Graft(image);
//	m_curvedImages[i] = _image;
//	return true;
//}

bool PlaqueQuantImageManager::setCurvedImage(unsigned int i, vtkImageData * image)
{
	if (i >= m_curvedImages.size()) {
		return false;
	}
	if (!image) {
		m_curvedImages[i] = nullptr;
		return false;
	}
	vtkSmartPointer<IVtkImageData> _image =
		vtkSmartPointer<IVtkImageData>::New();
	_image->ShallowCopy(image);
	m_curvedImages[i] = _image;
	return true;
}

IVtkImageData * PlaqueQuantImageManager::getCurvedImage(unsigned int i) const
{
	if (i >= m_curvedImages.size()) {
		return nullptr;
	}
	return m_curvedImages[i];
}

IVtkImageData * PlaqueQuantImageManager::getCurvedImage(QString modalityName) const
{
	unsigned int index = modalityName.indexOf(modalityName);
	if (index < 0) {
		return nullptr;
	}
	return getCurvedImage(index);
}

PlaqueQuantOverlay * PlaqueQuantImageManager::getPlaqueQuantOverlay() const
{
	return static_cast<PlaqueQuantOverlay*>(m_overlay.data());
}

PlaqueQuantOverlay * PlaqueQuantImageManager::getCurvedPlaqueQuantOverlay() const
{
	return static_cast<PlaqueQuantOverlay*>(m_curvedOverlay.data());
}

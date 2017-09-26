#include "IADEImageManager.h"

#include "IADEOverlay.h"

#include <itkCastImageFilter.h>

IADEImageManager::IADEImageManager(unsigned int numOfImages, QObject * parent)
	:ImageManager(numOfImages, parent)
{
	setNumOfImages(numOfImages);
}

IADEOverlay * IADEImageManager::getIADEOverlay() const
{
	return static_cast<IADEOverlay*>(m_overlay.data());
}

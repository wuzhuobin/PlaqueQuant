#ifndef __PLAQUE_QUANT_IMAGE_MANAGER_H__
#define __PLAQUE_QUANT_IMAGE_MANAGER_H__

#include "ImageManager.h"

class PlaqueQuantOverlay;

class PlaqueQuantImageManager: public ImageManager
{
	Q_OBJECT;

public:
	PlaqueQuantImageManager(unsigned int numOfImages = 0, QObject* parent = nullptr);

	virtual void setNumOfImages(unsigned int num) override;

	//virtual bool setOverlay(IVtkImageData::itkImageType::Pointer image);
	//virtual bool setOverlay(OverlayImageData::itkImageType::Pointer image);
	virtual bool setCurvedOverlay(vtkImageData* image);
	//virtual bool setCurvedOverlay(IVtkImageData::itkImageType::Pointer image);
	//virtual bool setCurvedOverlay(OverlayImageData::itkImageType::Pointer image);
	//bool setCurvedImage(unsigned int i, IVtkImageData::itkImageType::Pointer image);
	bool setCurvedImage(unsigned int i, vtkImageData* image);
	IVtkImageData* getCurvedImage(unsigned int i) const;
	IVtkImageData* getCurvedImage(QString modalityName) const;
	PlaqueQuantOverlay* getPlaqueQuantOverlay() const;
	PlaqueQuantOverlay* getCurvedPlaqueQuantOverlay() const;

protected:
	QList<vtkSmartPointer<IVtkImageData>> m_curvedImages;
	QSharedPointer<Overlay> m_curvedOverlay;



private:

};



#endif // !__PLAQUE_QUANT_IMAGE_MANAGER_H__

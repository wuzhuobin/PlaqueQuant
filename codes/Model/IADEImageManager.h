#ifndef __IADE_IMAGE_MANAGER_H__
#define __IADE_IMAGE_MANAGER_H__

#include "ImageManager.h"

class IADEOverlay;

class IADEImageManager: public ImageManager
{
	Q_OBJECT;

public:
	IADEImageManager(unsigned int numOfImages = 0, QObject* parent = nullptr);

	IADEOverlay* getIADEOverlay() const;

protected:


private:

};



#endif // !__IADE_IMAGE_MANAGER_H__

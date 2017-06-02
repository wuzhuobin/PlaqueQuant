#ifndef __IADE_IO_MANAGER_H__
#define __IADE_IO_MANAGER_H__

#include "IOManager.h"

class IADEIOManager :public IOManager 
{
	Q_OBJECT;

public:
	IADEIOManager(QObject* parent = nullptr);

	public slots:
	virtual void slotOpenSegmentation(QString fileName) override;
	
	virtual void slotInitializeOverlay() override;
	
	virtual	void slotInitializeOverlay(IVtkImageData::itkImageType::Pointer image) override;


};



#endif // !__IADE_IO_MANAGER_H__

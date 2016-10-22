#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QStringList>

#include <itkImage.h>

//#include "ImageData.h"
#include "ImageRegistration.h"


class MyImageManager;

typedef itk::Image<float, 3> ImageType;

class IOManager:public QWidget
{
	Q_OBJECT
public:

	IOManager(QWidget* parent = NULL);
	~IOManager();

	void enableRegistration(bool flag);
	virtual void setMyImageManager(MyImageManager* myImageManager);
	virtual void setUniqueKeys(QStringList keys);

	void setListOfFileNames(QList<QStringList> listOfFileNames);
	void addToListOfFileNames(QStringList fineNames);
	const QList<QStringList> getListOfFileNames();
public slots:

	virtual void slotOpenWithWizard();

	virtual void slotOpenWithWizard(QString dir);

	virtual void slotOpenMultiImages();

	//virtual void slotOpenOneImage();
	virtual void slotOpenOneImage(QStringList fileNames);

	//virtual void slotOpenSegmentation();
	virtual void slotOpenSegmentationWithDiaglog();

	virtual void slotOpenSegmentation(QString fileName);

	virtual void slotSaveSegmentaitonWithDiaglog();

	virtual void slotSaveSegmentation(QString path);
	//virtual void slotSaveImage(QString uniqueName, QString path);
	//virtual void slotSaveImage(int index, QString path);

	//virtual void slotSaveSemgentation(QString uniqueName, QString path);
	//virtual void slotSaveSemgentation(int index, QString path);


	//void addFileNames(QStringList fileNames);

signals:
	void finishOpenMultiImages();
	void finishOpenOneImage();
	void finishOpenSegmentation();

protected:
	virtual bool LoadImageData(QStringList fileNames);
	//virtual bool SaveImageData(Image*, QString);


	ImageType::Pointer imageAlignment(ImageType::Pointer alignedTo,
		ImageType::Pointer toBeAligned);

	//virtual Overlay* LoadSegmentation();
	//virtual bool SaveSegmentation(Overlay*, QString);

private:
	QList<QStringList> listOfFileNames;
	QStringList uniqueKeys; 
	MyImageManager* myImageManager;
	bool registrationFlag = false;
	ImageRegistration registration;

};

#endif // !IO_MANAGER_H

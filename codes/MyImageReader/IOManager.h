#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

//#include "ImageData.h"
#include "Overlay.h"
#include "ImageRegistration.h"


class MyImageManager;
class IOManager:public QObject
{
	Q_OBJECT
public:
	IOManager(QObject* parent = NULL);
	~IOManager();

	virtual void setMyImageManager(MyImageManager* myImageManager);


//public slots:

	virtual void slotOpenMultiImages();

	virtual void slotOpenOneImage();
	virtual void slotOpenOneImage(QStringList fileNames);

	virtual void slotOpenSegmentation();
	virtual void slotOpenSegmentation(QString);

	virtual void slotSaveImage(QString uniqueName, QString path);
	virtual void slotSaveImage(int index, QString path);

	virtual void slotSaveSemgentation(QString uniqueName, QString path);
	virtual void slotSaveSemgentation(int index, QString path);


	void addFileNames(QStringList fileNames);

	void setListOfFileNames(QList<QStringList> listOfFileNames);
	const QList<QStringList> getListOfFileNames();

protected:
	virtual bool LoadImageData(QStringList fileNames);
	virtual bool SaveImageData(Image*, QString);


	ImageType::Pointer imageAlignment(ImageType::Pointer alignedTo,
		ImageType::Pointer toBeAligned);

	virtual Overlay* LoadSegmentation();
	virtual bool SaveSegmentation(Overlay*, QString);

private:
	QList<QStringList> listOfFileNames;
	MyImageManager* myImageManager;
	bool registrationFlag = false;
	ImageRegistration registration;

};

#endif // !IO_MANAGER_H

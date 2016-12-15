#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

#include <itkImage.h>

#include "ImageRegistration.h"


class MyImageManager;


class IOManager:public QObject
{
	Q_OBJECT
public:
	typedef itk::Image<float, 3> ImageType;
	IOManager(QWidget* parent = nullptr);
	IOManager(QObject* parent = nullptr, QWidget* mainWindow = nullptr);
	~IOManager();

	void enableRegistration(bool flag);
	void setMyImageManager(MyImageManager* myImageManager);
	//void setUniqueKeys(QStringList keys);

	void setListOfFileNames(QList<QStringList> listOfFileNames);
	void addToListOfFileNames(QStringList fineNames);
	const QList<QStringList> getListOfFileNames();

	void setFilePath(QString filePath);
	const QString getFilePath();

public slots:

	void slotOpenWithWizard();

	void slotOpenWithWizard(QString dir);

	void slotOpenMultiImages();

	void slotOpenOneImage(QStringList fileNames);

	void slotOpenSegmentationWithDiaglog();

	void slotOpenSegmentation(QString fileName);

	void slotSaveSegmentaitonWithDiaglog();

	void slotSaveSegmentation(QString path);

	void slotSaveContourWithDiaglog();
	
	void slotSaveContour(QString fileName);

signals:
	void finishOpenMultiImages();
	void finishOpenOneImage();
	void finishOpenSegmentation();

protected:
	bool loadImageData(QStringList fileNames);


	ImageType::Pointer imageAlignment(ImageType::Pointer alignedTo,
		ImageType::Pointer toBeAligned);

private:
	QList<QStringList> listOfFileNames;
	QString filePath;
	QStringList uniqueKeys; 
	MyImageManager* myImageManager;
	bool registrationFlag = false;
	ImageRegistration registration;

};

#endif // !IO_MANAGER_H

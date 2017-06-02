#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <qsharedpointer.h>

#include <itkImage.h>
#include <itkGDCMImageIO.h>

#include "ImageRegistration.h"
#include "Overlay.h"
#include "IVtkImageData.h"


class IOManager:public QObject
{
	Q_OBJECT;
public:

	IOManager(QObject* parent = nullptr);
	~IOManager();

	void enableRegistration(bool flag);

	const QList<QStringList> getListOfFileNames() const;

	/**
	* clean the list of FileNames, #listOfFileNames
	*/
	void clearListsOfFileNames();

	const QList<vtkSmartPointer<IVtkImageData>> getListOfImage() const;

	const QList<itk::GDCMImageIO::Pointer> getListOfDicomIOs() const;

	void clearListOfDicoms();

	const QSharedPointer<Overlay> getOverlay() const;

public slots:

	void slotAddToListOfFileNamesAndOpen(QList<QStringList>* listOfFileNames);
	/**
	 * @brief	add a file name to #listOfFileNames
	 * @param	fileNames	a image fileName.
	 */
	void slotAddToListOfFileNames(QStringList fileNames);

	void slotAddToListOfFileNames(QStringList* fileNames);


	void slotOpenMultiImages();

	void slotOpenOneImage(QStringList fileNames);

	void slotCleanImagesAndDicomIOs();

	virtual void slotInitializeOverlay();

	virtual void slotInitializeOverlay(IVtkImageData::itkImageType::Pointer image);

	virtual void slotOpenSegmentation(QString fileName);

	virtual void slotSaveSegmentation(QString path);

	void slotSaveSegmentation(OverlayImageData::itkImageType::Pointer input, QString path);

	virtual void slotGenerateReport(QString path);

signals:
	void signalFinishOpenMultiImages();

	void signalFinishOpenOneImage();

	void signalFinishOpenOverlay();

protected:
	bool loadImageData(QStringList fileNames);


	IVtkImageData::itkImageType::Pointer imageAlignment(IVtkImageData::itkImageType::Pointer alignedTo,
		IVtkImageData::itkImageType::Pointer toBeAligned);

	QList<QStringList> listOfFileNames;
	QList<vtkSmartPointer<IVtkImageData>> listOfImages;
	QList<itk::GDCMImageIO::Pointer> listOfDicomIOs;

	QSharedPointer<Overlay> overlay;

	//QString filePath;
	bool registrationFlag = false;
	ImageRegistration registration;

};

#endif // !IO_MANAGER_H

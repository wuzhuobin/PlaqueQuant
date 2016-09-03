#ifndef __MYIMAGEMANAGER_H__
#define __MYIMAGEMANAGER_H__

#include <QObject>
#include <QList>
#include <QStringList>
#include <QMap>

#include <vtkImageData.h>
#include <vtkSmartPointer.h>

#include <itkImage.h>
using namespace itk;


class MyImageManager : public QObject
{
	Q_OBJECT
public:
	MyImageManager(QObject* parent = NULL);
	~MyImageManager();

	/**
	 * load Images whose path are listOfFileNames
	 * @return	true load images successfully
	 *			false load images fail
	 */
	int getNumberOfImages();
	const QList<vtkSmartPointer<vtkImageData>> getListOfVtkImages();
	const QList<Image<float, 3>::Pointer> getListOfItkImages();
	const QList<QMap<QString, QString>*> getListOfDICOMHeader();

	const QMap<QString, Image<float, 3>::Pointer> getMapOfItkImages();
	const QMap<QString, vtkSmartPointer<vtkImageData>> getMapOfVtkImages();
	const QMap<QString, QString>* getDICOMHeader(Image<float, 3>::Pointer itkImage);

private:
	friend class IOManager;
	
	// QList
	QList<Image<float, 3>::Pointer> listOfItkImages;
	QList<vtkSmartPointer<vtkImageData>> listOfVtkImages;
	QList<QMap<QString, QString>*> listOfDICOMHeader;

	// QMap
	QMap<QString, Image<float, 3>::Pointer> mapOfItkImages;
	QMap<QString, vtkSmartPointer<vtkImageData>> mapOfVtkImages;
	QMap<Image<float, 3>::Pointer, QMap<QString, QString>*> mapOfDICOMHeader;

};

#endif // !__MYIMAGEMANAGER_H__

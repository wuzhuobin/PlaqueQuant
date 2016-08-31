#ifndef __MYIMAGEREADER_H__
#define __MYIMAGEREADER_H__

#include <qobject.h>
#include <qstringlist.h>
#include <qlist.h>
#include <qmap.h>

#include <vtkImageData.h>
#include <vtkSmartPointer.h>

#include "ImageRegistration.h"
using namespace itk;


class MyImageReader : public QObject
{
	Q_OBJECT
public:
	MyImageReader(QList<QStringList> listOfFileNames = QList<QStringList>(),
		bool registrationFlag = false, QObject* parent = NULL);
	~MyImageReader();

	/**
	 * load Images whose path are listOfFileNames
	 * @return	true load images successfully
	 *			false load images fail
	 */
	bool loadImages();
	void enableRegistration(bool flag);
	QList<QStringList> getListOfFileNames();
	ImageType::Pointer imageAlignment(ImageType::Pointer alignedTo, 
		ImageType::Pointer toBeAligned);

	QList<vtkSmartPointer<vtkImageData>> getListOfVtkImages();
	QList<Image<float, 3>::Pointer> getListOfItkImages();


//private:
	QList<QStringList> listOfFileNames;
	QList<Image<float, 3>::Pointer> listOfItkImages;
	QList<vtkSmartPointer<vtkImageData>> listOfVtkImages;
	QMap<QString, Image<float, 3>::Pointer> mapOfItkImages;
	QMap<QString, vtkSmartPointer<vtkImageData>> mapOfVtkImages;
	QMap<Image<float, 3>::Pointer, QMap<QString, QString>> mapOfDICOMHeader;

	bool registrationFlag = false;
	ImageRegistration registration;
};

#endif // !__MYIMAGEREADER_H__

#include <qdebug.h>
#include <qstring.h>
#include <qfileinfo.h>
#include <qdir.h>

#include "MyImageReader.h"

////Hide the cmd
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
using namespace std;
int main(int argc, char** argv)
{
	QFileInfoList fileInfoList = QDir(
		//"C:/Users/jieji/Desktop/ccode/blood_vessel/JackyData/dicom/CUBE_T1/1"
		"E:/Andy/blood_vessel_v_1.0.0/Data/JackyData/dicom/MPRAGE/1"
	).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
	QStringList fileList;
	for (QFileInfoList::ConstIterator cir = fileInfoList.cbegin();
		cir != fileInfoList.cend(); ++cir) {
		fileList.append(cir->absoluteFilePath());
	}
	//QStringList fileList = QStringList(QFileInfo(
	//	"C:/Users/jieji/Desktop/ccode/blood_vessel/JackyData/nifti_corrected/"
	//	"CUBE T1 corrected.nii").absoluteFilePath());
	QList<QStringList> fileListList;
	fileListList.append(fileList);
	MyImageReader reader(fileListList);
	reader.loadImages();
	//for (int i = 0; i < reader.listOfItkImages.size(); ++i) {
	//	qDebug() << "hi";
	//	QMap<QString, QString> map = reader.mapOfDICOMHeader.value(reader.listOfItkImages[i]);
	//	reader.listOfItkImages[i]->Print(cout);
	//	for (QMap<QString, QString>::ConstIterator cir = map.cbegin();
	//		cir != map.cend(); ++cir) {
	//		qDebug() << cir.key();
	//		qDebug() << cir.value();
	//	}
	//	
	//}
	cin >> *argv;

	return 0;
}

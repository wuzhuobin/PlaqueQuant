#include <qdebug.h>
#include <qstring.h>
#include <qfileinfo.h>

#include "MyImageReader.h"

////Hide the cmd
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
using namespace std;
int main(int argc, char** argv)
{
	QStringList fileList = QStringList(QFileInfo(
		"C:/Users/jieji/Desktop/ccode/blood_vessel/JackyData/nifti_corrected/CUBE T1 corrected.nii").absoluteFilePath());
	QList<QStringList> fileListList;
	fileListList.append(fileList);
	MyImageReader reader(fileListList);
	reader.loadImages();
	for (int i = 0; i < reader.listOfItkImages.size(); ++i) {
		reader.listOfItkImages[i]->Print(std::cout);
	}
	cin >> *argv;

	return 0;
}

#include <QApplication>
#include "MainWindow.h"

////Hide the cmd
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
 
int main( int argc, char** argv )
{
  QApplication app( argc, argv );

  MainWindow mainWnd;
  mainWnd.show();
  
  if (argc==2)
  {
	  QString folder = argv[1];
	  folder.replace("\\","/");
	  mainWnd.slotOpenImage(folder);
  }

  return app.exec();
}

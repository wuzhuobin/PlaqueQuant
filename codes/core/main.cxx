#include <QApplication>
#include "MainWindow.h"

////Hide the cmd
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")

 
int main( int argc, char** argv )
{
  QApplication app( argc, argv );

  MainWindow mainWnd;
  mainWnd.show();
  
  if (argc==2)
  {
	  QDir dir(argv[1]);
	  mainWnd.slotOpenImage(dir.absolutePath());
  }

  return app.exec();
}

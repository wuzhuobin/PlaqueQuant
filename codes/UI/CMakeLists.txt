
project(UI)
SET(
  CMAKE_PREFIX_PATH
  "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"
  )
#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

#VMTK
find_package(VMTK REQUIRED)
if (NOT VMTK_FOUND)
	message(FATAL_ERROR "error: VMTK is not found")
endif()
include(${VMTK_USE_FILE})

if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  # set(CMAKE_AUTOMOC ON)
  find_package(Qt5PrintSupport REQUIRED)
  find_package(Qt5Widgets REQUIRED QUIET)
  # add_definitions(-DQT5_IS_LOADED)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()

#link other libraries
link_directories(
	${CMAKE_SOURCE_DIR}/lib
	)

include_directories(
	${MAIN_INCLUDE_DIRS}
	${PROJECT_BINARY_DIR}
	)


SET(UI_QT_INC 
	${PROJECT_SOURCE_DIR}/ModuleWidget.h
	${PROJECT_SOURCE_DIR}/MyThread.h
	${PROJECT_SOURCE_DIR}/InfoDialog.h
	${PROJECT_SOURCE_DIR}/MainWindow.h
	${PROJECT_SOURCE_DIR}/MyProgressDialog.h
)

SET(UI_QT_SRC 
	${PROJECT_SOURCE_DIR}/ModuleWidget.cxx
	${PROJECT_SOURCE_DIR}/MyThread.cxx
	${PROJECT_SOURCE_DIR}/InfoDialog.cxx
	${PROJECT_SOURCE_DIR}/MainWindow.cxx
	${PROJECT_SOURCE_DIR}/MyProgressDialog.cxx
)

SET(UI_QT_UI 
	${PROJECT_SOURCE_DIR}/ModuleWidget.ui
	${PROJECT_SOURCE_DIR}/InfoDialog.ui
	${PROJECT_SOURCE_DIR}/MainWindow.ui
)

#Generate moc and ui file
QT5_WRAP_UI(UISrcs ${UI_QT_UI})
QT5_WRAP_CPP(MOCSrcs ${UI_QT_INC})

add_library(
	UI 
	${UISrcs}
	${MOCSrcs} 
	${UI_QT_SRC} 
	${UI_QT_INC}
	${UI_QT_UI}
	)

#link libraries
target_link_libraries(
	UI
	${LINK_LIBRARIE}
	)

#Grouping in MSVC
SOURCE_GROUP(
	"Generated Files"
	FILES 
	${UISrcs}
	${MOCSrcs}
	)
SOURCE_GROUP(
	"Forms FIles" 	
	FILES 
	${UI_QT_UI}
	)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${UI_QT_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${UI_QT_SRC}
	)


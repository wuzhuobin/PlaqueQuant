
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
Set(
	QT_RSRS 
	${CMAKE_SOURCE_DIR}/resources/Project.qrc
)

Set(
	RC_FILE
	${CMAKE_SOURCE_DIR}/resources/Project.rc
)
#Generate moc and ui file
QT5_WRAP_UI(UISrcs ${UI_QT_UI})
QT5_WRAP_CPP(MOCSrcs ${UI_QT_INC})
QT5_ADD_RESOURCES(RCC_RSRS ${QT_RSRS})

add_library(
	UI 
	${UISrcs}
	${MOCSrcs} 
	${RCC_RSRS}
	${UI_QT_SRC} 
	${UI_QT_INC}
	${UI_QT_UI}
	${QT_RSRS}
	${RC_FILE}
	)

#link libraries
target_link_libraries(
	UI
	# ${LINK_LIBRARIE}
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	${VMTK_LIBRARIES} 
	Qt5::Widgets 
	Qt5::PrintSupport
	# InteractorStyles
	# AbstractData
	# Widgets 
	# GPURenderWithLUT
	# Measurements
	# Managers
	# MaximumWallThickness
	# CenterlineBranching
	# MyImageReader
	)

#Grouping in MSVC
SOURCE_GROUP(
	"Generated Files"
	FILES 
	${UISrcs}
	${MOCSrcs}
	${RCC_RSRS}
	)
SOURCE_GROUP(
	"UI FILES" 	
	FILES 
	${MODULE_QT_UI}
	)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${MODULE_QT_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${MODULE_QT_SRC}
	)


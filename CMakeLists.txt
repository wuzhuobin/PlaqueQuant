cmake_minimum_required(VERSION 2.8)
 
project( PlaqueQuant )

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)


#ITK
find_package(ITK REQUIRED)
if (NOT ITK_FOUND)
	message(FATAL_ERROR "error: ITK is not found")
endif()
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

#QT
if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  # set(CMAKE_AUTOMOC ON)
  find_package(Qt5 COMPONENTS Widgets REQUIRED)
  find_package(Qt5 COMPONENTS PrintSupport REQUIRED)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
  include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
  # add_definitions(-DQT5_IS_LOADED)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()


#VMTK
find_package(VMTK REQUIRED)
if (NOT VMTK_FOUND)
	message(FATAL_ERROR "error: VMTK is not found")
endif()
include(${VMTK_USE_FILE})

Add_subdirectory(codes)

include_directories(
	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/build
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/codes/core
	${CMAKE_SOURCE_DIR}/codes/Module/Pipeline
	${CMAKE_SOURCE_DIR}/codes/Module/GUI

)

SET(
	QT_SRC
	  ${CMAKE_SOURCE_DIR}/codes/core/main.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/MainWindow.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/InfoDialog.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/MyProgressDialog.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/RegistrationWizard.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/ImageRegistration.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/Overlay.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/SurfaceCreator.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/AbstractFilter.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/ReportGenerator.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/ExtractCenterline.cpp
	  ${CMAKE_SOURCE_DIR}/codes/core/VesselSegmentation.cpp

)

SET(
	QT_HEAD
	  ${CMAKE_SOURCE_DIR}/codes/core/MainWindow.h
	  ${CMAKE_SOURCE_DIR}/codes/core/InfoDialog.h
	  ${CMAKE_SOURCE_DIR}/codes/core/MyProgressDialog.h
	  ${CMAKE_SOURCE_DIR}/codes/core/RegistrationWizard.h
	  ${CMAKE_SOURCE_DIR}/codes/core/ImageRegistration.h
	  ${CMAKE_SOURCE_DIR}/codes/core/Overlay.h
	  ${CMAKE_SOURCE_DIR}/codes/core/SurfaceCreator.h
	  ${CMAKE_SOURCE_DIR}/codes/core/AbstractFilter.h
	  ${CMAKE_SOURCE_DIR}/codes/core/ReportGenerator.h
	  ${CMAKE_SOURCE_DIR}/codes/core/ExtractCenterline.h
	  ${CMAKE_SOURCE_DIR}/codes/core/VesselSegmentation.h
)

SET(
	QT_UI
	  ${CMAKE_SOURCE_DIR}/codes/core/MainWindow.ui
	  ${CMAKE_SOURCE_DIR}/codes/core/InfoDialog.ui
)

SET(
	SRC
	  ${CMAKE_SOURCE_DIR}/codes/core/MyImageViewer.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/MyVtkInteractorStyleImage.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/MyPlaneWidget.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/MultiplanarViewController.cpp
	  ${CMAKE_SOURCE_DIR}/codes/core/MyVtkCommand.cpp

)

SET(
	HEAD
	  ${CMAKE_SOURCE_DIR}/codes/core/MyImageViewer.h
	  ${CMAKE_SOURCE_DIR}/codes/core/MyVtkInteractorStyleImage.h
	  ${CMAKE_SOURCE_DIR}/codes/core/MyPlaneWidget.h
	  ${CMAKE_SOURCE_DIR}/include/Define.h
	  	  ${CMAKE_SOURCE_DIR}/codes/core/MultiplanarViewController.h
	${CMAKE_SOURCE_DIR}/codes/core/MyVtkCommand.h
)

Set(
	 QT_QRC 
	  ${CMAKE_SOURCE_DIR}/resources/Project.qrc
)

Set(
	RESOURCE
	${CMAKE_SOURCE_DIR}/resources/Project.rc
)
#Generate moc and ui file


QT5_WRAP_UI(QT_UI_HEAD ${QT_UI})
QT5_WRAP_CPP(QT_MOC ${QT_HEAD})
QT5_ADD_RESOURCES(QT_RESOURCES ${QT_QRC})

add_executable(
	PlaqueQuant
	MACOSX_BUNDLE 
	${QT_HEAD}
	${QT_SRC}
	${QT_UI}
	${QT_QRC}
	${HEAD}
	${SRC}
	${QT_UI_HEAD}
	${QT_MOC}
	${QT_RESOURCES}
	${RESOURCE}
)

#Link all libraries
find_library(MODULE_LIBRARIES 
	NAMES 	Module.lib
	PATHS 	${CMAKE_SOURCE_DIR}/lib/Debug
			${CMAKE_SOURCE_DIR}/lib/Release
)


target_link_libraries(
	PlaqueQuant
	${Qt5Widgets_LIBRARIES}
	${Qt5PrintSupport_LIBRARIES}
	${VTK_LIBRARIES} 
	${VMTK_LIBRARIES} 
	${ITK_LIBRARIES} 
	${MODULE_LIBRARIES}
)

#Grouping in MSVC
source_group("generated"
	FILES 	${QT_UI_HEAD}
			${QT_MOC}
			${QT_RESOURCES}
)

source_group("form"
	FILES	${QT_UI}
)
source_group("head"
	FILES	${QT_HEAD}
			${HEAD}
			
)
source_group("source"
	FILES	${QT_SRC}
			${SRC}
			${RESOURCE}
		
)
 
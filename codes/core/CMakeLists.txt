cmake_minimum_required(VERSION 2.8)
 
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

#VMTK
find_package(VMTK REQUIRED)
if (NOT VMTK_FOUND)
	message(FATAL_ERROR "error: VMTK is not found")
endif()
include(${VMTK_USE_FILE})

#QT
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


SET(
	QT_SRC
	${CMAKE_SOURCE_DIR}/main.cxx
	${CMAKE_SOURCE_DIR}/MainWindow.cxx
	${CMAKE_SOURCE_DIR}/InfoDialog.cxx
	${CMAKE_SOURCE_DIR}/MyProgressDialog.cxx
	${CMAKE_SOURCE_DIR}/RegistrationWizard.cxx
	${CMAKE_SOURCE_DIR}/ImageRegistration.cxx
	${CMAKE_SOURCE_DIR}/Overlay.cxx
	${CMAKE_SOURCE_DIR}/SurfaceCreator.cxx
	${CMAKE_SOURCE_DIR}/AbstractFilter.cxx
	${CMAKE_SOURCE_DIR}/ReportGenerator.cxx
	${CMAKE_SOURCE_DIR}/ExtractCenterline.cpp
	${CMAKE_SOURCE_DIR}/VesselSegmentation.cpp

)

SET(
	QT_INC
	${CMAKE_SOURCE_DIR}/MainWindow.h
	${CMAKE_SOURCE_DIR}/InfoDialog.h
	${CMAKE_SOURCE_DIR}/MyProgressDialog.h
	${CMAKE_SOURCE_DIR}/RegistrationWizard.h
	${CMAKE_SOURCE_DIR}/ImageRegistration.h
	${CMAKE_SOURCE_DIR}/Overlay.h
	${CMAKE_SOURCE_DIR}/SurfaceCreator.h
	${CMAKE_SOURCE_DIR}/AbstractFilter.h
	${CMAKE_SOURCE_DIR}/ReportGenerator.h
	${CMAKE_SOURCE_DIR}/ExtractCenterline.h
	${CMAKE_SOURCE_DIR}/VesselSegmentation.h
)

SET(
	QT_UI
	${CMAKE_SOURCE_DIR}/MainWindow.ui
	${CMAKE_SOURCE_DIR}/InfoDialog.ui
)

SET(
	CORE_SRC
	${CMAKE_SOURCE_DIR}/MyImageViewer.cxx
	# ${CMAKE_SOURCE_DIR}/codes/core/MyVtkInteractorStyleImage.cxx
	${CMAKE_SOURCE_DIR}/MyPlaneWidget.cxx
)

SET(
	CORE_INC
	${CMAKE_SOURCE_DIR}MyImageViewer.h
	  # ${CMAKE_SOURCE_DIR}/codes/core/MyVtkInteractorStyleImage.h
	${CMAKE_SOURCE_DIR}/MyPlaneWidget.h
	${CMAKE_SOURCE_DIR}/include/Define.h

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

QT5_WRAP_UI(UISrcs ${QT_UI})
QT5_WRAP_CPP(MOCSrcs ${QT_INC})
QT5_ADD_RESOURCES(RCC_RSRS ${QT_RSRS})

add_executable(
	Main
	MACOSX_BUNDLE 
	${QT_WRAP} 
	${UISrcs} 
	${MOCSrcs}
	${QT_SRC}
	${QT_INC}
	${CORE_INC}
	${CORE_SRC}
	${RCC_RSRS}
	${RC_FILE}
)

#Link all libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(
	Main
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets 
	Qt5::PrintSupport
	${VTK_LIBRARIES} 
	${VMTK_LIBRARIES} 
	${ITK_LIBRARIES} 
	Module
)


#Grouping in MSVC
SOURCE_GROUP(
	"_Generated" 	
	FILES 
	${UISrcs}
	${RCC_RSRS}
	${MOCSrcs}
	)
SOURCE_GROUP(
	"UI FILES" 	
	FILES 
	${QT_UI}
	)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${CORE_INC}
	${QT_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${CORE_SRC}
	${QT_SRC}
)
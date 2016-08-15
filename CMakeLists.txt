cmake_minimum_required(VERSION 2.8)
 
PROJECT(Project)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)


SET(
  CMAKE_PREFIX_PATH
  "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"
  )

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
	MAIN_INCLUDE_DIRS
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/codes/core
	${PROJECT_SOURCE_DIR}/build/codes/Module
	${PROJECT_SOURCE_DIR}/codes/Module/GUI
	${PROJECT_SOURCE_DIR}/codes/Module/Pipeline
	${PROJECT_SOURCE_DIR}/codes/AbstractInteractorStyles
	# Source for Widgets
	${PROJECT_SOURCE_DIR}/codes/Widgets

)

include_directories(
	${MAIN_INCLUDE_DIRS}
)


Add_subdirectory(codes)

file(
	GLOB 
	ABSTRACT_INTERACTORS_SRC 
	${CMAKE_SOURCE_DIR}/codes/AbstractInteractorStyles/Abstract*.cxx
	)
file(
	GLOB 
	ABSTRACT_INTERACTORS_H 
	${CMAKE_SOURCE_DIR}/codes/AbstractInteractorStyles/Abstract*.h
	)
file(
	GLOB 
	INTERACTORS_SRC  
	${CMAKE_SOURCE_DIR}/codes/AbstractInteractorStyles/InteractorStyle*.cxx 
	PARENT_SCOPE
	)
file(
	GLOB 
	INTERACTORS_INC  
	${CMAKE_SOURCE_DIR}/codes/AbstractInteractorStyles/InteractorStyle*.h 
	${CMAKE_SOURCE_DIR}/codes/AbstractInteractorStyles/StyleSwitchMacro.h 
	PARENT_SCOPE
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
	QT_INC
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
	CORE_SRC
	  ${CMAKE_SOURCE_DIR}/codes/core/MyImageViewer.cxx
	  # ${CMAKE_SOURCE_DIR}/codes/core/MyVtkInteractorStyleImage.cxx
	  ${CMAKE_SOURCE_DIR}/codes/core/MyPlaneWidget.cxx
	${ABSTRACT_INTERACTORS_SRC}
	${INTERACTORS_SRC}
)

SET(
	CORE_INC
	  ${CMAKE_SOURCE_DIR}/codes/core/MyImageViewer.h
	  # ${CMAKE_SOURCE_DIR}/codes/core/MyVtkInteractorStyleImage.h
	  ${CMAKE_SOURCE_DIR}/codes/core/MyPlaneWidget.h
	  ${CMAKE_SOURCE_DIR}/include/Define.h
	${ABSTRACT_INTERACTORS_H}
	${INTERACTORS_INC}
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
	Project
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

qt5_use_modules(Project Core Gui Widgets PrintSupport)
target_link_libraries(
	Project
#	Qt5::Widgets 
#	Qt5::PrintSupport
	${VTK_LIBRARIES} 
	${VMTK_LIBRARIES} 
	${ITK_LIBRARIES} 
	Module
	Widgets
)


#Grouping in MSVC
SOURCE_GROUP("_Generated" 	FILES ${UISrcs})
SOURCE_GROUP("_Generated" 	FILES ${RCC_RSRS})
SOURCE_GROUP("_Moc Files" 	REGULAR_EXPRESSION moc_.*cpp)
SOURCE_GROUP("UI FILES" 	FILES ${QT_UI})
SOURCE_GROUP("Header Files" FILES ${QT_INC})
SOURCE_GROUP("Header Files" FILES ${CORE_INC})
SOURCE_GROUP("Source Files" FILES ${QT_SRC})
SOURCE_GROUP("Source Files" FILES ${CORE_SRC})

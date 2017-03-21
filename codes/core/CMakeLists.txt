cmake_minimum_required(VERSION 2.8)

project(Core)
include_directories(
	${MAIN_INCLUDE_DIRS}
	)

 # message("${PROJECT_BINARY_DIR}")
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

#Link all libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)


SET(
	QT_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractFilter.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/ExtractCenterline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ReportGenerator.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/SurfaceCreator.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/VesselSegmentation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Core.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/MyImageViewer.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/MyResliceMapper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vtkPolylineToTubularVolume.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vtkvmtkCurvedMPRImageFilterNearestInterpolation.cpp
)

SET(
	QT_INC
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractFilter.h
	${CMAKE_CURRENT_SOURCE_DIR}/ExtractCenterline.h
	${CMAKE_CURRENT_SOURCE_DIR}/ReportGenerator.h
	${CMAKE_CURRENT_SOURCE_DIR}/VesselSegmentation.h
	${CMAKE_CURRENT_SOURCE_DIR}/SurfaceCreator.h
	${CMAKE_CURRENT_SOURCE_DIR}/Core.h
	${CMAKE_CURRENT_SOURCE_DIR}/MyImageViewer.h
	${CMAKE_CURRENT_SOURCE_DIR}/vtkPolylineToTubularVolume.h
	${CMAKE_CURRENT_SOURCE_DIR}/MyResliceMapper.h
	${CMAKE_CURRENT_SOURCE_DIR}/vtkvmtkCurvedMPRImageFilterNearestInterpolation.h
	# ${PROJECT_SOURCE_DIR}/MyPlaneWidget.h
)


#Generate moc and ui file
QT5_WRAP_CPP(MOCSrcs ${QT_INC})


add_library(
	Core
	${MOCSrcs}
	${QT_SRC}
	${QT_INC}
)


target_link_libraries(
	Core
	${LINK_LIBRARIE}
)

#Grouping in MSVC
SOURCE_GROUP(
	"Generated FIles" 
	FILES
	${MOCSrcs}
)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${QT_INC}
)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${QT_SRC}
)

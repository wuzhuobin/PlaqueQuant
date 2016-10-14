cmake_minimum_required(VERSION 2.8)
project(Widgets)
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

#link other libraries
link_directories(
	${PROJECT_SOURCE_DIR}/lib
	)

include_directories(
	${MAIN_INCLUDE_DIRS}
	)

SET(WIDGET_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/Distance3DWidget.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/MyBorderWidget.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/MyWidgetManager.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/MyPlaneWidget.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/vtkROIWidget.cxx
	)

SET(WIDGET_INC 
	${CMAKE_CURRENT_SOURCE_DIR}/Distance3DWidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/MyBorderWidget.h	
	${CMAKE_CURRENT_SOURCE_DIR}/MyWidgetManager.h
	${CMAKE_CURRENT_SOURCE_DIR}/MyPlaneWidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/vtkROIWidget.h
	)

QT5_WRAP_CPP(WIDGET_SRC_MOC ${WIDGET_INC})

ADD_LIBRARY(
	Widgets 
	${WIDGET_SRC} 
	${WIDGET_INC}
	${WIDGET_SRC_MOC}
	)
target_link_libraries(
	Widgets 
	${LINK_LIBRARIE}
	)

#Grouping in MSVC
SOURCE_GROUP(
	"Generated FIles" 
	FILES
	${WIDGET_SRC_MOC}
)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${WIDGET_INC}
)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${WIDGET_SRC}
)

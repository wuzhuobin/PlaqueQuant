project(Widgets)
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

#QT5
find_package(Qt5Core)
find_package(Qt5Widgets)
if(NOT Qt5Widgets_FOUND)
	message(FATAL_ERROR "error: Qt5Widgets is not found")
endif()

add_subdirectory( ContourWidgetSeries)

SET(WIDGET_SRC
#	${CMAKE_CURRENT_SOURCE_DIR}/Distance3DWidget.cxx
#	${CMAKE_CURRENT_SOURCE_DIR}/MyBorderWidget.cxx
#	${CMAKE_CURRENT_SOURCE_DIR}/MyWidgetManager.cxx
#	${CMAKE_CURRENT_SOURCE_DIR}/MyPlaneWidget.cxx
	${PROJECT_SOURCE_DIR}/vtkROIWidget.cxx
	${PROJECT_SOURCE_DIR}/PerpendicularMeasurementLineWidget.cxx
	${PROJECT_SOURCE_DIR}/vtkCircleBorderRepresentation.cxx
	)

SET(WIDGET_INC 
#	${CMAKE_CURRENT_SOURCE_DIR}/Distance3DWidget.h
#	${CMAKE_CURRENT_SOURCE_DIR}/MyBorderWidget.h	
#	${CMAKE_CURRENT_SOURCE_DIR}/MyWidgetManager.h
#	${CMAKE_CURRENT_SOURCE_DIR}/MyPlaneWidget.h
	${PROJECT_SOURCE_DIR}/vtkROIWidget.h
	${PROJECT_SOURCE_DIR}/PerpendicularMeasurementLineWidget.h
	${PROJECT_SOURCE_DIR}/vtkCircleBorderRepresentation.h
	)

QT5_WRAP_CPP(
	WIDGET_SRC_MOC 
	${PROJECT_SOURCE_DIR}/vtkROIWidget.h
	)

ADD_LIBRARY(
	Widgets 
	${WIDGET_SRC} 
	${WIDGET_INC}
	${WIDGET_SRC_MOC}
	)

target_link_libraries(
	Widgets 
	Qt5::Core
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	)

target_include_directories(
	Widgets 
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
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


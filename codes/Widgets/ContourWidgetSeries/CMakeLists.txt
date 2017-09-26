project(ContourWidgetSeries)

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

SET(
	CONTOUR_WIDGET_SERIES_CXX
	${PROJECT_SOURCE_DIR}/ContourWidgetSeries.cxx
	${PROJECT_SOURCE_DIR}/ContourWidgetSeriesRepresentation.cxx
	${PROJECT_SOURCE_DIR}/ContourWidgetSeriesOrientedGlyphRepresentation.cxx
	)

SET(
	CONTOUR_WIDGET_SERIES_H 
	${PROJECT_SOURCE_DIR}/ContourWidgetSeries.h
	${PROJECT_SOURCE_DIR}/ContourWidgetSeriesRepresentation.h	
	${PROJECT_SOURCE_DIR}/ContourWidgetSeriesOrientedGlyphRepresentation.h
	)

set(
	CONTOUR_WIDGET_SERIES_TEST
	${PROJECT_SOURCE_DIR}/main.cpp
)

ADD_LIBRARY(
	ContourWidgetSeries 
	${CONTOUR_WIDGET_SERIES_H} 
	${CONTOUR_WIDGET_SERIES_CXX}
	)

target_link_libraries(
	ContourWidgetSeries 
	${VTK_LIBRARIES}
	)

target_include_directories(
	ContourWidgetSeries 
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

add_executable(
	ContourWidgetSeries_test
	${CONTOUR_WIDGET_SERIES_TEST}
)

target_link_libraries(
	ContourWidgetSeries_test
	ContourWidgetSeries 
)

#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${CONTOUR_WIDGET_SERIES_H}
)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${CONTOUR_WIDGET_SERIES_CXX}
	${CONTOUR_WIDGET_SERIES_TEST}
)


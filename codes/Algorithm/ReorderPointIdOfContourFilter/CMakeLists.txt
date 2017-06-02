
project(ReorderPointIdOfContourFilter)

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
	REORDER_POINT_ID_OF_CONTOUR_FILTER_CXX
	ReorderPointIdOfContourFilter.cxx
)

set(
	REORDER_POINT_ID_OF_CONTOUR_FILTER_H	
	ReorderPointIdOfContourFilter.h
)

set(
	REORDER_POINT_ID_OF_CONTOUR_FILTER_TEST
	ReorderPointIdOfContourFilterTest.cxx
)

add_library(
	ReorderPointIdOfContourFilter 
	${REORDER_POINT_ID_OF_CONTOUR_FILTER_CXX}
	${REORDER_POINT_ID_OF_CONTOUR_FILTER_H}
	)

#link libraries
target_link_libraries(
	ReorderPointIdOfContourFilter
	${VTK_LIBRARIES}
	)

target_include_directories(
	ReorderPointIdOfContourFilter
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${REORDER_POINT_ID_OF_CONTOUR_FILTER_H}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${REORDER_POINT_ID_OF_CONTOUR_FILTER_CXX}
	)


add_executable(
	ReorderPointIdOfContourFilterTest
	${REORDER_POINT_ID_OF_CONTOUR_FILTER_TEST}
)

target_link_libraries(
	ReorderPointIdOfContourFilterTest
	ReorderPointIdOfContourFilter
	${VTK_LIBRARIES}
)

SOURCE_GROUP(
	"Source Files" 
	FILES 
	${REORDER_POINT_ID_OF_CONTOUR_FILTER_TEST}
	)
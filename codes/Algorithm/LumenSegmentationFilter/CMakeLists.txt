
project(LumenSegmentationFilter)

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
    LUMEN_SEGMENTATION_FILTER_CXX
	LumenSegmentationFilter.cxx
	LumenSegmentationFilter2.cxx
)

set(
	LUMEN_SEGMENTATION_FILTER_H
	LumenSegmentationFilter.h
	LumenSegmentationFilter2.h
)

set(
	LUMEN_SEGMENTATION_FILTER_TEST
	LumenSegmentationFilterTest.cxx
)

add_library(
	LumenSegmentationFilter 
	SHARED
	${LUMEN_SEGMENTATION_FILTER_CXX}
	${LUMEN_SEGMENTATION_FILTER_H}
	)

#link libraries
target_link_libraries(
	LumenSegmentationFilter
	${VTK_LIBRARIES}
	ReorderPointIdOfContourFilter
	)

target_include_directories(
	LumenSegmentationFilter
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)


set_target_properties(
    LumenSegmentationFilter
    PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS 
    true
    # RUNTIME_OUTPUT_DIRECTORY
	# ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${LUMEN_SEGMENTATION_FILTER_H}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${LUMEN_SEGMENTATION_FILTER_CXX}
	)


add_executable(
	LumenSegmentationFilterTest
	${LUMEN_SEGMENTATION_FILTER_TEST}
)

target_link_libraries(
	LumenSegmentationFilterTest
	LumenSegmentationFilter
	${VTK_LIBRARIES}
)

SOURCE_GROUP(
	"Source Files" 
	FILES 
	${LUMEN_SEGMENTATION_FILTER_TEST}
	)
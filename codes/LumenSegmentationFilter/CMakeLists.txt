
project(LumenSegmentationFilter)

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
    LUMEN_SEGMENTATION_FILTER_SRC
	${PROJECT_SOURCE_DIR}/LumenSegmentationFilter.cxx
	${PROJECT_SOURCE_DIR}/LumenSegmentationFilter2.cxx
)

set(
	LUMEN_SEGMENTATION_FILTER_INC
	${PROJECT_SOURCE_DIR}/LumenSegmentationFilter.h
	${PROJECT_SOURCE_DIR}/LumenSegmentationFilter2.h
)

set(
	LUMEN_SEGMENTATION_FILTER_TEST
	LumenSegmentationFilterTest.cxx
)

add_library(
	LumenSegmentationFilter 
	SHARED
	${LUMEN_SEGMENTATION_FILTER_SRC}
	${LUMEN_SEGMENTATION_FILTER_INC}
	)

#link libraries
target_link_libraries(
	LumenSegmentationFilter
	${VTK_LIBRARIES}
	)
set_target_properties(
    LumenSegmentationFilter
    PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS 
    true
    # RUNTIME_OUTPUT_DIRECTORY
	# ${CMAKE_SOURCE_DIR}/bin
)

#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${LUMEN_SEGMENTATION_FILTER_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${LUMEN_SEGMENTATION_FILTER_SRC}
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
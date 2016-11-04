
project(LumenSegmentationFilter)
SET(
  CMAKE_PREFIX_PATH
  "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"
  )

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
    LUMEN_SEGMENTATION_FILTER_SRC
	${PROJECT_SOURCE_DIR}/LumenSegmentationFilter.cxx
)

set(
	LUMEN_SEGMENTATION_FILTER_INC
	${PROJECT_SOURCE_DIR}/LumenSegmentationFilter.h
)



add_library(
	LumenSegmentationFilter 
	${LUMEN_SEGMENTATION_FILTER_SRC}
	${LUMEN_SEGMENTATION_FILTER_INC}
	)

#link libraries
target_link_libraries(
	LumenSegmentationFilter
	${VTK_LIBRARIES}
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


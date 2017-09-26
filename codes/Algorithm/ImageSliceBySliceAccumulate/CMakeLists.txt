
project(ImageSliceBySliceAccumulate)

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
    IMAGE_SLICE_BY_SLICE_ACCUMULATE_SRC
	${PROJECT_SOURCE_DIR}/ImageSliceBySliceAccumulate.cxx
)

set(
	IMAGE_SLICE_BY_SLICE_ACCUMULATE_H
	${PROJECT_SOURCE_DIR}/ImageSliceBySliceAccumulate.h
)

add_library(
	ImageSliceBySliceAccumulate 
	SHARED
	${IMAGE_SLICE_BY_SLICE_ACCUMULATE_SRC}
	${IMAGE_SLICE_BY_SLICE_ACCUMULATE_H}
	)
	
include(GenerateExportHeader)
generate_export_header(ImageSliceBySliceAccumulate)

#link libraries
target_link_libraries(
	ImageSliceBySliceAccumulate
	${VTK_LIBRARIES}
	)

target_include_directories(
	ImageSliceBySliceAccumulate
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

add_executable(
    ImageSliceBySliceAccumulate_test
    ImageSliceBySliceAccumulateTest.cpp
)

target_link_libraries(
	ImageSliceBySliceAccumulate_test
	ImageSliceBySliceAccumulate
	)


#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${LUMENEXTRATION_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${LUMENEXTRATION_SRC}
	)


Project(MagneticContourLineInterpolator)
#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

SET(
	MAGNETIC_CONTOUR_LINE_INTERPOLATOR_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/MagneticContourLineInterpolator.cxx
	)
SET(
	MAGNETIC_CONTOUR_LINE_INTERPOLATOR_INC
	${CMAKE_CURRENT_SOURCE_DIR}/MagneticContourLineInterpolator.h
	)

add_library(
	MagneticContourLineInterpolator
	${MAGNETIC_CONTOUR_LINE_INTERPOLATOR_INC}
	${MAGNETIC_CONTOUR_LINE_INTERPOLATOR_SRC}
	)

target_link_libraries(
	MagneticContourLineInterpolator
	${VTK_LIBRARIES}
)

target_include_directories(
	MagneticContourLineInterpolator
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}	
)

source_group(
	"Header Files" 
	FILES 
	${MAGNETIC_CONTOUR_LINE_INTERPOLATOR_INC}
	)
source_group(
	"Source Files"
	FILES
	${MAGNETIC_CONTOUR_LINE_INTERPOLATOR_SRC}
	)
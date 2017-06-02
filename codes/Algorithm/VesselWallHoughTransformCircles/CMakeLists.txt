
project(VesselWallHoughTransformCircles)

#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
    VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_SRC
	${PROJECT_SOURCE_DIR}/VesselWallHoughTransformCircles.cxx
	
)

set(
	VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_H
	${PROJECT_SOURCE_DIR}/VesselWallHoughTransformCircles.h
	${PROJECT_SOURCE_DIR}/IADEHoughTransform2DCirclesImageFilter.h
	${PROJECT_SOURCE_DIR}/IADEHoughTransform2DCirclesImageFilter.hxx
)

add_library(
	VesselWallHoughTransformCircles 
	SHARED
	${VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_SRC}
	${VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_H}
	)

#link libraries
target_link_libraries(
	VesselWallHoughTransformCircles
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	)

target_include_directories(
	VesselWallHoughTransformCircles
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

set_target_properties(
    VesselWallHoughTransformCircles
    PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS 
    true
    # RUNTIME_OUTPUT_DIRECTORY
	# ${CMAKE_SOURCE_DIR}/bin
)

add_executable(
    VesselWallHoughTransformCircles_test
    main.cpp
)

target_link_libraries(
	VesselWallHoughTransformCircles_test
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	VesselWallHoughTransformCircles
	Model
	InteractorStyles
	)
#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_H}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${VESSEL_WALL_HOUGH_TRANSFORM_CIRCLES_SRC}
	)


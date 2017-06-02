project(PolylineToTubularVolume)

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
    POLYLINE_TO_TUBULAR_VOLUME_SRC
	${PROJECT_SOURCE_DIR}/vtkPolylineToTubularVolume.cpp
)

set(
	POLYLINE_TO_TUBULAR_VOLUME_INC
	${PROJECT_SOURCE_DIR}/vtkPolylineToTubularVolume.h
)

add_library(
	PolylineToTubularVolume 
	${POLYLINE_TO_TUBULAR_VOLUME_SRC}
	${POLYLINE_TO_TUBULAR_VOLUME_INC}
	)

#link libraries
target_link_libraries(
	PolylineToTubularVolume
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	)

target_include_directories(
	PolylineToTubularVolume
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

# add_executable(
    # PolylineToTubularVolume_test
    # main.cpp
# )

# target_link_libraries(
	# PolylineToTubularVolume_test
	# ${ITK_LIBRARIES}  
	# ${VTK_LIBRARIES}
	# PolylineToTubularVolume
	# )
#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${POLYLINE_TO_TUBULAR_VOLUME_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${POLYLINE_TO_TUBULAR_VOLUME_SRC}
	)


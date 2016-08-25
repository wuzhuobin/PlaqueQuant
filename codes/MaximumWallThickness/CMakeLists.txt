Project(MaximumWallThickness)
#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

#ITK
find_package(ITK REQUIRED)
if (NOT ITK_FOUND)
	message(FATAL_ERROR "error: ITK is not found")
endif()
include(${ITK_USE_FILE})

INCLUDE_DIRECTORIES(
	${MAIN_INCULDE_DIRS}
	)

SET(
	LIBRARY_OUTPUT_PATH
	${CMAKE_SOURCE_DIR}/lib
	${EXECUTABLE_OUTPUT_PATH}
)
SET(
	MAXIMUMWALLTHICKNESS_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/MaximumWallThickness.cpp
	)
SET(
	MAXIMUMWALLTHICKNESS_INC
	${CMAKE_CURRENT_SOURCE_DIR}/MaximumWallThickness.h
	)

add_library(
	MaximumWallThickness
	${MAXIMUMWALLTHICKNESS_INC}
	${MAXIMUMWALLTHICKNESS_SRC}
	)

target_link_libraries(
	MaximumWallThickness
	${VTK_LIBRARIES}
	${ITK_LIBRARIES}
)

source_group(
	"Header Files" 
	FILES 
	${MAXIMUMWALLTHICKNESS_INC}
	)
source_group(
	"Source Files"
	FILES
	${MAXIMUMWALLTHICKNESS_SRC}
	)
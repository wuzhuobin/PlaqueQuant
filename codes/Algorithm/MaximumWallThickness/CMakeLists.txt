Project(MaximumWallThickness)
#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

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
)

target_include_directories(
	MaximumWallThickness
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}	
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
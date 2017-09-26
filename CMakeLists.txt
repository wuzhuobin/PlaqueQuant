cmake_minimum_required(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

project(PlaqueQuant)

# MSUT find vtk first
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


#VMTK
find_package(VMTK REQUIRED)
if (NOT VMTK_FOUND)
	message(FATAL_ERROR "error: VMTK is not found")
endif()
include(${VMTK_USE_FILE})

#QT5
find_package(Qt5Core)
find_package(Qt5Widgets)

# multi-processor build
set(
	CMAKE_CXX_FLAGS 
	"${CMAKE_CXX_FLAGS} /MP"
)
set(
	CMAKE_ARCHIVE_OUTPUT_DIRECTORY
	${CMAKE_SOURCE_DIR}/lib
)

set(
	CMAKE_LIBRARY_OUTPUT_DIRECTORY
	${CMAKE_SOURCE_DIR}/lib
)
set(
	CMAKE_RUNTIME_OUTPUT_DIRECTORY 
	${CMAKE_SOURCE_DIR}/bin
)

set(
	MAIN_LINK_DIRECTORIES
	${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
	${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

link_directories(
	${MAIN_LINK_DIRECTORIES}
	)

add_subdirectory(codes/Main)
add_subdirectory(codes/View)
add_subdirectory(codes/Core)
add_subdirectory(codes/Controller)
add_subdirectory(codes/Model)
add_subdirectory(codes/Widgets)
add_subdirectory(codes/Algorithm)

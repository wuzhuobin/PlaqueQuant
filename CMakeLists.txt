cmake_minimum_required(VERSION 2.8)
 
PROJECT(PlaqueQuant)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

#ITK
find_package(ITK REQUIRED)
if (NOT ITK_FOUND)
	message(FATAL_ERROR "error: ITK is not found")
endif()
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

#VMTK
find_package(VMTK REQUIRED)
if (NOT VMTK_FOUND)
	message(FATAL_ERROR "error: VMTK is not found")
endif()
include(${VMTK_USE_FILE})

#QT
if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  # set(CMAKE_AUTOMOC ON)
  find_package(Qt5PrintSupport REQUIRED)
  find_package(Qt5Widgets REQUIRED QUIET)
  # add_definitions(-DQT5_IS_LOADED)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()

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

link_directories(
	${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
	${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
	)

SET(
	MAIN_INCLUDE_DIRS
	### Directories for MOC Sources ###
	${PROJECT_BINARY_DIR}
	${PROJECT_BINARY_DIR}/resources
	${PROJECT_BINARY_DIR}/codes/UI
	${PROJECT_BINARY_DIR}/codes/Core
	${PROJECT_BINARY_DIR}/codes/Main
	# AbstractInteractorStyles
	${PROJECT_BINARY_DIR}/codes/InteractorStyles
	# Abstract Data structure
	# Source for Widgets
	${PROJECT_BINARY_DIR}/codes/Widgets
	# Source for GPURender
	${PROJECT_BINARY_DIR}/codes/GPURenderWithLUT
	${PROJECT_BINARY_DIR}/codes/Measurements 
	# Source for maximum wall thickness
	${PROJECT_BINARY_DIR}/codes/MaximumWallThickness
	${PROJECT_BINARY_DIR}/codes/CenterlineBranching
	${PROJECT_BINARY_DIR}/codes/MyImageReader
	${PROJECT_BINARY_DIR}/codes/Managers
	${PROJECT_BINARY_DIR}/codes/License
	${PROJECT_BINARY_DIR}/codes/LumenExtraction
	${PROJECT_BINARY_DIR}/codes/LumenSegmentationFilter
	${PROJECT_BINARY_DIR}/codes/ReorderPointIdOfContourFilter

	### Directories for headers ###
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/resources
	${PROJECT_SOURCE_DIR}/codes/UI
	${PROJECT_SOURCE_DIR}/codes/Core
	${PROJECT_SOURCE_DIR}/codes/Main
	# AbstractInteractorStyles
	${PROJECT_SOURCE_DIR}/codes/InteractorStyles
	# Abstract Data structure
	# Source for Widgets
	${PROJECT_SOURCE_DIR}/codes/Widgets
	# Source for GPURender
	${PROJECT_SOURCE_DIR}/codes/GPURenderWithLUT
	${PROJECT_SOURCE_DIR}/codes/Measurements 
	# Source for maximum wall thickness
	${PROJECT_SOURCE_DIR}/codes/MaximumWallThickness
	${PROJECT_SOURCE_DIR}/codes/CenterlineBranching
	${PROJECT_SOURCE_DIR}/codes/MyImageReader
	${PROJECT_SOURCE_DIR}/codes/License
	${PROJECT_SOURCE_DIR}/codes/LumenExtraction
	${PROJECT_SOURCE_DIR}/codes/LumenSegmentationFilter
	${PROJECT_SOURCE_DIR}/codes/ReorderPointIdOfContourFilter
	)

include_directories(
	${MAIN_INCLUDE_DIRS}
)


set(
	LINK_LIBRARIE
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	${VMTK_LIBRARIES} 
	Qt5::Widgets 
	Qt5::PrintSupport
	UI
	Core
	Widgets
	InteractorStyles
	Widgets 
	GPURenderWithLUT
	Measurements
	MaximumWallThickness
	CenterlineBranching
	MyImageReader
	Widget_License
	LumenExtraction
	LumenSegmentationFilter
	EncryptionAuthentication
	ReorderPointIdOfContourFilter
)

add_subdirectory(codes/UI)
add_subdirectory(codes/Widgets)
add_subdirectory(codes/Main)
add_subdirectory(codes/InteractorStyles)
add_subdirectory(codes/GPURenderWithLUT)
add_subdirectory(codes/Core)
add_subdirectory(codes/Measurements)
add_subdirectory(codes/MaximumWallThickness)
add_subdirectory(codes/CenterlineBranching)
add_subdirectory(codes/MyImageReader)
add_subdirectory(codes/LumenExtraction)
add_subdirectory(codes/License)
add_subdirectory(codes/LumenSegmentationFilter)
add_subdirectory(codes/EncryptionAuthentication)
add_subdirectory(codes/ReorderPointIdOfContourFilter)




project(VesselTest)

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

if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  # set(CMAKE_AUTOMOC ON)
  find_package(Qt5Widgets REQUIRED QUIET)
  # add_definitions(-DQT5_IS_LOADED)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()

#link other libraries
link_directories(
	${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
	${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
	)

include_directories(
	${MAIN_INCLUDE_DIRS}
)

file(
	GLOB
	VESSEL_TEST_CXX
	*.cxx
)

file(
	GLOB
	VESSEL_TEST_H
	*.h
)

set(
	MAIN
	main.cpp
)

add_library(
	VesselTest
	SHARED
	${VESSEL_TEST_CXX}
	${VESSEL_TEST_H}
)

set_target_properties(
    VesselTest
    PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS 
    true
)

target_link_libraries(
	VesselTest
	${VTK_LIBRARIES}
	${ITK_LIBRARIES}
	Qt5::Core
)

add_executable(
	VesselTest_Test
	${MAIN}
)
target_link_libraries(
	VesselTest_Test
	${LINK_LIBRARIE}
	VesselTest
)

source_group(
	"Header Files" 
	FILES 
	${VESSEL_TEST_H}
	)
source_group(
	"Source Files"
	FILES
	${VESSEL_TEST_CXX}
	)

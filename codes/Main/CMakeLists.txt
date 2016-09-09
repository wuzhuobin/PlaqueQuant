cmake_minimum_required(VERSION 2.8)

project(Main)
include_directories(
	${MAIN_INCLUDE_DIRS}
	)

 # message("${PROJECT_BINARY_DIR}")
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

Set(
	QT_RSRS 
	${CMAKE_SOURCE_DIR}/resources/Project.qrc
	)

QT5_ADD_RESOURCES(RCC_RSRS ${QT_RSRS})

add_executable(
	Main
	main.cxx
	${QT_RSRS}
	${QT_RSRS}
)

#Link all libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(
	Main
	${LINK_LIBRARIE}
)


#Grouping in MSVC

SOURCE_GROUP(
	"Source Files" 
	FILES 
	main.cxx
)

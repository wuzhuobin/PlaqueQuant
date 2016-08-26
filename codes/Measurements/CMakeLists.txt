
project(Measurements)
SET(
  CMAKE_PREFIX_PATH
  "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"
  )


#ITK
find_package(ITK REQUIRED)
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
  find_package(Qt5PrintSupport REQUIRED)
  find_package(Qt5Widgets REQUIRED QUIET)
  # add_definitions(-DQT5_IS_LOADED)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()

#link other libraries
link_directories(
	${PROJECT_SOURCE_DIR}/lib
	)

include_directories(
	${PROJECT_BINARY_DIR}
	${MAIN_INCLUDE_DIRS}
	)

SET(
	LIBRARY_OUTPUT_PATH
	${CMAKE_SOURCE_DIR}/lib
	${EXECUTABLE_OUTPUT_PATH}
)

set(
	MEASUREMENTFOR3D_SRC
	${PROJECT_SOURCE_DIR}/MeasurementFor3D.cxx
	${PROJECT_SOURCE_DIR}/MeasurementFor2D.cxx
)

set(
	MEASUREMENTFOR3D_INC
	${PROJECT_SOURCE_DIR}/MeasurementFor3D.h
	${PROJECT_SOURCE_DIR}/MeasurementFor2D.h
)



add_library(
	Measurements 
	${MEASUREMENTFOR3D_INC}
	${MEASUREMENTFOR3D_SRC}
	)

#link libraries
target_link_libraries(
	Measurements
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	)

#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${MEASUREMENTFOR3D_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${MEASUREMENTFOR3D_SRC}
	)


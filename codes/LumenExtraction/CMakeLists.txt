
project(LumenExtraction)
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
	${MAIN_INCLUDE_DIRS}
	)

set(
    LUMENEXTRATION_SRC
	${PROJECT_SOURCE_DIR}/LumenExtraction.cpp
)

set(
	LUMENEXTRATION_INC
	${PROJECT_SOURCE_DIR}/LumenExtraction.h
)



add_library(
	LumenExtraction 
	${LUMENEXTRATION_SRC}
	${LUMENEXTRATION_INC}
	)

add_executable(
    LumenExtraction_test
    main.cpp
	${LUMENEXTRATION_SRC}
	${LUMENEXTRATION_INC}
)

#link libraries
target_link_libraries(
	LumenExtraction
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	Qt5::Core 

	)
target_link_libraries(
	LumenExtraction_test
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	Qt5::Core 

	)
#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${LUMENEXTRATION_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${LUMENEXTRATION_SRC}
	)


cmake_minimum_required(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

project(Main)

#Link all libraries
link_directories(
   	${MAIN_LINK_DIRECTORIES}
)

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

#QT5
find_package(Qt5Core)
find_package(Qt5Widgets)

set(
    MAIN_SRC
    main.cpp
)

set(
	VIEW_RC
	${PROJECT_SOURCE_DIR}/resources/IADE_Analyzer.rc
)

set(
	VIEW_QRC
	${PROJECT_SOURCE_DIR}/resources/IADE_Analyzer.qrc
)
qt5_add_resources(
	VIEW_QRC_CPP
	${VIEW_QRC}
)

add_executable(
	Main
    ${MAIN_SRC}
	${VIEW_QRC}
	${VIEW_QRC_CPP}
	${VIEW_RC}
)

target_link_libraries(
	Main
	Core
	# ${ITK_LIBRARIES}
	# ${VTK_LIBRARIES}
	# Qt5::Core
	# Qt5::Widgets
)
target_include_directories(
	Main
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

#Grouping in MSVC
SOURCE_GROUP(
	"Source Files" 
	FILES 
	main.cpp
)
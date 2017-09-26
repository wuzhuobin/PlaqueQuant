cmake_minimum_required(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

project(Core)

find_package(Qt5Core)

set(
	CORE_H
	Core.h
)

set(
	CORE_SRC
	Core.cpp
)

#Generate moc and ui file
qt5_wrap_cpp(
	CORE_MOC
	${CORE_H}
)

add_library(
	Core
	${CORE_H}
	${CORE_SRC}
	${CORE_MOC}
)

#link libraries
target_link_libraries(
	Core
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	Qt5::Core
	Qt5::Widgets
	Controller
	Model
	View
	)
target_include_directories(
	Core
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

#Grouping in MSVC
SOURCE_GROUP(
	"Generated Files"
	FILES
	${CORE_MOC}
	)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${CORE_H}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${CORE_SRC}
	)
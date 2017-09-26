cmake_minimum_required(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

project(ReportGenerator)

find_package(Qt5Core)
find_package(Qt5PrintSupport)

set(
	REPORT_GENERATOR_H
	AbstractFilter.h
	ReportGenerator.h
)

set(
	REPORT_GENERATOR_SRC
	AbstractFilter.cxx
	ReportGenerator.cxx
)

#Generate moc and ui file
qt5_wrap_cpp(
	REPORT_GENERATOR_MOC
	${REPORT_GENERATOR_H}
)


add_library(
	ReportGenerator
	SHARED
	${REPORT_GENERATOR_H}
	${REPORT_GENERATOR_SRC}
	${REPORT_GENERATOR_MOC}	
)

#link libraries
target_link_libraries(
	ReportGenerator
	Qt5::Core
	Qt5::PrintSupport
	)

target_include_directories(
	ReportGenerator
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

set_target_properties(
    ReportGenerator
    PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS 
    true
)

#Grouping in MSVC
SOURCE_GROUP(
	"Generated Files"
	FILES
	${VIEW_MOC}
	${VIEW_UI_H}
	)
SOURCE_GROUP(
	"Forms FIles" 	
	FILES 
	${VIEW_UI}
	)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${VIEW_H}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${VIEW_SRC}
	)
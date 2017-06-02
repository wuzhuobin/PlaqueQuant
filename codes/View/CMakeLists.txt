cmake_minimum_required(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

project(View)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)


add_subdirectory(RegistrationWizard)
add_subdirectory(ReportGenerator)
add_subdirectory(qtcsv)
set(
	VIEW_H
	MainWindow.h
	LabelWidget.h
	Switch2DWidget.h
	Switch3DWidget.h
	ViewerWidget.h
	MeasurementWidget.h
	IADEMeasurementWidget.h
)


set(
	VIEW_SRC
	MainWindow.cpp
	LabelWidget.cpp
	Switch2DWidget.cpp
	Switch3DWidget.cpp
	ViewerWidget.cpp
	MeasurementWidget.cpp
	IADEMeasurementWidget.cpp
)

set(
	VIEW_UI
	MainWindow.ui
	LabelWidget.ui
	Switch2DWidget.ui
	Switch3DWidget.ui
	ViewerWidget.ui
	MeasurementWidget.ui
	IADEMeasurementWidget.ui
)
#Generate moc and ui file
qt5_wrap_cpp(
	VIEW_MOC
	${VIEW_H}
)

qt5_wrap_ui(
	VIEW_UI_H
	${VIEW_UI}
)

add_library(
	View
	SHARED
	${VIEW_H}
	${VIEW_SRC}
	${VIEW_UI}
	${VIEW_MOC}
	${VIEW_UI_H}
)
# qt5_use_modules(View Core Widgets OpenGL PrintSupport)

#link libraries
target_link_libraries(
	View
	Qt5::Core
	Qt5::Widgets
	Qt5::OpenGL
	RegistrationWizard
	ReportGenerator
	qtcsv
	)

target_include_directories(
	View
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

set_target_properties(
    View
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
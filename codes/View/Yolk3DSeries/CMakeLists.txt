cmake_minimum_required(VERSION 2.8.11)
cmake_policy(SET CMP0043 NEW)
cmake_policy(SET CMP0028 NEW)

project(Yolk3DSeries)

#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5OpenGL)

set(
	YOLK_3D_SERIES_H
	Yolk3DSeries.h
)

set(
	YOLK_3D_SERIES_CPP
	Yolk3DSeries.cpp
)

set(
	YOLK_3D_SERIES_UI
	Yolk3DSeries.ui
)

# Output UI headers to specified location
qt5_wrap_ui(
	YOLK_3D_SERIES_UI_H
	${YOLK_3D_SERIES_UI}
)

qt5_wrap_cpp(
    YOLK_3D_SERIES_MOC
    ${YOLK_3D_SERIES_H}
)


add_library(
	Yolk3DSeries
	STATIC
	${YOLK_3D_SERIES_H}
	${YOLK_3D_SERIES_CPP}
	${YOLK_3D_SERIES_UI}
	${YOLK_3D_SERIES_UI_H}
	${YOLK_3D_SERIES_MOC}
)

target_include_directories(
	Yolk3DSeries
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

target_link_libraries(
	Yolk3DSeries
	RegistrationWizard
	${VTK_LIBRARIES}
	${ITK_LIBRARIES}
	Qt5::Widgets
	Qt5::OpenGL
	Qt5::Core
)


add_executable(
	Yolk3DSeries_example
	main.cpp
)


target_link_libraries(
	Yolk3DSeries_example
	Yolk3DSeries
)



#Grouping in MSVC
source_group(
	"Generated Files"
	FILES
	${PROJECT_MOC}
	${PROJECT_UI_H}
)
source_group(
	"Forms Files"
	FILES
	${PROJECT_UI}
)
source_group(
	"Header Files"
	FILES
	${PROJECT_INC}
)
source_group(
	"Source Files"
	FILES
	${PROJECT_SRC}
)

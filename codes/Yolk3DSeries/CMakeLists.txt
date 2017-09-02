cmake_minimum_required(VERSION 2.8.11)
cmake_policy(SET CMP0043 NEW)
cmake_policy(SET CMP0028 NEW)

get_filename_component(
	SCB_PROJECT_NAME
	${CMAKE_CURRENT_SOURCE_DIR}
	NAME
)

project(${SCB_PROJECT_NAME})

#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# Allow the developer to select if Dynamic or Static libraries are built
option (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
# Set the LIB_TYPE variable to STATIC
set (LIB_TYPE STATIC)
if (BUILD_SHARED_LIBS)
    # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
    set (LIB_TYPE SHARED)
endif (BUILD_SHARED_LIBS)

file(GLOB PROJECT_SRC *.c*)
file(GLOB PROJECT_INC *.h*)
FILE(GLOB PROJECT_UI *.ui)

# Output UI headers to specified location
qt5_wrap_ui(
	PROJECT_UI_H
	${PROJECT_UI}
)

qt5_wrap_cpp(
    PROJECT_MOC
    ${PROJECT_INC}
)

qt5_add_resources(
	RCC_RSRS
	${CMAKE_SOURCE_DIR}/resources/Project.qrc
)

add_library(
    ${PROJECT_NAME}
    ${PROJECT_SRC}
    ${PROJECT_INC}
    ${PROJECT_MOC}
	${PROJECT_UI}
	${PROJECT_UI_H}
)

add_executable(
	${PROJECT_NAME}_Test
	${CMAKE_CURRENT_SOURCE_DIR}/Test/main.cpp
	${CMAKE_SOURCE_DIR}/resources/Project.qrc
	${RCC_RSRS}
)

target_link_libraries(
    ${PROJECT_NAME}
	${VTK_LIBRARIES}
	${ITK_LIBRARIES}
)

target_link_libraries(
	${PROJECT_NAME}_Test
	${PROJECT_NAME}
	UI
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

target_include_directories(
	${PROJECT_NAME}_Test
	PUBLIC
	${PROJECT_NAME}
)

qt5_use_modules(${PROJECT_NAME}_Test Core Gui OpenGL)
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

MESSAGE(STATUS "Building submodule ${SCB_PROJECT_NAME} finished!")
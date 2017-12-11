cmake_minimum_required(VERSION 2.8.11)
cmake_policy(SET CMP0043 NEW)
cmake_policy(SET CMP0028 NEW)

get_filename_component(
	PROJECT_NAME
	${CMAKE_CURRENT_SOURCE_DIR} 
	NAME
)

MESSAGE(STATUS "Building submodule ${PROJECT_NAME}...")

project(${PROJECT_NAME})

#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED) 
include(${VTK_USE_FILE})

file(GLOB PROJECT_SRC *.c*)
file(GLOB PROJECT_INC *.h*)

add_library(
	${PROJECT_NAME}
	STATIC
    ${PROJECT_SRC}
    ${PROJECT_INC}
)

target_link_libraries(
    ${PROJECT_NAME}
	${VTK_LIBRARIES} 
	${ITK_LIBRARIES}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${PROJECT_SOURCE_DIR}
)

if (vtkUtilities_found)
	target_link_libraries(
		${PROJECT_NAME}
		${VTK_UTILITIES_LIBRARIES}
	)
endif()


#Grouping in MSVC
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

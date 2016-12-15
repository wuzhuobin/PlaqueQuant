
cmake_minimum_required(VERSION 2.8)

PROJECT(vtkImageToContourPolyData)

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
  SOURCE
  ${PROJECT_SOURCE_DIR}/vtkImageToContourPolyData.cxx
)

set(
  HEAD
  ${PROJECT_SOURCE_DIR}/vtkImageToContourPolyData.h
)

add_library(
  vtkImageToContourPolyData
  SHARED
  ${SOURCE}
  ${HEAD}
)

target_link_libraries(
  vtkImageToContourPolyData
  ${VTK_LIBRARIES}
)
set_target_properties(
    vtkImageToContourPolyData
    PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS 
    true
)

#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${HEAD}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${SOURCE}
	)

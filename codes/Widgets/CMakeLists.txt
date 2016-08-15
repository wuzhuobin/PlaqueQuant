cmake_minimum_required(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)


SET(
  CMAKE_PREFIX_PATH
  "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"
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

#VMTK
find_package(VMTK REQUIRED)
if (NOT VMTK_FOUND)
	message(FATAL_ERROR "error: VMTK is not found")
endif()
include(${VMTK_USE_FILE})

#link other libraries
link_directories(${PROJECT_SOURCE_DIR}/lib)

message(${PROJECT_SOURCE_DIR})

include_directories(
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/codes/core
	${PROJECT_SOURCE_DIR}/build/codes/Module
	${PROJECT_SOURCE_DIR}/codes/Module/GUI
	${PROJECT_SOURCE_DIR}/codes/Module/Pipeline
	${PROJECT_SOURCE_DIR}/codes/AbstractInteractorStyles
	)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

SET(WIDGET_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/Distance3DWidget.cxx
)

SET(WIDGET_INC 
	${CMAKE_CURRENT_SOURCE_DIR}/Distance3DWidget.h
)

ADD_LIBRARY(Widgets ${WIDGET_SRC} ${WIDGET_INC})
target_link_libraries(Widgets 
	${VTK_LIBRARIES} 
	${VMTK_LIBRARIES} 
	${ITK_LIBRARIES} )

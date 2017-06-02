project(Model)
#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})
#ITK
find_package(ITK REQUIRED)
if (NOT ITK_FOUND)
	message(FATAL_ERROR "error: ITK is not found")
endif()
include(${ITK_USE_FILE})

#QT5
find_package(Qt5Core)
find_package(Qt5Widgets REQUIRED)
if(NOT Qt5Widgets_FOUND)
	message(FATAL_ERROR "error: Qt5Widgets is not found")
endif()


SET(
	MODEL_SRC
	${PROJECT_SOURCE_DIR}/ImageManager.cpp
	${PROJECT_SOURCE_DIR}/IVtkImageData.cpp
	${PROJECT_SOURCE_DIR}/Overlay.cpp
	${PROJECT_SOURCE_DIR}/IADEOverlay.cpp
	${PROJECT_SOURCE_DIR}/IADEImageManager.cpp
	${PROJECT_SOURCE_DIR}/PlaqueQuantImageManager.cpp
	${PROJECT_SOURCE_DIR}/PlaqueQuantOverlay.cpp
	)
SET(
	MODEL_H
	${PROJECT_SOURCE_DIR}/ImageManager.h
	${PROJECT_SOURCE_DIR}/IVtkImageData.h
	${PROJECT_SOURCE_DIR}/Overlay.h
	${PROJECT_SOURCE_DIR}/IADEImageManager.h
	${PROJECT_SOURCE_DIR}/IADEOverlay.h
	${PROJECT_SOURCE_DIR}/PlaqueQuantImageManager.h
	${PROJECT_SOURCE_DIR}/PlaqueQuantOverlay.h
	)

qt5_wrap_cpp(
	MODEL_MOC 
	${MODEL_H}
	)

add_library(
	Model
	${MODEL_SRC}
	${MODEL_H}
	${MODEL_MOC}
	)

target_link_libraries(
	Model
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	Qt5::Core
	Qt5::Widgets
	MaximumWallThickness
)

target_include_directories(
	Model
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

source_group(
	"Generated FIles" 
	FILES
	${MODEL_MOC}
)
source_group(
	"Header Files" 
	FILES 
	${MODEL_H}
	)
source_group(
	"Source Files"
	FILES
	${MODEL_SRC}
	)
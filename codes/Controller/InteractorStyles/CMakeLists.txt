project(InteractorStyles)

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

#QT5
find_package(Qt5Core)
find_package(Qt5Widgets)
if(NOT Qt5Widgets_FOUND)
	message(FATAL_ERROR "error: Qt5Widgets is not found")
endif()

set(
	VIEWER_SRC
	${PROJECT_SOURCE_DIR}/ImageViewer.cxx
	${PROJECT_SOURCE_DIR}/ResliceImageViewer.cxx
	${PROJECT_SOURCE_DIR}/SurfaceViewer.cxx
	${PROJECT_SOURCE_DIR}/CenterlineSurfaceViewer.cxx
)

set(
	VIEWER_H
	${PROJECT_SOURCE_DIR}/ImageViewer.h
	${PROJECT_SOURCE_DIR}/ResliceImageViewer.h
	${PROJECT_SOURCE_DIR}/SurfaceViewer.h	
	${PROJECT_SOURCE_DIR}/CenterlineSurfaceViewer.h
)

file(
	GLOB 
	ABSTRACT_INTERACTORS_SRC 
	${PROJECT_SOURCE_DIR}/Abstract*.c*
	)
file(
	GLOB 
	ABSTRACT_INTERACTORS_H 
	${PROJECT_SOURCE_DIR}/Abstract*.h*
	)
file(
	GLOB 
	INTERACTORS_SRC  
	${PROJECT_SOURCE_DIR}/InteractorStyle*.c* 
	)
file(
	GLOB 
	INTERACTORS_INC  
	${PROJECT_SOURCE_DIR}/InteractorStyle*.h*
	# ${PROJECT_SOURCE_DIR}/StyleSwitchMacro.h*
	)
file(
	GLOB
	QINTERACTORS_SRC
	${PROJECT_SOURCE_DIR}/QAbstract*.c*
	${PROJECT_SOURCE_DIR}/QInteractorStyle*.c*
	)
file(
	GLOB
	QINTERACTORS_INC
	${PROJECT_SOURCE_DIR}/QAbstract*.h*
	${PROJECT_SOURCE_DIR}/QInteractorStyle*.h*
	)
qt5_wrap_cpp(
	QINTERACTORS_INC_MOC
	${QINTERACTORS_INC}
	)
file(
	GLOB
	QINTERACTORS_UI
	${PROJECT_SOURCE_DIR}/QAbstract*.ui
	${PROJECT_SOURCE_DIR}/QInteractorStyle*.ui
)
qt5_wrap_ui(
	QINTERACTORS_UI_SRC
	${QINTERACTORS_UI}
)
add_library(
	InteractorStyles
	${VIEWER_SRC}
	${VIEWER_H}
	${ABSTRACT_INTERACTORS_SRC}
	${ABSTRACT_INTERACTORS_H}
	${INTERACTORS_INC}
	${INTERACTORS_SRC}
	${QINTERACTORS_INC}
	${QINTERACTORS_SRC}
	${QINTERACTORS_INC_MOC}
	${QINTERACTORS_UI}
	${QINTERACTORS_UI_SRC}
)

target_link_libraries(
	InteractorStyles
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	${VMTK_LIBRARIES}
	Qt5::Core
	Qt5::Widgets
	Widgets
	LumenExtraction
	PolylineToTubularVolume
	ReorderPointIdOfContourFilter
	LumenSegmentationFilter
	MaximumWallThickness
	ContourWidgetSeries
	VesselWallHoughTransformCircles
	MagneticContourLineInterpolator
)

target_include_directories(
	InteractorStyles
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

source_group(
	"Header Files" 
	FILES 
	${IMAGE_VIEWER_H}
	${ABSTRACT_INTERACTORS_H}
	${INTERACTORS_INC}
	)
source_group(
	"Source Files"
	FILES
	${IMAGE_VIEWER_SRC}
	${ABSTRACT_INTERACTORS_SRC}
	${INTERACTORS_SRC}
	)
SOURCE_GROUP(
	"Generated FIles" 
	FILES
	${QINTERACTORS_INC_MOC}
	${QINTERACTORS_UI_SRC}
)
SOURCE_GROUP(
	"Forms FIles" 	
	FILES 
	${QINTERACTORS_UI}
	)
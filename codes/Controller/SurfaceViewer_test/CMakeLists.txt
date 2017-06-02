project(SurfaceViewer_test)
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

set(
	SURFACE_VIEWER_TEST_SRC
	SurfaceViewer_test.cpp
)

add_executable(
	SurfaceViewer_test
	${SURFACE_VIEWER_TEST_SRC}
)

target_link_libraries(
	SurfaceViewer_test
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	Qt5::Core
	Qt5::Widgets
	InteractorStyles
)

source_group(
	"Source Files"
	FILES
	${SURFACE_VIEWER_TEST_SRC}
	)

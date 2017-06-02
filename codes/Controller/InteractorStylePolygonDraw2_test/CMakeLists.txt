project(InteractorStylePolygonDraw2_test)
#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
	INTERACTOR_STYLE_POLYGON_DRAW2_TEST_SRC
	InteractorStylePolygonDraw2_test.cpp
)

add_executable(
	InteractorStylePolygonDraw2_test
	${INTERACTOR_STYLE_POLYGON_DRAW2_TEST_SRC}
)

target_link_libraries(
	InteractorStylePolygonDraw2_test
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	Qt5::Core
	Qt5::Widgets
	InteractorStyles
)

source_group(
	"Source Files"
	FILES
	${INTERACTOR_STYLE_VOI_TEST_SRC}
	)

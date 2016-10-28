Project(Testing)
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

INCLUDE_DIRECTORIES(
	${MAIN_INCULDE_DIRS}
	)

add_executable(
	Test_MaximumWallThickness
	${CMAKE_CURRENT_SOURCE_DIR}/Test_MaimumWallThickness.cpp)
target_link_libraries(
	Test_MaximumWallThickness
	MaximumWallThickness
	${VTK_LIBRARIES}
	${ITK_LIBRARIES}
)


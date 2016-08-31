project(MyImageReader)
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
find_package(Qt5Widgets REQUIRED)
if(NOT Qt5Widgets_FOUND)
	message(FATAL_ERROR "error: Qt5Widgets is not found")
endif()
#link other libraries
link_directories(
	${PROJECT_SOURCE_DIR}/lib
	)

include_directories(
	${PROJECT_BINARY_DIR}
	${MAIN_INCLUDE_DIRS}
	)
SET(
	LIBRARY_OUTPUT_PATH
	${CMAKE_SOURCE_DIR}/lib
	${EXECUTABLE_OUTPUT_PATH}
)
SET(
	MYIMAGEREADER_SRC
	${PROJECT_SOURCE_DIR}/MyImageReader.cpp
	${PROJECT_SOURCE_DIR}/ImageRegistration.cxx
	)
SET(
	MYIMAGEREADER_INC
	${PROJECT_SOURCE_DIR}/MyImageReader.h
	${PROJECT_SOURCE_DIR}/ImageRegistration.H
	)
set(
	MAIN_SRC
	${PROJECT_SOUREC_DIR}main.cpp
	)
qt5_wrap_cpp(
	QMYIMAGEREADER_SRC 
	${MYIMAGEREADER_INC}
	)
add_library(
	MyImageReader
	${MYIMAGEREADER_INC}
	${MYIMAGEREADER_SRC}
	${QMYIMAGEREADER_SRC}
	)
add_executable(
	MyImageReaderTest
	${MYIMAGEREADER_INC}
	${MYIMAGEREADER_SRC}
	${QMYIMAGEREADER_SRC}
	${MAIN_SRC}
)
target_link_libraries(
	MyImageReader
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	Qt5::Widgets
)
target_link_libraries(
	MyImageReaderTest
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	Qt5::Widgets
)
source_group(
	"Generated FIles" 
	FILES
	${QMYIMAGEREADER_SRC}
)
source_group(
	"Header Files" 
	FILES 
	${MYIMAGEREADER_INC}
	)
source_group(
	"Source Files"
	FILES
	${MYIMAGEREADER_SRC}
	${MAIN_SRC}
	)
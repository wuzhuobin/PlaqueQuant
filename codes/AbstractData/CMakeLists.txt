cmake_minimum_required(VERSION 2.6)
project(TPNS_AbstractData)

SET(
	CMAKE_PREFIX_PATH
	"C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"
	)

file(GLOB ABSTRACT_SRC Abstract*)
file(GLOB SOURCE_FILES_CPP *.cpp)
file(GLOB SOURCE_FILES_CXX *.cxx)
file(GLOB HEADER_HXX *.hxx)
file(GLOB HEADER_H *.h)

include_directories(
	${MAIN_INCLUDE_DIRS}
	)
# find_package(ITK REQUIRED)
# include(${ITK_USE_FILE})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  set(CMAKE_AUTOMOC ON)
  find_package(Qt5Widgets REQUIRED QUIET)
  add_definitions(-DQT5_IS_LOADED)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()

set(QT_WRAP 
		${SOURCE_FILES_CPP}
		${SOURCE_FILES_CXX}
		${HEADER_H}
		)

add_library(AbstractData ${ABSTRACT_SRC} ${QT_WRAP})
qt5_use_modules(AbstractData Core Widgets)
target_link_libraries(AbstractData ${VTK_LIBRARIES} ${ITK_LIBRARIES} )



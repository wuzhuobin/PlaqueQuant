Project(License)

message("TFUJEIFJEISJAFEISAJF")
#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED QUIET)


#link other libraries
link_directories(${PROJECT_SOURCE_DIR}/lib)

include_directories(
	${MAIN_INCLUDE_DIRECTORIES}
	${CMAKE_CURRENT_SOURCE_DIR}/IPHelper/include
	${CMAKE_CURRENT_BINARY_DIR}
	)

# Library for IPHelper
SET(IPHELPER_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/IPHelper/lib/x64/iphlpapi.lib)


#SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
SET(LICENSE_KEY_INC 
	${CMAKE_CURRENT_SOURCE_DIR}/keyconfirm.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/getmacadd.h
	${CMAKE_CURRENT_SOURCE_DIR}/MacEncoding.h
	)
SET(LICENSE_KEY_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/keyconfirm.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/key_generator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/getmacadd.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MacEncoding.cpp
	)
SET(LICENSE_KEY_UI ${CMAKE_CURRENT_SOURCE_DIR}/keyconfirm.ui)

SET(LICENSE_KEY_WIDGET_INC ${LICENSE_KEY_INC} PARENT_SCOPE)

SET(QT_WRAP ${LICENSE_KEY_UI} ${LICENSE_KEY_INC})


qt5_wrap_ui(Widget_License_UISrcs ${LICENSE_KEY_UI})
add_library(Widget_License ${QT_WRAP} ${LICENSE_KEY_SRC} ${Widget_License_UISrcs})
qt5_use_modules(Widget_License Core)
target_link_libraries(Widget_License ${ITK_LIBRARIES}  ${VTK_LIBRARIES} ${IPHELPER_LIBRARY})

#Grouping in MSVC
SOURCE_GROUP("UI"	FILES ${Widget_License_UISrcs} ${LICENSE_KEY_UI})
SOURCE_GROUP("Files" 	FILES ${MODULE_QT_INC}  ${MODULE_QT_SRC})

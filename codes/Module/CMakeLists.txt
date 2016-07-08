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

#QT
if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  # set(CMAKE_AUTOMOC ON)
  find_package(Qt5 COMPONENTS Widgets REQUIRED)
  find_package(Qt5 COMPONENTS PrintSupport REQUIRED)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
  include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
  # add_definitions(-DQT5_IS_LOADED)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()

#link other libraries
link_directories(${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/codes/core)
include_directories(${PROJECT_SOURCE_DIR}/build/codes/Module)
include_directories(${PROJECT_SOURCE_DIR}/codes/Module/GUI)
include_directories(${PROJECT_SOURCE_DIR}/codes/Module/Pipeline)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

SET(MODULE_INC_LIST
	${PROJECT_SOURCE_DIR}/include/Define.h
)

SET(MODULE_SRC_LIST

)

SET(MODULE_QT_INC 
	${PROJECT_SOURCE_DIR}/codes/Module/GUI/ModuleWidget.h
	${PROJECT_SOURCE_DIR}/codes/Module/GUI/MyThread.h
)

SET(MODULE_QT_SRC 
	${PROJECT_SOURCE_DIR}/codes/Module/GUI/ModuleWidget.cxx
	${PROJECT_SOURCE_DIR}/codes/Module/GUI/MyThread.cxx
)

SET(MODULE_QT_UI 
	${PROJECT_SOURCE_DIR}/codes/Module/GUI/ModuleWidget.ui
)

#Generate moc and ui file
QT5_WRAP_UI(UISrcs ${MODULE_QT_UI})
QT5_WRAP_CPP(MOCSrcs ${MODULE_QT_INC})

add_library(Module STATIC ${MODULE_SRC_LIST} ${MODULE_INC_LIST} ${UISrcs} ${MOCSrcs} ${MODULE_QT_SRC} ${MODULE_QT_INC})

#link libraries
target_link_libraries(Module ${ITK_LIBRARIES}  ${VTK_LIBRARIES} Qt5::Widgets Qt5::PrintSupport)

#Grouping in MSVC
SOURCE_GROUP("_Generated"	FILES ${UISrcs})
SOURCE_GROUP("_Moc Files" 	FILES ${MOCSrcs})
SOURCE_GROUP("UI FILES" 	FILES ${MODULE_QT_UI})
SOURCE_GROUP("Header Files" FILES ${MODULE_QT_INC})
SOURCE_GROUP("Header Files" FILES ${MODULE_INC_LIST})
SOURCE_GROUP("Source Files" FILES ${MODULE_QT_SRC})
SOURCE_GROUP("Source Files" FILES ${MODULE_SRC_LIST})


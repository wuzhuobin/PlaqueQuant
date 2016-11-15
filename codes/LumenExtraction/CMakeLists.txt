
project(LumenExtraction)
SET(
  CMAKE_PREFIX_PATH
  "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"
  )


#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

#link other libraries
link_directories(
	${PROJECT_SOURCE_DIR}/lib
	)

include_directories(
	${MAIN_INCLUDE_DIRS}
	)

set(
    LUMENEXTRATION_SRC
	${PROJECT_SOURCE_DIR}/LumenExtraction.cpp
	${PROJECT_SOURCE_DIR}/LumenExtractionFilter.cpp
	
)

set(
	LUMENEXTRATION_INC
	${PROJECT_SOURCE_DIR}/LumenExtraction.h
	${PROJECT_SOURCE_DIR}/LumenExtractionFilter.h
	${PROJECT_SOURCE_DIR}/LumenExtractionFilter.hxx
)



add_library(
	LumenExtraction 
	${LUMENEXTRATION_SRC}
	${LUMENEXTRATION_INC}
	)

add_executable(
    LumenExtraction_test
    main.cpp
)

#link libraries
target_link_libraries(
	LumenExtraction
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	)
target_link_libraries(
	LumenExtraction_test
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	LumenExtraction
	)
#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${LUMENEXTRATION_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${LUMENEXTRATION_SRC}
	)


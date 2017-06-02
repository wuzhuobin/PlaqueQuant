
project(LumenExtraction)

#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

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

#link libraries
target_link_libraries(
	LumenExtraction
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	)

target_include_directories(
	LumenExtraction
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

add_executable(
    LumenExtraction_test
    main.cpp
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


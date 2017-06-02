project(RegistrationWizard)
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
find_package(Qt5Core)
find_package(Qt5Widgets REQUIRED)
if(NOT Qt5Widgets_FOUND)
	message(FATAL_ERROR "error: Qt5Widgets is not found")
endif()

SET(
	REGISTRATION_WIZARD_SRC
	${PROJECT_SOURCE_DIR}/RegistrationWizard.cpp
	)
SET(
	REGISTRATION_WIZARD_H
	${PROJECT_SOURCE_DIR}/RegistrationWizard.h
	)

qt5_wrap_cpp(
	REGISTRATION_WIZARD_MOC 
	${REGISTRATION_WIZARD_H}
	)

add_library(
	RegistrationWizard
	${REGISTRATION_WIZARD_SRC}
	${REGISTRATION_WIZARD_H}
	${REGISTRATION_WIZARD_MOC}
	)

target_link_libraries(
	RegistrationWizard
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	Qt5::Core
	Qt5::Widgets
)

target_include_directories(
	RegistrationWizard
	INTERFACE
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}	
)

add_executable(
    RegistrationWizard_test
    ${PROJECT_SOURCE_DIR}/main.cpp
)

target_link_libraries(
    RegistrationWizard_test
    RegistrationWizard
)

source_group(
	"Generated FIles" 
	FILES
	${REGISTRATION_WIZARD_MOC}
)
source_group(
	"Header Files" 
	FILES 
	${REGISTRATION_WIZARD_H}
	)
source_group(
	"Source Files"
	FILES
	${REGISTRATION_WIZARD_SRC}
    ${PROJECT_SOURCE_DIR}/main.cpp
	)
cmake_minimum_required(VERSION 2.8)

project(EncryptionAuthentication)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

file(
    GLOB
    VIKEY_H
    Vikey.h
)

file(
    GLOB
    MAIN
    VikeyTest.cpp
)

set(
    ENCRYPTION_AUTHENTICATION_H
    EncryptionAuthentication.h
)

set(
    ENCRYPTION_AUTHENTICATION_CXX
    EncryptionAuthentication.cxx
)

qt5_wrap_cpp(
    ENCRYPTION_AUTHENTICATION_MOC
    ${ENCRYPTION_AUTHENTICATION_CXX}
)

find_library(
    VIKEY_LIB
    NAMES ViKey
    PATHS
    "${PROJECT_SOURCE_DIR}/DLL(X64)"
)
file(
    COPY
    "DLL(X64)/ViKey.dll"
    DESTINATION
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
)

file(
    COPY
    "DLL(X64)/ViKey.dll"
    DESTINATION
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
)

add_library(
    EncryptionAuthentication
    SHARED
    ${VIKEY_H}
    ${ENCRYPTION_AUTHENTICATION_H}
    ${ENCRYPTION_AUTHENTICATION_CXX}
    ${ENCRYPTION_AUTHENTICATION_MOC}
)

target_include_directories(
    EncryptionAuthentication
    PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    EncryptionAuthentication
    ${VIKEY_LIB}
   	Qt5::Widgets
)
set_target_properties(
    EncryptionAuthentication
    PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS 
    true
    # RUNTIME_OUTPUT_DIRECTORY
	# ${CMAKE_SOURCE_DIR}/bin
)

# include(GenerateExportHeader)

# GENERATE_EXPORT_HEADER(
#     EncryptionAuthentication           
#     BASE_NAME EncryptionAuthentication
#     EXPORT_MACRO_NAME EncryptionAuthentication_EXPORTS
#     EXPORT_FILE_NAME EncryptionAuthentication_exports.h
#     STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

add_executable(
    EncryptionAuthenticationTest
    ${MAIN}
)

target_link_libraries(
    EncryptionAuthenticationTest
    EncryptionAuthentication
)

#Grouping in MSVC
SOURCE_GROUP(
	"Generated Files"
	FILES 
	${ENCRYPTION_AUTHENTICATION_MOC}
	)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${ENCRYPTION_AUTHENTICATION_H}
    ${VIKEY_H}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${ENCRYPTION_AUTHENTICATION_CXX}
	)


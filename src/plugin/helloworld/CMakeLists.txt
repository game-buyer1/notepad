cmake_minimum_required(VERSION 3.16)
project(helloworld)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Concurrent Network PrintSupport Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Concurrent Network PrintSupport Xml)

add_definitions(-D_UNICODE -DUNICODE)




file(GLOB UI_SRC ${PROJECT_SOURCE_DIR}/*.ui)
file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB MOC_HEADER ${PROJECT_SOURCE_DIR}/*.h)
# add_executable(${PROJECT_NAME} ${IS_WIN} ${SRC} ${UI_SRC} ${PROJECT_SOURCE_DIR}/src/RealCompare.qrc)

add_library(${PROJECT_NAME} SHARED ${SRC} ${UI_SRC} ${MOC_HEADER})

target_include_directories(${PROJECT_NAME} PRIVATE
${PROJECT_SOURCE_DIR}

${PROJECT_SOURCE_DIR}/../../include
${PROJECT_SOURCE_DIR}/../../qscint/src
${PROJECT_SOURCE_DIR}/../../qscint/src/Qsci
${PROJECT_SOURCE_DIR}/../../qscint/scintilla/src
${PROJECT_SOURCE_DIR}/../../qscint/scintilla/include
${PROJECT_SOURCE_DIR}/../../qscint/scintilla/lexlib
${PROJECT_SOURCE_DIR}/../../qscint/scintilla/boostregex
)

target_link_libraries(${PROJECT_NAME}
    qscint
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Xml)

# if(NOT DEFINED ${notepad--_BINARY_DIR})
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${notepad--_BINARY_DIR}/bin/plugin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${notepad--_BINARY_DIR}/bin/plugin)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${notepad--_BINARY_DIR}/bin/plugin)
# set(LIBRARY_OUTPUT_PATH ${notepad--_BINARY_DIR}/bin/plugin)
# set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${notepad--_BINARY_DIR}/bin/plugin)
# endif()

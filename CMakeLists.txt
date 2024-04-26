cmake_minimum_required(VERSION 3.24)

project(test2gis VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/macro.cmake)

AUTO_BUILD_RESOURCES_QT5()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

INCLUDE_SOURCES(test2gis)

set(QT_COMPONENTS
    QuickWidgets
    Core
    Qml
    QuickControls2
    QmlModels
    QmlWorkerScript
    Quick
    Qml
    QuickShapes
    QuickTemplates2
)

file(COPY ${CMAKE_PREFIX_PATH}/qml DESTINATION ${CMAKE_BINARY_DIR}/${BASE_OUTPUT_DIR})

#Удалить дебажные библиотеки QML
file(GLOB_RECURSE DEBUG_FILES "${CMAKE_BINARY_DIR}/${BASE_OUTPUT_DIR}/qml/*.debug")

foreach(DEBUG_FILE ${DEBUG_FILES})
    file(REMOVE "${DEBUG_FILE}")
endforeach()

add_executable(${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES})

target_include_directories(test2gis
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

FIND_AND_LINK_QT(TARGET
    test2gis
    QT_COMPONENTS
    ${QT_COMPONENTS})


macro(PARSE_SUBDIRECTORIES)
    foreach(arg IN ITEMS ${ARGN})
        add_subdirectory(${arg})
    endforeach()
endmacro()

macro(AUTO_BUILD_RESOURCES_QT5)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
endmacro()

macro(FIND_AND_LINK_QT)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS TARGET)
    set(_MULTI_VALUE_ARGS QT_COMPONENTS)
    cmake_parse_arguments(_LIBS_LINKING
        "${_OPTIONS_ARGS}"
        "${_ONE_VALUE_ARGS}"
        "${_MULTI_VALUE_ARGS}"
        ${ARGN})

    if (NOT _LIBS_LINKING_TARGET)
        message(FATAL_ERROR "FIND_AND_LINK_QT : TARGET is not set")
    endif()

    if (NOT _LIBS_LINKING_QT_COMPONENTS)
        message(FATAL_ERROR "FIND_AND_LINK_QT : QT_COMPONENTS is not set")
    endif()

    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${_LIBS_LINKING_QT_COMPONENTS})
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${_LIBS_LINKING_QT_COMPONENTS})

    foreach(COMPONENT ${_LIBS_LINKING_QT_COMPONENTS})
        message(STATUS "LINK - ${_LIBS_LINKING_TARGET}
            Qt${QT_VERSION_MAJOR}::${COMPONENT}")
        target_link_libraries(
                ${PROJECT_NAME}
                PRIVATE
                Qt${QT_VERSION_MAJOR}::${COMPONENT}
        )
    endforeach()
endmacro()

macro(INCLUDE_SOURCES NAME)
    set(_OPTIONS_ARGS NOT_RECURSIVE)
    set(_ONE_VALUE_ARGS DIR)
    set(_MULTI_VALUE_ARGS EXCLUDE_LIST)

    cmake_parse_arguments(_INCLUDE_SOURCES
        "${_OPTIONS_ARGS}"
        "${_ONE_VALUE_ARGS}"
        "${_MULTI_VALUE_ARGS}"
        ${ARGN})

    if (NOT _INCLUDE_SOURCES_DIR)
        set(_INCLUDE_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    if (_INCLUDE_SOURCES_NOT_RECURSIVE)
        set(RECURSIVE_FLAG GLOB)
    else()
        set(RECURSIVE_FLAG GLOB_RECURSE)
    endif()

    # Ищем исходники в подпроекте, исходя из переменной DIR
    file(${RECURSIVE_FLAG} HEADERS ${_INCLUDE_SOURCES_DIR}/*.h)
    file(${RECURSIVE_FLAG} SOURCES ${_INCLUDE_SOURCES_DIR}/*.cpp *.cc)
    file(${RECURSIVE_FLAG} RESOURCES ${_INCLUDE_SOURCES_DIR}/*.qrc)
    file(${RECURSIVE_FLAG} MANIFESTS ${_INCLUDE_SOURCES_DIR}/*.rc README.txt)
    file(${RECURSIVE_FLAG} METADATA ${_INCLUDE_SOURCES_DIR}/*.json README.txt)
    file(${RECURSIVE_FLAG} UI_FILES ${_INCLUDE_SOURCES_DIR}/*.ui)

    # Исключаем файлы, которые были переданы в EXCLUDE_LIST
    foreach(EXCLUDE_FILE ${_INCLUDE_SOURCES_EXCLUDE_LIST})
        set(REGEX "^(.*)${EXCLUDE_FILE}(.*)$")
        list(FILTER HEADERS EXCLUDE REGEX ${REGEX})
        list(FILTER SOURCES EXCLUDE REGEX ${REGEX})
        list(FILTER RESOURCES EXCLUDE REGEX ${REGEX})
        list(FILTER MANIFESTS EXCLUDE REGEX ${REGEX})
        list(FILTER METADATA EXCLUDE REGEX ${REGEX})
        list(FILTER UI_FILES EXCLUDE REGEX ${REGEX})
    endforeach()

    # Выделяем группу исходников (для отображения в IDE)
    source_group("Header Files" FILES ${HEADERS})
    source_group("Source Files" FILES ${SOURCES})
    source_group("Resources" FILES ${RESOURCES})
    source_group("Manifests" FILES ${MANIFESTS})
    source_group("Proto Files" FILES ${PROTOS})
    source_group("Metadata Files" FILES ${METADATA})
    source_group("UI Files" FILES ${UI_FILES})

    message(STATUS "SOURCES - configured for ${NAME}")
endmacro()

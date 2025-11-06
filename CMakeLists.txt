
# *** Project Initialization ***


cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME iFoRsRadio)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(PROJECT_VERSION_SUFFIX "beta")
set(FULL_VERSION "${PROJECT_VERSION}-${PROJECT_VERSION_SUFFIX}")
message(STATUS "Building ${PROJECT_NAME} version ${FULL_VERSION}")


# *** Version Configuration ***


configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h
)


# *** Platform-Specific Settings ***


if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/app/app.rc")
endif()


# *** C++ Standard & Qt Automatic Features ***


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_QML_COMPILER ON CACHE BOOL "Enable QML compiler")


# *** Paths & Environment ***


set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(QT_BIN_DIR ${Qt6_DIR}/../../../bin)
set(QT_PLUGINS_DIR ${Qt6_DIR}/../../../plugins)
set(QT_QML_DIR ${Qt6_DIR}/../../../qml)


# *** Dependencies ***


set(DEP_LIST
    Quick
    Multimedia
)

include("${ROOT_DIR}/cmake/utils.cmake")
include("${ROOT_DIR}/cmake/options.cmake")

foreach(dep IN LISTS DEP_LIST)
    find_package(Qt6 REQUIRED COMPONENTS ${dep})
endforeach()


# *** Source Files ***


include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRC
    "src/*.tpp"
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
    "api/*.cpp"
    "api/*.h"
)
list(FILTER SRC EXCLUDE REGEX
    ".*/shared/.*"
)


# *** Qt Project Setup ***


qt_standard_project_setup(REQUIRES 6.8)
qt_add_resources(RESOURCE_FILES resources.qrc)


# *** Executable Target ***


if(WIN32)
    add_executable(${PROJECT_NAME}
        WIN32
        src/main.cpp
        ${APP_ICON_RESOURCE_WINDOWS}
    )
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
    ${SRC}
    ${RESOURCE_FILES}
)


# *** Linking Dependencies ***


foreach(dep IN LISTS DEP_LIST)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt6::${dep}
    )
endforeach()


# *** External Libraries ***


add_subdirectory(${ROOT_DIR}/src/shared/subscriptionService)
target_link_libraries(${PROJECT_NAME} PRIVATE SubscriptionService)


# *** Output Directories ***


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    WIN32_EXECUTABLE FALSE
)


# *** Installation ***


include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


# *** Deployment (Qt DLLs, Plugins, QML) ***


copy_dir("${QT_PLUGINS_DIR}/platforms"    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms")
copy_dir("${QT_PLUGINS_DIR}/imageformats" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/imageformats")
copy_dir("${QT_PLUGINS_DIR}/styles"       "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/styles")
copy_dir("${QT_QML_DIR}"                  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qml")

copy_qt_dlls_from_deps(${DEP_LIST})

if(WIN32)
    run_windeployqt6()
endif()

remove_qt_folder()

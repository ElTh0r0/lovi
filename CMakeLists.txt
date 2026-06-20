cmake_minimum_required(VERSION 3.17)
project(lovi
    VERSION 0.2.0
    DESCRIPTION "Log file viewer"
    HOMEPAGE_URL "https://github.com/agateau/lovi"
)
include(CTest)

set(AUTHOR_NAME "Aurélien Gâteau")
set(AUTHOR_EMAIL "mail@agateau.com")
set(ORGANIZATION_NAME "agateau.com")
set(INVERSE_ORGANIZATION_NAME "com.agateau")

set(APP_HUMAN_NAME "Lovi")

set(APP_NAME ${PROJECT_NAME})
set(APPLIB_NAME ${PROJECT_NAME}lib)

set(LOVI_USE_PORTABLE_CONFIG
  FALSE
  CACHE BOOL "Store config in application folder")
if(LOVI_USE_PORTABLE_CONFIG)
add_compile_definitions(USE_PORTABLE_CONFIG)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Dependencies
find_package(Qt6 CONFIG REQUIRED Core Widgets Test LinguistTools Core5Compat) # Try to remove Core5Compat
find_package(ECM REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(KF6GuiAddons CONFIG REQUIRED)

# Installation directories
if (UNIX AND NOT APPLE)
    include(GNUInstallDirs)
    set(DATA_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/${APP_NAME}")
    set(BIN_INSTALL_DIR "bin")
    set(BIN_TO_DATA_DIR "../${DATA_INSTALL_DIR}")
endif()
if (WIN32)
    set(DATA_INSTALL_DIR ".")
    set(BIN_INSTALL_DIR ".")
    set(BIN_TO_DATA_DIR ".")
endif()
if (APPLE)
    set(DATA_INSTALL_DIR "${APP_NAME}.app/Contents/Resources")
    set(BIN_INSTALL_DIR "${APP_NAME}.app/Contents/MacOS")
    set(BIN_TO_DATA_DIR "../Resources")
endif()

# Build flags
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    add_compile_options(-Wall -Woverloaded-virtual)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# Source dirs
add_subdirectory(third-party)
add_subdirectory(src)
if (BUILD_TESTING)
    enable_testing() # must come *before* adding tests directory
    add_subdirectory(tests)
endif()
add_subdirectory(packaging)

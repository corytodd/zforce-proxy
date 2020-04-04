cmake_minimum_required(VERSION 3.9.2)

project(zforce_proxy
        LANGUAGES C
        VERSION 0.1.0.0)
set(CMAKE_C_STANDARD C11)

include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Windows compat -------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif ()

# Cross compat --------------------------------------------------------------------
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options -------------------------------------------------------------------------
option(ENABLE_LOGGING "Enable info and error logging" ON)
option(ENABLE_ANSI_COLORS "Enable ANSI console colors" OFF)

if (ENABLE_LOGGING)
    add_definitions(-DENABLE_LOGGING=1)
else ()
    add_definitions(-DENABLE_LOGGING=0)
endif ()

if (ENABLE_ANSI_COLORS)
    add_definitions(-DENABLE_ANSI_COLORS=1)
else ()
    add_definitions(-DENABLE_ANSI_COLORS=0)
endif ()


# Application --------------------------------------------------------------------
add_subdirectory(src)

# Visual Studio ------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(VS_STARTUP_PROJECT cli)
set(CMAKE_DEFAULT_STARTUP_PROJECT cli)
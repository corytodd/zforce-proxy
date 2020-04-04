# FindzForce.cmake
#
# Locate zForce SDK
#
# INPUT
#    ZFORCE_SDK: Your path to the SDK
#
# OUTPUT
#    ZFORCE_FOUND: True if SDK was found
#    ZFORCE_INCLUDE_DIRS: Use for target_include_directories
#    ZFORCE_LIBS: Use for target_link_libraries

unset(ZFORCE_FOUND)
unset(ZFORCE_INCLUDE_DIRS)
unset(ZFORCE_LIBS)

# Only 64-bit support
if("{CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    message(ERROR "32-bit is not supported")
endif()

# Set base path for zForce library
#
# SDK/
#    Linux/
#          *.so
#    Windows/
#          *.{dll, lib}
#    *.h
if (WIN32)
    message(STATUS "zForce for Windows")
    set(_ZFORCE_PATH_HINT ${ZFORCE_SDK}/Windows/x86-64)
else()
    message(STATUS "zForce for Linux")
    set(_ZFORCE_PATH_HINT ${ZFORCE_SDK}/Linux/x86-64)

    #TODO do we want ARMv6+VFPv2? I don't have any hardware to test it.
endif (WIN32)

set(_ZFORCE_LIB_NAME zForce)

find_path(ZFORCE_INCLUDE_DIRS NAMES
        zForce.h
        HINTS
        ${ZFORCE_SDK})

find_library(ZFORCE_LIBS NAMES
        ${_ZFORCE_LIB_NAME}
        HINTS
        ${_ZFORCE_PATH_HINT})

# debugging
message(DEBUG "Using SDK root: ${ZFORCE_SDK} and looking for ${_ZFORCE_LIB_NAME} on ${_ZFORCE_PATH_HINT}")

if (ZFORCE_INCLUDE_DIRS AND ZFORCE_LIBS)
    set(ZFORCE_FOUND 1)
    message(STATUS "Found includes ${ZFORCE_INCLUDE_DIRS} and libs ${ZFORCE_LIBS}")
else ()
    message(STATUS "zForce SDK not found, did you set ZFORCE_SDK?")
endif (ZFORCE_INCLUDE_DIRS AND ZFORCE_LIBS)
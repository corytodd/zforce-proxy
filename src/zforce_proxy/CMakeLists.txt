set(LIBRARY_TARGET_NAME zforce_proxy)

# Sources --------------------------------------------------------------------
set(${LIBRARY_TARGET_NAME}_SRC
        src/zforce_proxy.c
        src/zforce_proxy_common.c
        )

set(${LIBRARY_TARGET_NAME}_HDR_PUBLIC
        include/zforce_proxy/zforce_proxy_common.h
        include/zforce_proxy/zforce_proxy.h
        )

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

if (MSVC)

    # Disable warning for iota usage and unknown pragma warnings
    add_compile_options("/wd4068")
    add_compile_options("/wd4996")

    include(generate_product_version)
    generate_product_version(
            VersionFilesOutputVariable
            ICON
            COMPANY_NAME "Ardonyx LLC"
            NAME "zForce Proxy"
            VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
            VERSION_MINOR ${PROJECT_VERSION_MINOR}
            VERSION_PATCH ${PROJECT_VERSION_PATCH}
            VERSION_REVISION ${PROJECT_VERSION_TWEAK}
    )
else()
    set(VersionFilesOutputVariable)
endif ()

# Dependencies --------------------------------------------------------------
set(ZFORCE_SDK ${CMAKE_SOURCE_DIR}/ext/zForceSDK)
find_package(zForce REQUIRED)

# Binary --------------------------------------------------------------------
add_library(${LIBRARY_TARGET_NAME}
        ${${LIBRARY_TARGET_NAME}_SRC}
        ${${LIBRARY_TARGET_NAME}_HDR_PUBLIC}
        ${VersionFilesOutputVariable})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

target_include_directories(${LIBRARY_TARGET_NAME} PRIVATE ${ZFORCE_INCLUDE_DIRS})
target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE ${ZFORCE_LIBS})

# Exports --------------------------------------------------------------------
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
        VERSION ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR_PUBLIC}")

target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(${LIBRARY_TARGET_NAME} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

# Installation---------------------------------------------------------------
install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev)

message(STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME} v${PROJECT_VERSION}.")
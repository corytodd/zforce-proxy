set(EXE_TARGET_NAME cli)

set(${EXE_TARGET_NAME}_SRC
        src/Main.c
        )

add_executable(${EXE_TARGET_NAME} ${${EXE_TARGET_NAME}_SRC})

target_link_libraries(${EXE_TARGET_NAME} PRIVATE
        zforce_proxy::zforce_proxy
        )

install(TARGETS ${EXE_TARGET_NAME} DESTINATION bin)

message(STATUS "Created target ${EXE_TARGET_NAME} for export ${PROJECT_NAME}")
add_executable(aero15x-fand aero15x_fand.cpp)
target_link_libraries(aero15x-fand PRIVATE aero15x)
target_include_directories(aero15x-fand
    PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_executable(aero15x-fan-full aero15x_fan_full.cpp)
target_link_libraries(aero15x-fan-full
    PRIVATE aero15x)
target_include_directories(aero15x-fan-full
    PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_executable(aero15x-fan-disable aero15x_fan_disable.cpp)
target_link_libraries(aero15x-fan-disable PRIVATE aero15x)
target_include_directories(aero15x-fan-disable
    PRIVATE ${PROJECT_SOURCE_DIR}/include)

set(PROGRAM_PERMISSIONS_DEFAULT
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

install(
    TARGETS
        aero15x-fand
        aero15x-fan-full
        aero15x-fan-disable
    PERMISSIONS
        ${PROGRAM_PERMISSIONS_DEFAULT} SETUID
    )

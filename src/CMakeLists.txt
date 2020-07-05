add_library(aero15x STATIC 
    aero15x.cpp
    change_detector.cpp
    logging.cpp
    median_filter.cpp)
target_compile_features(aero15x PUBLIC cxx_std_17)
target_include_directories(aero15x
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/aero15x
        ${CMAKE_CURRENT_SOURCE_DIR}
    )


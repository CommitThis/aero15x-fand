enable_testing()

add_executable(test_detector
    test_detector.cpp
    test_median_filter.cpp
)
target_compile_features(test_detector PUBLIC cxx_std_17)
target_link_libraries(test_detector PRIVATE aero15x gtest gtest_main pthread)
add_test(test_detector test_detector)

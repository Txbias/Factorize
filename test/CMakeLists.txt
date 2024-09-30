project(test)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(Tests_run big_int_test.cpp
        quadrativ_sieve_test.cpp
        poly_generator_test.cpp)

target_link_libraries(Tests_run factorize)

target_link_libraries(Tests_run gtest gtest_main)
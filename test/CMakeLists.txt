project(test)


find_path(GOOGLE_TEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/googletest)

if((NOT GOOGLE_TEST_INCLUDE_DIR) OR (NOT EXISTS ${GOOGLE_TEST_INCLUDE_DIR}))
    execute_process(COMMAND git submodule update --init)
endif()


add_subdirectory(../external/googletest lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(Tests_run big_int_test.cpp
        quadratic_sieve_test.cpp
        poly_generator_test.cpp)

target_link_libraries(Tests_run factorize)

target_link_libraries(Tests_run gtest gtest_main)
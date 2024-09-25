cmake_minimum_required(VERSION 3.29)
project(factorize)

set(CMAKE_CXX_STANDARD 26)


add_executable(factorize_run main.cpp)

include_directories(factorize)
add_subdirectory(factorize)

target_link_libraries(factorize_run factorize)

add_subdirectory(test)
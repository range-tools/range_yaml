cmake_minimum_required(VERSION 3.22)

project(range_yaml_tests LANGUAGES C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")

add_executable(parse_and_print parse_and_print.cpp)
target_link_libraries(parse_and_print PRIVATE range_yaml)

file(GLOB TEST_DATA "*.yaml")
file(COPY ${TEST_DATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

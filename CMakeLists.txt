cmake_minimum_required(VERSION 3.6)

project(range_yaml LANGUAGES C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")

set(BUILD_TESTING OFF)
include(FetchContent)
FetchContent_Declare(
  yaml
  GIT_REPOSITORY https://github.com/range-tools/libyaml.git
  GIT_TAG        842037dcee8e9589a073faeb906b6cdf6d8ddb83
  FIND_PACKAGE_ARGS NAMES yaml
)
FetchContent_MakeAvailable(yaml)

add_library(range_yaml range_yaml.cpp range_yaml.h)
target_link_libraries(range_yaml PUBLIC yaml)
target_include_directories(range_yaml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)

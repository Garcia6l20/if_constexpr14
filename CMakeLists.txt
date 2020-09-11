cmake_minimum_required(VERSION 3.17)
project(if_constexpr)

set(CMAKE_CXX_STANDARD 14)

add_library(if_constexpr INTERFACE)
target_include_directories(if_constexpr INTERFACE ${PROJECT_SOURCE_DIR})
add_library(if::constexpr ALIAS if_constexpr)

enable_testing()
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

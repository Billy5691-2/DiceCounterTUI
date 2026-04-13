cmake_minimum_required(VERSION 3.26)

include(CheckPIESupported)
check_pie_supported()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -D_GLIBCXX_ASSERTIONS")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wshadow")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(dice_counter)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy" "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions("DEBUG")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
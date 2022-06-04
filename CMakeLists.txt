cmake_minimum_required(VERSION 3.8)

project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${PROJECT_SOURCE_DIR}/bin>)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

add_subdirectory(lib)
add_subdirectory(src)

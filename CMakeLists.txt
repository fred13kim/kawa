cmake_minimum_required(VERSION 3.10)

project(Kawac
    VERSION 1.0
    DESCRIPTION "A compiler"
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)

add_subdirectory(src)


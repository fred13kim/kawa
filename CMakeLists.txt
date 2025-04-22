cmake_minimum_required(VERSION 3.10)

project(Kawac
    VERSION 1.0
    DESCRIPTION "A compiler"
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src)


cmake_minimum_required(VERSION 3.25)
include(cmake/fetchdependencies.cmake)
include(cmake/compile_options.cmake)
project(PBMA LANGUAGES CXX)

add_subdirectory(src/engine)
add_subdirectory(src/chapters)
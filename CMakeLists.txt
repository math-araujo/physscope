cmake_minimum_required(VERSION 3.25)
include(cmake/fetchdependencies.cmake)
include(cmake/compile_options.cmake)
project(physcope LANGUAGES CXX)

add_subdirectory(physcope/engine)
add_subdirectory(physcope/chapters)
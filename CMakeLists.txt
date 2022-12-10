cmake_minimum_required(VERSION 3.25)
include(cmake/fetchdependencies.cmake)
include(cmake/compile_options.cmake)
project(physscope LANGUAGES CXX)

add_subdirectory(physscope/engine)
add_subdirectory(physscope/chapters)
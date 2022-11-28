cmake_minimum_required(VERSION 3.25)
include(cmake/fetchdependencies.cmake)
project(PBMA LANGUAGES CXX)

add_executable(main 
    src/main.cpp
    src/semaphore.hpp src/semaphore.cpp
)
target_compile_features(main PRIVATE cxx_std_20)
set_target_properties(main PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(main PRIVATE polyscope implot)
target_include_directories(main PRIVATE ${polyscope_SOURCE_DIR}/polyscope/)

if (MSVC)
    target_compile_options(main PRIVATE /W3)
else()
    target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
endif()
project(engine)

add_library(engine STATIC 
    application.hpp application.cpp
    semaphore.hpp semaphore.cpp
    io.hpp io.cpp
    geometry.hpp geometry.cpp
    shapes/uv_sphere.hpp
)

target_compile_features(engine PRIVATE cxx_std_20)
set_target_properties(engine PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(engine PUBLIC polyscope implot imgui tinyobjloader)
target_include_directories(engine PUBLIC ${polyscope_SOURCE_DIR}/polyscope/)
target_include_directories(engine PUBLIC ${tinyobjloader_SOURCE_DIR}/)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_compile_options(engine)
add_library(physscope::engine ALIAS engine)
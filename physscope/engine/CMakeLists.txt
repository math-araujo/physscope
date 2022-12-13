project(engine)

add_library(engine STATIC 
    application.hpp application.cpp
    semaphore.hpp semaphore.cpp
    io.hpp io.cpp
    geometry.hpp geometry.cpp
    shapes/uv_sphere.hpp
)

prepare_target(engine)
target_link_libraries(engine PUBLIC polyscope implot imgui tinyobjloader)
target_include_directories(engine PUBLIC ${polyscope_SOURCE_DIR}/polyscope/)
target_include_directories(engine PUBLIC ${tinyobjloader_SOURCE_DIR}/)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(physscope::engine ALIAS engine)
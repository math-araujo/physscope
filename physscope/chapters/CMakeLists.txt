add_executable(main 
    template.cpp
)
target_compile_features(main PRIVATE cxx_std_20)
set_target_properties(main PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(main PRIVATE physscope::engine)

set_compile_options(main)
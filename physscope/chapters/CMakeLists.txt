add_executable(main 
    template/template.cpp
)

prepare_target(main)
target_link_libraries(main PRIVATE physscope::engine)
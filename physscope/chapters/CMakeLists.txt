# List of each executable name
set(executables 
    template
    chapter2b
)

# List of each executable path; there's a one-to-one
# mapping between executables and exec_paths lists.
set(exec_paths
    template/template.cpp
    chapter2/chapter2b.cpp
)

foreach(executable exec_path IN ZIP_LISTS executables exec_paths)
    add_executable(${executable} ${exec_path})
    prepare_target(${executable})
    target_link_libraries(${executable} PRIVATE physscope::engine)
endforeach()

add_executable(main 
    template/template.cpp
)

prepare_target(main)
target_link_libraries(main PRIVATE physscope::engine)
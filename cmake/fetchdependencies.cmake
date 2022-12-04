include(FetchContent)

FetchContent_Declare(
    polyscope
    SYSTEM
    GIT_REPOSITORY https://github.com/math-araujo/polyscope.git
)
FetchContent_MakeAvailable(polyscope)

FetchContent_Declare(
    tinyobjloader
    SYSTEM
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG v2.0.0rc10
)
FetchContent_MakeAvailable(tinyobjloader)
include(FetchContent)

FetchContent_Declare(
    polyscope
    SYSTEM
    GIT_REPOSITORY https://github.com/math-araujo/polyscope.git
)
FetchContent_MakeAvailable(polyscope)
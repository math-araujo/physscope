# Physscope

**Physscope** is a implementation of algorithms and techniques on *physically based simulation*, for learning purposes and future reference. Rather than a production-ready physics library, the project focuses on being relatively easy to understand and experiment with. 

The project uses the book "Foundations of Physically Based Modelling & Animation" by House and Keyser as a guide into the field and implements the main methods explained in the book. Other complementary sources were used; see the [References section](https://github.com/math-araujo/physscope/#references) below. 

Physically based simulations can be totally independent of a visual representation i.e. the output may be just an array of numbers. While it may be useful to inspect the numerical results, a visualization of the simulations is of great aid. To visualize the simulations, this project uses [Polyscope](polyscope.run/) as viewer, together with [ImPlot](https://github.com/epezent/implot) to draw plots of positions, velocities, energies etc. While using custom OpenGL/Vulkan/DirectX12/etc code would be doable, using a flexible library to render objects allows us to focuses on physics and animation and still have some nice visual results. 

## Features

TODO


## Build Instructions

### Dependencies

The main dependencies of this project are:

* C++20 compiler

* CMake 3.25 or higher

This project uses CMake as a build system generator, so you need to have it installed on your system. The CMake files are responsible for downloading and building the external dependencies, so it's expected that you don't need to download or build those dependencies yourself. Just build the project as described in the next subsection.

For reference, the external libraries used by this project are

* Polyscope (as main viewer)

* ImGui

* ImPlot

* tinyobjloader

* fmt (for cross-platform compatibility with `gcc-9`, that don't support `std::format`)

### Build

Having CMake and C++20 installed on your system, execute the following commands on a terminal:

```
# Just in case you hadn't clone the repository yet
git clone https://github.com/math-araujo/physscope.git

cd physscope

# On Windows + MSVC
cmake --preset=msvc

cmake --build build --config Release

# On Unix
cmake --preset=unix

cmake --build build
```

Then you can run the desired executables, that will be placed at `build/physscope/chapters/<CHAPTER_EXECUTABLE>`. 

Feel free to open an issue if any problem occurs.

## Usage

TODO

### Custom Applications


### Architecture


## References

* House, Donald, and John C. Keyser. Foundations of physically based modeling and animation. AK Peters/CRC Press, 2016.

* Gaffer on Games - Fix Your Timestep! Accessible [here](https://gafferongames.com/post/fix_your_timestep/)
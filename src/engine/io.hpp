#ifndef IO_HPP
#define IO_HPP

#include <array>
#include <glm/glm.hpp>
#include <string_view>
#include <utility>
#include <vector>

namespace pbma
{

/*
WARNING: for now, this function only accepts OBJ files with a single triangle mesh.
Some OBJ files store multiple triangle meshes. Currently there's no need for this
functionality, but it would be good to support this for improved flexibility.
TODO: refactor this function to correctly process multiple triangle meshes.
*/
std::pair<std::vector<glm::vec3>, std::vector<std::array<std::size_t, 3>>> read_triangle_mesh_obj(
    std::string_view filename, bool verbose = false);

} // namespace pbma

#endif // IO_HPP
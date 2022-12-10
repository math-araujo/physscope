#ifndef IO_HPP
#define IO_HPP

#include <array>
#include <glm/glm.hpp>
#include <string_view>
#include <utility>
#include <vector>

#include "geometry.hpp"

// Forward declaration of tinyobj::ObjReader
namespace tinyobj
{

class ObjReader;

} // namespace tinyobj

namespace physscope
{

/*
WARNING: for now, this function only accepts OBJ files with a single triangle mesh.
Some OBJ files store multiple triangle meshes. Currently there's no need for this
functionality, but it would be good to support this for improved flexibility.
TODO: refactor this function to correctly process multiple triangle meshes.
*/
geometry::IndexedTriangleMesh read_triangle_mesh_obj(std::string_view filename, bool verbose = false);

geometry::IndexedTriangleMesh read_triangle_mesh_obj(std::string_view wavefront, std::string_view mtl,
                                                     bool verbose = false);

geometry::IndexedTriangleMesh read_triangle_mesh_obj(tinyobj::ObjReader& reader, bool verbose = false);

} // namespace physscope

#endif // IO_HPP